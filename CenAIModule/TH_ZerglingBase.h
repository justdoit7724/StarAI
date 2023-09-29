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

private:
    StopWatch* m_timer;
};

