#pragma once
#include "SmallGoal.h"

class StopWatch;

class SG_Build :
    public SmallGoal
{
public:
    SG_Build(GoalIO* passData);
    ~SG_Build();

    void Update(const Controller* con) override;
    void Debug()override;
    void Init()override;

private:
    Unit m_worker;
    Unit m_build;
    UnitType m_type;
    TilePosition m_tPos;

    StopWatch* m_timer;
};

