//
// Created by Sunny on 24-11-9.
//

#include "canDevice.h"

#include <cstring>

#include "motorDevice.h"

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
	.ExtId = 0x1FF,
	.IDE = CAN_ID_STD,
	.RTR = CAN_RTR_DATA,
	.DLC = 8,
	.TransmitGlobalTime = DISABLE
};
uint32_t canTxMailbox;

CAN_RxHeaderTypeDef canRxHeader;
uint8_t canRxData[8];

extern MotorSet motorSet;

void canControllerRxHandle(CAN_HandleTypeDef* hcan)
{
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, canRxData);
	uint8_t canLine;
	if(hcan == &hcan1) canLine = 1;
	if(hcan == &hcan2) canLine = 2;
	uint8_t controllerId = canRxHeader.StdId - 0x200;
	motorSet.getMotorById(canLine, controllerId)->controllerRxHandle(canRxData);
}

uint8_t canTxData[4][8];
void canDeviceRoutine() // Attention! The "line" and "id" is 1 less than the real value!
{
	memset(canTxData, 0, sizeof(canTxData));
	for(auto motorPtr : motorSet)
	{
		uint8_t line = motorPtr->hardwareInfo.canLine - 1;
		uint8_t id = motorPtr->hardwareInfo.controllerId - 1;
		int16_t currentData = linearMappingFloat2Int(motorPtr->outputIntensity, -20.0f, 20.0f, -16384, 16384);
		canTxData[ (line << 1) + (id >> 2) ][ (id & 0x03) << 1 ] = currentData >> 8;
		canTxData[ (line << 1) + (id >> 2) ][ (id & 0x03) << 1 | 1 ] = currentData & 0xff;
	}
	HAL_CAN_AddTxMessage(&hcan1, &motorTxHeader1234, canTxData[0], &canTxMailbox);
	HAL_CAN_AddTxMessage(&hcan1, &motorTxHeader5678, canTxData[1], &canTxMailbox);
	HAL_CAN_AddTxMessage(&hcan2, &motorTxHeader1234, canTxData[2], &canTxMailbox);
	HAL_CAN_AddTxMessage(&hcan2, &motorTxHeader5678, canTxData[3], &canTxMailbox);
}