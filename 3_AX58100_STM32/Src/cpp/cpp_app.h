#ifndef __CPP_APP_H_
#define __CPP_APP_H_

#include "unitree.h"
#include "main.h"
#include "unitree_m8010.h"

// ���� CheckList ö��
enum CheckList
{
    A1_MOTOR1_TOE,
    A1_MOTOR2_TOE,
    A1_MOTOR3_TOE,
    A1_MOTOR4_TOE,
    A1_MOTOR5_TOE,
    A1_MOTOR6_TOE,
		M8_MOTOR1_TOE,
		M8_MOTOR2_TOE,
		M8_MOTOR3_TOE,
		M8_MOTOR4_TOE,
    ERROR_LIST_LENGHT,
};



// �����ⲿ�� error_list ����
extern error_t error_list[];  


#endif /* __CPP_APP_H_ */
