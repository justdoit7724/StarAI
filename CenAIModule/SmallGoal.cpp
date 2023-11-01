#include "pch.h"
#include "SmallGoal.h"
#include "SituationManager.h"


SmallGoal::SmallGoal(GoalIO* passData, Color debugCol)
	:Goal(debugCol), m_passData(passData), m_stage(1), m_isCleaned(false)
{
}

SmallGoal::~SmallGoal()
{
	if(m_passData->devs.find(this)!= m_passData->devs.end())
		SG_SITU.RemoveDevUnit(m_passData->devCenter, m_passData->devs[this].first, m_passData->devs[this].second);
}

