#ifndef __CPP_APP_H_
#define __CPP_APP_H_

#include "unitree.h"
#include "main.h"

// 定义 CheckList 枚举
enum CheckList
{
    A1_MOTOR1_TOE,
    A1_MOTOR2_TOE,
    A1_MOTOR3_TOE,
    A1_MOTOR4_TOE,
    A1_MOTOR5_TOE,
    A1_MOTOR6_TOE,
    ERROR_LIST_LENGHT,
};



// 声明外部的 error_list 数组
extern error_t error_list[];  


#endif /* __CPP_APP_H_ */
