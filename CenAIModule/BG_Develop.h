#pragma once
#include "BigGoal.h"

class BG_Develop :
    public BigGoal
{
public:
    BG_Develop(Unit resourceDepot, float e);

    void Update(const Controller* con) override;

    void Debug() override;

    const Unit m_resourceDepot;
    const float m_e;

private:
    bool m_finish;
};

