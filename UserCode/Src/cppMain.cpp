//
// Created by Sunny on 24-11-9.
//

#include "cppMain.h"

#include "canDevice.h"
#include "remoteControlDevice.h"
#include "motorDevice.h"
#include "tim.h"

void mainDeviceInit()
{
	remoteControlDeviceInit();
	canDeviceInit();
	motorDeviceInit();
	HAL_TIM_Base_Start_IT(&htim6);
}

void mainDeviceRoutine()
{
	motorDeviceRoutine();
	canDeviceRoutine();
}

void cppMain()
{
	mainDeviceInit();
	while (1)
	{

	}
}