#pragma once
#include "Goal.h"

class SmallGoal;
class Theme;



class BigGoal :
    public Goal
{
public:
    BigGoal() = delete;
    BigGoal(Color debugCol);
    virtual ~BigGoal() override;

    virtual void Update(const Controller* con) override;

    void Init() override;

    virtual std::string ID()const = 0;

protected:
    std::vector< SmallGoal*> m_subGoals;

    GoalIO m_passData;

};

