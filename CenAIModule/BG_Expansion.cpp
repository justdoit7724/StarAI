#include "pch.h"
#include "BG_Expansion.h"
#include "SG_Build.h"
#include "DebugManager.h"

BG_Expansion::BG_Expansion(Position pos)
    :BigGoal(Colors::Purple), m_pos(pos)
{
	auto buildGasGoal = new SG_Build(&m_passData);
	m_passData.unitTypes[buildGasGoal].push_back(UnitTypes::Zerg_Hatchery);
	m_passData.poses[buildGasGoal].push_back(pos);
	m_passData.devs[buildGasGoal] ={UnitTypes::Zerg_Hatchery,1};
	m_passData.devCenter = nullptr;

	m_subGoals.push_back(buildGasGoal);
}

void BG_Expansion::Debug(int depth)
{
	SG_DEBUGMGR.DrawTextScn(m_pos, ID());

	Position ltPos = m_pos;
	ltPos.x -= UnitTypes::Zerg_Hatchery.width() / 2;
	ltPos.y -= UnitTypes::Zerg_Hatchery.height() / 2;
	SG_DEBUGMGR.DrawBox(ltPos, UnitTypes::Zerg_Hatchery.width(), UnitTypes::Zerg_Hatchery.height(), m_debugColor);
}

std::string BG_Expansion::ID() const
{
    return std::string();
}
