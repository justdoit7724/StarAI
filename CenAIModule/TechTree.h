#pragma once
#include "Singleton.h"

template<typename T>
class Tree
{
public:
	Tree(T data) :m_data(data) {}

	const T m_data;
	std::vector<Tree*> m_children;
};

struct TechNode
{
	bool isUnit;
	union {
		BWAPI::UpgradeType upgType;
		BWAPI::UnitType unitType;
	};

	TechNode() :isUnit(false), unitType(UnitTypes::None) {}
	TechNode(BWAPI::UpgradeType type) :isUnit(false), upgType(type) {}
	TechNode(BWAPI::UnitType type) :isUnit(true), unitType(type) {}

	std::string Key()
	{
		return isUnit ? unitType.getName() : upgType.getName();
	}
};

#define SG_TECH TechTreeZerg::Instance()
class TechTreeZerg : public Singleton< TechTreeZerg>
{
public:
	TechTreeZerg();
		
	bool IsValid(TechNode tech, TechNode* req=nullptr);
	void GetTotalReqRes(TechNode tech, int& mineral, int& gas);

private:
	Tree<TechNode>* m_root;
	
	std::unordered_map<std::string, Tree<TechNode>*> m_lookup;

	void AddTree(TechNode tech,TechNode req=UnitTypes::None);
};


