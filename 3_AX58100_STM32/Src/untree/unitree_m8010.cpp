
#include "unitree_m8010.h"

#include "crc.h"
#include "string.h"
#include "user_lib.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


extern Unitree_M8010 m8_joint1_ ,m8_joint2_ , m8_joint3_ , m8_joint4_ ; 


/**
 * @brief Initializes the Unitree Motor.
 *
 * This function is used to initialize the Unitree Motor with the specified UART
 * handle, ID, and mode.
 *
 * @param _p_huart Pointer to the UART handle.
 * @param _id The ID of the motor.
 * @param _mode The mode of the motor.
 */
void Unitree_M8010::Init(UART_HandleTypeDef* _p_huart, uint8_t _id,
                         uint8_t _mode, float _ang_bias ) {
  p_huart_ = _p_huart;
  id_ = _id;
  mode_ = _mode;
  ang_bias_ = _ang_bias;
  if (p_huart_->Instance == USART1) {
    p_port_ = GPIOC;
    pin_ = RS485_HIGH_RE1_Pin;
  } else if (p_huart_->Instance == USART6) {
    p_port_ = GPIOC;
    pin_ = RS485_HIGH_RE2_Pin;
  }

}
												 
void Unitree_M8010::Init_CAN(CAN_HandleTypeDef* _p_can,uint8_t _moduleId, uint8_t _id, uint8_t _mode, float _ang_bias)
{
		id_ = _id;
	p_can_ = _p_can;
	mode_ = _mode;
  ang_bias_ = _ang_bias;
	moduleId_  = _moduleId;
}

/**
 * @brief Sets the motor data for the Unitree_M8010 class.
 *
 * @param _Pos The position value.
 * @param _T The torque value.
 * @param _W The angular velocity value.
 * @param _K_P The proportional gain value.
 * @param _K_W The angular velocity gain value.
 */
void Unitree_M8010::SetMotorData(float _Pos, float _T, float _W, float _K_P,
                                 float _K_W) {
  motor_send_m8010_.id = id_;
  motor_send_m8010_.mode = mode_;
  motor_send_m8010_.Pos = _Pos;
  motor_send_m8010_.T = _T;
  motor_send_m8010_.W = _W;
  motor_send_m8010_.K_P = _K_P;
  motor_send_m8010_.K_W = _K_W;
}

/**
 * @brief Sets the position of the motor.
 *
 * @param _Pos The desired position of the motor.
 */
void Unitree_M8010::SetMotorPos(float _Pos) {
  SetMotorData(_Pos, 0, 0, 0.2f, 3.0f);
}

/**
 * @brief Sets the motor torque.
 *
 * This function is used to set the torque of the motor.
 *
 * @param _T The torque value to be set.
 */
void Unitree_M8010::SetMotorT(float _T) {
  SetMotorData(0, Math::AbsLimit(_T, 30.f) / 9.1f, 0, 0, 0.8f);
}

void Unitree_M8010::SetMotorZero() {
  SetMotorData(0, 0, 0, 0, 0);
}

void Unitree_M8010::SetMotorSpeed(float _speed)
{
	SetMotorData(0, 0, _speed, 0, 0.02f);
}


/**
 * @brief Sends data from the Unitree_M8010 class.
 *
 * This function is responsible for sending data from the Unitree_M8010 class.
 * It performs the necessary operations to send the data.
 *
 * @return void
 */
int Unitree_M8010::SendData() {
	
	
	SATURATE(motor_send_m8010_.K_P,  0.0f,   25.599f);
	SATURATE(motor_send_m8010_.K_W,  0.0f,   25.599f);
	SATURATE(motor_send_m8010_.T,   -127.99f,  127.99f);
	SATURATE(motor_send_m8010_.W,   -804.00f,  804.00f);
	SATURATE(motor_send_m8010_.Pos, -411774.0f,  411774.0f);
	
  motor_send_m8010_.ComData.head.start[0] = 0xFE;
  motor_send_m8010_.ComData.head.start[1] = 0xEE;
  motor_send_m8010_.ComData.head.motorID = motor_send_m8010_.id;
  motor_send_m8010_.ComData.Mdata.mode = motor_send_m8010_.mode;
  motor_send_m8010_.ComData.Mdata.ModifyBit = 0xFF;
	
	
  motor_send_m8010_.ComData.Mdata.Pos = motor_send_m8010_.Pos /6.2832f*32768;
  motor_send_m8010_.ComData.Mdata.T = motor_send_m8010_.T * 256;
  motor_send_m8010_.ComData.Mdata.W = motor_send_m8010_.W /6.2832f*256;
  motor_send_m8010_.ComData.Mdata.K_P = motor_send_m8010_.K_P *1280.0f;//25.6f*32768;
  motor_send_m8010_.ComData.Mdata.K_W = motor_send_m8010_.K_W *1280.0f;///25.6f*32768;
  motor_send_m8010_.ComData.CRC32 =
   		HAL_CRC_Calculate(&hcrc, (uint32_t*)&motor_send_m8010_.ComData, 15);

  return 0;
}

/**
 * @brief Updates the Unitree Motor.
 *
 * This function is responsible for updating the Unitree Motor based on the
 * provided data.
 *
 * @param pData Pointer to the data used for updating the motor.
 * @return None.
 */
int Unitree_M8010::Update(uint8_t* pData,uint32_t CRC_data) {
  if (CRC_data == HAL_CRC_Calculate(&hcrc, (uint32_t*)(pData), 18)) {
		
		memcpy((uint8_t*)&motor_recv_m8010_.ServoData,pData,sizeof(motor_recv_m8010_.ServoData));
    motor_recv_m8010_.resv_time = HAL_GetTick();
		DetectHook(motor_recv_m8010_.resv_time);
		
		motor_recv_m8010_.motor_id = motor_recv_m8010_.ServoData.head.motorID;
    motor_recv_m8010_.mode = motor_recv_m8010_.ServoData.Mdata.mode;
    motor_recv_m8010_.Temp = motor_recv_m8010_.ServoData.Mdata.Temp;
    motor_recv_m8010_.MError = motor_recv_m8010_.ServoData.Mdata.MError;
    motor_recv_m8010_.W = ((float)motor_recv_m8010_.ServoData.Mdata.W /256)*6.2832f ;
    motor_recv_m8010_.T = (float)motor_recv_m8010_.ServoData.Mdata.T / 256;
    motor_recv_m8010_.Pos =
        6.2832f * ((float)motor_recv_m8010_.ServoData.Mdata.Pos) / 32768;
  }
  return 0;
}

/**
 * @brief Controls the Unitree motor.
 *
 * This function is responsible for controlling the Unitree motor.
 * It returns the status of the operation.
 *
 * @return HAL_StatusTypeDef The status of the operation.
 */
void Unitree_M8010::Ctrl() {
  SendData();
  HAL_GPIO_WritePin(p_port_, pin_, GPIO_PIN_SET);
	
  HAL_UART_Transmit_DMA(p_huart_, (uint8_t*)&motor_send_m8010_.ComData,
                        sizeof(motor_send_m8010_.ComData));
	
  HAL_UART_Receive_DMA(p_huart_, (uint8_t*)&motor_recv_m8010_.ServoData,
                      sizeof(motor_recv_m8010_.ServoData));

//  uint8_t* rp = (uint8_t*)&motor_recv_m8010_.ServoData;
//  if (rp[0] == 0xFE && rp[1] == 0xEE) {
//		if(motor_recv_m8010_.ServoData.head.motorID == id_)
//		{
//			Update((uint8_t*)&motor_recv_m8010_.ServoData);
//			error_p_->data_is_error = 0;
//			memset((uint8_t*)&motor_recv_m8010_.ServoData, 0,  sizeof(motor_recv_m8010_.ServoData));
//		}
//	memset((uint8_t*)&motor_recv_m8010_.ServoData, 0,  sizeof(motor_recv_m8010_.ServoData));
//	}
//	else
//		error_p_->data_is_error = 1;
}

float Unitree_M8010::GetAngle() {
  return (motor_recv_m8010_.Pos - ang_bias_) / 9.1f;
}

float Unitree_M8010::GetSpeed() {
  return motor_recv_m8010_.W / 9.1f;
}

float Unitree_M8010::GetTor() {
  return motor_recv_m8010_.T * 9.1f;
}

uint32_t Unitree_M8010::GetResvTime(){
	return motor_recv_m8010_.resv_time;
}

void Unitree_M8010::Error_init(error_m8010_t* _error_p){
	
		error_p_ = _error_p;
}

void Unitree_M8010::DetectHook(uint32_t resv_time){
	
	error_p_->last_time = error_p_ ->new_time;
	error_p_->new_time = resv_time ; 
	
	if (error_p_->is_lost)
    {
        error_p_->is_lost = 0;
        error_p_->work_time = error_p_->new_time;
    }
}

uint8_t Unitree_M8010::GetID(){
	
	return motor_recv_m8010_.ServoData.head.motorID;
}

void Unitree_M8010::error_data_update(uint8_t data_error)
{
		error_p_->data_is_error = data_error;
}

void Unitree_M8010::CAN_Update(uint8_t* rx_data,uint32_t ext_id)
{
			motor_recv_m8010_.Temp = ext_id & 0xFF;
	
		motor_recv_m8010_.resv_time = HAL_GetTick();
		DetectHook(motor_recv_m8010_.resv_time);
			
				// 解析实际力矩、速度和位置
    uint16_t temp_torque = (rx_data[6] | (rx_data[7] << 8));
    uint16_t temp_speed =  (rx_data[4] | (rx_data[5] << 8));
    uint32_t temp_position = (rx_data[0] | (rx_data[1] << 8) | (rx_data[2] << 16) | (rx_data[3] << 24));
	// 将整数数据转换回浮点数
    motor_recv_m8010_.T = (float)temp_torque / 256.0f;
    motor_recv_m8010_.W = (float)temp_speed / 256.0f * (2 * M_PI);
    motor_recv_m8010_.Pos = (float)temp_position / 32768.0f * (2 * M_PI);

}

void Unitree_M8010::CAN_Set(){
	uint32_t ext_id_ = ConstructExtendedID(moduleId_, id_, mode_, 11); //控制模式11 - 设置KposKspd
	uint32_t   TxMailbox;
	
	SendData();
	
	// 设置CAN消息属性
    can_tx_message.StdId = 0; // 标准ID不使用
    can_tx_message.IDE = CAN_ID_EXT; // 设置IDE标志表示使用扩展ID
    can_tx_message.RTR = CAN_RTR_DATA; // 表示这是一个数据帧
    can_tx_message.DLC = 8; // 数据长度码，表示发送8个字节的数据
    can_tx_message.ExtId = ext_id_; // 设置扩展ID
		//can_tx_message.TransmitGlobalTime = DISABLE;

//  // 将浮点数增益值转换为整数，以便在CAN帧中传输
//  // 乘以1280是为了提高精度，同时保持在16位整数范围内
//  uint16_t KspdSet = (uint16_t)(Kspd * 1280.0f);
//  uint16_t KposSet = (uint16_t)(Kpos * 1280.0f);
  
  // 将转换后的整数数据填充到发送数据数组中
  can_send_data[0] = motor_send_m8010_.ComData.Mdata.K_W & 0xFF;        // Kspd低8位
  can_send_data[1] = (motor_send_m8010_.ComData.Mdata.K_W >> 8) & 0xFF; // Kspd高8位
  can_send_data[2] = motor_send_m8010_.ComData.Mdata.K_P & 0xFF;        // Kpos低8位
  can_send_data[3] = (motor_send_m8010_.ComData.Mdata.K_P >> 8) & 0xFF; // Kpos高8位
  // TxData[4]到TxData[7]保持为0，因为未使用

		HAL_CAN_AddTxMessage(p_can_, &can_tx_message, can_send_data, &TxMailbox);
}


void Unitree_M8010::CAN_Ctrl(){
		
	uint32_t ext_id_ = ConstructExtendedID(moduleId_, id_, mode_, 10); //控制模式10 - 目标状态
	
	uint32_t   TxMailbox;
	
	SendData();
	
	// 设置CAN消息属性
    can_tx_message.StdId = 0; // 标准ID不使用
    can_tx_message.IDE = CAN_ID_EXT; // 设置IDE标志表示使用扩展ID
    can_tx_message.RTR = CAN_RTR_DATA; // 表示这是一个数据帧
    can_tx_message.DLC = 8; // 数据长度码，表示发送8个字节的数据
    can_tx_message.ExtId = ext_id_; // 设置扩展ID
		
//	  // 将浮点数转换为整数，以便在CAN帧中传输
//  uint32_t Pset = (uint32_t)(position * 32768 / (2 * M_PI));
//  uint16_t Wset = (uint16_t)(speed * 256.0f / (2 * M_PI));
//  uint16_t Tset = (uint16_t)(torque * 256.0f);
  
  // 将转换后的整数数据填充到发送数据数组中
  can_send_data[0] = motor_send_m8010_.ComData.Mdata.Pos & 0xFF;
  can_send_data[1] = (motor_send_m8010_.ComData.Mdata.Pos >> 8) & 0xFF;
  can_send_data[2] = (motor_send_m8010_.ComData.Mdata.Pos >> 16) & 0xFF;
  can_send_data[3] = (motor_send_m8010_.ComData.Mdata.Pos >> 24) & 0xFF;
  can_send_data[4] = motor_send_m8010_.ComData.Mdata.W & 0xFF;
  can_send_data[5] = (motor_send_m8010_.ComData.Mdata.W >> 8) & 0xFF;
  can_send_data[6] = motor_send_m8010_.ComData.Mdata.T & 0xFF;
  can_send_data[7] = (motor_send_m8010_.ComData.Mdata.T >> 8) & 0xFF;
	
	HAL_CAN_AddTxMessage(p_can_, &can_tx_message, can_send_data, &TxMailbox);
}


uint32_t ConstructExtendedID(uint8_t moduleId, uint8_t motorId, uint8_t motorStatus, uint8_t mode)
{
    uint32_t ext_id = 0;  // 初始化 ext_id

    ext_id = ((moduleId & 0x3) << 27) |
                   (0 << 26) |  // 下发标志
                   (0 << 24) |  // 数据内容：电机控制
                   (mode << 16) | // 控制模式11 - 设置KposKspd
                   ((motorId & 0xF) << 8) |
									 (motorStatus << 12) | // motorStatus默认为1
									 (0 << 15) | // 超时标志
                   0; // 其余位无意义

    return ext_id;
}

//// 发送CAN命令
//void CAN_cmd_send(uint8_t moduleId, uint8_t motorId, uint8_t motorStatus, ControlParameters params, uint8_t mode)
//{
//    //uint32_t send_mail_box;
//    uint32_t ext_id = ConstructExtendedID(moduleId, motorId, motorStatus, mode);

//    // 设置CAN消息属性
//    gimbal_tx_message.StdId = 0; // 标准ID不使用
//    gimbal_tx_message.IDE = CAN_ID_EXT; // 设置IDE标志表示使用扩展ID
//    gimbal_tx_message.RTR = CAN_RTR_DATA; // 表示这是一个数据帧
//    gimbal_tx_message.DLC = 8; // 数据长度码，表示发送8个字节的数据
//    gimbal_tx_message.ExtId = ext_id; // 设置扩展ID

//    // 根据控制模式填充数据字段
//    switch (mode) {
//        case 10: // Mode 1
//            // 每控制一次电机CAN就返回一次电机数据
//            // 填充数据字段
//            // 将pos分解为4个字节
//            gimbal_can_send_data[3] = (params.pos >> 24) & 0xFF; // pos最高字节
//            gimbal_can_send_data[2] = (params.pos >> 16) & 0xFF;
//            gimbal_can_send_data[1] = (params.pos >> 8) & 0xFF;
//            gimbal_can_send_data[0] = params.pos & 0xFF; // pos最低字节
//            
//            // 期望速度
//            gimbal_can_send_data[5] = (params.speed >> 8) & 0xFF; // 期望速度高8位
//            gimbal_can_send_data[4] = params.speed & 0xFF; // 期望速度低8位
//            
//            // 期望力矩
//            gimbal_can_send_data[7] = (params.torque >> 8) & 0xFF; // 期望力矩高8位
//            gimbal_can_send_data[6] = params.torque & 0xFF; // 期望力矩低8位
//            break;

//        case 11: // Mode 2
//            // 设置阻尼系数 ($\kappa_{spd}$) 和刚度系数 ($\kappa_{pos}$)
//            // 其他字段可以置零或保持默认值
//						gimbal_can_send_data[0] = params.k_spd & 0xFF; // 刚度系数低8位 ($\kappa_{pos}$)
//            gimbal_can_send_data[1] = (params.k_spd >> 8) & 0xFF; // 刚度系数高8位 ($\kappa_{pos}$)
//					  gimbal_can_send_data[2] = params.k_pos & 0xFF; // 阻尼系数低8位 ($\kappa_{spd}$)
//            gimbal_can_send_data[3] = (params.k_pos >> 8) & 0xFF; // 阻尼系数高8位 ($\kappa_{spd}$)

//            // 其余字段可以置零
//            for(int i = 4; i < 8; i++) {
//                gimbal_can_send_data[i] = 0;
//            }
//    }

//    // 发送CAN消息
//		Can_TxMessage(1,ext_id,8,gimbal_can_send_data);
//    //HAL_CAN_AddTxMessage(&GIMBAL_CAN, &gimbal_tx_message, gimbal_can_send_data, &send_mail_box);
//}




 
//uint8_t Can_TxMessage(uint8_t ide,uint32_t id,uint8_t len,uint8_t *data)
//{
//	uint32_t   TxMailbox;
//	CAN_TxHeaderTypeDef CAN_TxHeader;
//	HAL_StatusTypeDef   HAL_RetVal; 
//	uint16_t i=0;
//	if(ide == 0)
//	{
//		CAN_TxHeader.IDE = CAN_ID_STD;	//标准帧
//		CAN_TxHeader.StdId = id;
//	}
//	else 
//	{
//		CAN_TxHeader.IDE = CAN_ID_EXT;			//扩展帧
//		CAN_TxHeader.ExtId = id;
//	}
//	CAN_TxHeader.DLC = len;
//	CAN_TxHeader.RTR = CAN_RTR_DATA;//数据帧,CAN_RTR_REMOTE遥控帧
//	CAN_TxHeader.TransmitGlobalTime = DISABLE;
//	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0)
//	{
//		i++;
//		if(i>0xfffe)
//			return 1;
//	}
//	HAL_Delay(1);
//	HAL_RetVal = HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeader,data,&TxMailbox);
//	if(HAL_RetVal != HAL_OK)
//		return 1;
//	return 0;
//}



/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
		HAL_StatusTypeDef status;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    // 解析接收到的数据
    uint8_t received_motor_id = rx_header.ExtId >> 8 & 0x0F;
    // 验证接收到的数据是否匹配发送的请求
	
		switch (received_motor_id)
		{
			case 0:
				m8_joint1_.CAN_Update(rx_data,rx_header.ExtId);
				m8_joint1_.error_data_update(0);
				break;
			case 1:
				m8_joint2_.CAN_Update(rx_data,rx_header.ExtId);
				m8_joint2_.error_data_update(0);
				break;
			case 2:
				m8_joint3_.CAN_Update(rx_data,rx_header.ExtId);
				m8_joint3_.error_data_update(0);
				break;
			case 3:
				m8_joint4_.CAN_Update(rx_data,rx_header.ExtId);
				m8_joint4_.error_data_update(0);
				break;
			
		}
				
}



//void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
//  // same as the operation in Sendcommand(), we need to change the huart to
//  // receive mode
//  if (huart == &huart1) {
//    HAL_GPIO_WritePin(GPIOC, RS485_HIGH_RE1_Pin, GPIO_PIN_RESET);
//  }
//  if (huart == &huart6) {
//    HAL_GPIO_WritePin(GPIOC, RS485_HIGH_RE2_Pin, GPIO_PIN_RESET);
//  }
//}


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//		if (huart == &huart1) {
//			HAL_GPIO_WritePin(GPIOC, RS485_HIGH_RE1_Pin, GPIO_PIN_SET);
//			
//			HAL_UART_Receive_DMA(&huart1, (uint8_t*)&motor_recv_raw.ServoData, sizeof(motor_recv_raw.ServoData));
//			uint8_t* rp = (uint8_t*)&motor_recv_raw.ServoData;
//			if (rp[0] == 0xFE && rp[1] == 0xEE) {
//				switch (motor_recv_raw.ServoData.head.motorID)
//				{
//					case 0:
//						a1_joint1_.Update((uint8_t*)&motor_recv_raw.ServoData,motor_recv_raw.ServoData.CRCdata);
//						a1_joint1_.error_data_update(0);//data_is_error = 0;
//					break;
//					case 1:
//						a1_joint2_.Update((uint8_t*)&motor_recv_raw.ServoData,motor_recv_raw.ServoData.CRCdata);
//						a1_joint2_.error_data_update(0);//data_is_error = 0;
//					
//					break;
//					case 2:
//						a1_joint3_.Update((uint8_t*)&motor_recv_raw.ServoData,motor_recv_raw.ServoData.CRCdata);
//						a1_joint3_.error_data_update(0);//data_is_error = 0;
//					break;
//				}
//				memset((uint8_t*)&motor_recv_raw.ServoData, 0,  sizeof(motor_recv_raw.ServoData));
//			}
//			}
//		if (huart == &huart6) {
//			HAL_GPIO_WritePin(GPIOC, RS485_HIGH_RE2_Pin, GPIO_PIN_SET);
//			
//			HAL_UART_Receive_DMA(&huart6, (uint8_t*)&motor_recv_raw.ServoData, sizeof(motor_recv_raw.ServoData));
//			uint8_t* rp = (uint8_t*)&motor_recv_raw.ServoData;
//			if (rp[0] == 0xFE && rp[1] == 0xEE) {
//				switch (motor_recv_raw.ServoData.head.motorID)
//				{
//						case 0:
//						a1_joint4_.Update((uint8_t*)&motor_recv_raw.ServoData,motor_recv_raw.ServoData.CRCdata);
//						a1_joint4_.error_data_update(0);//data_is_error = 0;
//					break;
//					case 1:
//						a1_joint5_.Update((uint8_t*)&motor_recv_raw.ServoData,motor_recv_raw.ServoData.CRCdata);
//						a1_joint5_.error_data_update(0);//data_is_error = 0;
//					
//					break;
//					case 2:
//						a1_joint6_.Update((uint8_t*)&motor_recv_raw.ServoData,motor_recv_raw.ServoData.CRCdata);
//						a1_joint6_.error_data_update(0);//data_is_error = 0;
//					break;
//				}
//				memset((uint8_t*)&motor_recv_raw.ServoData, 0,  sizeof(motor_recv_raw.ServoData));
//			}
//  }
//}

