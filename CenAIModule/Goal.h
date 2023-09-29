#pragma once

class Controller;
class BigGoal;

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
	
	bool Finished();

	const std::string m_id;
	const Color m_debugColor;

protected:
	bool m_isFinished;
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