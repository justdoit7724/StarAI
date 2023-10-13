#include "pch.h"
#include "SituationManager.h"
#include "DebugManager.h"



void SituationManager::Update()
{
	for (auto it = m_regUnits.begin(); it!= m_regUnits.end(); ++it)
	{
		for (auto u : it->second)
		{
			if(!u->exists())
				UnregisterUnit(u);
		}
	}
}

bool SituationManager::IsExist(bool isAlly, BWAPI::UnitType type)
{
	auto unitTeam = isAlly ? Broodwar->self() : Broodwar->enemy();

	auto units = unitTeam->getUnits();

	std::vector<Unit> ret;
	for (auto u : units)
	{
		if (!u->exists())
			continue;

		if (u->getType() != type)
			continue;

		return true;
	}

	return false;
}

int SituationManager::OpenMinerals(Unit resourceDepot)
{
	auto mineralUnits = resourceDepot->getUnitsInRadius(300, BWAPI::Filter::IsMineralField); 
	auto workerUnits = resourceDepot->getUnitsInRadius(300, BWAPI::Filter::IsWorker);

	int workingCount = 0;
	for (auto w : workerUnits)
	{
		if (!w->exists())
			continue;

		if (IsUnitRegistered(w))
			continue;

		if (!w->isIdle())
		{
			workingCount++;
		}
	}

	int val = mineralUnits.size() - workingCount;
	if (val < 0)
		val = 0;

	return val;
}

bool SituationManager::GetOpenPositionNear(TilePosition pos, TilePosition& outPos)
{
	const int LeastDistFromResource = 125;

	outPos.x = -1;
	outPos.y = -1;

	Unit gas=nullptr;
	for (auto unit : Broodwar->getGeysers()) {
		if (unit->isVisible()) {
			if (unit->getDistance(Position(pos.x * 32 + 16, pos.y * 32 + 16)) < 200)
			{
				gas = unit;
				break;
			}
		}
	}

	const float PI = 3.14;
	const float PI2 = 2 * PI;
	const float rad2 = PI / 8;
	const float dist2 = 5;
	for (float r = 0; r <= PI2; r += rad2)
	{
		float curX, curY;
		Rotate(pos.x, pos.y, pos.x + dist2, pos.y, r, curX, curY);

		if (!IsBuildable(TilePosition(curX -1, curY -1),3,3))
			continue;

		TilePosition openPos = TilePosition(curX, curY);
		Position pPos = Position(openPos.x * 32 + 16, openPos.y * 32 + 16);
		auto resourcesInRange = BWAPI::Broodwar->getUnitsInRadius(pPos, LeastDistFromResource);
		bool isNearResource = false;
		for (auto resource : resourcesInRange) {


			if (resource->getType().isMineralField() || resource->getType().isRefinery()) {

				isNearResource = true;
				break;
			}
		}
		if (gas && gas->getDistance(pPos) < LeastDistFromResource)
			continue;

		if (isNearResource)
			continue;


		outPos= TilePosition(curX, curY);

		return true;
	}
	const float rad1 = PI / 6;
	const float dist1 = 4;
	for (float r = 0; r <= PI2; r += rad1)
	{
		float curX, curY;
		Rotate(pos.x, pos.y, pos.x + dist1, pos.y, r, curX, curY);
		if (!IsBuildable(TilePosition(curX - 1, curY - 1), 3, 3))
			continue;

		TilePosition openPos = TilePosition(curX, curY);
		Position pPos = Position(openPos.x * 32 + 16, openPos.y * 32 + 16);
		auto resourcesInRange = BWAPI::Broodwar->getUnitsInRadius(pPos, LeastDistFromResource);
		bool isNearResource = false;
		for (auto resource : resourcesInRange) {

			if (resource->getType().isMineralField() || resource->getType().isRefinery()) {

				isNearResource = true;
				break;
			}
		}
		if (gas && gas->getDistance(pPos) < LeastDistFromResource)
		{
			continue;
		}

		if (isNearResource)
			continue;




		outPos= TilePosition(curX, curY);
		return true;

	}
	const float rad3 = PI / 10;
	const float dist3 = 6;
	for (float r = 0; r <= PI2; r += rad3)
	{
		float curX, curY;
		Rotate(pos.x, pos.y, pos.x + dist3, pos.y, r, curX, curY);
		if (!IsBuildable(TilePosition(curX - 1, curY - 1), 3, 3))
			continue;

		TilePosition openPos = TilePosition(curX, curY);
		Position pPos = Position(openPos.x * 32 + 16, openPos.y * 32 + 16);
		auto resourcesInRange = BWAPI::Broodwar->getUnitsInRadius(pPos, LeastDistFromResource);
		bool isNearResource = false;
		for (auto resource : resourcesInRange) {

			if (resource->getType().isMineralField() || resource->getType().isRefinery()) {

				isNearResource = true;
				break;
			}
		}
		if (gas && gas->getDistance(pPos) < LeastDistFromResource)
		{
			continue;
		}

		if (isNearResource)
			continue;


		outPos= TilePosition(curX, curY);
		return true;
	}

	return false;
}

bool SituationManager::IsBuildable(TilePosition ltPos, int w, int h)
{	
	int maxW = Broodwar->mapWidth();
	int maxH = Broodwar->mapHeight();

	bool isAreaBuildable = true;;
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {

			int mx = ltPos.x + x;
			int my = ltPos.y + y;

			if (mx >= maxW || my >= maxH)
				return false;

			if (!BWAPI::Broodwar->isBuildable(mx, my) /*||
				!BWAPI::Broodwar->getUnitsOnTile(mx, my, BWAPI::Filter::IsMineralField).empty() ||
				!BWAPI::Broodwar->getUnitsOnTile(mx, my).empty() ||
				!BWAPI::Broodwar->getUnitsOnTile(mx, my, BWAPI::Filter::IsBuilding).empty()*/) {
				return false;
			}
		}
	}

	return true;
}


void SituationManager::RegisterUnit(const BigGoal* goal, Unit unit)
{
	assert(!IsUnitRegistered(unit));

	m_regUnits[goal].insert(unit);
;}

void SituationManager::UnregisterUnit(Unit unit)
{
	assert(IsUnitRegistered(unit));

	for (auto g = m_regUnits.begin(); g!= m_regUnits.end(); ++g)
	{
		if (g->second.find(unit) == g->second.end())
			continue;

		g->second.erase(unit);
		break;
	}

	assert(!IsUnitRegistered(unit));

}


void SituationManager::UnregisterGoal(const BigGoal* goal)
{
}

bool SituationManager::IsUnitRegistered(Unit unit)
{
	for (auto it : m_regUnits)
	{
		if (it.second.find(unit) != it.second.end())
			return true;
	}

	return false;
}
std::unordered_set<Unit> SituationManager::GetRegistered(const BigGoal* goal)
{
	return m_regUnits[goal];
}
std::vector<Unit> SituationManager::UnitsInRange(bool isAlly, TilePosition center, UnitType type, int range)
{
	auto unitTeam = isAlly ? Broodwar->self() : Broodwar->enemy();

	auto units = unitTeam->getUnits();

	Position mPos = Position(center);
	mPos.x += 16;
	mPos.y += 16;

	std::vector<Unit> ret;
	for (auto u : units)
	{
		if(!u->exists())
			continue;

		if (u->getDistance(mPos) > range)
			continue;

		if (u->getType() != type)
			continue;

		ret.push_back(u);
	}

	return ret;
}

std::vector<Unit> SituationManager::UnitsInRange(bool isAlley, Unit centerUnit, UnitType type, int range)
{
	return UnitsInRange(isAlley, GetCenterTPos(centerUnit), type, range);
}

std::vector<Unit> SituationManager::AllUnitsinRange(bool isAlly, TilePosition center, int range)
{
	auto unitTeam = isAlly ? Broodwar->self() : Broodwar->enemy();

	auto units = unitTeam->getUnits();

	Position mPos = Position(center);
	mPos.x += 16;
	mPos.y += 16;

	std::vector<Unit> ret;
	for (auto u : units)
	{
		if (!u->exists())
			continue;

		if (u->getDistance(mPos) > range)
			continue;

		ret.push_back(u);
	}

	return ret;
}

TilePosition SituationManager::GetCenterTPos(Unit unit)
{
	BWAPI::Position unitPosition = unit->getPosition();

	// Calculate the center point of the unit
	int center_x = unitPosition.x + unit->getType().dimensionLeft()/2;
	int center_y = unitPosition.y + unit->getType().dimensionUp()/2;

	// Convert the center point to a TilePosition
	BWAPI::TilePosition centerTilePosition(center_x / 32, center_y / 32);

	return centerTilePosition;
}

int SituationManager::CurMineral()
{
	return   Broodwar->self()->gatheredMinerals() - Broodwar->self()->spentMinerals();
}

int SituationManager::CurGas()
{
	return   Broodwar->self()->gatheredGas() - Broodwar->self()->spentGas();
}

