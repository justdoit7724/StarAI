#pragma once
#include "SmallGoal.h"
class SG_Build :
    public SmallGoal
{
public:
    SG_Build(GoalIO* passData);

    void Update(const Controller* con) override;
    void Debug()override;
    void Init()override;

private:
    Unit m_worker;
    Unit m_build;
    UnitType m_type;
    TilePosition m_tPos;
};

