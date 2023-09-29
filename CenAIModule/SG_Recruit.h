#pragma once
#include "SmallGoal.h"
class SG_Recruit :
    public SmallGoal
{
public:
    SG_Recruit(GoalIO* passData);
    void Update(const Controller* con) override;

    void Debug()override;

    void Init()override;

private:
    int m_num;

};

