#include "pch.h"
#include "TechTree.h"
#include "SituationManager.h"

TechTreeZerg::TechTreeZerg()
{
    using namespace UnitTypes;

    m_root = new Tree<TechNode>(None);
    m_lookup[None.getName()] = m_root;
    
    AddTree(Zerg_Hatchery);
    AddTree(Zerg_Extractor);
    AddTree(Zerg_Creep_Colony);
    AddTree(Zerg_Evolution_Chamber, Zerg_Hatchery);
    AddTree(Zerg_Spawning_Pool, Zerg_Hatchery);
    AddTree(Zerg_Zergling, Zerg_Spawning_Pool);
    AddTree(Zerg_Spore_Colony, Zerg_Evolution_Chamber);
    AddTree(Zerg_Sunken_Colony, Zerg_Spawning_Pool);


}

bool TechTreeZerg::IsValid(TechNode tech, TechNode* req)
{
    auto curTechTr = m_lookup[tech.Key()];
    assert(curTechTr != nullptr);

    for (auto required : curTechTr->m_children)
    {
        if (required->m_data.isUnit)
        {
            if (!SG_SITU.IsExist(true, required->m_data.unitType))
            {
                if(req)
                    *req = required->m_data;
                return false;
            }
        }
        else
        {
            if (!Broodwar->self()->getUpgradeLevel(required->m_data.upgType))
            {
                if (req)
                    *req = required->m_data;
                return false;
            }
        }

    }
    return true;
}

void TechTreeZerg::GetTotalReqRes(TechNode tech, int& mineral, int& gas)
{
    auto curTechTr = m_lookup[tech.Key()];
    assert(curTechTr != nullptr);

    mineral = 0;
    gas = 0;

    if (curTechTr == m_root)
        return;

    for (auto required : curTechTr->m_children)
    {
        if (required->m_data.isUnit)
        {
            if (SG_SITU.IsExist(true, required->m_data.unitType))
            {
                continue;
            }
            else
            {
                int subMineral, subGas;
                GetTotalReqRes(required->m_data.unitType, subMineral,subGas);

                mineral += required->m_data.unitType.mineralPrice() + subMineral;
                gas += required->m_data.unitType.gasPrice() + subGas;
            }
        }
        else
        {
            if (Broodwar->self()->getUpgradeLevel(required->m_data.upgType))
            {
                continue;
            }
            else
            {
                int subMineral, subGas;
                GetTotalReqRes(required->m_data.upgType, subMineral, subGas);

                mineral += required->m_data.upgType.mineralPrice() + subMineral;
                gas += required->m_data.upgType.gasPrice() + subGas;
            }
        }

    }
}


void TechTreeZerg::AddTree(TechNode tech, TechNode req)
{
    assert(m_lookup.find(req.Key()) != m_lookup.end());
    assert(m_lookup.find(tech.Key()) == m_lookup.end());

    auto newTechTr = new Tree<TechNode>(tech);
    m_lookup[tech.Key()] = newTechTr;

    auto reqTechTr = m_lookup[req.Key()];
    newTechTr->m_children.push_back(reqTechTr);
}

