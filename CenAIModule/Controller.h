#pragma once

class BigGoal;

class Controller
{
public:
	void Attack(Unit unit, Position pos) const;
	bool Move(Unit unit, TilePosition pos) const;
	bool Move(Unit unit, Position pos)const;
	bool Build(Unit worker, UnitType build, TilePosition pos) const;
	bool Train(Unit building, UnitType type)const;
	void ResourceWork(Unit worker, Unit resource)const;

private:
	std::vector< BigGoal*> m_BGoals;
};

