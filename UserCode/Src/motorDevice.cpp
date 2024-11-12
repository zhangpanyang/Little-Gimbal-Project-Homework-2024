//
// Created by Sunny on 24-11-9.
//

#include "motorDevice.h"

Motor* motorList[2][8] = {nullptr};

Motor::Motor(float pReductionRatio)
{
	hardwareInfo = {0, 0};
	reductionRatio = pReductionRatio;
	state = {0, 0, 0, 0};
	feedback = {0, 0, 0};
	lastFeedbackAngle = 0;
	outputIntensity = 0;
	stopFlag = 0;
}
MotorSpeed::MotorSpeed(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed): Motor(pReductionRatio)
{
	controlSpeed = {
		.targetValue = 0,
		.pid = *pPidSpeed,
		.feedForward = pFeedForwardSpeed,
	};
}
MotorAngle::MotorAngle(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed, PID* pPidAngle, float pFeedForwardAngle) : MotorSpeed(pReductionRatio, pPidSpeed, pFeedForwardSpeed)
{
	controlAngle = {
		.targetValue = 0,
		.pid = *pPidAngle,
		.feedForward = pFeedForwardAngle,
	};
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
	outputIntensity = 0;
}
void MotorSpeed::updateControl()
{
	if (stopFlag)
	{
		outputIntensity = 0;
		return;
	}
	outputIntensity = controlSpeed.pid.compute(controlSpeed.targetValue, state.speed, 0.001) + controlSpeed.feedForward;
}
void MotorAngle::updateControl()
{
	if (stopFlag)
	{
		outputIntensity = 0;
		return;
	}
	controlSpeed.targetValue = controlAngle.pid.compute(controlAngle.targetValue, state.angle, 0.001) + controlAngle.feedForward;
	outputIntensity = controlSpeed.pid.compute(controlSpeed.targetValue, state.speed, 0.001) + controlSpeed.feedForward;
}

void Motor::Stop()
{
	stopFlag = true;
}
void Motor::Start()
{
	stopFlag = false;
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