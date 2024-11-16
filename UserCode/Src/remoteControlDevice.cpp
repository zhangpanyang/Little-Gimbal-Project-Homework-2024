//
// Created by Sunny on 24-11-9.
//

#include "remoteControlDevice.h"

uint8_t rcRxBuf[RC_RX_BUF_SIZE];
uint8_t rcRxData[RC_RX_DATA_SIZE];

void RemoteControl::init()
{
	channel_.l_col = 0;
	channel_.l_row = 0;
	channel_.r_col = 0;
	channel_.r_row = 0;
	switch_.l = MID_POS;
	switch_.r = MID_POS;
}

RemoteControl remoteControl;
void remoteControlDeviceInit()
{
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rcRxBuf, RC_RX_BUF_SIZE);
	remoteControl.init();
	memset(rcRxBuf, 0, sizeof(rcRxBuf));
	memset(rcRxData, 0, sizeof(rcRxData));
}

void RemoteControl::frameHandle()
{
	channel_.r_row = linearMappingInt2Float((rcRxData[0] | (rcRxData[1] << 8)) & 0x07ff, 364, 1684, -1.0, 1.0);
	channel_.l_col = linearMappingInt2Float(((rcRxData[1] >> 3) | (rcRxData[2] << 5)) & 0x07ff, 364, 1684, -1.0,
											1.0);
	channel_.r_col = linearMappingInt2Float(((rcRxData[2] >> 6) | (rcRxData[3] << 2) | (rcRxData[4] << 10)) &0x07ff, 364, 1684, -1.0, 1.0);
	channel_.l_row = linearMappingInt2Float( ((rcRxData[4] >> 1) | (rcRxData[5] << 7)) & 0x07ff, 364, 1684, -1.0, 1.0);

	switch_.r = RCSwitchStates[((rcRxData[5] >> 4) & 0x0003) - 1];
	switch_.l = RCSwitchStates[((rcRxData[5] >> 4) & 0x000C) - 1];

	// mouse_.x = linearMappingInt2Float((uint16_t)((uint16_t)rcRxData[6] << 8 | rcRxData[7]), -32768, 32767, -1.0, 1.0);
	// mouse_.y = linearMappingInt2Float((uint16_t)((uint16_t)rcRxData[8] << 8 | rcRxData[9]), -32768, 32767, -1.0, 1.0);
	// mouse_.z = linearMappingInt2Float((uint16_t)((uint16_t)rcRxData[10] << 8 | rcRxData[11]), -32768, 32767, -1.0, 1.0);
	//
	// mouse_.l = RCMouseClickStates[rcRxData[12]];
	// mouse_.l = RCMouseClickStates[rcRxData[13]];
	//
	// keys_ = rcRxData[14];
}