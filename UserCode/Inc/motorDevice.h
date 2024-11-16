//
// Created by Sunny on 24-11-9.
//

#ifndef MOTORDEVICE_H
#define MOTORDEVICE_H

#include "pidTools.h"
#include "main.h"

struct motorType_t
{
	float reductionRatio;
	float intensityLimit;
	float intensityDataRatio;

	uint8_t canTxPosInd[12];
	uint16_t canTxIdList[12];
	// uint16_t canRxIdList[12];
};

struct motorHardwareInfo_t
{
	uint8_t canLine;
	uint8_t controllerId;
};

struct controllerRx_t
{
	int16_t angle;
	int16_t speed;
	int16_t moment;
	int16_t temperature;
	int16_t lastFeedbackAngle;
};

struct motorState_t
{
	float speed; //dps
	int32_t angleInt; //8192
	float angle; //degree
	float temperature;
};

struct motorControlUnit_t
{
	float targetValue;
	PID pid;
	float compute(float actualValue, float deltaTime)
	{
		return pid.compute(targetValue, actualValue, deltaTime);
	}
};

class Motor
{
public:
	motorType_t* motorType;
	motorHardwareInfo_t hardwareInfo{};
	controllerRx_t feedback{};
	motorState_t state{};
	float outputIntensity;
	uint8_t stopFlag;

	virtual ~Motor() = default;
	explicit Motor(motorType_t* pMotorType);
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
	MotorSpeed(motorType_t* pMotorType, PID* pPidSpeed);
	void updateControl() override;
	virtual void setSpeed(float speed);
};

class  MotorAngle : public MotorSpeed
{
public:
	motorControlUnit_t controlAngle;
	MotorAngle(motorType_t* pMotorType, PID* pPidSpeed, PID* pPidAngle);
	void updateControl() override;
	virtual void setAngle(float angle);
	void zeroSet();
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
	Motor* motorMap[2][12] = {nullptr};
	Motor* motorList[24] = {nullptr};
	uint8_t size;
};

void motorDeviceInit();
void motorDeviceRoutine();

extern MotorSet motorSet;

extern motorType_t m3508;
extern motorType_t m2006;
extern motorType_t gm6020_i;
extern motorType_t gm6020_v;

#endif //MOTORDEVICE_H
