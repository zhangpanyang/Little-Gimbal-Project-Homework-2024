//
// Created by Sunny on 24-11-11.
//

#include "remoteControlTask.h"
#include "motorDevice.h"

extern RemoteControl remoteControl;
extern Motor* motorList[2][8];

void remoteControlTaskInit()
{

}

void remoteControlTaskRoutine()
{
	if(remoteControl.switch_.r == DOWN_POS)
	{
		for (uint8_t canLine = 1; canLine <=2; canLine++)
			for (uint8_t motorId = 1; motorId <= 8; motorId++)
				if(motorList[canLine-1][motorId-1] != nullptr)
					motorList[canLine-1][motorId-1]->Stop();
	}
	else
	{
		for (uint8_t canLine = 1; canLine <=2; canLine++)
			for (uint8_t motorId = 1; motorId <= 8; motorId++)
				if(motorList[canLine-1][motorId-1] != nullptr)
					motorList[canLine-1][motorId-1]->Start();
	}
}