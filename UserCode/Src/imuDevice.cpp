//
// Created by Sunny on 24-11-2.
//

#include "imuDevice.h"
#include "pidTools.h"
#include "spi.h"
#include <cmath>

void BMI088TransmitByte(uint8_t txData)
{
	HAL_SPI_Transmit(&hspi1, &txData, 1, HAL_MAX_DELAY);
	// while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);
}

void BMI088ReceiveByte(uint8_t* rxData)
{
	HAL_SPI_Receive(&hspi1, rxData, 1, HAL_MAX_DELAY);
}

void BMI088ReceiveMultipleBytes(uint8_t* rxData, uint8_t length)
{
	HAL_SPI_Receive(&hspi1, rxData, length, HAL_MAX_DELAY);
}

void BMI088WriteSingleByte(GPIO_TypeDef* ssPort, uint8_t ssPin, uint8_t reg, uint8_t txData)
{
	HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_RESET);
	BMI088TransmitByte(reg | 0x00);
	BMI088TransmitByte(txData);
	HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_SET);
}

void BMI088ReadSingleByte(GPIO_TypeDef* ssPort, uint8_t ssPin, uint8_t reg, uint8_t* rxData)
{
	HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_RESET);
	BMI088TransmitByte(reg | 0x80);
	BMI088ReceiveByte(rxData);
	HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_SET);
}

void BMI088ReadMultipleByte(GPIO_TypeDef* ssPort, uint8_t ssPin, uint8_t reg, uint8_t* rxData, uint8_t length)
{
	HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_RESET);
	BMI088TransmitByte(reg | 0x80);
	BMI088ReceiveMultipleBytes(rxData, length);
	HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_SET);
}

float linearMapping(int32_t in, int32_t inMin, int32_t inMax, float outMin, float outMax)
{
	return (float)(in-inMin)*(outMax-outMin)/(float)(inMax-inMin)+outMin;
}

imuAccelTypedef imuAccel;
uint8_t accData[6+1];

void BMI088ReadAccel()
{
	BMI088ReadMultipleByte(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, ACC_DATA_START_REG, accData, 6+1);
	int16_t accelXInt16 = (int16_t)(accData[2] << 8 | accData[1]);
	int16_t accelYInt16 = (int16_t)(accData[4] << 8 | accData[3]);
	int16_t accelZInt16 = (int16_t)(accData[6] << 8 | accData[5]);
	imuAccel.accelX = linearMapping(accelXInt16, -32767, 32767, -12, 12);
	imuAccel.accelY = linearMapping(accelYInt16, -32767, 32767, -12, 12);
	imuAccel.accelZ = linearMapping(accelZInt16, -32767, 32767, -12, 12);
}

imuGyroTypedef imuGyro;
uint8_t rateData[6];

void BMI088ReadGyro()
{
	BMI088ReadMultipleByte(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, RATE_DATA_START_REG, rateData, 6);
	int16_t rateXInt16 = (int16_t)(rateData[1] << 8 | rateData[0]);
	int16_t rateYInt16 = (int16_t)(rateData[3] << 8 | rateData[2]);
	int16_t rateZInt16 = (int16_t)(rateData[5] << 8 | rateData[4]);
	imuGyro.rateX = linearMapping(rateXInt16, -32767, 32767, -2000, 2000);
	imuGyro.rateY = linearMapping(rateYInt16, -32767, 32767, -2000, 2000);
	imuGyro.rateZ = linearMapping(rateZInt16, -32767, 32767, -2000, 2000);
}

uint8_t chipID;
void BMI088Init()
{
	BMI088ReadMultipleByte(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, 0x00, &chipID, 1);
	BMI088WriteSingleByte(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, ACC_RANGE_REG, ACC_RANGE_12G);
}

attitudeTypedef attitude;
attitudeTypedef rateAtt;
attitudeTypedef lastRateAtt;

const float imuTimePeriod = 0.001f;

PIDInitializer imuPidInitializer
{
	.Kp = 0.0f,
	.Ki = 0.0f,
	.Kd = 0.0f,
	.outputMax = 0.1f,
	.pMax = 0.1f,
	.integralMax = 0.0f
};
PID pidImuRoll(&imuPidInitializer);
PID pidImuPitch(&imuPidInitializer);

void ImuRoutine()
{
	BMI088ReadAccel();
	BMI088ReadGyro();

	// Gyro Solve
	float sr = sinf(attitude.roll), cr = cosf(attitude.roll);
	float sp = sinf(attitude.pitch), cp = cosf(attitude.pitch);
	float sy = sinf(attitude.yaw), cy = cosf(attitude.yaw);
	rateAtt.roll = imuGyro.rateX + imuGyro.rateY * sp*sr/cp + imuGyro.rateZ * cr*sp/cp;
	rateAtt.pitch = imuGyro.rateY * cr - imuGyro.rateZ * sr;
	rateAtt.yaw = imuGyro.rateY * sr/cp + imuGyro.rateZ * cr/cp;

	// Accel Solve
	float roll = atan2f(imuAccel.accelZ, imuAccel.accelY);
	float pitch = -atan2f(imuAccel.accelX, sqrtf(imuAccel.accelY*imuAccel.accelY + imuAccel.accelZ*imuAccel.accelZ));

	// update values
	attitude.roll += (lastRateAtt.roll * 0.5f + rateAtt.roll * 0.5f) * imuTimePeriod;
	attitude.roll += pidImuRoll.compute(roll, attitude.roll, imuTimePeriod);

	attitude.pitch += (lastRateAtt.roll * 0.5f + rateAtt.roll * 0.5f) * imuTimePeriod;
	attitude.pitch += pidImuPitch.compute(pitch, attitude.pitch, imuTimePeriod);

	lastRateAtt = rateAtt;
}