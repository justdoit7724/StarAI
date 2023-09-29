#include "pch.h"
#include "BG_Test.h"
#include "SituationManager.h"
#include "MapManager.h"
#include "SG_Attack.h"

BG_Test::BG_Test()
	:BigGoal("BG_Test", Color(0,0,255))
{

}

void BG_Test::Update(const Controller* con)
{
	static int stage = 0;
	switch (stage)
	{
	case 0:
	{
		auto pts = SG_MAP.GetPts();
		if (pts.size())
		{
			m_passData.poses.clear();
			m_passData.poses.push_back(pts[0]);

			m_subGoals.push_back(new SG_Attack(&m_passData));

			stage++;
		}
	}
		break;
	case 1:
		if (!IsGoalExist("SG_Attack"))
			stage++;

		break;
	case 2:
		stage++;
		break;
	case 3:
		stage++;
		break;
	default:
		m_isFinished = true;
		return;
	}

	BigGoal::Update(con);
	m_isFinished = false;

}

void BG_Test::Debug()
{
}
