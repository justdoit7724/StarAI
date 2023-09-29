#include "pch.h"
#include "BigGoal.h"
#include "SmallGoal.h"
#include "LogManager.h"

BigGoal::BigGoal(std::string id, Color debugCol)
	:Goal(id, debugCol)
{
}

void BigGoal::Update(const Controller* con)
{

	if(m_subGoals.size())
	{
		m_subGoals[0]->Init();
		m_subGoals[0]->Update(con);
		m_subGoals[0]->Debug();

		if (m_subGoals[0]->Finished())
		{
			delete m_subGoals[0];
			m_subGoals.erase(m_subGoals.begin());
		}
	}

	if (m_subGoals.empty())
		m_isFinished = true;



}

bool BigGoal::IsGoalExist(std::string id)
{
    for (auto g : m_subGoals)
    {
        if (g->m_id == id)
        {
            return true;

        }

    }
    return false;
}
