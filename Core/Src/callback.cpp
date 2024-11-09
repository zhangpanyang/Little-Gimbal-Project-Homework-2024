//
// Created by Sunny on 24-11-9.
//

#include "main.h"
#include "remoteControlDevice.h"

extern uint8_t rcRxBuf[RC_RX_BUF_SIZE];
extern uint8_t rcRxData[RC_RX_DATA_SIZE];
extern RemoteControl remoteControl;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
	if (huart == &huart3)
	{
		std::memcpy(rcRxData, rcRxBuf, RC_RX_BUF_SIZE);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rcRxBuf, RC_RX_BUF_SIZE);
		if (Size == RC_RX_BUF_SIZE)
		{
			remoteControl.frameHandle();
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if (huart == &huart3)
	{
		remoteControl.frameHandle();
	}
}
