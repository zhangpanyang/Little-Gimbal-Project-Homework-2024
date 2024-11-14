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

	MotorAngleLimited(motorType_t* pMotorType, PID* pPidSpeed, FeedBackPtr pFeedForwardSpeed, PID* pPidAngle, FeedBackPtr pFeedForwardAngle, float pAngleMin, float pAngleMax);
	void setAngle(float angle) override;
	void addToAngle(float deltaAngle);
};

void gimbalTaskInit();

#endif //GINBALTASK_H
