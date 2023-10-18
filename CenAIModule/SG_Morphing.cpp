#include "pch.h"
#include "SG_Morphing.h"
#include "LogManager.h"
#include "SituationManager.h"

SG_Morphing::SG_Morphing(GoalIO* passData)
    :SmallGoal(passData, Colors::Purple)
{
}

SG_Morphing::~SG_Morphing()
{
    SG_SITU.RegisterUnit(m_passData->bigGoalPtr, m_unit);
    SG_SITU.RemoveDevUnit(m_type);
}

void SG_Morphing::Update(const Controller* con)
{
	if (Finished())
		return;

    switch (m_stage)
    {
    case 0:

        if (!m_unit->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - build");
            m_result = GOAL_RESULT_FAILED;
            return;
        }

        if (m_unit->morph(m_type))
            m_stage++;

        break;
    case 1:

        if (!m_unit->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - build");
            m_result = GOAL_RESULT_FAILED;
            return;
        }

        if (!m_unit->isMorphing())
        {
            m_stage++;
        }

        break;
    default:
        m_result = GOAL_RESULT_SUCCESS;
        break;
    }
}

void SG_Morphing::Init()
{
	if (m_isInitialized)
		return;
	m_isInitialized = true;

    if (m_passData->unitTypes.empty() || m_passData->poses.empty())
    {
        SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - build");
        m_result = GOAL_RESULT_FAILED;
        return;
    }
    m_type = m_passData->unitTypes[m_passData->curIndex];
    m_unit = m_passData->units[m_passData->curIndex];

    SG_SITU.AddDevUnit(m_type);
    SG_SITU.RegisterUnit(m_passData->bigGoalPtr, m_unit);

    m_passData->curIndex++;
}
