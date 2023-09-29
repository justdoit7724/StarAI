#pragma once
#include "SmallGoal.h"

class SG_Attack :
    public SmallGoal
{
public:
    SG_Attack(GoalIO* passData);

    void Update(const Controller* con) override;

    void Debug()override;

    void Init()override;

private:
    Position m_attPos;
};

