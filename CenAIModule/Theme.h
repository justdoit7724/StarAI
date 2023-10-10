#pragma once
#include "BigGoal.h"

class Controller; 
class BigGoal;

class Theme
{
public:
	Theme() 
	{

	}
	virtual ~Theme() {}
	virtual void Update(const Controller* con);
	

protected:
	std::unordered_map<std::string, std::vector<BigGoal*>> m_goals;

	bool IsGoalExist(std::string id);
	void AddGoal(std::string name, BigGoal* goal);

};

