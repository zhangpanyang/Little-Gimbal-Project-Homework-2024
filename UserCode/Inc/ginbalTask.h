//
// Created by Sunny on 24-11-13.
//

#ifndef GINBALTASK_H
#define GINBALTASK_H

#include "motorDevice.h"
#include "imuDevice.h"
#include "cppMain.h"

class MotorAnglePitch : public MotorAngle
{
public:
	float angleMin;
	float angleMax;

	using FeedForwardPtr = float (*) (float);
	FeedForwardPtr feedForwardAngle;

	MotorAnglePitch(motorType_t* pMotorType, PID* pPidSpeed, PID* pPidAngle, FeedForwardPtr pFeedForwardAngle, float pAngleMin, float pAngleMax);
	void updateControl() override;
	void setAngle(float angle) override;
	void addToAngle(float deltaAngle);
};

void gimbalTaskInit();
void gimbalTaskRoutine();

#endif //GINBALTASK_H
