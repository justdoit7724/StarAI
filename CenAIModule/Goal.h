#pragma once

class Controller;
class BigGoal;

#define GOAL_RESULT_FAILED -1
#define GOAL_RESULT_SUCCESS 1
#define GOAL_RESULT_PROCESSING 0

class Goal
{
public:
	Goal() = delete;
	Goal(Color debugCol = Colors::Black);
	virtual ~Goal();

	virtual void Update(const Controller* con) = 0;
	virtual void Draw() {}
	virtual void Debug();

	virtual void GetParam(int index, void*& data, int& size);

	static void ResetGlobalDebug();
	static void UpdateGlobalDebug();
	static void AddGlobalDebug(std::string str, Color col);
	
	int Finished();

	const Color m_debugColor;

protected:
	int m_result;
};


struct GoalIO
{
	std::queue<Unit> units;
	std::queue<UnitType> unitTypes;
	std::queue<UpgradeType> upgType;
	std::queue<Position> poses;
	std::queue<int> iValues;
	std::queue<float> fValues;
	std::queue<bool> bValues;
	int count;

	Position dbBigGoalPos;
	BigGoal* bigGoalPtr;
	Position attPos;
};