/*=============================================================================*/
// Copyright 2017-2018 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for Behavior Tree Demo
// Leander De Hertogh: Added my own functions and deleted some other functions
// for my AI
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "EliteMath/EMathUtilities.h"
#include "EBehaviorTree.h"
#include "SteeringAgent.h"
#include "SteeringBehaviors.h"

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------
inline bool HasTarget(Elite::Blackboard *pBlackBoard)
{
	std::pair<TargetType, TargetData>* goal = nullptr;
	if (!pBlackBoard->GetData("Goal", goal))
		return false;
	
	return goal->first != TargetType::NOTHING;
}

inline bool HasNoTarget(Elite::Blackboard *pBlackboard)
{
	return !HasTarget(pBlackboard);
}

inline bool InGrabRange(Elite::Blackboard *pBlackBoard)
{
	SteeringAgent* pAgent = nullptr;
	std::pair<TargetType,TargetData>* pGoal = nullptr;

	bool dataAvailable = pBlackBoard->GetData("Agent", pAgent) 
		&& pBlackBoard->GetData("Goal", pGoal);

	if (!dataAvailable) return false;

	if (!pAgent) return false;

	if (pGoal->first != TargetType::ITEM) return false;

	if (Elite::Distance(pAgent->GetPosition(), pGoal->second.Position) < pAgent->GetGrabRange())
	{
		pGoal->first = TargetType::NOTHING;
		return true;
	}

	return false;
}

inline bool NotInGrabRange(Elite::Blackboard *pBlackboard)
{
	return !InGrabRange(pBlackboard);
}

inline bool HasGun(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;

	if (!pBlackboard->GetData("Inventory", inventory))
		return false;

	if (std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) {return a.first == eItemType::PISTOL;}) != inventory->end())
		return true;
	
	return false;
}

inline bool HasNoGun(Elite::Blackboard *pBlackboard)
{
	return !HasGun(pBlackboard);
}

inline bool HasFood(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;

	if (!pBlackboard->GetData("Inventory", inventory))
		return false;
	
	if (std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::FOOD; }) != inventory->end())
		return true;
	
	return false;
}

inline bool HasNoFood(Elite::Blackboard *pBlackboard)
{
	return !HasFood(pBlackboard);
}

inline bool HasMedKit(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;

	if (!pBlackboard->GetData("Inventory", inventory))
		return false;

	if (std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::MEDKIT; }) != inventory->end())
		return true;

	return false;
}

inline bool HasNoMedKit(Elite::Blackboard *pBlackboard)
{
	return !HasMedKit(pBlackboard);
}

inline bool AgentBitten(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* agent = nullptr;
	if(!pBlackboard->GetData("Agent", agent))
		return false;

	return agent->GetAgentInfo().Bitten;
}

inline bool HealthIsLow(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* agent = nullptr;
	if (!pBlackboard->GetData("Agent", agent))
		return false;

	return agent->GetAgentInfo().Health < 5.f;
}

inline bool EnergyIsLow(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* agent = nullptr;
	if (!pBlackboard->GetData("Agent", agent))
		return false;

	return agent->GetAgentInfo().Energy < 5.f;
}

inline bool StaminaLow(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* agent = nullptr;
	if (!pBlackboard->GetData("Agent", agent))
		return false;

	return agent->GetAgentInfo().Stamina < 0.01f;
}

inline bool StaminaNotLow(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* agent = nullptr;
	if (!pBlackboard->GetData("Agent", agent))
		return false;

	return agent->GetAgentInfo().Stamina > 5.f;
}

inline bool HasFreeSlot(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	if (!pBlackboard->GetData("Inventory", inventory))
		return false;

	return std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::RANDOM_DROP; }) != inventory->end();
}

inline bool HasNoFreeSlot(Elite::Blackboard *pBlackboard)
{
	return !HasFreeSlot(pBlackboard);
}

inline bool NeedsGun(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	if (!pBlackboard->GetData("Inventory", inventory))
		return false;

	int counter = 0;
	for (auto item : *inventory)
	{
		if (item.first == eItemType::PISTOL)
			++counter;
	}
	return counter == 0;
}

inline bool IsGarbage(Elite::Blackboard *pBlackboard)
{
	ItemInfo* pItem = nullptr;
	if (!pBlackboard->GetData("GrabbedItem", pItem)) return false;

	return pItem->Type == eItemType::GARBAGE;
}

inline bool IsGun(Elite::Blackboard *pBlackboard)
{
	ItemInfo* pItem = nullptr;
	if (!pBlackboard->GetData("GrabbedItem", pItem)) return false;

	return pItem->Type == eItemType::PISTOL;
}

inline bool IsFood(Elite::Blackboard *pBlackboard)
{
	ItemInfo* pItem = nullptr;
	if (!pBlackboard->GetData("GrabbedItem", pItem)) return false;

	return pItem->Type == eItemType::FOOD;
}

inline bool IsMedKit(Elite::Blackboard *pBlackboard)
{
	ItemInfo* pItem = nullptr;
	if (!pBlackboard->GetData("GrabbedItem", pItem)) return false;

	return pItem->Type == eItemType::MEDKIT;
}

inline bool HasTrash(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	if (!pBlackboard->GetData("Inventory", inventory))
		return false;

	return std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::GARBAGE; }) != inventory->end();
}

inline bool EnemyInFOV(Elite::Blackboard *pBlackboard)
{
	IExamInterface* inter = nullptr;
	std::pair<TargetType, TargetData>* pGoal = nullptr;
	EnemyInfo* pEnemy = nullptr;

	if (!(pBlackboard->GetData("Interface", inter)
		&& pBlackboard->GetData("Goal", pGoal)
		&& pBlackboard->GetData("Enemy", pEnemy))) return false;

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (inter->Fov_GetEntityByIndex(i, ei))
		{
			if (ei.Type == eEntityType::ENEMY)
			{
				inter->Enemy_GetInfo(ei,*pEnemy);
				pGoal->first = TargetType::ENEMY;
				pGoal->second.Position = ei.Location;
				return true;
			}
			continue;
		}
		break;
	}

	return false;
}

inline bool EnemyNotInFOV(Elite::Blackboard *pBlackboard)
{
	IExamInterface* inter = nullptr;
	EnemyInfo* pEnemy = nullptr;

	if (!(pBlackboard->GetData("Interface", inter)
		&& pBlackboard->GetData("Enemy", pEnemy))) return false;

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (inter->Fov_GetEntityByIndex(i, ei))
		{
			if (ei.Type == eEntityType::ENEMY)
			{
				return false;
			}
			continue;
		}
		break;
	}

	return true;
}

inline bool ItemInPOV(Elite::Blackboard *pBlackboard)
{
	IExamInterface* inter = nullptr;
	std::vector<std::pair<eItemType, Elite::Vector2>>* pFoundItems = nullptr;
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!(pBlackboard->GetData("Interface", inter) 
		&& pBlackboard->GetData("Goal", pGoal)
		&& pBlackboard->GetData("FoundItems", pFoundItems))) return false;

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (inter->Fov_GetEntityByIndex(i, ei))
		{
			if (ei.Type == eEntityType::ITEM)
			{
				if (pGoal->first != TargetType::ITEM)
				{
					auto it = std::find_if(pFoundItems->begin(), pFoundItems->end(), [&ei](const std::pair<eItemType, Elite::Vector2>& a) { return a.second == ei.Location; });
					if (HasFreeSlot(pBlackboard) || it == pFoundItems->end())
					{
						pGoal->first = TargetType::ITEM;
						pGoal->second.Position = ei.Location;
						return true;
					}
					continue;
				}
			}
			continue;
		}
		break;
	}

	return false;
}

inline bool HouseInPOV(Elite::Blackboard *pBlackboard)
{
	IExamInterface* inter = nullptr;
	std::vector<HouseInfo>* pHouses = nullptr;
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!(pBlackboard->GetData("Interface", inter)
		&& pBlackboard->GetData("Goal", pGoal)
		&& pBlackboard->GetData("Houses", pHouses))) return false;

	if (pGoal->first == TargetType::HOUSE) return false;

	if (pHouses->size() > 10) pHouses->clear();

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (inter->Fov_GetHouseByIndex(i, hi))
		{
			if (std::find_if(pHouses->begin(), pHouses->end(), [&hi](const HouseInfo& a) { return a.Center == hi.Center; }) == pHouses->end())
			{
				pHouses->push_back(hi);
				pGoal->first = TargetType::HOUSE;
				pGoal->second = hi.Center;
				return true;
			}
			continue;
		}
		break;
	}

	return false;
}

bool IsOverlapping(const Vector2& segmentPoint0, const Vector2& segmentPoint1, const Vector2& circleCenter, const float& radius)
{
	Vector2 seg0{ segmentPoint0 - circleCenter }, seg1{ segmentPoint1 - circleCenter };
	Vector2 segmentVector = seg1 - seg0;
	return Square(seg0.x * seg1.y - seg1.x * seg0.y) < Square(radius) * (Square(segmentVector.x) + Square(segmentVector.y));
}

inline bool TargetNotItem(Elite::Blackboard *pBlackboard)
{
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!pBlackboard->GetData("Goal", pGoal)) return false;

	if (pGoal->first != TargetType::ITEM) return true;

	return false;
}

inline bool AgentIsRunning(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* agent = nullptr;
	if (!pBlackboard->GetData("Agent", agent))
		return false;

	return agent->GetAgentInfo().RunMode;
}

inline bool AgentIsNotRunning(Elite::Blackboard *pBlackboard)
{
	return !AgentIsRunning(pBlackboard);
}

inline bool AimIsAccurate(Elite::Blackboard *pBlackboard)
{
	std::pair<TargetType, TargetData>* pGoal = nullptr;
	EnemyInfo* pEnemy = nullptr;
	SteeringAgent* pAgent = nullptr;

	if (!(pBlackboard->GetData("Goal", pGoal) && pBlackboard->GetData("Agent", pAgent) && pBlackboard->GetData("Enemy", pEnemy))) return false;

	float degrees = pAgent->GetOrientation() - float(b2_pi / 2.f);
	Elite::Vector2 forwardVec{ Elite::OrientationToVector(degrees)};
	Elite::Vector2 bulletVector{ pAgent->GetAgentInfo().FOV_Range * cos(degrees), pAgent->GetAgentInfo().FOV_Range * sin(degrees) };

	if (IsOverlapping(pAgent->GetPosition(), pAgent->GetPosition() + bulletVector, pGoal->second.Position, pEnemy->Size))
	{
		return true;
	}
	
	return false;
}

inline bool GoalReached(Elite::Blackboard *pBlackboard)
{
	std::pair<TargetType, TargetData>* pGoal = nullptr;
	SteeringAgent* pAgent = nullptr;

	if (!(pBlackboard->GetData("Agent", pAgent)
		&& pBlackboard->GetData("Goal", pGoal)))
		return false;

	if (Elite::Distance(pGoal->second.Position, pAgent->GetPosition()) < pAgent->GetAgentInfo().GrabRange)
	{
		pGoal->first = TargetType::NOTHING;
		return true;
	}

	return false;
}

inline bool MaxMedKitReached(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;

	if (!pBlackboard->GetData("Inventory", inventory))
		return true;

	int counter = 0;

	for (auto item : *inventory)
	{
		if (item.first == eItemType::MEDKIT) ++counter;
	}

	if (counter > 2) return true;

	return true;
}

inline bool IsMedKitOnMax(Elite::Blackboard* pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;

	if (!pBlackboard->GetData("Inventory", inventory))
		return true;

	int counter = 0;

	for (auto item : *inventory)
	{
		if (item.first == eItemType::MEDKIT) ++counter;
	}

	if (counter == 2) return true;

	return true;
}

inline bool MaxFoodReached(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;

	if (!pBlackboard->GetData("Inventory", inventory))
		return true;

	int counter = 0;

	for (auto item : *inventory)
	{
		if (item.first == eItemType::FOOD) ++counter;
	}

	if (counter > 2) return true;

	return true;
}

inline bool IsFoodOnMax(Elite::Blackboard* pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;

	if (!pBlackboard->GetData("Inventory", inventory))
		return true;

	int counter = 0;

	for (auto item : *inventory)
	{
		if (item.first == eItemType::FOOD) ++counter;
	}

	if (counter == 2) return true;

	return true;
}

inline bool MaxHousesVisited(Elite::Blackboard *pBlackboard)
{
	std::vector<HouseInfo>* pHouses = nullptr;

	if (!pBlackboard->GetData("Houses", pHouses)) return false;

	if (pHouses->size() > 11)
		return true;

	return false;
}

inline bool GoalIsEnemy(Elite::Blackboard *pBlackboard)
{
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!pBlackboard->GetData("Goal", pGoal)) return false;

	return pGoal->first == TargetType::ENEMY;
}

inline bool GoalIsItem(Elite::Blackboard *pBlackboard)
{
	return !TargetNotItem(pBlackboard);
}

inline bool GoalNotHouse(Elite::Blackboard *pBlackboard)
{
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!pBlackboard->GetData("Goal", pGoal)) return false;

	return pGoal->first != TargetType::HOUSE;
}

//**********************************
//*************ACTIONS**************
//**********************************
inline BehaviorState ChangeToSeek(Elite::Blackboard *pBlackboard)
{
	ISteeringBehavior *pBehavior = nullptr;
	SteeringAgent* pAgent = nullptr;

	bool dataAvailable = pBlackboard->GetData("Agent", pAgent)
		&& pBlackboard->GetData("SeekBehavior", pBehavior);
	if (!dataAvailable) return Failure;

	if (!pAgent || !pBehavior) return Failure;

	if (pAgent->GetSteeringBehavior() != pBehavior)
	{
		pAgent->SetSteeringBehavior(pBehavior);
	}

	return Success;
}

inline BehaviorState GrabItem(Elite::Blackboard* pBlackboard)
{
	ItemInfo* grabbedItem = nullptr;
	IExamInterface* inter = nullptr;
	std::vector<std::pair<eItemType, Elite::Vector2>>* pFoundItems = nullptr;
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!(pBlackboard->GetData("Interface", inter) 
		&& pBlackboard->GetData("GrabbedItem", grabbedItem)
		&& pBlackboard->GetData("Goal", pGoal)
		&& pBlackboard->GetData("FoundItems", pFoundItems)))
		return Failure;

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (inter->Fov_GetEntityByIndex(i, ei))
		{
			if (ei.Type == eEntityType::ITEM)
			{
				if (ei.Location == pGoal->second.Position)
				{
					break;
				}
			}
			continue;
		}
		break;
	}

	ItemInfo i{};
	if (inter->Item_Grab(ei, i))
	{
		*grabbedItem = i;
		pGoal->first = TargetType::NOTHING;

		if (std::find_if(pFoundItems->begin(), pFoundItems->end(), [&i](const std::pair<eItemType, Elite::Vector2>& a) { return a.second == i.Location; }) == pFoundItems->end())
			pFoundItems->push_back({ i.Type, i.Location });
		return Success;
	}

	return Failure;
}

inline BehaviorState Shoot(Elite::Blackboard *pBlackboard)
{
	IExamInterface* inter = nullptr;
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	int* timesGunUsed{nullptr};

	bool dataAvailable = pBlackboard->GetData("Interface", inter) && pBlackboard->GetData("Inventory", inventory) && pBlackboard->GetData("TimesGunUsed", timesGunUsed);

	if (!dataAvailable) return Failure;

	auto it = std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::PISTOL; });

	if (it == inventory->end()) return Failure;

	if (inter->Inventory_UseItem((*it).second))
	{
		(*timesGunUsed)++;
		std::cout << "Times gun used: " << std::to_string(*timesGunUsed) << std::endl;
		return Success;
	}
	else
	{
		inter->Inventory_RemoveItem((*it).second);
		(*it).first = eItemType::RANDOM_DROP;
		return Failure;
	}
}

inline BehaviorState AddToInventory(Elite::Blackboard *pBlackboard)
{
	int* itemToRemove = nullptr;
	ItemInfo* grabbedItem = nullptr;
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	IExamInterface* inter = nullptr;
	int* pGunID = nullptr;

	bool dataAvailable = pBlackboard->GetData("Inventory", inventory) 
		&& pBlackboard->GetData("Interface", inter)
		&& pBlackboard->GetData("GrabbedItem", grabbedItem)
		&& pBlackboard->GetData("ItemToRemove", itemToRemove)
		&& pBlackboard->GetData("GunID", pGunID);

	if (!dataAvailable) return Failure;

	auto it = std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::RANDOM_DROP; });

	if (it == inventory->end()) return Failure;

	if (inter->Inventory_AddItem((*it).second, *grabbedItem))
	{
		std::pair<eItemType, int> a{(*it)};
		a.first = grabbedItem->Type;
		(*inventory)[(*it).second] = a;
		*itemToRemove = a.second;



		return Success;
	}

	return Failure;
}

inline BehaviorState RemoveFromInventory(Elite::Blackboard *pBlackboard)
{
	int* itemToRemove = nullptr;
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	IExamInterface* inter = nullptr;

	bool dataAvailable = pBlackboard->GetData("ItemToRemove", itemToRemove)
		&& pBlackboard->GetData("Inventory", inventory)
		&& pBlackboard->GetData("Interface", inter);

	if (!dataAvailable) return Failure;

	if (inter->Inventory_RemoveItem(*itemToRemove))
	{
		(*inventory)[*itemToRemove].first = eItemType::RANDOM_DROP;
		return Success;
	}

	return Failure;
}

inline BehaviorState UseMedKit(Elite::Blackboard *pBlackboard)
{
	int* itemToRemove = nullptr;
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	IExamInterface* inter = nullptr;

	bool dataAvailable = pBlackboard->GetData("Inventory", inventory)
		&& pBlackboard->GetData("Interface", inter)
		&& pBlackboard->GetData("ItemToRemove", itemToRemove);

	if (!dataAvailable) return Failure;

	auto it = std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::MEDKIT; });

	if (it == inventory->end()) return Failure;

	if (inter->Inventory_UseItem((*it).second))
	{
		std::pair<eItemType, int> a{ (*it) };
		a.first = eItemType::RANDOM_DROP;
		(*inventory)[(*it).second] = a;
		*itemToRemove = a.second;

		return Success;
	}

	return Failure;
}

inline BehaviorState EatFood(Elite::Blackboard *pBlackboard)
{
	int* itemToRemove = nullptr;
	std::vector<std::pair<eItemType, int>>* inventory = nullptr;
	IExamInterface* inter = nullptr;

	bool dataAvailable = pBlackboard->GetData("Inventory", inventory)
		&& pBlackboard->GetData("Interface", inter)
		&& pBlackboard->GetData("ItemToRemove", itemToRemove);

	if (!dataAvailable) return Failure;

	auto it = std::find_if(inventory->begin(), inventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::FOOD; });

	if (it == inventory->end()) return Failure;

	if (inter->Inventory_UseItem((*it).second))
	{
		std::pair<eItemType, int> a{ (*it) };
		a.first = eItemType::RANDOM_DROP;
		(*inventory)[(*it).second] = a;

		*itemToRemove = a.second;
		return Success;
	}

	return Failure;
}

inline BehaviorState StopRunning(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* pAgent = nullptr;

	if (!pBlackboard->GetData("Agent", pAgent)) return Failure;

	pAgent->SetRunMode(false);

	return Success;
}

inline BehaviorState Run(Elite::Blackboard *pBlackboard)
{
	SteeringAgent* pAgent = nullptr;

	if (!pBlackboard->GetData("Agent", pAgent)) return Failure;

	pAgent->SetRunMode(true);

	return Success;
}

inline BehaviorState GoToCheckPoint(Elite::Blackboard *pBlackboard)
{
	IExamInterface* inter = nullptr;
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!(pBlackboard->GetData("Goal", pGoal) && pBlackboard->GetData("Interface", inter))) return Failure;

	pGoal->second.Position = inter->World_GetCheckpointLocation();
	pGoal->first = TargetType::CHECKPOINT;

	return Success;
}

inline BehaviorState SelectGun(Elite::Blackboard *pBlackboard)
{
	std::vector<std::pair<eItemType, int>>* pInventory = nullptr;
	int* selectedItem = nullptr;

	if (!(pBlackboard->GetData("Inventory", pInventory)
		&& pBlackboard->GetData("ItemToRemove", selectedItem))) return Failure;

	auto it = std::find_if(pInventory->begin(), pInventory->end(), [](const std::pair<eItemType, int>& a) { return a.first == eItemType::PISTOL; });

	if (it != pInventory->end())
	{
		*selectedItem = (*it).second;
		return Success;
	}

	return Failure;
}

inline BehaviorState ClearGoal(Elite::Blackboard *pBlackboard)
{
	std::pair<TargetType, TargetData>* pGoal = nullptr;

	if (!pBlackboard->GetData("Goal", pGoal)) return Failure;

	pGoal->first = TargetType::NOTHING;

	return Success;
}

inline BehaviorState ClearHouses(Elite::Blackboard *pBlackboard)
{
	std::vector<HouseInfo>* pHouses = nullptr;

	if (!pBlackboard->GetData("Houses", pHouses)) return Failure;

	for (size_t i = 0; i < pHouses->size(); ++i)
	{
		pHouses->pop_back();
	}

	return Success;
}
#endif