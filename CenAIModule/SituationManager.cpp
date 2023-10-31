#include "pch.h"
#include "SituationManager.h"
#include "DebugManager.h"
#include "MapManager.h"
#include "StopWatch.h"


SituationManager::SituationManager()
	:m_plCurTotal(0)
{
	m_mInterval = 20;
	m_secTimer = new StopWatch();
}

SituationManager::~SituationManager()
{
	delete m_secTimer;
}

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

	if (m_secTimer->elapsed() > 1)
	{
		m_secTimer->reset();

		m_minerals.push_back(CurMineral());

		if (m_minerals.size() >= 2)
		{
			m_mDifs.push_back(m_minerals[m_minerals.size()-1] - m_minerals[m_minerals.size()-2]);
		}

		float total = 0; 
		auto it = m_mDifs.rbegin();
		for (int i = 0; i < m_mInterval && it!=m_mDifs.rend(); ++i)
		{
			total += *it;
			it++;
		}
		m_mDifAvgs.push_back(total / m_mInterval);
		
		if (m_minerals.size() > (m_mInterval * 2))
			m_minerals.erase(m_minerals.begin());
		if (m_mDifs.size() > m_mInterval)
			m_mDifs.erase(m_mDifs.begin());
		if (m_mDifAvgs.size() > 50)
			m_mDifAvgs.erase(m_mDifAvgs.begin());


		m_plResourceTotals.push_back(m_plCurTotal);
		if (m_plResourceTotals.size() > 50)
			m_plResourceTotals.erase(m_plResourceTotals.begin());
	}

	int dx = 10;
	int dy = 130;
	for (auto units : m_devUnits)
	{
		if (units.second <= 0)
			continue;
		UnitType ut = (UnitType)units.first;

		std::string str = ut.getName() + ":" + std::to_string(units.second);

		SG_DEBUGMGR.DrawTextFix(dx, dy, str);
		dy += 15;
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

		if (u->isMorphing())
			continue;

		if (u->getType() != type)
			continue;

		return true;
	}

	return false;
}


int SituationManager::OpenMinerals(Unit resourceDepot)
{
	auto mineralUnits = GetMineralsNear(resourceDepot->getPosition());
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
		PI / 5,
		 PI / 7,
		 PI / 9
	};
	const float dist[3] = {
		160, //close
		220,  //far
		280
	};

	std::vector<std::pair<float, float>> ptList;

	for (int i = 0; i < 3; ++i)
	{
		for (float r = 0; r <= PI2; r += rad[i])
		{
			ptList.push_back({dist[i], r});
		}
	}
	for (int i = 0; i < ptList.size() / 2; ++i)
	{
		swap(ptList[rand() % ptList.size()], ptList[rand() % ptList.size()]);
	}
	for (auto& p  : ptList)
	{
		float curX, curY;
		Rotate(pos.x, pos.y, pos.x + p.first, pos.y, p.second, curX, curY);
		Position curPos(curX-32, curY-32);

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

	return false;
}

Unit SituationManager::GetGasNear(Position pos, int range)
{

	for (auto unit : Broodwar->getGeysers()) {
		if (unit->isVisible()) {
			if (unit->getDistance(pos) < range)
			{
				return unit;
				break;
			}
		}
	}

	auto gasUnits = UnitsInRange(true, TilePosition(pos), UnitTypes::Zerg_Extractor, range);

	return gasUnits.size()? gasUnits[0]:nullptr;
}

std::vector<Unit> SituationManager::GetMineralsNear(Position pos, int range)
{
	auto mineralUnits = Broodwar->getUnitsInRadius(pos,range, BWAPI::Filter::IsMineralField);
	std::vector<Unit> ret;
	for (auto m : mineralUnits)
	{
		ret.push_back(m);
	}
	return ret;
}

bool SituationManager::IsBuildable(TilePosition pos, int w, int h, bool isNeedCreep)
{	
	int maxW = Broodwar->mapWidth();
	int maxH = Broodwar->mapHeight();

	bool isAreaBuildable = true;;
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {

			TilePosition mtPos = TilePosition(pos.x + x, pos.y + y);

			if (mtPos.x >= maxW || mtPos.y >= maxH)
				return false;

			if (isNeedCreep && !Broodwar->hasCreep(mtPos))
				return false;
			WalkPosition wPos = WalkPosition(mtPos);
			wPos.x += 1;
			wPos.y += 1;

			if (0 > wPos.x || wPos.x >= (maxW * 4) || 0 > wPos.y || wPos.y >= (maxH * 4))
				return false;

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

void SituationManager::AddDevUnit(UnitType unit, int count)
{
	if (unit == UnitTypes::Zerg_Spawning_Pool)
		int a = 234;

	m_devUnits[unit]+=count;
}
void SituationManager::RemoveDevUnit(UnitType u, int count)
{
	m_devUnits[u]-=count;

	if(m_devUnits[u]<0)
		m_devUnits[u] = 0;
}



int SituationManager::IsDeveloping(UnitType type)
{
	if (m_devUnits[type] > 0)
		return m_devUnits[type];

	return 0;
}

void SituationManager::GetUnitPrice(UnitType type, int& mineral, int& gas)
{
	mineral = type.mineralPrice();
	gas = type.gasPrice();
}

void SituationManager::DisplayMinDif(Position pos)
{
	//axis
	const int h = 40;
	const int w = 200;
	const float center = 0.7;
	const int xUnitCount = 30;
	int wUnit = w * center / 30;
	SG_DEBUGMGR.DrawBoxFix(pos, w+2, h+4, Colors::Green);

	//0
	SG_DEBUGMGR.DrawLineFix(Position(pos.x, pos.y + h / 2), Position(pos.x+w,pos.y+h/2), Colors::Red);

	//cur
	Position curLineCPos = Position(pos.x + w * center, pos.y+h/2);
	SG_DEBUGMGR.DrawLineFix(Position(curLineCPos.x, curLineCPos.y-h/2), Position(curLineCPos.x, curLineCPos.y + h / 2), Colors::Orange);

	//datas
	float curMaxVal = 10;
	for (int i = 0; i<m_mDifAvgs.size() && i < xUnitCount; ++i)
	{
		if (abs(m_mDifAvgs[m_mDifAvgs.size() - 1 - i]) > curMaxVal)
		{
			curMaxVal += 10;
		}
	}

	SG_DEBUGMGR.DrawTextFix(pos.x, pos.y - 13, "Mineral diff avg (-"+ std::to_string((int)curMaxVal)+"~"+ std::to_string((int)curMaxVal)+")");

	int curDrawX = curLineCPos.x;
	Position prevPt = Position(0,0);
	for (int i=m_mDifAvgs.size()-1; i>=0; i--)
	{
		float my = (-m_mDifAvgs[i] / curMaxVal) * h/2;

		Position newPt = Position(curDrawX, curLineCPos.y + my);
		SG_DEBUGMGR.DrawLineFix(newPt, prevPt == Position(0,0)?newPt:prevPt, m_mDifAvgs[i]>=0? Colors::Cyan : Colors::Red);

		prevPt = newPt;

		curDrawX -= wUnit;
		if (curDrawX < pos.x)
			break;
	}

	if (m_mDifAvgs.size())
	{
		float my = (-m_mDifAvgs.back() / curMaxVal) * h / 2;

		Position newPt = Position(curLineCPos.x, curLineCPos.y + my);
		SG_DEBUGMGR.DrawLineFix(newPt, Position(pos.x+w, newPt.y), m_mDifAvgs.back() >= 0 ? Colors::Cyan : Colors::Red);

	}
}

float SituationManager::GetCurMDifAvg()
{
	return m_mDifAvgs.size()? m_mDifAvgs.back() : 0;
}

void SituationManager::DisplayPL(Position pos)
{
	//axis
	const int h = 40;
	const int w = 200;
	const float center = 0.7;
	const int xUnitCount = 30;
	int wUnit = w * center / 30;
	SG_DEBUGMGR.DrawBoxFix(pos, w + 2, h + 4, Colors::Green);

	//0
	SG_DEBUGMGR.DrawLineFix(Position(pos.x, pos.y + h / 2), Position(pos.x + w, pos.y + h / 2), Colors::Red);

	//cur
	Position curLineCPos = Position(pos.x + w * center, pos.y + h / 2);
	SG_DEBUGMGR.DrawLineFix(Position(curLineCPos.x, curLineCPos.y - h / 2), Position(curLineCPos.x, curLineCPos.y + h / 2), Colors::Orange);

	//datas
	float curMaxVal = 100;
	for (int i = 0; i < m_plResourceTotals.size() && i < xUnitCount; ++i)
	{
		auto val = abs(m_plResourceTotals[m_plResourceTotals.size() - 1 - i]);
		auto newMaxVal = (val / 250 + 1) * 250;

		if (newMaxVal > curMaxVal)
		{
			curMaxVal = newMaxVal;
		}
	}

	SG_DEBUGMGR.DrawTextFix(pos.x, pos.y - 13, "Profit & Loss (-" + std::to_string((int)curMaxVal) + "~" + std::to_string((int)curMaxVal) + ")");

	int curDrawX = curLineCPos.x;
	Position prevPt = Position(0, 0);
	for (int i = m_plResourceTotals.size() - 1; i >= 0; i--)
	{
		float my = (-m_plResourceTotals[i] / curMaxVal) * h / 2;

		Position newPt = Position(curDrawX, curLineCPos.y + my);
		SG_DEBUGMGR.DrawLineFix(newPt, prevPt == Position(0, 0) ? newPt : prevPt, m_plResourceTotals[i] >= 0 ? Colors::Cyan : Colors::Red);

		prevPt = newPt;

		curDrawX -= wUnit;
		if (curDrawX < pos.x)
			break;
	}

	if (m_plResourceTotals.size())
	{
		float my = (-m_plResourceTotals.back() / curMaxVal) * h / 2;

		Position newPt = Position(curLineCPos.x, curLineCPos.y + my);
		SG_DEBUGMGR.DrawLineFix(newPt, Position(pos.x + w, newPt.y), m_plResourceTotals.back() >= 0 ? Colors::Cyan : Colors::Red);

	}
}

void SituationManager::UpdatePL(bool ally, UnitType type)
{
	int price =  type.gasPrice() + type.mineralPrice();

	if (ally)
		price *= -1;

	m_plCurTotal += price;
}

int SituationManager::GetPL()
{
	return m_plCurTotal;
}


void SituationManager::ResetPL()
{
	m_plCurTotal = 0;
}
