#pragma once

class Controller;
class BigGoal;
class SmallGoal;

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

	virtual void Init() { m_isInitialized = true; }


	const Color m_debugColor;

protected:
	int m_result;
	bool m_isInitialized;
};


struct GoalIO
{
	std::unordered_map<const SmallGoal*, std::vector<Unit>> units;
	std::unordered_map<const SmallGoal* ,std::vector<UnitType>> unitTypes;
	std::unordered_map<const SmallGoal* ,std::vector<UpgradeType>> upgType;
	std::unordered_map<const SmallGoal* ,std::vector<Position>> poses;
	std::unordered_map<const SmallGoal* ,std::vector<int>> iValues;
	std::unordered_map<const SmallGoal* ,std::vector<float>> fValues;
	std::unordered_map<const SmallGoal* ,std::vector<bool>> bValues;
	std::unordered_map<const SmallGoal*, const SmallGoal*> nextSmallGoalPtr;
	std::unordered_map<const SmallGoal*, std::vector<std::pair<int, int>>> devs;
	int count;

	Position dbBigGoalPos;
	BigGoal* bigGoalPtr;
	SmallGoal* nextGoalPtr;
	Position attPos;
};