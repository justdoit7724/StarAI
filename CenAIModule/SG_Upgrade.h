#pragma once
#include "SmallGoal.h"

class StopWatch;

class SG_Upgrade :
    public SmallGoal
{
    void Update(const Controller* con) override;

    void Init()override;

private:
    Unit m_unit;
    UpgradeType m_type;
    StopWatch* m_timer;
};

