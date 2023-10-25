#include "pch.h"
#include "DebugManager.h"
#include "SituationManager.h"

void DebugManager::DrawTextFix(int x, int y, std::string str)
{
	Broodwar->drawText(BWAPI::CoordinateType::Screen, x, y, "%s", str.c_str());
}

void DebugManager::DrawTextScn(Position pos, std::string str)
{
	auto scnPos = Broodwar->getScreenPosition();

	pos.x -= scnPos.x;
	pos.y -= scnPos.y;

	Broodwar->drawText(BWAPI::CoordinateType::Screen, pos.x, pos.y, "%s", str.c_str());
}

void DebugManager::DrawCircle(Position pos, float radius, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	pos.x -= scnPos.x;
	pos.y -= scnPos.y;

	Broodwar->drawCircle(BWAPI::CoordinateType::Screen, pos.x, pos.y, radius, col);
}

void DebugManager::DrawBox(TilePosition pos, float w, float h, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos;
	rpPos.x = pos.x * 32 - scnPos.x;
	rpPos.y = pos.y * 32 - scnPos.y;

	DrawBoxFix(rpPos, w, h, col);
}

void DebugManager::DrawBox(Position pos, float w, float h, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos;
	rpPos.x = pos.x - scnPos.x;
	rpPos.y = pos.y - scnPos.y;

	DrawBoxFix(rpPos, w, h, col);
}

void DebugManager::DrawBoxFix(Position pos, int w, int h, Color col)
{

	Broodwar->drawBox(BWAPI::CoordinateType::Screen, pos.x, pos.y, pos.x + w, pos.y + h, col);

}

void DebugManager::DrawLine(Position p1, Position p2, Color col)
{
	auto scnPos = Broodwar->getScreenPosition();

	Position rpPos1;
	rpPos1.x = p1.x - scnPos.x;
	rpPos1.y = p1.y - scnPos.y;
	Position rpPos2;
	rpPos2.x = p2.x - scnPos.x;
	rpPos2.y = p2.y - scnPos.y;

	DrawLineFix(rpPos1, rpPos2, col);
}

void DebugManager::DrawLineFix(Position p1, Position p2, Color col)
{
	Broodwar->drawLine(BWAPI::CoordinateType::Screen, p1.x, p1.y, p2.x, p2.y, col);
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

void DebugManager::DrawDotFix(Position p, Color col)
{
	Broodwar->drawDot(CoordinateType::Screen, p.x, p.y, col);
}
