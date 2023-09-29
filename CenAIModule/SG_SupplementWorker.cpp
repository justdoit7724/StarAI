#include "pch.h"
#include "SG_SupplementWorker.h"


SG_SupplementWorker::SG_SupplementWorker()
    :SmallGoal("SG_SupplementWorker", Colors::Green)
{
    m_rd = g_data.units[0];
}
void SG_SupplementWorker::Update(const Controller* con)
{
    if (timer.elapsed() > 10000)
    {
        m_isFinished = true;
    }
}

void SG_SupplementWorker::Debug()
{
}

