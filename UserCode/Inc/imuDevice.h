//
// Created by Sunny on 24-11-2.
//

#ifndef IMUDEVICE_H
#define IMUDEVICE_H

#include "main.h"

void BMI088TransmitByte(uint8_t txData);

void BMI088ReceiveByte(uint8_t* rxData);

void BMI088ReceiveMultipleBytes(uint8_t* rxData, uint8_t length);

void BMI088WriteSingleByte(GPIO_TypeDef* ssPort, uint8_t ssPin, uint8_t reg, uint8_t txData);

void BMI088ReadSingleByte(GPIO_TypeDef* ssPort, uint8_t ssPin, uint8_t reg, uint8_t &rxData);

void BMI088ReadMultipleByte(GPIO_TypeDef* ssPort, uint8_t ssPin, uint8_t reg, uint8_t* rxData, uint8_t length);

float linearMapping(int32_t in, int32_t inMin, int32_t inMax, float outMin, float outMax);

void BMI088ReadAccel();

void BMI088ReadGyro();

void BMI088Init();

struct imuAccelTypedef
{
	float accelX;
	float accelY;
	float accelZ;
};

struct imuGyroTypedef
{
	float rateX;
	float rateY;
	float rateZ;
};

extern imuAccelTypedef imuAccel;
extern imuGyroTypedef imuGyro;

#define ACC_DATA_START_REG 0x12
#define RATE_DATA_START_REG 0x02
#define ACC_RANGE_REG 0x41

#define ACC_RANGE_3G 0x00
#define ACC_RANGE_6G 0x01
#define ACC_RANGE_12G 0x02
#define ACC_RANGE_24G 0x03

#endif //IMUDEVICE_H
