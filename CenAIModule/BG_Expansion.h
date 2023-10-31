#pragma once
#include "BigGoal.h"
class BG_Expansion :
    public BigGoal
{
public:
    BG_Expansion(Position pos);

    void Debug() override;

    virtual std::string ID() const override;

private:
    Position m_pos;
};

