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
	// HAL_CAN_ConfigFilter(&hcan2, &filterConfig);
	// HAL_CAN_Start(&hcan2);
	// HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

namespace CanRx
{
	CAN_RxHeaderTypeDef header;
	uint8_t data[8];

	void handle(CAN_HandleTypeDef* hcan)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data);
		uint8_t canLine;
		if(hcan == &hcan1) canLine = 1;
		if(hcan == &hcan2) canLine = 2;
		uint8_t controllerId = header.StdId - 0x200;
		motorSet.getMotorById(canLine, controllerId)->controllerRxHandle(data);
	}
}

namespace CanTx
{
	const uint8_t headerIdCnt = 5;
	uint16_t headerIdList[ headerIdCnt ] = {0x200, 0x1FF, 0x1FE, 0x2FE, 0x2FF};
	inline uint8_t findHeaderIndById(uint16_t targetId)
	{
		for (int i = 0; i < headerIdCnt; i++)
			if (headerIdList[i] == targetId)
				return i;
		return headerIdCnt;
	}
	CAN_TxHeaderTypeDef headerTemplate={
		.StdId = 0x000,
		.ExtId = 0x000,
		.IDE = CAN_ID_STD,
		.RTR = CAN_RTR_DATA,
		.DLC = 8,
		.TransmitGlobalTime = DISABLE
	};
	CAN_HandleTypeDef* lineList[] = { &hcan1, &hcan2 };
	uint32_t mailbox;
	uint8_t data[ 2 ][ headerIdCnt ][ 8 ]; // can line index, header index, data index
	uint8_t frameActivated[ 2 ][ headerIdCnt ];

	inline void init()
	{
		memset(data, 0, sizeof(data));
		memset(frameActivated, 0, sizeof(frameActivated));
	}

	CAN_TxHeaderTypeDef txHeader={
		.StdId = 0x1FF,
		.ExtId = 0x1FF,
		.IDE = CAN_ID_STD,
		.RTR = CAN_RTR_DATA,
		.DLC = 8,
		.TransmitGlobalTime = DISABLE
		};
	uint32_t txMailbox;
	uint8_t txData[8];

	void transmit()
	{
		txData[0] = 0x20;
		txData[1] = 0x70;
		txData[2] = 0x20;
		txData[3] = 0x70;
		txData[4] = 0x20;
		txData[5] = 0x70;
		txData[6] = 0x20;
		txData[7] = 0x70;
		if(HAL_CAN_AddTxMessage(&hcan1, &txHeader, txData, &txMailbox) != HAL_OK){
			txData[0] = 0;
		}
		// for (uint8_t headerInd = 0; headerInd < headerIdCnt; headerInd++)
		// {
		// 	for (uint8_t lineInd = 0; lineInd < 2; lineInd++)
		// 	{
		// 		if (!frameActivated[lineInd][headerInd])
		// 			continue;
		// 		auto a = HAL_CAN_GetTxMailboxesFreeLevel(lineList[lineInd]);
		// 		while(HAL_CAN_IsTxMessagePending(lineList[lineInd], mailbox)) {}
		// 		headerTemplate.StdId = headerIdList[ headerInd ];
		// 		HAL_CAN_AddTxMessage(lineList[lineInd], &headerTemplate, data[lineInd][headerInd], &mailbox);
		// 	}
		// }
	}
}
void canDeviceRoutine()
{
	CanTx::init();
	for(auto motorPtr : motorSet)
	{
		uint8_t lineInd = motorPtr->hardwareInfo.canLine - 1;
		uint8_t controllerInd = motorPtr->hardwareInfo.controllerId - 1;
		uint8_t headerInd = CanTx::findHeaderIndById(motorPtr->motorType->canTxIdList[ controllerInd ]);
		uint8_t dataPos = motorPtr->motorType->canTxPosInd[ controllerInd ];
		int16_t controlData = motorPtr->outputIntensity * motorPtr->motorType->intensityDataRatio;
		CanTx::data[ lineInd ][ headerInd ][ dataPos ] = controlData >> 8;
		CanTx::data[ lineInd ][ headerInd ][ dataPos + 1 ] = controlData & 0xff;
		CanTx::frameActivated[ lineInd ][ headerInd ] = true;
	}
	CanTx::transmit();
}