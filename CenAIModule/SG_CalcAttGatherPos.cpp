#include "pch.h"
#include "SG_CalcAttGatherPos.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "MapManager.h"
#include "LogManager.h"

SG_CalcAttGatherPos::SG_CalcAttGatherPos(GoalIO* passData)
	:SmallGoal(passData, Color(0,155,155))
{
}

void SG_CalcAttGatherPos::Update(const Controller* con)
{
	if (Finished())
		return;

	switch (m_stage)
	{
	case 1:
	{
		auto units = SG_SITU.GetRegistered(m_passData->bigGoalPtr);
		if (units.empty())
		{
			SG_LOGMGR.Record("GOAL_EXCEPT", "no registered unit");
			m_result = GOAL_RESULT_FAILED;
			return;
		}
		TilePosition midPos = TilePosition(0,0);
		for (auto u : units)
		{
			midPos += u->getTilePosition();
		}
		midPos /= units.size();
		Position pPos = Position(midPos);
		Position pAttPos = Position(m_attPos);
		Position pDir = (pAttPos - pPos)/20;
		Position step = pPos;
		for (int i = 0; i < 15, step += pDir; ++i) {

			if (SG_MAP.GetTerrain(WalkPosition(step.x / 8, step.y / 8)) & MAP_MOVABLE == MAP_MOVABLE)
				break;
		}


		m_passData->poses.push(step);
		m_stage++;
	}
		break;
	default:
		m_result = GOAL_RESULT_SUCCESS;
		break;
	}
}

void SG_CalcAttGatherPos::Debug()
{
	SG_DEBUGMGR.DrawTextScn(m_attPos, "CalcAttGatherPos");
	SG_DEBUGMGR.DrawBox(m_attPos, 90, 30, m_debugColor);

	auto units = SG_SITU.GetRegistered(m_passData->bigGoalPtr);

	for (auto u : units)
	{
		SG_DEBUGMGR.DrawLine(m_attPos, u->getPosition(), m_debugColor);
	}
}

void SG_CalcAttGatherPos::Init()
{
	if (m_isInitialized)
		return;
	m_isInitialized = true;

	if (!SG_MAP.IsValidPos(m_passData->attPos))
	{
		SG_LOGMGR.Record("GOAL_EXCEPT", "unvalid attPos");
		m_result = GOAL_RESULT_FAILED;
		return;
	}
	m_attPos = m_passData->attPos;
}
