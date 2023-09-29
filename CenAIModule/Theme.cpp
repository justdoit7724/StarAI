#include "pch.h"
#include "Theme.h"

bool Theme::IsGoalExist(std::string id)
{
	return ((m_goals.find(id) != m_goals.end()) && m_goals[id].size());
}

void Theme::AddGoal(std::string name, BigGoal* goal)
{
	m_goals[name].push_back(goal);
}
