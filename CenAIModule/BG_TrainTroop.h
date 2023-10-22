#pragma once
#include "BigGoal.h"
class BG_TrainTroop :
    public BigGoal
{
public:
    BG_TrainTroop(Unit rd, std::vector<Unit> units, std::vector<float> weighs);

    void Debug() override;


private:
    Unit m_rd;
};

