//
// Created by Sunny on 24-11-9.
//

#ifndef CPPMAIN_H
#define CPPMAIN_H

#ifdef __cplusplus
extern "C"{
#endif

	#include "main.h"

	void cppMain();
	void mainDeviceInit();
	void mainDeviceRoutine();
	void mainTaskInit();
	void mainTaskRoutine();

	extern uint32_t mainTick;

#ifdef __cplusplus
}
#endif

#endif //CPPMAIN_H
