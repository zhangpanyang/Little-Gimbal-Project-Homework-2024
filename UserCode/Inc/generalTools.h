//
// Created by Sunny on 24-11-9.
//

#ifndef GENERALTOOLS_H
#define GENERALTOOLS_H

#include "main.h"

#define INRANGE(NUM, MIN, MAX)\
{\
if(NUM<MIN){\
NUM=MIN;\
}else if(NUM>MAX){\
NUM=MAX;\
}\
}

inline float linearMappingInt2Float(int in,int in_min,int in_max,float out_min,float out_max)
{
	return (out_max - out_min) * (float)(in - in_min) / (float)(in_max - in_min) + out_min;
}
inline int32_t linearMappingFloat2Int(float in,float in_min,float in_max,int out_min,int out_max)
{
	return (out_max - out_min) * (in - in_min) / (in_max - in_min) + out_min;
}

inline bool isInRange(float in,float in_min,float in_max)
{
	return in>=in_min && in<=in_max;
}
inline float clampInRange(float in, float in_min, float in_max)
{
	if(in<in_min) return in_min;
	if(in>in_max) return in_max;
	return in;
}

#endif //GENERALTOOLS_H
