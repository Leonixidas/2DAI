/*=============================================================================*/
// Copyright 2017-2018 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "SteeringHelpers.h"
#include "Exam_HelperStructs.h"

class SteeringAgent;
using namespace Elite;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringPlugin_Output CalculateSteering(float deltaT, SteeringAgent* pAgent) = 0;

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }
	int temp_id = 0;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	//Seek Functions
	virtual void SetTarget(const TargetData* pTarget) { m_pTargetRef = pTarget; }

protected:
	const TargetData* m_pTargetRef = nullptr;
};

//////////////////////////
//WANDER
//******
class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetWanderOffset(float offset) { m_Offset = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_AngleChange = rad; }

protected:
	float m_Offset = 6.f; //Offset (Agent Direction)
	float m_Radius = 4.f; //WanderRadius
	float m_AngleChange = ToRadians(45); //Max WanderAngle change per frame
	float m_WanderAngle = 0.f; //Internal

private:
	void SetTarget(const TargetData* ) override {} //Hide SetTarget, No Target needed for Wander
};

////////////////////////////
//PURSUIT
//******
class Pursuit : public Seek
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	//Pursuit Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	//Pursuit Functions (Hides Seek::SetTarget)
	virtual void SetTarget(const TargetData* pTarget) override { m_pTargetRef = pTarget; }

protected:
	const TargetData* m_pTargetRef = nullptr;
};

/////////////////////////
//FLEE
//****
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Seek Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

//////////////////////////
//EVADE
//*****
class Evade : public Flee
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	//Evade Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	//Evade Functions (Hides Flee::SetTarget)
	void SetTarget(const TargetData* pTarget) override { m_pTargetRef = pTarget; }

protected:
	const TargetData* m_pTargetRef = nullptr;
};

/////////////////////////////////////////
//ARRIVE
//******
class Arrive : public ISteeringBehavior
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Arrive Behavior
	SteeringPlugin_Output CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	//Arrive Functions
	virtual void SetTarget(const TargetData* pTarget) { m_pTargetRef = pTarget; }
	void SetSlowRadius(float radius) { m_SlowRadius = radius; }
	void SetTargetRadius(float radius) { m_TargetRadius = radius; }

protected:

	const TargetData* m_pTargetRef = nullptr;
	float m_SlowRadius = 15.f;
	float m_TargetRadius = 3.f;
};

////////////////////////
//Avoid Obstacle
//*************
struct Obstacle
{
	Elite::Vector2 center;
	float radius;
};

class AvoidObstacle : public ISteeringBehavior
{
public:
	AvoidObstacle(const vector<Obstacle>& obstacles) :
		m_Obstacles{ obstacles },
		m_MaxAvoidanceForce{ 10.f },
		m_PredictionTime{ .5f },
		m_AvoidMargin{ 50.f }
	{};
	virtual ~AvoidObstacle() = default;
	SteeringPlugin_Output CalculateSteering(float dt, SteeringAgent* pAgent) override;
	void SetMaxAvoidanceForce(float max)
	{
		m_MaxAvoidanceForce = max;
	}
protected:
	vector<Obstacle> m_Obstacles;
	float m_MaxAvoidanceForce;
private:
	float m_PredictionTime;
	float m_AvoidMargin;
};

class EmptyBehavior : public ISteeringBehavior
{
public:
	EmptyBehavior() = default;
	~EmptyBehavior() = default;

	SteeringPlugin_Output CalculateSteering(float , SteeringAgent* ) override
	{
		SteeringPlugin_Output steering{};
		steering.AutoOrientate = false;
		steering.LinearVelocity = {};
		return steering;
	}
};
#endif