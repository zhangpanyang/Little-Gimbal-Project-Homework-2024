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

void gimbalTaskInit()
{

}