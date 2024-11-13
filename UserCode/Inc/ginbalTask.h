//
// Created by Sunny on 24-11-13.
//

#ifndef GINBALTASK_H
#define GINBALTASK_H

#include "motorDevice.h"

class MotorAngleLimited : public MotorAngle
{
public:
	float angleMin;
	float angleMax;

	MotorAngleLimited(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed, PID* pPidAngle, float pFeedForwardAngle, float pAngleMin, float pAngleMax);
	void setAngle(float angle) override;
	void addToAngle(float deltaAngle);
};

#endif //GINBALTASK_H
