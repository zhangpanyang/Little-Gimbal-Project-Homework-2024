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
	int16_t temp;
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
	// float moment;
	float temperature;
} motorState_t;

typedef struct
{
	float reductionRatio;
	PID* pidSpeed;
	PID* pidAngle;
} motorInit_t;

class Motor
{
public:
	float reductionRatio;
	float targetSpeed;
	float targetAngle;

	PID pidSpeed;
	PID pidAngle;
	controllerRx_t feedback;
	motorState_t state;

	explicit Motor(motorInit_t* motorInit);
	void controllerRxHandle(uint8_t* data);
	void updateState();

private:
	uint32_t lastAngleInt;
};

#endif //MOTORDEVICE_H
