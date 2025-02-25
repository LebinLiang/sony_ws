// cpp_main.cpp

#include "cpp_app.h"


float hz_detect = 0 ;
float hz_detect2 = 0;
int detect_count = 0;
int detect_count_m8 =0; 
int motor_choose = 0;
int can_choose = 0 ;
float a1_joint1_angle ,a1_joint2_angle ; 

uint16_t set_item[ERROR_LIST_LENGHT][3] =
        {
            {10, 10, 11},   //motor1
            {10, 10, 10},   //motor2
            {10, 10, 9},    //motor3
            {10, 10, 8},    //motor4
						{10, 10, 7},    //motor5
            {10, 10, 6},    //motor6
						{10, 10, 9},    //motorm1
            {10, 10, 8},    //motorm2
						{10, 10, 7},    //motorm3
            {10, 10, 6},    //motorm4
        };

error_t error_list[ERROR_LIST_LENGHT + 1];


Unitree_Motor a1_joint1_ , a1_joint2_ , a1_joint3_ , a1_joint4_ ,a1_joint5_, a1_joint6_; 

Unitree_M8010 m8_joint1_ ,m8_joint2_ , m8_joint3_ , m8_joint4_;

				
				
void detect_check(void)
{
	  static uint32_t system_time; 

		system_time  = HAL_GetTick();
		
		hz_detect =   detect_count * 10.0f  ;
	  detect_count = 0;
		hz_detect2 =  detect_count_m8 * 10.0f ;
		detect_count_m8 = 0;
	
		HAL_GPIO_TogglePin(GPIOG, LED_5_Pin);
		
    error_list[ERROR_LIST_LENGHT].is_lost = 0;
    error_list[ERROR_LIST_LENGHT].error_exist = 0;
	
		for (int i = 0; i < ERROR_LIST_LENGHT; i++)
        {
            //disable, continue
            //未使能，跳过
            if (error_list[i].enable == 0)
            {
                continue;
            }

            //judge offline.判断掉线
            if (system_time - error_list[i].new_time > error_list[i].set_offline_time)
            {
                if (error_list[i].error_exist == 0)
                {
                    //record error and time
                    //记录错误以及掉线时间
                    error_list[i].is_lost = 1;
                    error_list[i].error_exist = 1;
                    error_list[i].lost_time = system_time;
                }
                
                error_list[ERROR_LIST_LENGHT].is_lost = 1;
                error_list[ERROR_LIST_LENGHT].error_exist = 1;
            }
            else if (system_time - error_list[i].work_time < error_list[i].set_online_time)
            {
                //just online, maybe unstable, only record
                //刚刚上线，可能存在数据不稳定，只记录不丢失，
                error_list[i].is_lost = 0;
                error_list[i].error_exist = 1;
            }
            else
            {
                error_list[i].is_lost = 0;
                //判断是否存在数据错误
                //judge if exist data error
                if (error_list[i].data_is_error != NULL)
                {
                    error_list[i].error_exist = 1;
                }
                else
                {
                    error_list[i].error_exist = 0;
                }
                //calc frequency
                //计算频率
                if (error_list[i].new_time > error_list[i].last_time)
                {
                    error_list[i].frequency = 1000 / (float)(error_list[i].new_time - error_list[i].last_time);
                }
            }
        }
		
}

void cpp_main_init(void)
{
	
	a1_joint1_.Init(&huart1, 0, 0, 0);
	a1_joint1_.Error_init(&error_list[A1_MOTOR1_TOE]);
	
	a1_joint2_.Init(&huart1, 1, 0, 0);
	a1_joint2_.Error_init(&error_list[A1_MOTOR2_TOE]);
	
	a1_joint3_.Init(&huart1, 2, 0, 0);
	a1_joint3_.Error_init(&error_list[A1_MOTOR3_TOE]);
	
  a1_joint4_.Init(&huart6, 0, 0, 0);
	a1_joint4_.Error_init(&error_list[A1_MOTOR4_TOE]);
	
	a1_joint5_.Init(&huart6, 1, 0, 0);
	a1_joint5_.Error_init(&error_list[A1_MOTOR5_TOE]);
	
	a1_joint6_.Init(&huart6, 2, 0, 0);
	a1_joint6_.Error_init(&error_list[A1_MOTOR6_TOE]);
	
	
	m8_joint1_.Init_CAN(&hcan1,0x03,0,1,0);
	m8_joint1_.Error_init((error_m8010_t*)&error_list[M8_MOTOR1_TOE]);
	
	m8_joint2_.Init_CAN(&hcan1,0x03,1,1,0);
	m8_joint2_.Error_init((error_m8010_t*)&error_list[M8_MOTOR2_TOE]);
	
	m8_joint3_.Init_CAN(&hcan1,0x03,2,1,0);
	m8_joint3_.Error_init((error_m8010_t*)&error_list[M8_MOTOR3_TOE]);
	
	m8_joint4_.Init_CAN(&hcan1,0x03,3,1,0);
	m8_joint4_.Error_init((error_m8010_t*)&error_list[M8_MOTOR4_TOE]);
	
	
	uint32_t time = HAL_GetTick();

    for (uint8_t i = 0; i < ERROR_LIST_LENGHT; i++)
    {
        error_list[i].set_offline_time = set_item[i][0];
        error_list[i].set_online_time = set_item[i][1];
        error_list[i].priority = set_item[i][2];

        error_list[i].enable = 1;
        error_list[i].error_exist = 1;
        error_list[i].is_lost = 1;
        error_list[i].data_is_error = 1;
        error_list[i].frequency = 0.0f;
        error_list[i].new_time = time;
        error_list[i].last_time = time;
        error_list[i].lost_time = time;
        error_list[i].work_time = time;
    }
	
}


void cpp_main2(void)
{
		switch (can_choose)
	{
		case 0:
			//m8_joint1_.SetMotorZero();
			m8_joint1_.SetMotorSpeed(5.0f);	
			m8_joint1_.CAN_Set();
			m8_joint1_.CAN_Ctrl();
			break;
		case 1:
			//m8_joint2_.SetMotorZero(); 
			m8_joint2_.SetMotorSpeed(10.0f);	
			m8_joint2_.CAN_Set();
			m8_joint2_.CAN_Ctrl();
			break;
		case 2:
			//m8_joint3_.SetMotorZero();
			m8_joint3_.SetMotorSpeed(5.0f);	
			m8_joint3_.CAN_Set();
			m8_joint3_.CAN_Ctrl();
		
			break;
		case 3:
			//m8_joint4_.SetMotorZero(); 
			m8_joint4_.SetMotorSpeed(10.0f);	
			m8_joint4_.CAN_Set();
			m8_joint4_.CAN_Ctrl();
			break;
	}
	can_choose+=1;
	if (can_choose >= 4)
			can_choose = 0;
	
	detect_count_m8+=1;
}

void cpp_main(void)
{
		switch (motor_choose)
	{
		case 0:
			a1_joint1_.SetMotorT(1.0f);
			a1_joint1_.Ctrl();
			break;
		case 1:
			a1_joint5_.SetMotorT(1.0f); 
			a1_joint5_.Ctrl();
			break;
		case 2:
			a1_joint3_.SetMotorT(1.0f);
			a1_joint3_.Ctrl();
			break;
		case 3:
			a1_joint4_.SetMotorT(1.0f); 
			a1_joint4_.Ctrl();
			break;
		case 4:
			a1_joint2_.SetMotorT(1.0f);
			a1_joint2_.Ctrl();
			break;
		case 5:
			a1_joint6_.SetMotorT(1.0f);
			a1_joint6_.Ctrl();
			break;
	}
	motor_choose+=1;
	if (motor_choose >= 6)
			motor_choose = 0;
		
	detect_count+=1;
	
}
