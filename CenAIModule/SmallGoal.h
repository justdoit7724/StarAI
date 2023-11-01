#pragma once
#include "Goal.h"

class SmallGoal :
    public Goal
{
public:
    SmallGoal() = delete;
    SmallGoal(GoalIO* passData, Color debugCol);
    virtual ~SmallGoal() override;

protected:
    GoalIO* m_passData;
    
    bool m_isCleaned;

    unsigned int m_stage;
};

