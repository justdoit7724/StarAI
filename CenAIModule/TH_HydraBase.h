#pragma once
#include "Theme.h"

class StopWatch;

class TH_HydraBase :
    public Theme
{
public:
    TH_HydraBase();
    ~TH_HydraBase() override;
    void Update(const Controller* con)override;

    static std::string ID();

private:

    StopWatch* m_timer;
    StopWatch* m_timer2;
    int m_plCompensate;
    int m_hydraCount;
};

