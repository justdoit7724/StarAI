#include "pch.h"
#include "SG_Morphing.h"
#include "LogManager.h"
#include "SituationManager.h"
#include "DebugManager.h"

SG_Morphing::SG_Morphing(GoalIO* passData)
    :SmallGoal(passData, Colors::Purple)
{
}

SG_Morphing::~SG_Morphing()
{
    SG_SITU.UnregisterUnit(m_unit);
}

void SG_Morphing::Update(const Controller* con)
{
	if (Finished())
		return;

    switch (m_stage)
    {
    case 1:

        if (!m_unit->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - build");
            m_result = GOAL_RESULT_FAILED;
            return;
        }

        if (m_unit->morph(m_type))
            m_stage++;

        break;
    case 2:

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

void SG_Morphing::Debug(int depth)
{
    if (m_unit)
    {
        SG_DEBUGMGR.DrawTextScn(m_unit->getPosition(), "Morphing");

        SG_DEBUGMGR.DrawUnit(m_unit, m_debugColor);
    }
}

void SG_Morphing::Init()
{
	if (m_isInitialized)
		return;
	m_isInitialized = true;

    if (m_passData->unitTypes[this].empty() || m_passData->units[this].empty() || !m_passData->units[this][0]->canMorph())
    {
        SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - morphing");
        m_result = GOAL_RESULT_FAILED;
        return;
    }
    m_type = m_passData->unitTypes[this][0];
    m_unit = m_passData->units[this][0];

    SG_SITU.RegisterUnit(m_passData->bigGoalPtr, m_unit);

    if (m_passData->devs.find(this) != m_passData->devs.end())
        SG_SITU.AddDevUnit(m_passData->devCenter, m_passData->devs[this].first, m_passData->devs[this].second);

}
