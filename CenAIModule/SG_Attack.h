#pragma once
#include "SmallGoal.h"

class SG_Attack :
    public SmallGoal
{
public:
    SG_Attack(GoalIO* passData);
    ~SG_Attack() override;

    void Update(const Controller* con) override;

    void Debug(int depth = 0)override;

    void Init()override;

private:
    Position m_attPos;
};

