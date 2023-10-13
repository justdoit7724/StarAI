#include "pch.h"
#include "BigGoal.h"
#include "SmallGoal.h"
#include "LogManager.h"

BigGoal::BigGoal(std::string id, Color debugCol)
	:Goal(id, debugCol)
{
}

BigGoal::~BigGoal()
{
	int size = m_subGoals.size();
	for (int i = 0; i < size; ++i)
	{
		delete m_subGoals[0];
		m_subGoals.erase(m_subGoals.begin());
	}
}

void BigGoal::Update(const Controller* con)
{

	if(m_subGoals.size())
	{
		m_subGoals[0]->Init();
		m_subGoals[0]->Update(con);
		m_subGoals[0]->Debug();

		switch(m_subGoals[0]->Finished())
		{
		case GOAL_RESULT_SUCCESS:
			delete m_subGoals[0];
			m_subGoals.erase(m_subGoals.begin());

			if (m_subGoals.empty())
			{
				m_result = GOAL_RESULT_SUCCESS;
			}

			break;
		case GOAL_RESULT_FAILED:
		{
			m_result = GOAL_RESULT_FAILED;
			break;
		}
		}
	}


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
