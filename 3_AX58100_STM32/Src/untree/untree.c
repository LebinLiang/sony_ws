#include "stm32f4xx.h"                  // Device header
#include "usart.h"
#include "untree.h"
#include "motor_control.h"
#include "gpio.h""

uint32_t crc32_core(uint32_t *ptr, uint32_t len){

uint32_t xbit= 0;

uint32_t data = 0;
	
uint32_t i,bits;

uint32_t CRC32 = 0xFFFFFFFF;

const uint32_t dwPolynomial = 0x04c11db7;
	
for (i= 0;i< len; i++)
{

	xbit= 1 << 31;

	data = ptr[i];

	for ( bits= 0;bits < 32; bits++)
	{
			if (CRC32 & 0x80000000)
			{

				CRC32 <<= 1;

				CRC32^= dwPolynomial;
			}

			else

			CRC32 <<= 1;

			if (data & xbit) CRC32^= dwPolynomial;

				xbit>>= 1;

	}
}

		return CRC32;
}


int modify_data_A1(MOTOR_send_A1 *motor_s)
{

motor_s->hex_len = 34;

motor_s->motor_send_data.head.start[0] = 0xFE;

motor_s-> motor_send_data.head.start[1] = 0xEE;

motor_s->motor_send_data.head.motorlD = motor_s->id;

motor_s->motor_send_data.head.reserved = 0x0;

motor_s->motor_send_data.Mdata.mode = motor_s->mode;

motor_s->motor_send_data .Mdata.ModifyBit = 0xFF;

motor_s->motor_send_data.Mdata.ReadBit = 0x0;

motor_s->motor_send_data.Mdata.reserved = 0x0;

motor_s-> motor_send_data.Mdata.Modify.L = 0;

motor_s->motor_send_data.Mdata.T = motor_s->T * 256;

motor_s->motor_send_data.Mdata.W = motor_s->W * 128;

motor_s->motor_send_data.Mdata.Pos = (int)((motor_s->Pos/16.2832f) * 16384.0f);

motor_s->motor_send_data.Mdata.K_P = motor_s->K_P * 78.5577f;

motor_s->motor_send_data.Mdata.K_W = motor_s->K_W * 102400;

motor_s->motor_send_data.Mdata.LowHzMotorCmdIndex = 0;

motor_s->motor_send_data.Mdata. LowHzMotorCmdByte = 0;

motor_s->motor_send_data.Mdata.Res[0] = motor_s-> Res;

motor_s->motor_send_data.CRCdata.u32 = crc32_core((uint32_t *)(&(motor_s->motor_send_data)), 7);


return 0;
}



int extract_data_A1(MOTOR_recv_A1 *motor_r)
{

/*对比接收到的数据包CRC和计算的CRC32结果是否一致*/

/*由于反馈数据包长度为78Byte, 而CRC32输入参数为4Byte,78不能被4整除*//*因此反馈数据包的CRC结果前两个字节就不校验了*/

if (motor_r->motor_recv_data.CRCdata.u32 !=

crc32_core((uint32_t *)(&(motor_r->motor_recv_data)), 18))
{

// Receive data CRC error

motor_r->correct= 0;//标记该包结果不正确

}
else
{
motor_r->correct = 1;

motor_r->motor_id = motor_r-> motor_recv_data.head.motorlD;
motor_r->mode = motor_r-> motor_recv_data.Mdata.mode;

motor_r->Temp = motor_r->motor_recv_data.Mdata.Temp;
motor_r->MError = motor_r->motor_recv_data.Mdata.MError;
motor_r->T = ((float)motor_r-> motor_recv_data.Mdata.T)/ 256;
motor_r->W = ((float)motor_r->motor_recv_data.Mdata.W)/ 128;
motor_r->LW = motor_r->motor_recv_data.Mdata.LW;

motor_r->Acc = (int)motor_r-> motor_recv_data.Mdata.Acc;

motor_r->Pos = 6.2832f * ((float)motor_r->motor_recv_data.Mdata.Pos) / 16384.0f;

motor_r->gyro[0] = ((float)motor_r->motor_recv_data.Mdata.gyro[0]) * 0.00107993176f;
motor_r->gyro[1] = ((float)motor_r->motor_recv_data.Mdata.gyro[1]) * 0.00107993176f;
motor_r->gyro[2] = ((float)motor_r->motor_recv_data.Mdata.gyro[2]) * 0.00107993176f;

motor_r->acc[0] = ((float)motor_r->motor_recv_data.Mdata.acc[0]) * 0.0023911132f;
motor_r->acc[1] = ((float)motor_r->motor_recv_data.Mdata.acc[1]) * 0.0023911132f;
motor_r->acc[2] = ((float)motor_r->motor_recv_data.Mdata.acc[2]) * 0.0023911132f;

}

return motor_r->correct;
	
}

HAL_StatusTypeDef SERVO_Send_recv_A1(MOTOR_send_A1 *pData, MOTOR_recv_A1 *rData)
{

uint16_t rxlen = 0;
memset(rData,0,sizeof(rData));
modify_data_A1(pData);

SET_485_HIGH1_UP();
//SET_485_HIGH2_UP();
/*我这里便于演示使用了阻塞收发为了获得最佳性能建议您将收发方式改为DMA+IDLE中断方法*/
HAL_UART_Transmit(&huart1, (uint8_t *)pData, sizeof(pData-> motor_send_data), 10);
//HAL_UART_Transmit(&huart6, (uint8_t *)pData, sizeof(pData-> motor_send_data), 10);
SET_485_HIGH1_DOWN();
//SET_485_HIGH2_DOWN();
HAL_UARTEx_ReceiveToIdle(&huart1, (uint8_t *)rData, sizeof(rData->motor_recv_data), &rxlen, 10);
//HAL_UARTEx_ReceiveToIdle(&huart6, (uint8_t *)rData, sizeof(rData->motor_recv_data), &rxlen, 10);


if (rxlen== 0)

return HAL_TIMEOUT;

if (rxlen != sizeof(rData->motor_recv_data))
return HAL_ERROR;

uint8_t *rp =(uint8_t*)&rData->motor_recv_data;
if (rp[0] == 0xFE && rp[1] == 0xEE)
{
extract_data_A1(rData);
return HAL_OK;
}

return HAL_ERROR;
	
}
