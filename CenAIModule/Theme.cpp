#include "pch.h"
#include "Theme.h"

void Theme::Update(const Controller* con)
{

	std::vector<std::string> delList;
	for (auto it = m_goals.begin(); it != m_goals.end(); it++)
	{
		for (int i = 0; i < it->second.size();)
		{
			it->second[i]->Update(con);
#ifdef _DEBUG
			it->second[i]->Debug();
#endif

			switch (it->second[i]->Finished())
			{
			case GOAL_RESULT_SUCCESS:
			case GOAL_RESULT_FAILED:
				delete it->second[i];
				it->second.erase(it->second.begin() + i);
				break;
			default:
				i++;
				break;
			}
		}

		if (it->second.empty())
			delList.push_back(it->first);
	}

	for (int i = 0; i < delList.size(); ++i)
	{
		m_goals.erase(delList[i]);
	}
}

bool Theme::IsGoalExist(std::string id)
{
	return ((m_goals.find(id) != m_goals.end()) && m_goals[id].size());
}

void Theme::AddGoal(std::string name, BigGoal* goal)
{
	m_goals[name].push_back(goal);
}
