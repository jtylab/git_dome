/**
 * @file CloudPlatform.h
 * @author jiang
 * @brief 云台
 * @version 0.1
 * @date 2025-07-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef CloudPlatform_H
 #define CloudPlatform_H

#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

static struct Coordinate_t{
    int LU_x;
    int LU_y;
    int RU_x;
    int RU_y;
    int RD_x;
    int RD_y;
    int LD_x;
    int LD_y;
};

struct route_t{
    int x;
    int y;
};
int abs(int x);
void Key_detect(void);
void CloudPlatformTask_Init(void);
struct Coordinate_t* GetCoordinateport(void);

 #endif // !1