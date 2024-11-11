//
// Created by Sunny on 24-11-9.
//

#include "motorDevice.h"

Motor* motorList[2][8] = {nullptr};

Motor::Motor(motorInit_t* motorInit)
{
	hardwareInfo = {
		.canLine = motorInit->canLine,
		.controllerId = motorInit->controllerId
	};
	reductionRatio = motorInit->reductionRatio;
	control = {
		.targetAngle = 0,
		.targetSpeed = 0,
		.pidAngle = *motorInit->pidAngle,
		.pidSpeed = *motorInit->pidSpeed,
		.feedForward = motorInit->feedForward,
		.outputIntensity = 0,
		.stopFlag = 0
	};
	state = {0, 0, 0, 0};
	feedback = {0, 0, 0};
	lastFeedbackAngle = 0;
}

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

	int16_t thisAngle = feedback.angle;
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

void Motor::updateControl()
{
	if (control.stopFlag)
	{
		control.outputIntensity = 0;
		return;
	}
	control.targetSpeed = control.pidAngle.compute(control.targetAngle, state.angle, 0.001);
	control.outputIntensity = control.pidSpeed.compute(control.targetSpeed, state.speed, 0.001) + control.feedForward;
}

void Motor::setTargetAngle(float targetAngle)
{
	control.targetAngle = targetAngle;
}
void Motor::Stop()
{
	control.stopFlag = true;
}
void Motor::Start()
{
	control.stopFlag = false;
}




void motorDeviceInit()
{
}

void motorDeviceRoutine()
{
	for(uint8_t canLine = 1; canLine <= 2; canLine++)
	{
		for(uint8_t motorId = 1; motorId <= 8; motorId++)
		{
			if(motorList[canLine-1][motorId-1] == nullptr) continue;
			motorList[canLine-1][motorId-1]->updateState();
			motorList[canLine-1][motorId-1]->updateControl();
		}
	}
}