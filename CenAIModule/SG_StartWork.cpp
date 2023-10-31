#include "pch.h"
#include "SG_StartWork.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "Controller.h"
#include "LogManager.h"

SG_StartWork::SG_StartWork(GoalIO* passData)
    :SmallGoal(passData,Color(255,255,0))
{
}

void SG_StartWork::Update(const Controller* con)
{
    if (Finished())
        return;

    switch (m_stage)
    {
    case 1:
    {
        auto drones = SG_SITU.UnitsInRange(true, m_rd, UnitTypes::Zerg_Drone, 300);
        
        bool isExistValidDrone = false;
        for (auto d : drones)
        {
            if (!d->isIdle())
                continue;

            if (SG_SITU.IsUnitRegistered(d))
                continue;

            isExistValidDrone = true;

            if (m_isMineral)
            {
                auto mineralUnits = m_rd->getUnitsInRadius(300, Filter::IsMineralField);
                con->ResourceWork(d, (* mineralUnits.begin()));
            }
            else
            {
                auto extractors = SG_SITU.UnitsInRange(true, m_rd, UnitTypes::Zerg_Extractor, 400);
                con->ResourceWork(d, extractors[0]);
            }

            m_stage++;
            break;
        }

        if (!isExistValidDrone)
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "no valid drone");
            m_result = GOAL_RESULT_FAILED;
            return;
        }
    }
        break;

    default:

        m_result = GOAL_RESULT_SUCCESS;

        break;
    }

}

void SG_StartWork::Debug()
{
    Position pos = m_passData->dbBigGoalPos;
    pos.y += 32;

    SG_DEBUGMGR.DrawTextScn(pos, "StartWork");
    SG_DEBUGMGR.DrawBox(pos, 70, 20, m_debugColor);
}

void SG_StartWork::Init()
{
    if (m_isInitialized)
        return;
    m_isInitialized = true;

    if (m_passData->units[this].empty() || m_passData->bValues[this].empty())
    {
        SG_LOGMGR.Record("GOAL_EXCEPT", "no valid init in StartWork");
        m_result = GOAL_RESULT_FAILED;
        return;
    }
    m_rd = m_passData->units[this][0];
    m_isMineral = m_passData->bValues[this][0];

}
