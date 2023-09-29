#pragma once
#include "SmallGoal.h"

class SG_StartWork : public SmallGoal
{
public:
	SG_StartWork(GoalIO* passData);

	void Update(const Controller* con) override;
	void Debug()override;
	void Init()override;

private:
	Unit m_rd;
	bool m_isMineral; // mineral(true) or gas(false)
};

