/**
 * @file CloudPlatform.c
 * @author jiang
 * @brief 云台
 * @version 0.1
 * @date 2025-07-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */


 #include "CloudPlatform.h"

 #define x_mapping 1
 #define y_mapping 1
 #define x_positioning 1
 #define y_positioning 1

 struct Coordinate_t Coordinate;
 struct route_t rout[4];
 uint8_t Key_mode = 0;
 uint16_t X_Dutycycle_Now = 1500,Y_Dutycycle_Now = 1500;
 

 int abs(int x){
   if(x<0){
      return -x;
   }
   else return x;
 }

 /**
  * @brief 
  * 
  */
 void Key_detect(void){
   if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET){
      osDelay(100);
      if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_SET){
         Key_mode = 0;
      }
   }

   if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET){
      osDelay(100);
      if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET){
         Key_mode = 1;
      }
   }
 }

 /**
  * @brief 
  * 
  * @param x 
  * @param y 
  */
 void positioning(int x,int y){

   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

   X_Dutycycle_Now = x * x_positioning;
   Y_Dutycycle_Now = y * y_positioning;
   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,X_Dutycycle_Now);
   __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,Y_Dutycycle_Now);
   osDelay(2000);

   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
 }

 void mobility(int x, int y){
   int X_Dutycycle_change = x * x_mapping;
   int Y_Dutycycle_change = y * y_mapping;
   uint16_t X_Dutycycle_target = X_Dutycycle_Now + X_Dutycycle_change;
   uint16_t Y_Dutycycle_target = Y_Dutycycle_Now + Y_Dutycycle_change;
 }

 /**
  * @brief 
  * 
  * @param argument 
  */
 void CloudPlatformTask(void *argument){
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);

    while (1)
    {
      
       switch (Key_mode)
       {
       case 0:
         
         break;
       
       case 1:
         rout[0].x = Coordinate.RU_x - Coordinate.LU_x;
         rout[0].y = Coordinate.RU_y - Coordinate.LU_y;
         rout[1].x = Coordinate.RD_x - Coordinate.RU_x;
         rout[1].y = Coordinate.RD_y - Coordinate.RU_y;
         rout[2].x = Coordinate.RD_x - Coordinate.LD_x;
         rout[2].y = Coordinate.RD_y - Coordinate.LD_y;
         rout[3].x = Coordinate.LD_x - Coordinate.LU_x;
         rout[3].y = Coordinate.LD_y - Coordinate.LU_y;

         positioning(Coordinate.LU_x,Coordinate.LU_y);
         break;
       }


       osDelay(1);
    }
 }

 osThreadId_t CloudPlatformTask_Handle;
 osThreadAttr_t CloudPlatformTask_attributes = {"CloudPlatformTask", 0, 0, 0, 0, 2048, (osPriority_t)osPriorityRealtime1};

 void CloudPlatformTask_Init(void){
    
    taskENTER_CRITICAL();

    CloudPlatformTask_Handle = osThreadNew(CloudPlatformTask, NULL, &CloudPlatformTask_attributes);

    taskEXIT_CRITICAL();
 }

struct Coordinate_t* GetCoordinateport(void){
      return &Coordinate;
  }