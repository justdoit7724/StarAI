#include "pch.h"
#include "SG_Build.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "Controller.h"
#include "LogManager.h"
#include "StopWatch.h"

SG_Build::SG_Build(GoalIO* passData)
    :SmallGoal(passData,Colors::Blue),m_type(UnitTypes::None), m_worker(nullptr), m_build(nullptr)
{
    m_timer = new StopWatch();
}

SG_Build::~SG_Build()
{
    SG_SITU.RemoveDevUnit(m_type);
    SG_SITU.UnregisterUnit(m_worker);
    delete m_timer;

}

void SG_Build::Update(const Controller* con)
{
    if (Finished())
        return;

    switch (m_stage)
    {
    case 1:
    {
        auto units= SG_SITU.UnitsInRange(true, TilePosition(m_pos), UnitTypes::Zerg_Drone);
        for (auto u : units)
        {
            if (!u->getType().isWorker())
                continue;

            if (SG_SITU.IsUnitRegistered(u))
                continue;

            m_worker = u;
            if (m_worker->isIdle())
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

        if (con->Move(m_worker, m_pos))
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

        if (m_type.mineralPrice() <= SG_SITU.CurMineral() &&
            m_type.gasPrice() <= SG_SITU.CurGas())
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


        if (m_worker->isIdle())
        {
            con->Move(m_worker, m_pos);
        }

        if (m_worker->getPosition().getDistance(m_pos) < 16)
        {
            if (con->Build(m_worker, m_type, TilePosition(m_pos)))
            {
                m_timer->reset();
                m_stage++;
            }
        }
    }
        break;
    case 5:
    {
        if (m_timer->elapsed() > 1.5)
            m_stage++;
    }
    break;
    case 6:
    {
        auto buildings=SG_SITU.AllUnitsinRange(true, TilePosition(m_pos), 100);


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
    case 7:

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
    SG_DEBUGMGR.DrawTextScn(m_pos, "Build");

    int w=m_type.width();
    int h = m_type.height();

    SG_DEBUGMGR.DrawBox(m_pos, w, h, m_debugColor);

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
    m_type = m_passData->unitTypes[m_passData->curIndex];
    m_pos = m_passData->poses[m_passData->curIndex];

    SG_SITU.AddDevUnit(m_type);

    m_passData->curIndex++;
}

