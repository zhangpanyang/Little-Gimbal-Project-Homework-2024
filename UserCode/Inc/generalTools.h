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

float linearMappingInt2Float(int in,int in_min,int in_max,float out_min,float out_max);
int32_t linearMappingFloat2Int(float in,float in_min,float in_max,int out_min,int out_max);


#endif //GENERALTOOLS_H
