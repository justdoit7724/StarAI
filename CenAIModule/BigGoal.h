#pragma once
#include "Goal.h"

class SmallGoal;
class Theme;



class BigGoal :
    public Goal
{
public:
    BigGoal() = delete;
    BigGoal(std::string id, Color debugCol);
    virtual ~BigGoal() {}

    virtual void Update(const Controller* con) override;

protected:
    std::vector< SmallGoal*> m_subGoals;

    GoalIO m_passData;

    bool IsGoalExist(std::string id);
};

