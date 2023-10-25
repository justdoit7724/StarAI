#pragma once
#include "BigGoal.h"

class Controller; 
class BigGoal;

class Theme
{
public:
	Theme(std::string id);
	virtual ~Theme();
	virtual void Update(const Controller* con);

	const std::string ID;
	
protected:
	std::unordered_map<std::string, std::vector<BigGoal*>> m_goals;

	bool IsGoalExist(const BigGoal* goal);
	void AddGoal(BigGoal* goal);

};

