#pragma once
#include "SmallGoal.h"

class SG_CalcAttGatherPos :
    public SmallGoal
{
public:
    SG_CalcAttGatherPos(GoalIO* passData);
    void Update(const Controller* con) override;

    void Debug(int depth = 0)override;

    void Init()override;

    Position m_attPos;
};

