#include "pch.h"
#include "BigGoal.h"
#include "SmallGoal.h"
#include "LogManager.h"

BigGoal::BigGoal(Color debugCol)
	:Goal(debugCol)
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
	if (Finished())
		return;

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

void BigGoal::Init()
{
	if (m_isInitialized)
		return;
	m_isInitialized = true;

	for (int i=0; i< ((int)m_subGoals.size()-1); ++i)
	{
		m_passData.nextSmallGoalPtr[m_subGoals[i]] = m_subGoals[i + 1];
	}
}
