//
// Created by Sunny on 24-11-9.
//

#include "canDevice.h"

void canDeviceInit()
{
	CAN_FilterTypeDef filterConfig = {
		.FilterIdHigh = 0x0000,
		.FilterIdLow = 0x0000,
		.FilterMaskIdHigh = 0x0000,
		.FilterMaskIdLow = 0x0000,
		.FilterFIFOAssignment = CAN_FILTER_FIFO0,
		.FilterBank = 0,
		.FilterMode = CAN_FILTERMODE_IDMASK,
		.FilterScale = CAN_FILTERSCALE_32BIT,
		.FilterActivation = ENABLE
	};
	HAL_CAN_ConfigFilter(&hcan1, &filterConfig);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

CAN_TxHeaderTypeDef motorTxHeader1234={
	.StdId = 0x200,
	.ExtId = 0x200,
	.IDE = CAN_ID_STD,
	.RTR = CAN_RTR_DATA,
	.DLC = 8,
	.TransmitGlobalTime = DISABLE
};
CAN_TxHeaderTypeDef motorTxHeader5678={
	.StdId = 0x200,
	.ExtId = 0x200,
	.IDE = CAN_ID_STD,
	.RTR = CAN_RTR_DATA,
	.DLC = 8,
	.TransmitGlobalTime = DISABLE
};
uint32_t txMailbox;
uint8_t txData[8];

void canDeviceSendCurrent()
{

}
void SetMotorCurrent(float current)
{
	if(current > 0.2 || current < -0.2)
	{
		return;
	}
	int currentData = linearMappingFloat2Int(current, -20.0f, 20.0f, -16384, 16384);
	for (int i = 0; i < 8; i++)
	{
		txData[i] = (i&1) ? (currentData>>8) : (currentData&0xff);
	}
	HAL_CAN_AddTxMessage(&hcan1, &txHeader, txData, &txMailbox);
}