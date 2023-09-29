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
	Goal(std::string id, Color debugCol = Colors::Black);
	virtual ~Goal();

	virtual void Update(const Controller* con) = 0;
	virtual void Draw() {}
	virtual void Debug();

	virtual void GetParam(int index, void*& data, int& size);

	static void ResetGlobalDebug();
	static void UpdateGlobalDebug();
	static void AddGlobalDebug(std::string str, Color col);
	
	int Finished();

	const std::string m_id;
	const Color m_debugColor;

protected:
	int m_result;
};


struct GoalIO
{
	std::vector<Unit> units;
	std::vector<UnitType> unitTypes;
	std::vector<TilePosition> poses;
	std::vector<int> iValues;
	std::vector<float> fValues;
	std::vector<bool> bValues;

	TilePosition dbBigGoalPos;
	BigGoal* bigGoalPtr;
	Position attPos;
};