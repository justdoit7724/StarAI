#pragma once
#include "Theme.h"
class TH_HydraBase :
    public Theme
{
public:
    TH_HydraBase();
    void Update(const Controller* con)override;

    static std::string ID();
};

