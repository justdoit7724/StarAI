#include "pch.h"
#include "SG_Train.h"
#include "DebugManager.h"
#include "SituationManager.h"
#include "Controller.h"
#include "StopWatch.h"
#include "LogManager.h"

SG_Train::SG_Train(GoalIO* passData)
    :SmallGoal(passData, Color(0,255,100))
{
    m_timer = new StopWatch();
}

SG_Train::~SG_Train()
{
    delete m_timer;
}


void SG_Train::Update(const Controller* con)
{
    if (Finished())
        return;

    switch (m_stage)
    {
    case 1:
    {
        const int inc = (m_type == UnitTypes::Zerg_Zergling) ? 2 : 1;

        if (m_count )
        {
            con->Train(m_trainBuilding, m_type);

            m_count -= inc;
            m_timer->reset();
            m_stage++;

        }
        

    }
    break;
    case 2: 
    {
        auto ti = m_timer->elapsed();

        if (m_timer->elapsed() > 0.3)
        {
            m_stage++;
        }
    }
        break;
    case 3:
    {
        auto eggs = SG_SITU.UnitsInRange(true, m_trainBuilding, UnitTypes::Zerg_Egg, 200);

        for (auto e : eggs)
        {
            if (SG_SITU.IsUnitRegistered(e))
                continue;

            SG_SITU.RegisterUnit(m_passData->bigGoalPtr, e);

            m_eggs.push_back(e);
            break;
        }

        if (m_eggs.empty())
        {
            SG_LOGMGR.Record("GOAL_EXCEPT", "not eggs - train");
            m_result = GOAL_RESULT_FAILED;
        }


        m_stage = (m_count <= 0) ? m_stage + 1 : 1;
    }
        break;
    case 4:
    {
        bool isAllComplete = true;
        for (int e=0; e< m_eggs.size(); )
        {
            if (m_eggs[e]->exists() && m_eggs[e]->isMorphing())
            {
                isAllComplete = false;
                break;
            }

            SG_SITU.UnregisterUnit(m_eggs[e]);
            m_eggs.erase(m_eggs.begin() + e);
        }
        if (isAllComplete)
            m_stage++;
    }
        break;
    default:
        m_result = GOAL_RESULT_SUCCESS;
        break;
    }
}

void SG_Train::Debug()
{
    Position pos = m_passData->dbBigGoalPos;
    pos.y += 32;

    SG_DEBUGMGR.DrawTextScn(pos, "Train");
    SG_DEBUGMGR.DrawBox(pos, 70, 20, m_debugColor);

    for (auto e : m_eggs)
    {
        SG_DEBUGMGR.DrawUnit(e, m_debugColor);
    }
}

void SG_Train::Init()
{
    if (m_isInitialized)
        return;
    m_isInitialized = true;

    if (m_passData->units.empty() || m_passData->unitTypes.empty() || m_passData->iValues.empty())
    {
        SG_LOGMGR.Record("GOAL_EXCEPT", "not valid input - train");
        m_result = GOAL_RESULT_FAILED;
        return;
    }
    m_trainBuilding = m_passData->units[0];
    m_type = m_passData->unitTypes[0];
    m_count = m_passData->iValues[0];
}
