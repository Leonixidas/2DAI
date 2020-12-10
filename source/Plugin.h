#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "SteeringHelpers.h"
#include "EBehaviorTree.h"
#include <map>
#include "Enums.h"
#include <unordered_set>

class IBaseInterface;
class IExamInterface;
class SteeringAgent;
class ISteeringBehavior;

class Plugin :public IExamPlugin
{
public:
	Plugin() {};
	virtual ~Plugin();

	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void InitializeBT();
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:
	//Interface, used to request data from/perform actions with the AI Framework
	IExamInterface* m_pInterface = nullptr;

	//--- Datamembers ---
	SteeringAgent* m_pAgent = nullptr;
	std::vector<ISteeringBehavior*> m_vpBehaviorVec = {};

	std::vector<std::pair<eItemType, Elite::Vector2>> m_FoundItems = {}; // key is itemType and the second one is the location
	std::vector<std::pair<eItemType, int>> m_Inventory = {};
	std::vector<HouseInfo> m_Houses = {};

	Elite::BehaviorTree* m_pBehaviorTree = nullptr;
	std::pair<TargetType,TargetData> m_Goal = {};
	TargetData m_Target = {};

	EnemyInfo m_Enemy{};
	ItemInfo m_GrabbedItem{};

	int m_ItemToRemove = 0;
	int m_ResetHousesAt = 5;
	int m_TimesGunUsed = 0;
	int m_GunID = -1;
};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}