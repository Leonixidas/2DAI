//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "SteeringAgent.h"

//SEEK
//****
SteeringPlugin_Output Seek::CalculateSteering(float , SteeringAgent* pAgent)
{
	SteeringPlugin_Output steering = {};

	auto linearVelocity = (m_pTargetRef->Position - pAgent->GetPosition()).GetNormalized();
	linearVelocity *= pAgent->GetMaxLinearSpeed();
	steering.LinearVelocity = linearVelocity;

	return steering;
}

//WANDER (base> SEEK)
//******
SteeringPlugin_Output Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	//Calculate WanderOffset
	auto offset = pAgent->GetLinearVelocity();
	offset.Normalize();
	offset *= m_Offset;

	//WanderCircle offset (Polar to Cartesean coordinates
	auto circleOffset = Elite::Vector2(cos(m_WanderAngle) * m_Radius, sin(m_WanderAngle) * m_Radius);

	//Change the WanderAngle slightly
	m_WanderAngle += randomFloat() * m_AngleChange - (m_AngleChange * 0.5f); // RAND[-angleChange / 2, angleChange / 2] or just use the random float times 2

	//Set target as Seek::Target
	auto newTarget = TargetData(pAgent->GetPosition() + offset + circleOffset);

	Seek::m_pTargetRef = &newTarget; // it's blasphemy to reference local variables

	//Return Seek Output (with our wander target)
	return Seek::CalculateSteering(deltaT, pAgent);
}

//PURSUIT (base> SEEK)
//*******
SteeringPlugin_Output Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
//	float distance = (m_pTargetRef->Position - pAgent->GetPosition()).Normalize();
//	float time = distance / m_pTargetRef->LinearVelocity.Magnitude();
	Elite::Vector2 futurePos = m_pTargetRef->Position + m_pTargetRef->LinearVelocity;

	auto newTarget = TargetData(futurePos);
	Seek::m_pTargetRef = &newTarget;

	return Seek::CalculateSteering(deltaT, pAgent);
}

//FLEE
//****
SteeringPlugin_Output Flee::CalculateSteering(float , SteeringAgent* pAgent)
{
	SteeringPlugin_Output steering = {};

	auto linearVelocity = (pAgent->GetPosition() - m_pTargetRef->Position).GetNormalized();
	linearVelocity *= pAgent->GetMaxLinearSpeed();
	steering.LinearVelocity = linearVelocity;

	return steering;
}

//EVADE (base> FLEE)
//*****
SteeringPlugin_Output Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	float timeToTarget{ pAgent->GetPosition().Distance(m_pTargetRef->Position) / pAgent->GetMaxLinearSpeed() };
	TargetData targetFutureData = TargetData(m_pTargetRef->Position + m_pTargetRef->LinearVelocity * timeToTarget);
	Flee::m_pTargetRef = &targetFutureData;

	return Flee::CalculateSteering(deltaT, pAgent);
}

//ARRIVE
//******
SteeringPlugin_Output Arrive::CalculateSteering(float , SteeringAgent* pAgent)
{
	SteeringPlugin_Output steering = {};
	Elite::Vector2 toTarget{ m_pTargetRef->Position - pAgent->GetPosition() };
	float sqrDistanceToTarget(toTarget.SqrtMagnitude());
	if (m_SlowRadius * m_SlowRadius < sqrDistanceToTarget)
	{
		steering.LinearVelocity = toTarget.GetNormalized() * pAgent->GetMaxLinearSpeed();
	}
	else if (m_TargetRadius * m_TargetRadius < sqrDistanceToTarget)
	{
		float slowMultiplier = (sqrt(sqrDistanceToTarget) - m_TargetRadius) / (m_SlowRadius * m_SlowRadius - m_TargetRadius);
		steering.LinearVelocity = toTarget.GetNormalized() * pAgent->GetMaxLinearSpeed() * slowMultiplier;
	}
	else
	{
		steering.LinearVelocity = { 0, 0 };
	}
	return steering;
}

SteeringPlugin_Output AvoidObstacle::CalculateSteering(float , SteeringAgent * )
{
	SteeringPlugin_Output steeringOutput{};

	return steeringOutput;
}