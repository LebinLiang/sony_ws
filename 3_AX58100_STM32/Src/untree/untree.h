#ifndef  __UNTREE_H
#define  __UNTREE_H

#include <stdint.h>
#include "stm32f4xx.h"                  // Device header
#include "main.h"

#pragma pack(1) // 1Byte对齐!不可删除!

typedef union{

int32_t L;

	uint8_t u8[4];
	uint16_t u16[2];
	uint32_t u32;
	float F;

} COMData32_A1;

typedef struct{

//定义数据包头

	unsigned char start[2];//包头start[0] = 0xFE start[1] = 0xEE
	
	unsigned char motorlD; // 电机ID 0,1,2,3.. 0xBB 表示向所有电机广播(此时无返回)
	unsigned char reserved;

} COMHead_A1;

typedef struct

{//以4个字节一组排列，不然编译器会凑整

//定义数据

	uint8_t mode;//当前关节模式

	uint8_t ReadBit; //电机控制参数修改是否成功位



	int8_t Temp; // 电机当前平均温度

	uint8_t MError; // 电机错误标识

	COMData32_A1 Read;//读取的当前电机的控制数据

	int16_t T;//当前实际电机输出力矩 7+8描述

	int16_t W;//当前实际电机速度(高速) 8+7描述



float LW; // 当前实际电机速度(低速)

int16_t W2;//当前实际关节速度(高速)8+7描述



float LW2; // 当前实际关节速度(低速)

int16_t Acc;//电机转子加速度     15+0描述惯量较小



int16_t OutAcc; //输出轴加速度   12+3描述惯量较大



int32_t Pos; // 当前电机位置(主控0点修正，电机关节还是以编码器0点为准

int32_t Pos2;//关节编码器位置(输出编码器)

int16_t gyro[3];//电机驱动板6轴传感器数据

int16_t acc[3];

//力传感器的数据

int16_t Fgyro[3];

int16_t Facc[3];

int16_t Fmag[3];

uint8_t Ftemp;//8位表示的温度7 位(-28~100度) 1 位0.5度分辨率

int16_t Force16;//力传感器高16位数据
int8_t Force8; // 力传感器低8位数据

uint8_t FError;//足端传感器错误标识

int8_t Res[1];//通讯保留字节

} ServoComdV3_A1;// 加_上数据包的包头和CRC 78字节(4+ 70+4)

typedef struct{

COMHead_A1 head;

ServoComdV3_A1 Mdata;
COMData32_A1 CRCdata;

} ServoComdDataV3_A1; //电机控制命令数据包

typedef struct{

uint8_t none[8];//保留
} LowHzMotorCmd_A1;

typedef struct

{

//以4个字节一组排列，不然编译器会凑整//定义数据

uint8_t mode;//关节模式选择

uint8_t ModifyBit; //电机控制参数修改位

uint8_t ReadBit; // 电机控制参数发送位

uint8_t reserved;

COMData32_A1 Modify;//电机参数修改的数据

	//实际给FOC的指令力矩为:

// K_ _P*delta_ Pos + K_ _W*delta_ _W + T

int16_t T; // 期望关节的输出力矩(电机本身的力矩) x256, 7 +8描述

int16_t W; // 期望关节速度(电机本身的速度) x128,8+7描述

int32_t Pos;//期望关节位置x 16384/6.2832. 14位编码器(主控0 点修正，电机关节还是以编码器0点为准)

int16_t K_P;//关节刚度系数x78.5577 倍描述
int16_t K_W;//关节速度系数x102400倍描述

uint8_t LowHzMotorCmdIndex; //保留
uint8_t LowHzMotorCmdByte; // 保留

COMData32_A1 Res[1];//通讯保留字节用于实现别的一 些通讯内容
} MasterComdV3_A1; //加上数据包的包头和CRC34字节.

typedef struct

{

//定义电机控制命令数据包
COMHead_A1 head;

MasterComdV3_A1 Mdata;

COMData32_A1 CRCdata;

} MasterComdDataV3_A1; //返回数据

#pragma pack()//结束1Byte 对齐!不可删除!

typedef struct

{

//定义发送格式化数据.

MasterComdDataV3_A1 motor_send_data; //电机控制数据结构体

int hex_len;//发送的16进制命令数组长度, 34

long long send_time;//发送该命令的时间,微秒(us)



//待发送的各项数据.

unsigned short id; // 电机ID，0代表全部电机

unsigned short mode; // 0:空闲，5:开环转动, 10:闭环FOC控制

//实际给FOC的指令力矩为:

// K_ _P*delta_ _Pos + K_ _W*delta_ _W + T

float T; //期望关节的输出力矩(电机本身的力矩) (Nm)

float W;//期望关节速度(电机本身的速度) (rad/s)

float Pos;//期望关节位置(rad)

float K_P;//关节刚度系数

float K_W;//关节速度系数



COMData32_A1 Res;//通讯保留字节用 于实现别的一些通讯内容
} MOTOR_send_A1;

typedef struct{

//定义接收数据

ServoComdDataV3_A1 motor_recv_data; //电机接收数据结构体，详见motor_ _msg.h
int hex_len;//接收的16进制命令数组长度, 78

long long resv_time;//接收该命令的时间，微秒(us)

int correct;//接收数据是否完整(1完整，0不完整)



//解读得出的电机数据

unsigned char motor_id; //电机ID

unsigned char mode;// 0:空闲，5:开环转动, 10:闭环FOC控制

int Temp;//温度

unsigned char MError; // 错误码

float T; // 当前实际电机输出力矩

float W; // 当前实际电机速度(高速)

float LW; // 当前实际电机速度(低速)

int Acc; // 电机转子加速度

float Pos; //当前电机位置(主控0点修正，电机关节还是以编码器0点为准)

float gyro[3]; //电机驱动板6轴传感器数据
float acc[3];

} MOTOR_recv_A1;

//#define RS485_DE_GPIO_Port GPIOF
//#define RS485_RE_GPIO_Port GPIOF

//#define RS485_DE_Pin       GPIO_PIN_0
//#define RS485_RE_Pin       GPIO_PIN_1

/*注意:TTL转485模块的RE DE引脚我连接到了一起*/

//#define SET_485_DE_UP() HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET)
//#define SET_485_DE_DOWN() HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET)

//#define SET_485_RE_UP() HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET)
//#define SET_485_RE_DOWN() HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET)

uint32_t crc32_core(uint32_t *ptr, uint32_t len);

int modify_data_A1(MOTOR_send_A1 *motor_s);

int extract_data_A1(MOTOR_recv_A1 *motor_r);

HAL_StatusTypeDef SERVO_Send_recv_A1(MOTOR_send_A1 *pData, MOTOR_recv_A1 *rData);


#endif
