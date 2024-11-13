//
// Created by Sunny on 24-11-13.
//

#include "ginbalTask.h"

MotorAngleLimited::MotorAngleLimited(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed, PID* pPidAngle, float pFeedForwardAngle, float pAngleMin, float pAngleMax):
	MotorAngle(pReductionRatio, pPidSpeed, pFeedForwardSpeed, pPidAngle, pFeedForwardAngle)
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

