//
// Created by Sunny on 24-11-11.
//

#include "remoteControlTask.h"

#include "ginbalTask.h"
#include "motorDevice.h"

extern RemoteControl remoteControl;

extern MotorAnglePitch motorPitch;

void remoteControlTaskInit()
{

}

void remoteControlTaskRoutine()
{
	// if(remoteControl.switch_.r == DOWN_POS)
	// {
	// 	for(auto motorPtr : motorSet)
	// 		motorPtr->Stop();
	// }
	// else
	// {
	// 	if(remoteControl.switch_.l == UP_POS && remoteControl.switch_.r == UP_POS)
	// 	{
	// 		for(auto motorPtr : motorSet)
	// 			motorPtr->Start();
	// 		motorPitch.addToAngle(remoteControl.channel_.l_col*(-0.05f));
	// 	}
	// }
}