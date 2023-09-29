#pragma once
#include "Goal.h"

class SmallGoal :
    public Goal
{
public:
    SmallGoal(GoalIO* passData, std::string id, Color debugCol);

    virtual void Init() { m_isInitialized = true; }


protected:
    GoalIO* m_passData;
    
    bool m_isInitialized;

    unsigned int m_stage;
};

