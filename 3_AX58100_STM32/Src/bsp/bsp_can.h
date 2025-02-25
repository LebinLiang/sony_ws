/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       bsp_can.h
 * @brief      this file contains sd card basic operating function
 * @note         
 * @Version    V1.0.0
 * @Date       Jan-23-2018      
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */
  
#ifndef _BSP__CAN_H
#define _BSP__CAN_H

#include "stm32f4xx_HAL.h"

extern CAN_HandleTypeDef hcan1;
HAL_StatusTypeDef can_filter_init(CAN_HandleTypeDef* hcan);


#endif
