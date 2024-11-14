//
// Created by Sunny on 24-11-9.
//

#include "cppMain.h"

#include "canDevice.h"
#include "ginbalTask.h"
#include "remoteControlDevice.h"
#include "remoteControlTask.h"
#include "motorDevice.h"
#include "tim.h"

void mainDeviceInit()
{
	remoteControlDeviceInit();
	canDeviceInit();
	motorDeviceInit();
}

void mainTaskInit()
{
	remoteControlTaskInit();
	gimbalTaskInit();
}

void mainDeviceRoutine()
{
	motorDeviceRoutine();
	canDeviceRoutine();
}

void mainTaskRoutine()
{
	remoteControlTaskRoutine();
}

void cppMain()
{
	mainDeviceInit();
	mainTaskInit();
	HAL_TIM_Base_Start_IT(&htim6);
	while (1)
	{

	}
}