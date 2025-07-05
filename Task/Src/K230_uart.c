/**
 * @file K230_uart.c
 * @author jiang
 * @brief 视觉通信
 * @version 0.1
 * @date 2025-07-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "K230_Uart.h"

__attribute__((aligned(4))) uint8_t Data[256];  //数据


/**
 * @brief 
 * 
 * @param huart 
 * @param Size 
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size){

    struct Coordinate_t* Coordinate;
    Coordinate = GetCoordinateport();

    if(Data[0] == 0){
        memcpy(&Coordinate_RxData, &Data, sizeof(Coordinate_RxData));
        for(int i = 1,j = 4,k = 7,z = 10,x = 13,y = 16,q = 19, w = 22,m = 100; i <= 3; i++,j++,k++,z++,m /= 10){
            Coordinate->LU_x +=  Coordinate_RxData[i] * m;
            Coordinate->LU_y +=  Coordinate_RxData[j] * m;
            Coordinate->RU_x +=  Coordinate_RxData[k] * m;
            Coordinate->RU_y +=  Coordinate_RxData[z] * m;
            Coordinate->RD_x +=  Coordinate_RxData[x] * m;
            Coordinate->RD_y +=  Coordinate_RxData[y] * m;
            Coordinate->LD_x +=  Coordinate_RxData[q] * m;
            Coordinate->LD_y +=  Coordinate_RxData[w] * m;
        }
        
    }

    memset(Data, 0, sizeof(Data));
    HAL_UARTEx_ReceiveToIdle_DMA(&huart4,(uint8_t *)Data, sizeof(Data));
    __HAL_DMA_DISABLE_IT(huart4.hdmarx,DMA_IT_HT);
}



/**
 * @brief 
 * 
 * @param argument 
 */
 void K230_UartTask(void *argument){

    HAL_UARTEx_ReceiveToIdle_DMA(&huart4,(uint8_t *)Data, sizeof(Data));
    __HAL_DMA_DISABLE_IT(huart4.hdmarx,DMA_IT_HT);

    while (1)
    {
        Key_detect();
        osDelay(1);
        // osThreadDetach(K230_UartTask_Handle);
    }
 }

 osThreadId_t K230_UartTask_Handle;
 osThreadAttr_t K230_UartTask_attributes = {"K230_UartTask", 0, 0, 0, 0, 2048, (osPriority_t)osPriorityRealtime1};

void K230_UartTask_Init(void){
    taskENTER_CRITICAL();

    K230_UartTask_Handle = osThreadNew(K230_UartTask, NULL, &K230_UartTask_attributes);

    taskEXIT_CRITICAL();
}