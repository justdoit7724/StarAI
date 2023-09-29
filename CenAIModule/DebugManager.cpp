#include "pch.h"
#include "DebugManager.h"
#include "SituationManager.h"

void DebugManager::DrawTextFix(int x, int y, std::string str)
{
	Broodwar->drawText(BWAPI::CoordinateType::Screen, x, y, "%s", str.c_str());
}

void DebugManager::DrawTextScn(TilePosition pos, std::string str)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos;
	rpPos.x = pos.x * 32 - scnPos.x;
	rpPos.y = pos.y * 32 - scnPos.y;

	Broodwar->drawText(BWAPI::CoordinateType::Screen, rpPos.x, rpPos.y, "%s", str.c_str());
}

void DebugManager::DrawCircle(TilePosition pos, float radius, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos;
	rpPos.x = pos.x * 32 - scnPos.x;
	rpPos.y = pos.y * 32 - scnPos.y;

	Broodwar->drawCircle(BWAPI::CoordinateType::Screen, rpPos.x, rpPos.y, radius, col);
}

void DebugManager::DrawBox(TilePosition pos, float w, float h, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos;
	rpPos.x = pos.x * 32 - scnPos.x;
	rpPos.y = pos.y * 32 - scnPos.y;

	Broodwar->drawBox(BWAPI::CoordinateType::Screen, rpPos.x, rpPos.y, rpPos.x+w, rpPos.y + h, col);
}

void DebugManager::DrawBox(Position pos, float w, float h, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos;
	rpPos.x = pos.x - scnPos.x;
	rpPos.y = pos.y - scnPos.y;

	Broodwar->drawBox(BWAPI::CoordinateType::Screen, rpPos.x, rpPos.y, rpPos.x + w, rpPos.y + h, col);
}

void DebugManager::DrawLine(TilePosition p1, TilePosition p2, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos1;
	rpPos1.x = p1.x * 32 - scnPos.x;
	rpPos1.y = p1.y * 32 - scnPos.y;
	Position rpPos2;
	rpPos2.x = p2.x * 32 - scnPos.x;
	rpPos2.y = p2.y * 32 - scnPos.y;

	Broodwar->drawLine(BWAPI::CoordinateType::Screen, rpPos1.x, rpPos1.y, rpPos2.x, rpPos2.y,col);
}

void DebugManager::DrawUnit(Unit unit, Color col)
{
	int w = unit->getType().width();
	int h = unit->getType().height();

	auto pos = unit->getPosition();
	pos.x -= w / 2;
	pos.y -= h / 2;

	DrawBox(pos, w, h, col);
}
