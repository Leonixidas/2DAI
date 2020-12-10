/*=============================================================================*/
// Copyright 2017-2018 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringAgent.h: basic agent using steering behaviors
/*=============================================================================*/
#ifndef STEERING_AGENT_H
#define STEERING_AGENT_H

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "SteeringHelpers.h"
#include "Exam_HelperStructs.h"
class ISteeringBehavior;

class SteeringAgent final
{
public:
	//--- Constructor & Destructor ---
	SteeringAgent() = default;
	virtual ~SteeringAgent() = default;

	//--- Agent Functions ---
	SteeringPlugin_Output UpdateSteering(float dt);
	void Render(float dt) const;

	void SetAgentInfo(AgentInfo info){m_AgentInfo = info;}
	AgentInfo& GetAgentInfo() { return m_AgentInfo; }
	float GetGrabRange() { return m_AgentInfo.GrabRange; }

	Elite::Vector2 GetPosition() { return m_AgentInfo.Position; }

	float GetMaxLinearSpeed() const { return m_AgentInfo.MaxLinearSpeed; }
	float GetMaxAngularSpeed() const { return m_AgentInfo.MaxAngularSpeed; }
	float GetOrientation() { return m_AgentInfo.Orientation; }
	float GetAngularVelocity() { return m_AgentInfo.AngularVelocity; }
	Elite::Vector2 GetLinearVelocity() { return m_AgentInfo.LinearVelocity; }

	void SetSteeringBehavior(ISteeringBehavior* pBehavior) { m_pSteeringBehavior = pBehavior; }
	ISteeringBehavior* GetSteeringBehavior() const { return m_pSteeringBehavior; }

	void SetAutoOrient(bool autoOrient) { m_AutoOrient = autoOrient; }
	void SetRunMode(bool runMode) { m_RunMode = runMode; }

private:
	//--- Datamembers ---
	ISteeringBehavior* m_pSteeringBehavior = nullptr;
	AgentInfo m_AgentInfo = {};

	bool m_AutoOrient = true;
	bool m_RunMode = false;
};
#endif