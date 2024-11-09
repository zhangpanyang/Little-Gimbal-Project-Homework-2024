//
// Created by Sunny on 24-11-9.
//

#include "motorDevice.h"

Motor::Motor(motorInit_t* motorInit) :
pidSpeed(*(motorInit->pidAngle)),
pidAngle(*(motorInit->pidSpeed)),
feedback(),
state()
{
	reductionRatio = motorInit->reductionAngle;
	targetAngle = 0;
	targetSpeed = 0;
	lastAngleInt = 0;
}

void Motor::controllerRxHandle(uint8_t* data)
{

}