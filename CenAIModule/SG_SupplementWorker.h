#pragma once
#include "SmallGoal.h"
#include "StopWatch.h"
class SG_SupplementWorker :
    public SmallGoal
{
public:
    SG_SupplementWorker();

    void Update(const Controller* con) override;
    void Debug()override;


private:
    Unit m_rd;
    StopWatch timer;
};

