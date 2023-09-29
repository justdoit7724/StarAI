#include "pch.h"
#include "Controller.h"
#include "BigGoal.h"

void Controller::Attack(Unit unit, Position pos)const
{
	unit->attack(pos);
}

bool Controller::Move(Unit unit, TilePosition pos)const
{
	Position pPos = Position(pos);
	pPos.x += 16;
	pPos.y += 16;

	return unit->move(pPos);
}

bool Controller::Move(Unit unit, Position pos) const
{
	return unit->move(pos);
}

bool Controller::Build(Unit worker, UnitType build, TilePosition pos)const
{
	return worker->build(build, pos);
}

bool Controller::Train(Unit building, UnitType type) const
{
	return building->train(type);
}

void Controller::ResourceWork(Unit worker, Unit resource)const
{
	worker->gather(resource);
}
