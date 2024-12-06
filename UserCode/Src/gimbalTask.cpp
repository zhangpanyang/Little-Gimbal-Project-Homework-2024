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
	// return 4.2941e-04f*in*in*in -0.0919f*in*in -36.0257f*in -1.2199e+03f;
	return 0.0027f*in*in*in +0.1765f*in*in -25.8155f*in -1.0886e+03f;
	// return -0.0846f*in*in -32.9132f*in -1.1370e+03f;
}

// namespace Gimbal
// {
	PID pidPitchSpeed(new PIDInitializer{150, 0, 0, 25000, 25000, 25000});
	PID pidPitchAngle(new PIDInitializer{12, 0.03, 0, 25000, 25000, 1000});

	MotorAnglePitch motorPitch(&gm6020_v, &pidPitchSpeed, &pidPitchAngle, pitchFeedForward, -62.5, 0);
// }

PID pidYawSpeed(new PIDInitializer{150, 0, 0, 25000, 25000, 25000});
PID pidYawAngle(new PIDInitializer{7, 0.01, 0, 25000, 25000, 1000});

MotorAngle motorYaw(&gm6020_v, &pidYawSpeed, &pidYawAngle);
float debugAngleYaw = 0;

void gimbalTaskInit()
{
	motorSet.Append(&motorPitch, 1, 5);
	motorSet.Append(&motorYaw, 1, 7);
}
void gimbalTaskRoutine()
{
	/** Debug Use **/
	debugAngleYaw = motorYaw.state.angle;
	/** **/

	ImuRoutine();

	if(mainTick < 200)
	{
		for(auto motorPtr : motorSet)
			motorPtr->Stop();
	}
	else if(mainTick == 200)
	{
		motorPitch.zeroSet();
		motorYaw.zeroSet();
		for(auto motorPtr : motorSet)
			motorPtr->Start();
	}
}