#pragma once
#include "SmallGoal.h"
class SG_Morphing :
    public SmallGoal
{
public:
    SG_Morphing(GoalIO* passData);
    ~SG_Morphing() override;
    void Update(const Controller* con) override;
   
    void Init() override;

private:
    Unit m_unit;
    UnitType m_type;
};

