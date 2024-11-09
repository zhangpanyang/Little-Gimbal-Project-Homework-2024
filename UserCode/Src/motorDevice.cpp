//
// Created by Sunny on 24-11-9.
//

#include "motorDevice.h"

Motor* motorList[2][8];

Motor::Motor(motorInit_t* motorInit) :
	pidSpeed(*(motorInit->pidAngle)),
	pidAngle(*(motorInit->pidSpeed)),
	feedback(),
	state(),
	reductionRatio(motorInit->reductionRatio),
	targetAngle(0),
	targetSpeed(0),
	lastFeedbackAngle(0)
{}

void Motor::controllerRxHandle(uint8_t* data)
{
	feedback.angle = data[0]<<8 | data[1];
	feedback.speed = data[2]<<8 | data[3];
	// feedback.moment = data[4]<<8 | data[5];
	feedback.temperature = data[6];
}

void Motor::updateState()
{
	state.speed = feedback.speed / reductionRatio;
	state.temperature = feedback.temperature;

	int32_t thisAngle = feedback.angle;
	if (thisAngle <= lastFeedbackAngle) {
		if (lastFeedbackAngle - thisAngle > 4096)
			state.angleInt += (thisAngle + 8192 - lastFeedbackAngle);
		else
			state.angleInt -= (lastFeedbackAngle - thisAngle);
	}
	else {
		if (thisAngle - lastFeedbackAngle > 4096)
			state.angleInt -= (lastFeedbackAngle + 8192 - thisAngle);
		else
			state.angleInt += (thisAngle - lastFeedbackAngle);
	}
	lastFeedbackAngle = thisAngle;
	state.angle = state.angleInt * 0.0439453125f / reductionRatio;// * 360.0f / 8192.0f
}

void motorDeviceInit()
{
	for(uint8_t canLine = 1; canLine <= 2; canLine++)
		for(uint8_t motorId = 1; motorId <= 8; motorId++)
			motorList[canLine-1][motorId-1] = nullptr;
}

void motorDeviceRoutine()
{
	for(uint8_t canLine = 1; canLine <= 2; canLine++)
	{
		for(uint8_t motorId = 1; motorId <= 8; motorId++)
		{
			if(motorList[canLine-1][motorId-1] != nullptr)
				motorList[canLine-1][motorId-1]->updateState();
		}
	}
}