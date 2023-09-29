#include "pch.h"
#include "SG_StartWork.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "Controller.h"

SG_StartWork::SG_StartWork(GoalIO* passData)
    :SmallGoal(passData,"SG_StartWork",Color(255,255,0))
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
            m_isFinished=true;
    }
        break;

    default:

        m_isFinished = true;

        break;
    }

}

void SG_StartWork::Debug()
{
    TilePosition pos = m_passData->dbBigGoalPos;
    pos.y += 1;

    SG_DEBUGMGR.DrawTextScn(pos, m_id);
    SG_DEBUGMGR.DrawBox(pos, 70, 20, m_debugColor);
}

void SG_StartWork::Init()
{
    if (m_isInitialized)
        return;
    m_isInitialized = true;

    m_rd = m_passData->units[0];
    m_isMineral = m_passData->bValues[0];

}
