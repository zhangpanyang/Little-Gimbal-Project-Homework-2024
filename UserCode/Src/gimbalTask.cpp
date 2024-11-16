//
// Created by Sunny on 24-11-13.
//

#include "ginbalTask.h"

MotorAngleLimited::MotorAngleLimited(motorType_t* pMotorType, PID* pPidSpeed, FeedForwardPtr pFeedForwardSpeed, PID* pPidAngle, FeedForwardPtr pFeedForwardAngle, float pAngleMin, float pAngleMax):
	MotorAngle(pMotorType, pPidSpeed, pFeedForwardSpeed, pPidAngle, pFeedForwardAngle)
{
	angleMin = pAngleMin;
	angleMax = pAngleMax;
}
void MotorAngleLimited::setAngle(float angle)
{
	if (isInRange(angle, angleMin, angleMax))
		controlAngle.targetValue = angle;
}
void MotorAngleLimited::addToAngle(float deltaAngle)
{
	controlAngle.targetValue = clampInRange(controlAngle.targetValue + deltaAngle, angleMin, angleMax);
}

float emptyFeedForward(float in)
{
	return 0;
}

// namespace Gimbal
// {
	PID pidPitchSpeed(new PIDInitializer{150, 0, 0, 25000, 25000, 25000});
	PID pidPitchAngle(new PIDInitializer{0, 0, 0, 25000, 25000, 25000});

	MotorAngleLimited motorPitch(&gm6020_v, &pidPitchSpeed, emptyFeedForward, &pidPitchAngle, emptyFeedForward, -360, 360);
// }

void gimbalTaskInit()
{
	motorSet.Append(&motorPitch, 1, 5);
}