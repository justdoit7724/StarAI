#include "pch.h"
#include "Goal.h"
#include "SituationManager.h"

std::vector<int> globalDebugX;
std::vector<int> globalDebugY;
std::vector<Color> globalDebugCol;
std::vector<std::string> globalDebugStr;

Goal::Goal(Color debugCol)
	:m_debugColor(debugCol), m_result(GOAL_RESULT_PROCESSING)
{
}

Goal::~Goal()
{
}

int Goal::Finished()
{
	return m_result!=GOAL_RESULT_PROCESSING;
}

void Goal::Debug()
{
}

void Goal::GetParam(int index, void*& data, int& size)
{
	data = nullptr;
	size = 0;
}

void Goal::ResetGlobalDebug()
{
	globalDebugX.clear();
	globalDebugY.clear();
	globalDebugCol.clear();
	globalDebugStr.clear();
}

void Goal::UpdateGlobalDebug()
{
    for (int i = 0; i < globalDebugX.size(); ++i)
    {
        Broodwar->drawText(BWAPI::CoordinateType::Screen, globalDebugX[i], globalDebugY[i], "%s",globalDebugStr[i].c_str());
    }
}

void Goal::AddGlobalDebug(std::string str, Color col)
{
	globalDebugX.push_back(30);
	globalDebugY.push_back(30 + globalDebugY.size() * 15);
	globalDebugCol.push_back(col);
	globalDebugStr.push_back(str);
}

