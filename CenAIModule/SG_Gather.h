#pragma once
#include "SmallGoal.h"

class SG_Gather :
    public SmallGoal
{
public:
    SG_Gather(GoalIO* passData);

    void Update(const Controller* con) override;

    void Debug(int depth = 0) override;

    void Init()override;

private:
    Position m_pos;
};

