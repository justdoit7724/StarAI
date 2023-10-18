#pragma once
#include "BigGoal.h"

enum class BG_DevelopeKind
{
    None,
    Train_Supply,
    Train_Troop,
    Gathering,
    Build_Tech,
    Build_Defense
};

class BG_Develop :
    public BigGoal
{
public:
    BG_Develop(Unit resourceDepot, float e);

    void Update(const Controller* con) override;

    void Debug() override;

    std::string ID()const override;

    const Unit m_resourceDepot;
    const float m_e;

    static std::string ID(BG_DevelopeKind kind);

private:
    bool m_finish;

    BG_DevelopeKind m_devKind;
};

