/**
 *******************************************************************************
 * @file      : unitree.h
 * @brief     :
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      yyyy-mm-dd      <author>        1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, USTB.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UNITREE_M8010_H_
#define __UNITREE_M8010_H_


#include "bsp_uart.h"
#include "bsp_can.h"

#define M_PI 3.14159f


#define SATURATE(_IN, _MIN, _MAX) {\
 if (_IN < _MIN)\
 _IN = _MIN;\
 else if (_IN > _MAX)\
 _IN = _MAX;\
 } 



typedef __packed struct
{
    uint32_t new_time;
    uint32_t last_time;
    uint32_t lost_time;
    uint32_t work_time;
    uint16_t set_offline_time : 12;
    uint16_t set_online_time : 12;
    uint8_t enable : 1;
    uint8_t priority : 4;
    uint8_t error_exist : 1;
    uint8_t is_lost : 1;
    uint8_t data_is_error : 1;
    float frequency;
} error_m8010_t;


#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#pragma pack(1)

// ���� error_m8010_t �ṹ��

// �����õ����������ݽṹ
typedef union {
  int32_t L;
  uint8_t u8[4];
  uint16_t u16[2];
  uint32_t u32;
  float F;
} COMData32_m8010;

typedef struct {
  // ���� ���ݰ�ͷ
  unsigned char start[2];  // ��ͷ
  unsigned char
      motorID;  // ���ID  0,1,2,3 ...   0xBB ��ʾ�����е���㲥����ʱ�޷��أ�
  unsigned char reserved;
} COMHead_m8010;

#pragma pack()

#pragma pack(1)

typedef struct {
  uint8_t fan_d;   // �ؽ��ϵ�ɢ�ȷ���ת��
  uint8_t Fmusic;  // �������Ƶ��   /64*1000   15.625f Ƶ�ʷֶ�
  uint8_t Hmusic;  // �������ǿ��   �Ƽ�ֵ4  ����ǿ�� 0.1 �ֶ�
  uint8_t reserved4;

  uint8_t FRGB[4];  // ���LED

} LowHzMotorCmd_m8010;

typedef struct {      // �� 4���ֽ�һ������ ����Ȼ�����������
                      // ���� ����
  uint8_t mode;       // �ؽ�ģʽѡ��
  uint8_t ModifyBit; //= 0xFF;  // ������Ʋ����޸�λ
  uint8_t ReadBit; //= 0x00;    // ������Ʋ�������λ
  uint8_t reserved;

  COMData32_m8010 Modify;  // ��������޸� ������
  // ʵ�ʸ�FOC��ָ������Ϊ��
  //  K_P*delta_Pos + K_W*delta_W + T
  int16_t T;  // �����ؽڵ�������أ������������أ�x256, 7 + 8 ����
  int16_t W;  // �����ؽ��ٶ� �����������ٶȣ� x128,       8 + 7����
  int32_t Pos;  // �����ؽ�λ�� x 16384/6.2832,
                // 14λ������������0������������ؽڻ����Ա�����0��Ϊ׼��

  int16_t K_P;  // �ؽڸն�ϵ�� x2048  4+11 ����
  int16_t K_W;  // �ؽ��ٶ�ϵ�� x1024  5+10 ����

  uint8_t LowHzMotorCmdIndex;  // �����Ƶ�ʿ������������, 0-7,
                               // �ֱ����LowHzMotorCmd�е�8���ֽ�
  uint8_t LowHzMotorCmdByte;   // �����Ƶ�ʿ���������ֽ�

  COMData32_m8010 Res[1];  // ͨѶ �����ֽ�  ����ʵ�ֱ��һЩͨѶ����

} MasterComdV3_m8010;  // �������ݰ��İ�ͷ ��CRC 34�ֽ�

typedef struct {
  // ���� ��������������ݰ�
  COMHead_m8010 head;
  MasterComdV3_m8010 Mdata;
  uint32_t CRC32;
} MasterComdDataV3_m8010;  // ��������

// typedef struct {
// 	// ���� �ܵ�485 ���ݰ�

//   MasterComdData M1;
// 	MasterComdData M2;
// 	MasterComdData M3;

// }DMA485TxDataV3;

#pragma pack()

#pragma pack(1)

typedef struct
{
    uint8_t id     :4;      // ���ID: 0,1...,13,14 15��ʾ�����е���㲥����(��ʱ�޷���)
	
    uint8_t status :3;      // ����ģʽ: 0.���� 1.FOC�ջ� 2.������У׼ 3.����
    uint8_t none   :1;      // ����λ
} RIS_Mode_t;   // ����ģʽ 1Byte

/**
 * @brief ���״̬������Ϣ
 * 
 */
typedef struct
{
    int16_t tor_des;        // �����ؽ����Ť�� unit: N.m      (q8)
    int16_t spd_des;        // �����ؽ�����ٶ� unit: rad/s    (q8)
    int32_t pos_des;        // �����ؽ����λ�� unit: rad      (q15)
    int16_t k_pos;          // �����ؽڸն�ϵ�� unit: -1.0-1.0 (q15)
    int16_t k_spd;          // �����ؽ�����ϵ�� unit: -1.0-1.0 (q15)
    
} RIS_Comd_t;   // ���Ʋ��� 12Byte

/**
 * @brief ���״̬������Ϣ
 * 
 */
typedef struct
{
    int16_t  torque;        // ʵ�ʹؽ����Ť�� unit: N.m     (q8)
    int16_t  speed;         // ʵ�ʹؽ�����ٶ� unit: rad/s   (q8)
    int32_t  pos;           // ʵ�ʹؽ����λ�� unit: rad     (q15)
    int8_t   temp;          // ����¶�: -128~127��C
    uint8_t  MError :3;     // ��������ʶ: 0.���� 1.���� 2.���� 3.��ѹ 4.���������� 5-7.����
    uint16_t force  :12;    // �����ѹ���������� 12bit (0-4095)
    uint8_t  none   :1;     // ����λ
} RIS_Fbk_t;   // ״̬���� 11Byte



typedef struct
{ // �� 4���ֽ�һ������ ����Ȼ�����������
    // ���� ����
    uint8_t mode;       // ��ǰ�ؽ�ģʽ
    uint8_t ReadBit;    // ������Ʋ����޸�     �Ƿ�ɹ�λ
    int8_t Temp;        // �����ǰƽ���¶�
    uint8_t MError;     // ������� ��ʶ

    COMData32_m8010 Read;     // ��ȡ�ĵ�ǰ ��� �Ŀ�������
    int16_t T;          // ��ǰʵ�ʵ���������       7 + 8 ����

    int16_t W;          // ��ǰʵ�ʵ���ٶȣ����٣�   8 + 7 ����
    float LW;           // ��ǰʵ�ʵ���ٶȣ����٣�

    int16_t W2;         // ��ǰʵ�ʹؽ��ٶȣ����٣�   8 + 7 ����
    float LW2;          // ��ǰʵ�ʹؽ��ٶȣ����٣�

    int16_t Acc;        // ���ת�Ӽ��ٶ�       15+0 ����  ������С
    int16_t OutAcc;     // �������ٶ�         12+3 ����  �����ϴ�

    int32_t Pos;        // ��ǰ���λ�ã�����0������������ؽڻ����Ա�����0��Ϊ׼��
    int32_t Pos2;       // �ؽڱ�����λ��(���������)

    int16_t gyro[3];    // ���������6�ᴫ��������
    int16_t acc[3];

    // ��������������
    int16_t Fgyro[3];
    int16_t Facc[3];
    int16_t Fmag[3];
    uint8_t Ftemp;      // 8λ��ʾ���¶�  7λ��-28~100�ȣ�  1λ0.5�ȷֱ���

    int16_t Force16;    // ����������16λ����
    int8_t Force8;      // ����������8λ����

    uint8_t FError;     //  ��˴����������ʶ

    int8_t Res[1];      // ͨѶ �����ֽ�

} ServoComdV3_m8010; // �������ݰ��İ�ͷ ��CRC 78�ֽڣ�4+70+4��

typedef struct
{
    uint8_t head[2];    // ��ͷ         2Byte
    RIS_Mode_t mode;    // �������ģʽ  1Byte
    RIS_Fbk_t   fbk;    // ����������� 11Byte
    uint16_t  CRC16;    // CRC          2Byte
} MotorData_t;  //��������

typedef struct
{
    // ���� ��������������ݰ�
    uint8_t head[2];    // ��ͷ         2Byte
    RIS_Mode_t mode;    // �������ģʽ  1Byte
    RIS_Comd_t comd;    // ����������� 12Byte
    uint16_t   CRC16;   // CRC          2Byte
} ControlData_t;     //��������������ݰ�


typedef struct {
  // ���� ��������������ݰ�
  COMHead_m8010 head;
  ServoComdV3_m8010 Mdata;

  uint32_t CRCdata;

} ServoComdDataV3_m8010;  // ��������

#pragma pack()

typedef struct
{
    // ���� ���͸�ʽ������
    MasterComdDataV3_m8010 ComData;
    int hex_len;                        //���͵�16�����������鳤��, 34
    long long send_time;                //���͸������ʱ��, ΢��(us)
    // �����͵ĸ�������
    unsigned short id;                  //���ID��0����ȫ�����
    unsigned short mode;                // 0:����, 5:����ת��, 10:�ջ�FOC����
    //ʵ�ʸ�FOC��ָ������Ϊ��
    // K_P*delta_Pos + K_W*delta_W + T
    float T;                            //�����ؽڵ�������أ������������أ���Nm��
    float W;                            //�����ؽ��ٶȣ����������ٶȣ�(rad/s)
    float Pos;                          //�����ؽ�λ�ã�rad��
    float K_P;                          //�ؽڸն�ϵ��
    float K_W;                          //�ؽ��ٶ�ϵ��
    COMData32_m8010 Res;                    // ͨѶ �����ֽ�  ����ʵ�ֱ��һЩͨѶ����
} MOTOR_send_m8010;

typedef struct
{
    // ���� ��������
    ServoComdDataV3_m8010 ServoData;  // ����������ݽṹ�壬���motor_msg.h
    int hex_len;                        //���յ�16�����������鳤��, 78
    long long resv_time;                //���ո������ʱ��, ΢��(us)
    int correct;                        //���������Ƿ�������1������0��������
    //����ó��ĵ������
    unsigned char motor_id;             //���ID
    unsigned char mode;                 // 0:����, 5:����ת��, 10:�ջ�FOC����
    int Temp;                           //�¶�
    unsigned char MError;               //������
    float T;                            // ��ǰʵ�ʵ���������
		float W;														// speed
    float Pos;                          // ��ǰ���λ�ã�����0������������ؽڻ����Ա�����0��Ϊ׼��
		float footForce;												// �����ѹ���������� 12bit (0-4095)

} MOTOR_recv_m8010;


uint32_t ConstructExtendedID(uint8_t moduleId, uint8_t motorId, uint8_t motorStatus, uint8_t mode);


class Unitree_M8010 {
 public:
  void Init(UART_HandleTypeDef* _p_huart, uint8_t _id, uint8_t _mode, float _ang_bias);
  void Init_CAN(CAN_HandleTypeDef* _p_can,uint8_t _moduleId, uint8_t _id, uint8_t _mode, float _ang_bias);
 
  int SendData();
  void SetMotorData(float _Pos, float _T, float _W, float _K_P, float _K_W);
  void SetMotorPos(float _Pos);
  void SetMotorT(float _T);
	
	void SetMotorZero();
 
  int Update(uint8_t* pData,uint32_t CRC_data);
  float GetAngle();
  float GetSpeed();
	uint32_t GetResvTime();
  float GetTor();
  void Ctrl();
  void Receive();
  void Error_init(error_m8010_t* _error_p);
  void DetectHook(uint32_t resv_time);
	void error_data_update(uint8_t data_error );
  uint8_t GetID();
 
	void CAN_Set();
  void CAN_Ctrl();
  void CAN_Update(uint8_t* rx_data,uint32_t ext_id);
 
 private:
  UART_HandleTypeDef* p_huart_;
	CAN_HandleTypeDef* p_can_;
	
  CAN_TxHeaderTypeDef  can_tx_message;
  uint8_t      					can_send_data[8];
 
  uint8_t id_, mode_;
  GPIO_TypeDef* p_port_;
  uint16_t pin_;
  MOTOR_send_m8010 motor_send_m8010_;
	error_m8010_t* error_p_;
	MOTOR_recv_m8010 motor_recv_m8010_;
 
	uint8_t moduleId_;
  
  float ang_bias_;
 
	
};

/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/

#endif
#endif /* __UNITREE_H_ */
