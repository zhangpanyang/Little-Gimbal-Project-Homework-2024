//
// Created by Sunny on 24-11-9.
//

#ifndef REMOTECONTROLDEVICE_H
#define REMOTECONTROLDEVICE_H

#include "main.h"
#include "generalTools.h"
#include <usart.h>
#include <cstring>

const int RC_RX_BUF_SIZE = 18;
const int RC_RX_DATA_SIZE = 18;

typedef enum{UP_POS, DOWN_POS, MID_POS}  RCSwitchState_e;
const RCSwitchState_e RCSwitchStates[] = {UP_POS, DOWN_POS, MID_POS};

typedef enum{MOUSE_UP, MOUSE_DOWN}  RCMouseClickState_e;
const RCMouseClickState_e RCMouseClickStates[] = {MOUSE_UP, MOUSE_DOWN};

struct RCChannel{
	float r_row;
	float r_col;
	float l_row;
	float l_col;
};

struct RCSwitch {
	RCSwitchState_e l;
	RCSwitchState_e r;
};

struct RCMouse {
	float x;
	float y;
	float z;

	RCMouseClickState_e l;
	RCMouseClickState_e r;
};

class RemoteControl{
private:

public:

	RCChannel  channel_;
	RCSwitch switch_;
	RCMouse mouse_;
	uint8_t keys_;

	void init();
	void frameHandle();
};

enum {RC_KEY_W, RC_KEY_S, RC_KEY_A, RC_KEY_D, RC_KEY_Q, RC_KEY_E, RC_KEY_SHIFT, RC_KEY_CTRL};

void remoteControlDeviceInit();

#endif //REMOTECONTROLDEVICE_H
