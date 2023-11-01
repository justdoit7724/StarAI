#pragma once
#include "Theme.h"

class StopWatch;

class TH_ZerglingBase :
    public Theme
{
public:

    TH_ZerglingBase();
    ~TH_ZerglingBase()override;


    void Update(const Controller* con) override;

    static std::string GetID();

private:
    StopWatch* m_timer;
    StopWatch* m_timer2;

    int m_zerglingCount;
    int m_plCompensate;
    int m_curRD;
};

