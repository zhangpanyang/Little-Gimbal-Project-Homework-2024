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
	float targetAngle;
	float targetSpeed;

	PID pidAngle;
	PID pidSpeed;
	float feedForward;

	float outputIntensity;
	uint8_t stopFlag;
} motorControl_t;

typedef struct
{
	uint8_t canLine;
	uint8_t controllerId;

	float reductionRatio;
	PID* pidSpeed;
	PID* pidAngle;
	float feedForward;
} motorInit_t;

class Motor
{
public:
	float reductionRatio;
	motorHardwareInfo_t hardwareInfo;
	controllerRx_t feedback;
	motorState_t state;
	motorControl_t control;

	explicit Motor(motorInit_t* motorInit);
	void controllerRxHandle(uint8_t* data);
	void updateState();
	void updateControl();

private:
	int16_t lastFeedbackAngle;
};
void motorDeviceInit();
void motorDeviceRoutine();

#endif //MOTORDEVICE_H
