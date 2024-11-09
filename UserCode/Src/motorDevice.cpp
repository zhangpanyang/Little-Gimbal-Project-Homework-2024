//
// Created by Sunny on 24-11-9.
//

#include "motorDevice.h"

Motor* motorList[2][8];

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
	feedback.angle = data[0]<<8 | data[1];
	feedback.speed = data[2]<<8 | data[3];
	feedback.moment = data[4]<<8 | data[5];
	feedback.temp = data[6];
}

void Motor::updateState()
{

}

void motorRoutine()
{
	for(uint8_t canLine = 1; canLine <= 2; canLine++)
	{
		for(uint8_t motorId = 1; motorId <= 8; motorId++)
		{
			if(motorList[canLine-1][motorId-1] != nullptr)
				motorList[canLine-1][motorId-1]->updateState();
		}
	}
}