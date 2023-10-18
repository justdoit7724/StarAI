#pragma once
#include "Goal.h"

class SmallGoal :
    public Goal
{
public:
    SmallGoal(GoalIO* passData, Color debugCol);

    virtual void Init() { m_isInitialized = true; }


protected:
    GoalIO* m_passData;
    
    bool m_isInitialized;
    bool m_isCleaned;

    unsigned int m_stage;
};

