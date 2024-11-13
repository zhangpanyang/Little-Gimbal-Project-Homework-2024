//
// Created by Sunny on 24-11-9.
//

#ifndef MOTORDEVICE_H
#define MOTORDEVICE_H

#include "pidTools.h"
#include "main.h"

typedef struct
{
	uint8_t canLine;
	uint8_t controllerId;
} motorHardwareInfo_t;

typedef struct
{
	int16_t angle;
	int16_t speed;
	// int16_t moment;
	int16_t temperature;
	int16_t lastFeedbackAngle;
} controllerRx_t;

typedef struct
{
	float speed; //dps
	int32_t angleInt; //8192
	float angle; //degree
	float temperature;
} motorState_t;

typedef struct
{
	float targetValue;
	PID pid;
	float feedForward;
	float compute(float actualValue, float deltaTime)
	{
		return pid.compute(targetValue, actualValue, deltaTime) + feedForward;
	}
} motorControlUnit_t;

class Motor
{
public:
	float reductionRatio;
	motorHardwareInfo_t hardwareInfo{};
	controllerRx_t feedback{};
	motorState_t state{};
	float outputIntensity;
	uint8_t stopFlag;

	virtual ~Motor() = default;
	explicit Motor(float pReductionRatio);
	void setHardwareInfo(uint8_t pCanLine, uint8_t pControllerId);
	void controllerRxHandle(uint8_t* data);
	void updateState();
	virtual void updateControl();
	void Stop();
	void Start();
};

class MotorSpeed : public Motor
{
public:
	motorControlUnit_t controlSpeed;
	MotorSpeed(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed);
	void updateControl() override;
	virtual void setSpeed(float speed);
};

class  MotorAngle : public MotorSpeed
{
public:
	motorControlUnit_t controlAngle;
	MotorAngle(float pReductionRatio, PID* pPidSpeed, float pFeedForwardSpeed, PID* pPidAngle, float pFeedForwardAngle);
	void updateControl() override;
	virtual void setAngle(float angle);
private:
	using MotorSpeed::setSpeed;
};

class MotorSet
{
public:

	MotorSet();
	void Append(Motor* motorPtr, uint8_t canLine, uint8_t controllerId);
	Motor* getMotorById(uint8_t canLine, uint8_t controllerId);

	class Iterator
	{
	public:
		explicit Iterator(Motor** pPtr);
		Motor*& operator * () const;
		Iterator& operator ++ ();
		bool operator == (const Iterator& other) const;
		bool operator != (const Iterator& other) const;
	private:
		Motor** ptr;
	};
	Iterator begin();
	Iterator end();

protected:
	Motor* motorMap[2][8] = {nullptr};
	Motor* motorList[16] = {nullptr};
	uint8_t size;
};

void motorDeviceInit();
void motorDeviceRoutine();

#endif //MOTORDEVICE_H
