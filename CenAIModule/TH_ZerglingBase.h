#pragma once
#include "Theme.h"
class TH_ZerglingBase :
    public Theme
{
public:
    void Update(const Controller* con) override;
};

