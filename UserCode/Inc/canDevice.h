//
// Created by Sunny on 24-11-9.
//

#ifndef CANDEVICE_H
#define CANDEVICE_H

#include "can.h"
#include "generalTools.h"

void canDeviceInit();
void canControllerRxHandle(CAN_HandleTypeDef* hcan);
void canDeviceRoutine();

#endif //CANDEVICE_H
