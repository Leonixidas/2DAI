#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"
#include "SteeringAgent.h"
#include "SteeringBehaviors.h"
#include "Behaviors.h"

Plugin::~Plugin()
{
	for (auto pb : m_vpBehaviorVec)
		SAFE_DELETE(pb);
	m_vpBehaviorVec.clear();

	SAFE_DELETE(m_pBehaviorTree);
	SAFE_DELETE(m_pAgent);
}

//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	for (int i = 0, size = m_pInterface->Inventory_GetCapacity(); i < size; ++i)
	{
		m_Inventory.push_back({ eItemType::RANDOM_DROP,i });
	}

	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "Pog";
	info.Student_FirstName = "Leander";
	info.Student_LastName = "De Hertogh";
	info.Student_Class = "2DAE01";

	InitializeBT();
}

void Plugin::InitializeBT()
{
	//Called when the plugin is loaded
	//Create agent
	m_pAgent = new SteeringAgent();
	//Target
	m_Goal.second.Position = m_pAgent->GetPosition();
	m_Target.Position = m_pAgent->GetPosition();

	//Create behaviors
	auto pSeekBehavior = new Seek();

	//TODO Add Avoid to the party
	pSeekBehavior->SetTarget(&m_Target);

	m_pAgent->SetSteeringBehavior(pSeekBehavior);

	auto pBlackBoard = new Elite::Blackboard();
	pBlackBoard->AddData("Agent", m_pAgent);
	pBlackBoard->AddData("Interface", m_pInterface);
	pBlackBoard->AddData("Target", &m_Target);
	pBlackBoard->AddData("Goal", &m_Goal);
	pBlackBoard->AddData("Inventory", &m_Inventory);
	pBlackBoard->AddData("FoundItems", &m_FoundItems);
	pBlackBoard->AddData("Enemy", &m_Enemy);
	pBlackBoard->AddData("Houses", &m_Houses);
	pBlackBoard->AddData("ItemToRemove", &m_ItemToRemove);
	pBlackBoard->AddData("GrabbedItem", &m_GrabbedItem);
	pBlackBoard->AddData("ResetHousesAt", m_ResetHousesAt);
	pBlackBoard->AddData("TimesGunUsed", &m_TimesGunUsed);
	pBlackBoard->AddData("GunID", &m_GunID);
	pBlackBoard->AddData("SeekBehavior", static_cast<ISteeringBehavior*>(pSeekBehavior));

	BehaviorSequence* enemyInFOV = new BehaviorSequence
	({
		new BehaviorConditional(HasGun),
		new BehaviorConditional(EnemyInFOV),
		new BehaviorConditional(AimIsAccurate),
		new BehaviorAction(Shoot)
	});

	BehaviorSequence* clearEnemyTarget = new BehaviorSequence
	({
		new BehaviorConditional(EnemyNotInFOV),
		new BehaviorConditional(GoalIsEnemy),
		new BehaviorAction(ClearGoal)
	});

	BehaviorSequence* gunCheck = new BehaviorSequence
	({
		
	});

	BehaviorSequence* useFood = new BehaviorSequence
	({
		new BehaviorAction(EatFood),
		new BehaviorAction(RemoveFromInventory)
	});

	BehaviorSequence* foodCheck = new BehaviorSequence
	({
		new BehaviorConditional(HasFood),
		new BehaviorSelector
		({
			new BehaviorConditional(EnergyIsLow),
			new BehaviorConditional(MaxFoodReached)
		})
	});

	BehaviorSequence* useMedKit = new BehaviorSequence
	({
		new BehaviorAction(UseMedKit),
		new BehaviorAction(RemoveFromInventory)
	});

	BehaviorSequence* medKitCheck = new BehaviorSequence
	({
		new BehaviorConditional(HasMedKit),
		new BehaviorSelector
		({
			new BehaviorConditional(AgentBitten),
			new BehaviorConditional(HealthIsLow),
			new BehaviorConditional(MaxMedKitReached)
		})
	});

	BehaviorSequence* healthLow = new BehaviorSequence
	({
		new BehaviorConditional(HealthIsLow),
		new BehaviorConditional(HasMedKit),
		useMedKit
	});

	BehaviorSequence* EnergyLow = new BehaviorSequence
	({
		new BehaviorConditional(EnergyIsLow),
		new BehaviorConditional(HasFood),
		useFood
	});

	BehaviorSequence* isGarbage = new BehaviorSequence
	({
		new BehaviorConditional(IsGarbage),
		new BehaviorSelector
		({
			new BehaviorConditional(HasFreeSlot),
			new BehaviorSelector
			({
				healthLow,
				EnergyLow,
				foodCheck,
				medKitCheck
			})
		}),
		new BehaviorAction(AddToInventory),
		new BehaviorAction(RemoveFromInventory)
	});

	BehaviorSequence* isGun = new BehaviorSequence
	({
		new BehaviorConditional(IsGun),
		new BehaviorSelector
		({
			new BehaviorSequence
			({
				new BehaviorConditional(HasGun),
				new BehaviorAction(SelectGun),
				new BehaviorAction(RemoveFromInventory)
			}),
			new BehaviorSequence
			({
				new BehaviorConditional(HasNoGun),
				new BehaviorSelector
				({
					new BehaviorConditional(HasFreeSlot),
					new BehaviorSelector
					({
						foodCheck,
						medKitCheck
					})
				})
			}),
		}),
		new BehaviorAction(AddToInventory)
		
	});

	BehaviorSequence* isFood = new BehaviorSequence
	({
		new BehaviorConditional(IsFood),
		new BehaviorSelector
		({
			new BehaviorConditional(HasFreeSlot),
			new BehaviorSequence
			({
				new BehaviorSelector
				({
					medKitCheck,
					foodCheck,
					new BehaviorSequence
					({
						new BehaviorConditional(HasFood),
						new BehaviorConditional(IsFoodOnMax),
						new BehaviorAction(EatFood),
						new BehaviorAction(RemoveFromInventory)
					})
				})
			})
		}),
		new BehaviorAction(AddToInventory)
	});

	BehaviorSequence* isMedKit = new BehaviorSequence
	({
		new BehaviorConditional(IsMedKit),
		new BehaviorSelector
		({
			new BehaviorConditional(HasFreeSlot),
			new BehaviorSequence
			({
				new BehaviorSelector
				({
					foodCheck,
					medKitCheck,
					new BehaviorSequence
					({
						new BehaviorConditional(HasMedKit),
						new BehaviorConditional(IsMedKitOnMax),
						new BehaviorAction(UseMedKit),
						new BehaviorAction(RemoveFromInventory)
					})
				})
			})
		}),
		new BehaviorAction(AddToInventory)
	});

	BehaviorSequence* ItemInRange = new BehaviorSequence
	({
		new BehaviorConditional(InGrabRange),
		new BehaviorAction(GrabItem),
		new BehaviorSelector({isGarbage, isGun, isFood, isMedKit})
	});

	BehaviorSequence* isHouseInPOV = new BehaviorSequence
	({
		new BehaviorConditional(HouseInPOV),
		new BehaviorAction(ChangeToSeek)
	});

	BehaviorSequence* isItemInFOV = new BehaviorSequence
	({
		new BehaviorConditional(ItemInPOV),
		new BehaviorConditional(TargetNotItem),
		new BehaviorAction(ChangeToSeek)
	});

	BehaviorSequence* startRunning = new BehaviorSequence
	({
		new BehaviorConditional(AgentIsNotRunning),
		new BehaviorConditional(StaminaNotLow),
		new BehaviorAction(Run)
	});

	BehaviorSequence* stopRunning = new BehaviorSequence
	({
		new BehaviorConditional(AgentIsRunning),
		new BehaviorConditional(StaminaLow),
		new BehaviorAction(StopRunning)
	});

	BehaviorSequence* hasTarget = new BehaviorSequence
	({
		new BehaviorConditional(HasTarget),
		new BehaviorConditional(NotInGrabRange),
		new BehaviorAction(ChangeToSeek)
	});

	BehaviorSequence* reachedGoal = new BehaviorSequence
	({
		new BehaviorConditional(HasTarget),
		new BehaviorConditional(GoalReached),
		new BehaviorAction(GoToCheckPoint),
		new BehaviorAction(ChangeToSeek)
	});

	BehaviorSequence* hasNoTarget = new BehaviorSequence
	({
		new BehaviorConditional(HasNoTarget),
		new BehaviorAction(GoToCheckPoint),
		new BehaviorAction(ChangeToSeek)
	});

	//TODO: Create behavior tree
	m_pBehaviorTree = new BehaviorTree(pBlackBoard,
		new BehaviorSelector
		({
			enemyInFOV,
			clearEnemyTarget,
			ItemInRange,
			healthLow,
			EnergyLow,
			isItemInFOV,
			isHouseInPOV,
			startRunning,
			stopRunning,
			reachedGoal,
			hasTarget,
			hasNoTarget
		}));
}

//Called only once
void Plugin::DllInit()
{
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 20; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be usefull to inspect certain behaviours (Default = false)
							//params.LevelFile = "LevelTwo.gppl";
	params.AutoGrabClosestItem = false; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.OverrideDifficulty = false;
	params.Difficulty = 2.f;
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float )
{

}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{
	m_pAgent->SetAgentInfo(m_pInterface->Agent_GetInfo());
	m_pBehaviorTree->Update();
	m_Target.Position = m_pInterface->NavMesh_GetClosestPathPoint(m_Goal.second.Position);
	return m_pAgent->UpdateSteering(dt);
}

//This function should only be used for rendering debug elements
void Plugin::Render(float ) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Goal.second.Position, .7f, { 0,0 }, { 1, 0, 0 });
}
