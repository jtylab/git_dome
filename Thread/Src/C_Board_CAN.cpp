/**
 * @file C_Board_CAN.cpp
 * @author jiang
 * @brief 双C板通信
 * @version 0.1
 * @date 2025-07-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "C_Board_CAN.h"


 /**
  * @brief 接收回调函数
  * 
  * @param ID 
  * @param Data 
  */
void C_Board_CAN_Callback(uint32_t ID, uint8_t* Data){
	if(ID == C_Board_CAN_ID){ 
		memcpy(&Chassis_CAN_RxData,Data,sizeof(Chassis_CAN_RxData));
        osThreadFlagsSet(Chassis_C_CANThreadID, C_Board_CAN_ThreadFlag1);
	}
 }


void Chassis_C_CANTask(void *argument){
    Chassis_t* Chassis = ChassisPoint();

    BSP_CAN_SetRxCallbackFunc(C_Board_CAN_Port,C_Board_CAN_Callback);
    osThreadFlagsSet(Chassis_C_CANThreadID, C_Board_CAN_ThreadFlag1);        //

	osDelay(5000);
    
	while(1){
        uint32_t Flags = osThreadFlagsWait(0x01,osFlagsWaitAny,osWaitForever);

        Chassis->SetBehaviour((ChassisBehaviour_e)Chassis_CAN_RxData.ChassisBehaviour);
        Chassis->SetGimbalTargetSpeed(Chassis_CAN_RxData.Gimbal_Target_Speed_X, Chassis_CAN_RxData.Gimbal_Target_Speed_Y, Chassis_CAN_RxData.Gimbal_Target_Speed_Z);
        Chassis->SetGimbalTargetAngle(Gimbal_BigYawMotor,Position_Control,Chassis->Yaw_RelativeAngle + Chassis_CAN_RxData.Gimbal_Target_Angle);
        
        Chassis_CAN_TxData.Yaw_RelativeAngle =  Chassis->Yaw_RelativeAngle;
		BSP_CAN_SendStandard8Data(C_Board_CAN_Port,C_Board_CAN_ID,(uint8_t *)&Chassis_CAN_TxData);
		osDelay(4);
	}

 }



static const osThreadAttr_t Chassis_C_CANTask_attributes = {"Chassis_C_CANTask", 0, 0, 0, 0, 512, (osPriority_t)osPriorityNormal7};

/**
  * @brief 初始化
  * 
  */
 void IMU_CAN_Init(void){

    Chassis_C_CANThreadID = osThreadNew(Chassis_C_CANTask, NULL, &Chassis_C_CANTask_attributes);

}
