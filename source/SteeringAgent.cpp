#include "stdafx.h"
#include "SteeringAgent.h"
#include "SteeringBehaviors.h"

SteeringPlugin_Output SteeringAgent::UpdateSteering(float dt)
{
	if(m_pSteeringBehavior != nullptr)
	{
		auto output = m_pSteeringBehavior->CalculateSteering(dt, this);

		//Linear Movement
		//***************
		auto linVel = GetLinearVelocity();
		/*auto steeringForce = output.LinearVelocity - linVel;	

		output.LinearVelocity = linVel + (steeringForce*dt);*/

		//Angular Movement
		//****************
		if (m_AutoOrient)
			output.AngularVelocity = Elite::GetOrientationFromVelocity(output.LinearVelocity);
		else
			output.AngularVelocity = m_AgentInfo.AngularVelocity;
		
		output.RunMode = m_RunMode;
		return output;
	}
	return SteeringPlugin_Output();
}

void SteeringAgent::Render(float ) const
{
}