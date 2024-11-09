//
// Created by Sunny on 24-11-9.
//

#include "pidTools.h"

PID::PID(PIDInitializer* initializer)
{
	Kp = initializer->Kp;
	Ki = initializer->Ki;
	Kd = initializer->Kd;
	outputMax = initializer->outputMax;
	pMax = initializer->pMax;
	integralMax = initializer->integralMax;
	prevError = 0;
	integral = 0;
}

float PID::compute(float controlValue, float actualValue, float deltaTime)
{
	float error = controlValue - actualValue;
	integral += error;
	INRANGE(integral, -integralMax, integralMax);

	float compP = Kp * error;
	float compI = Ki * integral;
	float compD = Kd * (error - prevError) / deltaTime;
	INRANGE(compP, -pMax ,pMax);
	float output = compP + compI + compD;
	INRANGE(output, -outputMax, outputMax);

	prevError = error;
	return output;
}