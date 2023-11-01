#pragma once
#include "SmallGoal.h"
class SG_Recruit :
    public SmallGoal
{
public:
    SG_Recruit(GoalIO* passData);
    void Update(const Controller* con) override;

    void Debug(int depth = 0)override;

    void Init()override;

private:
    std::vector<int> m_unitCounts;
    std::vector<UnitType> m_units;

};

