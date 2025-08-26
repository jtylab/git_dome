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

__attribute__((aligned(4)))  CAN_RxData_t Chassis_CAN_RxData;
__attribute__((aligned(4)))  CAN_TxData_t Chassis_CAN_TxData;


 /**
  * @brief 接收回调函数
  * 
  * @param ID 
  * @param Data 
  */
void C_Board_CAN_Callback(uint32_t ID, uint8_t* Data){
	if(ID == C_Board_CAN_ID+1){ 
		memcpy(&Chassis_CAN_RxData.ChassisBehaviour,Data,sizeof(Data));
	}
    if(ID == C_Board_CAN_ID+2){ 
		memcpy(&Chassis_CAN_RxData.Gimbal_Target_Speed_X,Data,sizeof(Data));
	}
    if(ID == C_Board_CAN_ID+3){ 
		memcpy(&Chassis_CAN_RxData.Gimbal_Target_Speed_Y,Data,sizeof(Data));
	}
    if(ID == C_Board_CAN_ID+4){ 
		memcpy(&Chassis_CAN_RxData.Gimbal_Target_Speed_Z,Data,sizeof(Data));
	}
    if(ID == C_Board_CAN_ID+5){ 
		memcpy(&Chassis_CAN_RxData.Gimbal_Target_Angle,Data,sizeof(Data));    
	}

 }


void Chassis_C_CANTask(void *argument){
    Chassis_t* Chassis = ChassisPoint();
    imu_t* imu = imuPoint();

    BSP_CAN_SetRxCallbackFunc(C_Board_CAN_Port,C_Board_CAN_Callback);
    osThreadFlagsSet(Chassis_C_CANThreadID, C_Board_CAN_ThreadFlag1);   

	osDelay(5000);
    
	while(1){
        // uint32_t Chassis_C_CANFlags = osThreadFlagsWait(0x01,osFlagsWaitAny,osWaitForever);

        Chassis->SetBehaviour((ChassisBehaviour_e)Chassis_CAN_RxData.ChassisBehaviour);
        Chassis->SetChassisTargetSpeed(Chassis_CAN_RxData.Gimbal_Target_Speed_X, Chassis_CAN_RxData.Gimbal_Target_Speed_Y, Chassis_CAN_RxData.Gimbal_Target_Speed_Z);
        // Chassis->SetGimbalTargetAngle(Gimbal_BigYawMotor,Speed_Control,Chassis_CAN_RxData.Gimbal_Target_Angle*2);
        Chassis->SetGimbalTargetSpeed(Chassis_CAN_RxData.Gimbal_Target_Angle);
        if (Chassis_CAN_RxData.Gimbal_Target_Angle != 0)
        {
          Chassis->SetGimbalTargetAngle(Imu,Position_Control,imu->yaw);
          Chassis->SetGimbalTargetAngle(Machine,Position_Control,Chassis->Gimbal_Motor[Gimbal_BigYawMotor]->GetAngle());
        }
        
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
 void Chassis_C_CANTaskInit(void){

    Chassis_C_CANThreadID = osThreadNew(Chassis_C_CANTask, NULL, &Chassis_C_CANTask_attributes);

}
