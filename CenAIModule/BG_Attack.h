#pragma once
#include "BigGoal.h"
class BG_Attack :
    public BigGoal
{
public:
    BG_Attack(TilePosition pos);
    ~BG_Attack()override;

    void Update(const Controller* con) override;

    void Debug() override;

private:
    bool m_finish;

    TilePosition m_attPos;

};

