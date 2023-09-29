#include "pch.h"
#include "BG_Attack.h"
#include "SG_Gather.h"
#include "SG_Attack.h"
#include "SG_Recruit.h"
#include "SG_CalcAttGatherPos.h"
#include "SituationManager.h"

BG_Attack::BG_Attack(BWAPI::TilePosition pos)
	:m_finish(false), m_attPos(pos),BigGoal("BG_Attack", Color(255,0,0))
{
	m_passData.bigGoalPtr = this;
	m_passData.dbBigGoalPos = m_attPos;
	m_passData.attPos = Position(pos);
	m_passData.iValues.clear();
	//DEBUG
	m_passData.iValues.push_back(/*6*/4);


	m_subGoals.push_back(new SG_Recruit(&m_passData));
	m_subGoals.push_back(new SG_CalcAttGatherPos(&m_passData));
	m_subGoals.push_back(new SG_Gather(&m_passData));
	m_subGoals.push_back(new SG_Attack(&m_passData));
}

BG_Attack::~BG_Attack()
{
}

void BG_Attack::Update(const Controller* con)
{
	if (Finished())
		return;

	BigGoal::Update(con);
}


void BG_Attack::Debug()
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos;
	rpPos.x = m_attPos.x * 32 - scnPos.x;
	rpPos.y = m_attPos.y * 32 - scnPos.y;

	Broodwar->drawCircle(BWAPI::CoordinateType::Screen, rpPos.x, rpPos.y, 100, m_debugColor);
}

