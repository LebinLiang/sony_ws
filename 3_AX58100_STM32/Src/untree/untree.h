#ifndef  __UNTREE_H
#define  __UNTREE_H

#include <stdint.h>
#include "stm32f4xx.h"                  // Device header
#include "main.h"

#pragma pack(1) // 1Byte����!����ɾ��!

typedef union{

int32_t L;

	uint8_t u8[4];
	uint16_t u16[2];
	uint32_t u32;
	float F;

} COMData32_A1;

typedef struct{

//�������ݰ�ͷ

	unsigned char start[2];//��ͷstart[0] = 0xFE start[1] = 0xEE
	
	unsigned char motorlD; // ���ID 0,1,2,3.. 0xBB ��ʾ�����е���㲥(��ʱ�޷���)
	unsigned char reserved;

} COMHead_A1;

typedef struct

{//��4���ֽ�һ�����У���Ȼ�����������

//��������

	uint8_t mode;//��ǰ�ؽ�ģʽ

	uint8_t ReadBit; //������Ʋ����޸��Ƿ�ɹ�λ



	int8_t Temp; // �����ǰƽ���¶�

	uint8_t MError; // ��������ʶ

	COMData32_A1 Read;//��ȡ�ĵ�ǰ����Ŀ�������

	int16_t T;//��ǰʵ�ʵ��������� 7+8����

	int16_t W;//��ǰʵ�ʵ���ٶ�(����) 8+7����



float LW; // ��ǰʵ�ʵ���ٶ�(����)

int16_t W2;//��ǰʵ�ʹؽ��ٶ�(����)8+7����



float LW2; // ��ǰʵ�ʹؽ��ٶ�(����)

int16_t Acc;//���ת�Ӽ��ٶ�     15+0����������С



int16_t OutAcc; //�������ٶ�   12+3���������ϴ�



int32_t Pos; // ��ǰ���λ��(����0������������ؽڻ����Ա�����0��Ϊ׼

int32_t Pos2;//�ؽڱ�����λ��(���������)

int16_t gyro[3];//���������6�ᴫ��������

int16_t acc[3];

//��������������

int16_t Fgyro[3];

int16_t Facc[3];

int16_t Fmag[3];

uint8_t Ftemp;//8λ��ʾ���¶�7 λ(-28~100��) 1 λ0.5�ȷֱ���

int16_t Force16;//����������16λ����
int8_t Force8; // ����������8λ����

uint8_t FError;//��˴����������ʶ

int8_t Res[1];//ͨѶ�����ֽ�

} ServoComdV3_A1;// ��_�����ݰ��İ�ͷ��CRC 78�ֽ�(4+ 70+4)

typedef struct{

COMHead_A1 head;

ServoComdV3_A1 Mdata;
COMData32_A1 CRCdata;

} ServoComdDataV3_A1; //��������������ݰ�

typedef struct{

uint8_t none[8];//����
} LowHzMotorCmd_A1;

typedef struct

{

//��4���ֽ�һ�����У���Ȼ�����������//��������

uint8_t mode;//�ؽ�ģʽѡ��

uint8_t ModifyBit; //������Ʋ����޸�λ

uint8_t ReadBit; // ������Ʋ�������λ

uint8_t reserved;

COMData32_A1 Modify;//��������޸ĵ�����

	//ʵ�ʸ�FOC��ָ������Ϊ:

// K_ _P*delta_ Pos + K_ _W*delta_ _W + T

int16_t T; // �����ؽڵ��������(������������) x256, 7 +8����

int16_t W; // �����ؽ��ٶ�(���������ٶ�) x128,8+7����

int32_t Pos;//�����ؽ�λ��x 16384/6.2832. 14λ������(����0 ������������ؽڻ����Ա�����0��Ϊ׼)

int16_t K_P;//�ؽڸն�ϵ��x78.5577 ������
int16_t K_W;//�ؽ��ٶ�ϵ��x102400������

uint8_t LowHzMotorCmdIndex; //����
uint8_t LowHzMotorCmdByte; // ����

COMData32_A1 Res[1];//ͨѶ�����ֽ�����ʵ�ֱ��һ ЩͨѶ����
} MasterComdV3_A1; //�������ݰ��İ�ͷ��CRC34�ֽ�.

typedef struct

{

//�����������������ݰ�
COMHead_A1 head;

MasterComdV3_A1 Mdata;

COMData32_A1 CRCdata;

} MasterComdDataV3_A1; //��������

#pragma pack()//����1Byte ����!����ɾ��!

typedef struct

{

//���巢�͸�ʽ������.

MasterComdDataV3_A1 motor_send_data; //����������ݽṹ��

int hex_len;//���͵�16�����������鳤��, 34

long long send_time;//���͸������ʱ��,΢��(us)



//�����͵ĸ�������.

unsigned short id; // ���ID��0����ȫ�����

unsigned short mode; // 0:���У�5:����ת��, 10:�ջ�FOC����

//ʵ�ʸ�FOC��ָ������Ϊ:

// K_ _P*delta_ _Pos + K_ _W*delta_ _W + T

float T; //�����ؽڵ��������(������������) (Nm)

float W;//�����ؽ��ٶ�(���������ٶ�) (rad/s)

float Pos;//�����ؽ�λ��(rad)

float K_P;//�ؽڸն�ϵ��

float K_W;//�ؽ��ٶ�ϵ��



COMData32_A1 Res;//ͨѶ�����ֽ��� ��ʵ�ֱ��һЩͨѶ����
} MOTOR_send_A1;

typedef struct{

//�����������

ServoComdDataV3_A1 motor_recv_data; //����������ݽṹ�壬���motor_ _msg.h
int hex_len;//���յ�16�����������鳤��, 78

long long resv_time;//���ո������ʱ�䣬΢��(us)

int correct;//���������Ƿ�����(1������0������)



//����ó��ĵ������

unsigned char motor_id; //���ID

unsigned char mode;// 0:���У�5:����ת��, 10:�ջ�FOC����

int Temp;//�¶�

unsigned char MError; // ������

float T; // ��ǰʵ�ʵ���������

float W; // ��ǰʵ�ʵ���ٶ�(����)

float LW; // ��ǰʵ�ʵ���ٶ�(����)

int Acc; // ���ת�Ӽ��ٶ�

float Pos; //��ǰ���λ��(����0������������ؽڻ����Ա�����0��Ϊ׼)

float gyro[3]; //���������6�ᴫ��������
float acc[3];

} MOTOR_recv_A1;

//#define RS485_DE_GPIO_Port GPIOF
//#define RS485_RE_GPIO_Port GPIOF

//#define RS485_DE_Pin       GPIO_PIN_0
//#define RS485_RE_Pin       GPIO_PIN_1

/*ע��:TTLת485ģ���RE DE���������ӵ���һ��*/

//#define SET_485_DE_UP() HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET)
//#define SET_485_DE_DOWN() HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET)

//#define SET_485_RE_UP() HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET)
//#define SET_485_RE_DOWN() HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET)

uint32_t crc32_core(uint32_t *ptr, uint32_t len);

int modify_data_A1(MOTOR_send_A1 *motor_s);

int extract_data_A1(MOTOR_recv_A1 *motor_r);

HAL_StatusTypeDef SERVO_Send_recv_A1(MOTOR_send_A1 *pData, MOTOR_recv_A1 *rData);


#endif
