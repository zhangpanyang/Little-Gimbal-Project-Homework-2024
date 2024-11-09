//
// Created by Sunny on 24-11-9.
//

#include "generalTools.h"

float linearMappingInt2Float(int in, int in_min, int in_max, float out_min, float out_max)
{
	return (out_max - out_min) * (float)(in - in_min) / (float)(in_max - in_min) + out_min;
}

int linearMappingFloat2Int(float in, float in_min, float in_max, int out_min, int out_max)
{
	return (out_max - out_min) * (in - in_min) / (in_max - in_min) + out_min;
}