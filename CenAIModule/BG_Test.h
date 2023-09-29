#pragma once
#include "BigGoal.h"
#include "StopWatch.h"

class BG_Test :
    public BigGoal
{
public:
    BG_Test();

    void Update(const Controller* con) override;

    void Debug() override;


};

