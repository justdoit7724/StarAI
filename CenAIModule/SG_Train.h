#pragma once
#include "SmallGoal.h"

class StopWatch;

class SG_Train :
    public SmallGoal
{
public:
    SG_Train(GoalIO* passData);
    ~SG_Train()override;

    void Update(const Controller* con) override;
    void Debug()override;
    void Init()override;
private:
    std::vector<Unit> m_eggs;
    Unit m_trainBuilding;
    UnitType m_type;
    int m_count;
    StopWatch* m_timer;

};

