//
// Created by Sunny on 24-11-9.
//

#ifndef PIDTOOLS_H
#define PIDTOOLS_H

#include "generalTools.h"

struct PIDInitializer
{
	float Kp;  // 比例增益
	float Ki;  // 积分增益
	float Kd;  // 微分增益
	float outputMax;
	float pMax;
	float integralMax;
};

class PID
{
public:
	float Kp;  // 比例增益
	float Ki;  // 积分增益
	float Kd;  // 微分增益
	float outputMax;
	float pMax;
	float integralMax;
	float prevError;  // 前一次误差
	float integral;   // 积分项累积

	PID();
	explicit PID(PIDInitializer* initializer);
	float compute(float controlValue, float actualValue, float deltaTime);

private:
};

#endif //PIDTOOLS_H
