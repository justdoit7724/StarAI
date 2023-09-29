#include "pch.h"
#include "SmallGoal.h"

SmallGoal::SmallGoal(GoalIO* passData, std::string id, Color debugCol)
	:Goal(id, debugCol), m_passData(passData), m_isInitialized(false), m_stage(1)
{
}