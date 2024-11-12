//
// Created by Sunny on 24-11-11.
//

#include "remoteControlTask.h"
#include "motorDevice.h"

extern RemoteControl remoteControl;
extern MotorSet motorSet;

void remoteControlTaskInit()
{

}

void remoteControlTaskRoutine()
{
	if(remoteControl.switch_.r == DOWN_POS)
	{
		for(auto motorPtr : motorSet)
			motorPtr->Stop();
	}
	else
	{
		for(auto motorPtr : motorSet)
			motorPtr->Start();
	}
}