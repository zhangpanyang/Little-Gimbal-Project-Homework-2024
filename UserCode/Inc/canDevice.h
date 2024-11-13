//
// Created by Sunny on 24-11-9.
//

#ifndef CANDEVICE_H
#define CANDEVICE_H

#include "can.h"
#include "generalTools.h"

namespace CanRx
{
	void handle(CAN_HandleTypeDef* hcan);
}
void canDeviceInit();
void canDeviceRoutine();

#endif //CANDEVICE_H
