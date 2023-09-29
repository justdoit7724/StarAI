#include "pch.h"
#include "SG_Recruit.h"
#include "DebugManager.h"
#include "SituationManager.h"

SG_Recruit::SG_Recruit(GoalIO* passData)
	:SmallGoal(passData, "SG_Recruit", Color(0, 0, 155))
{
}
void SG_Recruit::Update(const Controller* con)
{
	if (Finished())
		return;

	//DEBUG
	auto zerglings = SG_SITU.UnitsInRange(true, TilePosition(0, 0), UnitTypes::/*Zerg_Zergling*/Zerg_Drone, 10000);
	std::vector<Unit> validZerglings;
	for (auto z : zerglings)
	{
		if (SG_SITU.IsUnitRegistered(z))
			continue;

		validZerglings.push_back(z);
	}
	
	if (m_num == validZerglings.size())
	{
		for (auto z : validZerglings)
		{
			SG_SITU.RegisterUnit(m_passData->bigGoalPtr, z);
		}

		m_isFinished = true;
	}
}

void SG_Recruit::Debug()
{
	auto pos = m_passData->dbBigGoalPos;
	pos.y += 1;
	SG_DEBUGMGR.DrawTextScn(pos, m_id);
	SG_DEBUGMGR.DrawBox(pos, 100, 30, m_debugColor);
}

void SG_Recruit::Init()
{
	if (m_isInitialized)
		return;
	m_isInitialized = true;

	m_num = m_passData->iValues[0];

	
}
