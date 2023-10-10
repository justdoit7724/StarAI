#include "pch.h"
#include "SG_Build.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "Controller.h"
#include "LogManager.h"

SG_Build::SG_Build(GoalIO* passData)
    :SmallGoal(passData,"SG_Build",Colors::Blue), m_worker(nullptr), m_build(nullptr)
{
}

void SG_Build::Update(const Controller* con)
{
    if (Finished())
        return;

    switch (m_stage)
    {
    case 1:
    {
        auto units= SG_SITU.UnitsInRange(true, m_tPos, UnitTypes::Zerg_Drone);
        for (auto u : units)
        {
            if (!u->getType().isWorker())
                continue;

            if (SG_SITU.IsUnitRegistered(u))
                continue;

            m_worker = u;
            break;
        }

        if (m_worker)
        {
            SG_SITU.RegisterUnit(m_passData->bigGoalPtr, m_worker);
            m_stage++;
        }
        else
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "no worker - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }
    }
        break;
    case 2:
    {
        if (!m_worker->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "worker is unavailable - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }

        Position pos = Position(m_tPos);
        int w = m_type.width();
        int h = m_type.height();
        pos.x += w / 2;
        pos.y += h / 2;

        if (con->Move(m_worker, pos))
            m_stage++;
    }
        break;
    case 3:

        if (!m_worker->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "worker is unavailable2 - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }

        if (m_type.mineralPrice() <= Broodwar->self()->gatheredMinerals() &&
            m_type.gasPrice() <= Broodwar->self()->gatheredGas())
        {
            m_stage++;
        }
       

        break;
    case 4:
    {
        if (!m_worker->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "worker is unavailable3 - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }

        auto pos=m_worker->getTilePosition();
        auto idle = m_worker->isIdle();
        if (m_worker->getTilePosition() == m_tPos && m_worker->isIdle())
        {
            if (con->Build(m_worker, m_type, m_tPos))
                m_stage++;
        }
    }
        break;
    case 5:
    {
        auto buildings=SG_SITU.AllUnitsinRange(true, m_tPos, 100);


        for (auto b : buildings)
        {
            if (b->isMorphing())
            {
                m_build = b;
                m_stage++;
                break;
            }
        }

        if (!m_build)
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "morphing is unavailable - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }

    }
        break;
    case 6:

        if (!m_build->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "morphing is unavailable - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }

        if (m_build->isCompleted())
        {
            m_stage++;
        }
        break;
    default:
        m_result = GOAL_RESULT_SUCCESS;
        break;
    }
}

void SG_Build::Debug()
{
    SG_DEBUGMGR.DrawTextScn(m_tPos, m_id);

    int w=m_type.width();
    int h = m_type.height();

    SG_DEBUGMGR.DrawBox(m_tPos, w, h, m_debugColor);

}

void SG_Build::Init()
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
    m_type = m_passData->unitTypes[0];
    m_tPos = m_passData->poses[0];
}
