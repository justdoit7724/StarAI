#pragma once
#include "BigGoal.h"
class BG_Attack :
    public BigGoal
{
public:
    BG_Attack(Position pos, std::vector<UnitType> units, std::vector<int> unitCounts);
    ~BG_Attack()override;

    void Update(const Controller* con) override;

    void Debug() override;

    std::string ID()const override;

private:
    bool m_finish;

    Position m_attPos;

};

