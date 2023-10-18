#include "pch.h"
#include "SmallGoal.h"

SmallGoal::SmallGoal(GoalIO* passData, Color debugCol)
	:Goal(debugCol), m_passData(passData), m_isInitialized(false), m_stage(1), m_isCleaned(false)
{
}