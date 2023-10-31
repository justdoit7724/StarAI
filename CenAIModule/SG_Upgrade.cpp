#include "pch.h"
#include "SG_Upgrade.h"
#include "LogManager.h"
#include "SituationManager.h"

void SG_Upgrade::Update(const Controller* con)
{
    if (Finished())
        return;

    switch (m_stage)
    {
    case 1:
        //update
        if (!m_unit->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not valid unit - upgrade");
            m_result = GOAL_RESULT_FAILED;
            return;
        }

        if (m_unit->upgrade(m_type))
        {
            m_stage++;
            break;

            
        }

        break;
    case 2:
        //wait
        if (!m_unit->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not valid unit - upgrade");
            m_result = GOAL_RESULT_FAILED;
            return;
        }
        if (!m_unit->isUpgrading())
        {
            m_stage++;
        }
        break;

    default:
        m_result = GOAL_RESULT_SUCCESS;
        break;
    }
}

void SG_Upgrade::Init()
{
    if (m_isInitialized)
        return;
    m_isInitialized = true;

    if (m_passData->upgType.empty() || m_passData->units.empty())
    {
        SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - upgrade");
        m_result = GOAL_RESULT_FAILED;
        return;
    }

    m_unit = m_passData->units[this][0];
    m_type = m_passData->upgType[this][0];

    SG_SITU.RegisterUnit(m_passData->bigGoalPtr, m_unit);
}
