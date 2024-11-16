//
// Created by Sunny on 24-11-13.
//

#include "ginbalTask.h"

MotorAnglePitch::MotorAnglePitch(motorType_t* pMotorType, PID* pPidSpeed, PID* pPidAngle, FeedForwardPtr pFeedForwardAngle, float pAngleMin, float pAngleMax):
	MotorAngle(pMotorType, pPidSpeed, pPidAngle)
{
	angleMin = pAngleMin;
	angleMax = pAngleMax;
	feedForwardAngle = pFeedForwardAngle;
}
void MotorAnglePitch::updateControl()
{
	if (stopFlag)
	{
		outputIntensity = 0;
		return;
	}
	controlSpeed.targetValue = controlAngle.compute(state.angle, 0.001);
	outputIntensity = controlSpeed.compute(state.speed, 0.001) + feedForwardAngle(state.angle);
}

void MotorAnglePitch::setAngle(float angle)
{
	if (isInRange(angle, angleMin, angleMax))
		controlAngle.targetValue = angle;
}
void MotorAnglePitch::addToAngle(float deltaAngle)
{
	controlAngle.targetValue = clampInRange(controlAngle.targetValue + deltaAngle, angleMin, angleMax);
}

float emptyFeedForward(float in)
{
	return 0;
}

float pitchFeedForward(float in)
{
	return -0.0084f*in*in*in -0.8771f*in*in -53.2058f*in -1.4395e+03f;
}

// namespace Gimbal
// {
	PID pidPitchSpeed(new PIDInitializer{150, 0, 0, 25000, 25000, 25000});
	PID pidPitchAngle(new PIDInitializer{15, 0.03, 0, 25000, 25000, 25000});

	MotorAnglePitch motorPitch(&gm6020_v, &pidPitchSpeed, &pidPitchAngle, pitchFeedForward, -62.5, 0);
// }

void gimbalTaskInit()
{
	motorSet.Append(&motorPitch, 1, 5);
}
void gimbalTaskRoutine()
{
	if(mainTick < 200)
	{
		for(auto motorPtr : motorSet)
			motorPtr->Stop();
	}
	else if(mainTick == 200)
	{
		motorPitch.zeroSet();
		for(auto motorPtr : motorSet)
			motorPtr->Start();
	}
}