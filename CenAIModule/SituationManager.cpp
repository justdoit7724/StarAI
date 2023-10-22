#include "pch.h"
#include "SituationManager.h"
#include "DebugManager.h"
#include "MapManager.h"


void SituationManager::Update()
{
	std::vector<Unit> deleList;

	for (auto it = m_regUnits.begin(); it!= m_regUnits.end(); ++it)
	{
		for (auto u : it->second)
		{
			if (!u->exists())
			{
				deleList.push_back(u);
			}
		}
	}

	for (auto d : deleList)
	{
		UnregisterUnit(d);
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

bool SituationManager::GetOpenPositionNear(Position pos, Position& outPos)
{
	const int LeastDistFromResource = 96;

	outPos.x = -1;
	outPos.y = -1;

	Unit gas = GetGasNear(pos);

	const float PI = 3.14;
	const float PI2 = 2 * PI;
	const float rad[3] = {
		PI / 6,
		 PI / 4,
		 PI / 7
	};
	const float dist[3] = {
		160, //mid
		128, //close
		192  //far
	};
	for (int i = 0; i < 3; ++i)
	{
		for (float r = 0; r <= PI2; r += rad[i])
		{
			float curX, curY;
			Rotate(pos.x, pos.y, pos.x + dist[i], pos.y, r, curX, curY);
			Position curPos(curX, curY);

			if (!IsBuildable(TilePosition(curPos), 3, 3))
				continue;

			//resource skip
			auto resourcesInRange = BWAPI::Broodwar->getUnitsInRadius(curPos, LeastDistFromResource);
			bool isNearResource = false;
			for (auto resource : resourcesInRange) {


				if (resource->getType().isMineralField() || resource->getType().isRefinery()) {

					isNearResource = true;
					break;
				}
			}
			if (isNearResource)
				continue;
			if (gas && gas->getDistance(curPos) < LeastDistFromResource)
				continue;




			outPos = curPos;

			return true;
		}
	}

	return false;
}

Unit SituationManager::GetGasNear(Position pos)
{
	for (auto unit : Broodwar->getGeysers()) {
		if (unit->isVisible()) {
			if (unit->getDistance(pos) < 300)
			{
				return unit;
				break;
			}
		}
	}

	return nullptr;
}

bool SituationManager::IsBuildable(TilePosition pos, int w, int h)
{	
	int maxW = Broodwar->mapWidth();
	int maxH = Broodwar->mapHeight();

	bool isAreaBuildable = true;;
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {

			int mx = pos.x + x;
			int my = pos.y + y;

			if (mx >= maxW || my >= maxH)
				return false;

			WalkPosition wPos = WalkPosition(TilePosition(mx, my));
			wPos.x += 1;
			wPos.y += 1;


			int curTerr = SG_MAP.GetTerrain(wPos);
			if (((curTerr & MAP_BUILDABLE) != MAP_BUILDABLE) ||
				((curTerr & MAP_UNIT) == MAP_UNIT))
				return false;


		}
	}

	return true;
}


void SituationManager::RegisterUnit(const BigGoal* goal, Unit unit)
{
	assert(!IsUnitRegistered(unit));

	if (!unit)
	{

	}

	m_regUnits[goal].insert(unit);
;}

void SituationManager::UnregisterUnit(Unit unit)
{
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

int SituationManager::GetValidSupply()
{
	return Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();
}

void SituationManager::AddDevUnit(UnitType type)
{
	m_devUnits.insert(type);
}

void SituationManager::RemoveDevUnit(UnitType type)
{
	m_devUnits.erase(type);
}


bool SituationManager::IsDeveloping(UnitType type)
{
	return m_devUnits.find(type) != m_devUnits.end();
}

void SituationManager::GetUnitPrice(UnitType type, int& mineral, int& gas)
{
	mineral = type.mineralPrice();
	gas = type.gasPrice();
}
