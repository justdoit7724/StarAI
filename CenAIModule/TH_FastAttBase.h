#pragma once
#include "Theme.h"

class StopWatch;

class TH_FastAttBase :
    public Theme
{
public:
    TH_FastAttBase();
    ~TH_FastAttBase() override;
    void Update(const Controller* con)override;

    static std::string GetID();

private:

    int m_totalTroopCount;
    StopWatch* m_timer;
};

