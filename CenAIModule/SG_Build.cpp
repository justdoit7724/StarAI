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
    SG_SITU.UnregisterUnit(m_worker);
    SG_SITU.UnregisterUnit(m_build);

    if (m_build && m_passData->nextSmallGoalPtr[this])
    {
        m_passData->units[m_passData->nextSmallGoalPtr[this]].push_back(m_build);
    }

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
            m_timer->reset();
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

        if (m_worker->getPosition().getDistance(m_pos) < m_type == UnitTypes::Zerg_Extractor? 20 : 10)
        {
            m_stage++;
            m_timer->reset();
        }

        if (m_timer->elapsed() > 10)
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not buildable - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }
    }
        break;
    case 5:
    {
        TilePosition buildPos;
        buildPos.x = (m_pos.x - m_type.width() / 2) / 32;
        buildPos.y = (m_pos.y - m_type.height() / 2) / 32;

        if (con->Build(m_worker, m_type, buildPos))
        {
            m_timer->reset();
            m_stage++;

            SG_SITU.UnregisterUnit(m_worker);
            m_worker = nullptr;
        }

        if (m_timer->elapsed() > 3)
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not buildable - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }
    }
        break;
    case 6:
    {

        if (m_timer->elapsed() > 1.5)
        {
            m_stage++;
        }
    }
    break;
    case 7:
    {
        auto buildings = SG_SITU.AllUnitsinRange(true, TilePosition(m_pos), 50);

        for (auto b : buildings)
        {
            if (b->getType() != m_type)
                continue;

            if (b->isMorphing())
            {
                m_build = b;
                SG_SITU.RegisterUnit(m_passData->bigGoalPtr, m_build);
                m_stage++;
                break;
            }
        }

        if (!m_build)
        {
            auto str = m_type.getName();

            SG_LOGMGR.Record("GOAL_EXCEPT", "morphing is unavailable1 - build");
            m_result = GOAL_RESULT_FAILED;
            break;
        }
    }
        break;
    case 8:

        if (!m_build->exists())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "morphing is unavailable2 - build");
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

void SG_Build::Debug(int depth)
{
    SG_DEBUGMGR.DrawTextScn(m_pos, "Build");

    if (m_worker)
    {
        SG_DEBUGMGR.DrawUnit(m_worker, m_debugColor);
    }
    if (m_build)
    {
        SG_DEBUGMGR.DrawUnit(m_build, m_debugColor);
    }

}

void SG_Build::Init()
{
    if (m_isInitialized)
        return;
    m_isInitialized = true;

    if (m_passData->unitTypes[this].empty() || m_passData->poses[this].empty())
    {
        SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - build");
        m_result = GOAL_RESULT_FAILED;
        return;
    }
    m_type = m_passData->unitTypes[this][0];
    auto str = m_type.getName();

    m_pos = m_passData->poses[this][0];

    
    if(m_passData->devs.find(this) != m_passData->devs.end())
        SG_SITU.AddDevUnit(m_passData->devCenter, m_passData->devs[this].first, m_passData->devs[this].second);
}

