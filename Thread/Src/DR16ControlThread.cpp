/**
 * @file DR16ControlThread.cpp
 * @author jiang
 * @brief DR16遥控器控制线程
 * @version 0.1
 * @date 2025-04-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "DR16ControlThread.h"


/**
 * @brief DR16遥控器控制任务
 * 
 */
 void DR16ControlTask(void){

//   ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
    


    DR16_t    *DR16 = DR16_Point();
    static Chassis_t *Chassis = ChassisPoint();

    
    while(true){
		

        switch(DR16->SW1){  // 底盘 云台


			case REMOTE_SW_UP:
				Chassis->SetBehaviour(CHASSIS_ZERO_FORCE);

		// 		Gimbal->SetBehaviour(GIMBAL_ZERO_FORCE);
				break;
			case REMOTE_SW_MID:
				Chassis->SetBehaviour(CHASSIS_NO_FOLLOW);  // CHASSIS_FOLLOW_GIMBAL CHASSIS_NO_FOLLOW
                Chassis->SetChassisSpeed((float)DR16->CH1 / 660.0f * Scaler_Chassis, (float)DR16->CH2 / 660.0f * Scaler_Chassis, (float)DR16->CH3 );
		// 		Gimbal->SetBehaviour(GIMBAL_ZERO_FORCE); // GIMBAL_ENABLE
		// 		Gimbal->SetSpeed(((float)DR16->CH3) / 660.0f * 0.6f * K_Gimbal, ((float)DR16->CH4) / 660.0f * 0.4f * K_Gimbal);
				
				// Chassis->SetTrendAngle(Gimbal->GetRelativeAngle());

				break;
			case REMOTE_SW_DOWN:
				Chassis->SetBehaviour(CHASSIS_SPIN);  // CHASSIS_FOLLOW_GIMBAL CHASSIS_SPIN
                Chassis->SetChassisSpeed((float)DR16->CH1 / 660.0f * Scaler_Chassis, (float)DR16->CH2 / 660.0f * Scaler_Chassis, 0);
		// 		Gimbal->SetBehaviour(GIMBAL_ZERO_FORCE); // GIMBAL_ENABLE
		// 		Gimbal->SetSpeed(((float)DR16->CH3) / 660.0f * 0.6f * K_Gimbal, ((float)DR16->CH4) / 660.0f * 0.4f * K_Gimbal);
				
				// Chassis->SetTrendAngle(Gimbal->GetRelativeAngle());
				
				break;
			default:
				Chassis->SetBehaviour(CHASSIS_ZERO_FORCE);
				
				// Gimbal->SetBehaviour(GIMBAL_ZERO_FORCE);
				break;
		// }
		// switch (DR16->SW2) {  // 发弹
		// 	case REMOTE_SW_UP:
		// 		Booster->SetState(BOOSTER_DISABLE);
		// 		Booster->SetSpeed(0);

		// 		K_Gimbal -= DR16->Wheel / 660.0 * 0.004;  // 这个 K_Gimbal 是在用遥控器手操测试发弹时，用来微调云台控制灵敏度的
		// 		K_Gimbal = LimitBoth(K_Gimbal, 2.0, 0.25);
		// 		break;
		// 	case REMOTE_SW_MID:
		// 		Booster->SetState(BOOSTER_ENABLE);
		// 		Booster->SetSpeed(6000);  // 3v3英雄 优先初速上限16m/s 爆发优先初速上限10m/s

		// 		if (DR16->Wheel > 450) {
		// 			Boost_Press_Last = 1;
		// 		} else {
		// 			if (Boost_Press_Last) Booster->Boost();
		// 			Boost_Press_Last = 0;
		// 		}
		// 		break;
		// 	case REMOTE_SW_DOWN:
		// 		Booster->SetState(BOOSTER_ENABLE);
		// 		Booster->SetSpeed(8191);  // 8191:16m/s  8000:14m/s  6000:12m/s  4000:10m/s

		// 		if (DR16->Wheel > 450) {
		// 			Boost_Press_Last = 1;
		// 		} else {
		// 			if (Boost_Press_Last) Booster->Boost();
		// 			Boost_Press_Last = 0;
		// 		}
		// 		break;
		// 	default:
		// 		Booster->SetState(BOOSTER_DISABLE);
		// 		Booster->SetSpeed(0);
		// 		break;
		}

        osDelay(4);
    }
 }




void DR16ControlThread_Init(void){
    // BaseType_t xRutern = pdPASS;

    xTaskCreate((TaskFunction_t)DR16ControlTask,"DR16ControlTask",2048,NULL,25,&DR16ControlThread_Handle);

    // if(xRutern == pdPASS){
    //     vTaskStartScheduler();
    // }
    // else{
    //    while(true);
    // }
}