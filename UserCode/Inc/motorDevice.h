//
// Created by Sunny on 24-11-9.
//

#ifndef MOTORDEVICE_H
#define MOTORDEVICE_H

#include "pidTools.h"
#include "main.h"

typedef struct
{
	int16_t angle;
	int16_t speed;
	// int16_t moment;
	int16_t temperature;
} controllerRx_t;

typedef struct
{
	uint8_t canLine;
	uint8_t controllerId;
} motorHardwareInfo_t;

typedef struct
{
	float speed; //dps
	int32_t angleInt; //8192
	float angle; //degree
	float temperature;
} motorState_t;

typedef struct
{
	float targetValue;
	PID pid;
	float feedForward;
} motorControlUnit_t;

class Motor
{
public:
	float reductionRatio;
	motorHardwareInfo_t hardwareInfo{};
	controllerRx_t feedback{};
	motorState_t state{};
	float outputIntensity;
	uint8_t stopFlag;

	virtual ~Motor() = default;
	explicit Motor(float pReductionRatio);
	void controllerRxHandle(uint8_t* data);
	void updateState();
	virtual void updateControl();
	void Stop();
	void Start();

protected:
	int16_t lastFeedbackAngle;
};

class MotorSpeed : public Motor
{
public:
	motorControlUnit_t controlSpeed;
	MotorSpeed(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed);
	void updateControl() override;
};

class  MotorAngle : public MotorSpeed
{
public:
	motorControlUnit_t controlAngle;
	MotorAngle(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed, PID* pPidAngle, float pFeedForwardAngle);
	void updateControl() override;
};

void motorDeviceInit();
void motorDeviceRoutine();

#endif //MOTORDEVICE_H
