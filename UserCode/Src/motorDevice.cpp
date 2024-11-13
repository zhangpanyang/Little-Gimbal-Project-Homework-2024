//
// Created by Sunny on 24-11-9.
//

#include "motorDevice.h"

Motor::Motor(float pReductionRatio)
{
	reductionRatio = pReductionRatio;
	outputIntensity = 0;
	stopFlag = true;
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

void Motor::setHardwareInfo(uint8_t pCanLine, uint8_t pControllerId)
{
	hardwareInfo.canLine = pCanLine;
	hardwareInfo.controllerId = pControllerId;
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

	int16_t thisAngle = feedback.angle, lastAngle = feedback.lastFeedbackAngle;
	if (thisAngle <= lastAngle) {
		if (lastAngle - thisAngle > 4096)
			state.angleInt += (thisAngle + 8192 - lastAngle);
		else
			state.angleInt -= (lastAngle - thisAngle);
	}
	else {
		if (thisAngle - lastAngle > 4096)
			state.angleInt -= (lastAngle + 8192 - thisAngle);
		else
			state.angleInt += (thisAngle - lastAngle);
	}
	feedback.lastFeedbackAngle = thisAngle;
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
	outputIntensity = controlSpeed.compute(state.speed, 0.001);
}
void MotorAngle::updateControl()
{
	if (stopFlag)
	{
		outputIntensity = 0;
		return;
	}
	controlSpeed.targetValue = controlAngle.compute(state.angle, 0.001);
	outputIntensity = controlSpeed.compute(state.speed, 0.001);
}
void MotorSpeed::setSpeed(float speed)
{
	controlSpeed.targetValue = speed;
}
void MotorAngle::setAngle(float angle)
{
	controlAngle.targetValue = angle;
}

void Motor::Stop()
{
	stopFlag = true;
}
void Motor::Start()
{
	stopFlag = false;
}

MotorSet::MotorSet() : size(0){}
MotorSet::Iterator::Iterator(Motor** pPtr) : ptr(pPtr){}
Motor*& MotorSet::Iterator::operator * () const
{
	return *ptr;
}
MotorSet::Iterator& MotorSet::Iterator::operator ++ ()
{
	++ptr;
	return *this;
}
bool MotorSet::Iterator::operator == (const Iterator& other) const
{
	return ptr == other.ptr;
}
bool MotorSet::Iterator::operator != (const Iterator& other) const
{
	return ptr != other.ptr;
}
MotorSet::Iterator MotorSet::begin()
{
	return Iterator(motorList);
}
MotorSet::Iterator MotorSet::end()
{
	return Iterator(motorList+size);
}

void MotorSet::Append(Motor* motorPtr, uint8_t canLine, uint8_t controllerId)
{
	motorPtr->setHardwareInfo(canLine, controllerId);
	motorMap[canLine-1][controllerId-1] = motorPtr;
	motorList[size++] = motorPtr;
}
Motor* MotorSet::getMotorById(uint8_t canLine, uint8_t controllerId)
{
	return motorMap[canLine-1][controllerId-1];
}

MotorSet motorSet;

void motorDeviceInit()
{
}

void motorDeviceRoutine()
{
	for(auto motorPtr : motorSet)
	{
		motorPtr->updateState();
		motorPtr->updateControl();
	}
}