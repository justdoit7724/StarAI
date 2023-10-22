#include "pch.h"
#include "CenAIModule.h"
#include <iostream>
#include <fstream>
#include "MapManager.h"
#include "TechTree.h"
#include "BG_Attack.h"
#include "SituationManager.h"
#include "DebugManager.h"

using namespace BWAPI;
using namespace Filter;

void CenAIModule::onStart()
{
    m_brain = std::make_unique< Brain>();
    m_controller = std::make_unique< Controller>();

    Broodwar->sendText("black sheep wall");

    // Enable the UserInput flag, which allows us to control the bot and type messages.
    Broodwar->enableFlag(Flag::UserInput);

    // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
    Broodwar->enableFlag(Flag::CompleteMapInformation);

    // Set the command optimization level so that common commands can be grouped
    // and reduce the bot's APM (Actions Per Minute).
    Broodwar->setCommandOptimizationLevel(2);

    Broodwar->setLocalSpeed(4);

    srand(time(nullptr));

}

void CenAIModule::onEnd(bool isWinner)
{


}




void CenAIModule::onFrame()
{
    Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
    Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

#ifndef _DEBUG
    if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
        return;
#endif

    Goal::ResetGlobalDebug();


    SG_MAP.Update();
    SG_SITU.Update();
    m_brain->Update(m_controller.get());




    
    Goal::UpdateGlobalDebug();
}

void CenAIModule::onSendText(std::string text)
{
}

void CenAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
    // Parse the received text
    Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void CenAIModule::onPlayerLeft(BWAPI::Player player)
{
}

void CenAIModule::onNukeDetect(BWAPI::Position target)
{
}

void CenAIModule::onUnitDiscover(BWAPI::Unit unit)
{
}

void CenAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void CenAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void CenAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void CenAIModule::onUnitCreate(BWAPI::Unit unit)
{
}

void CenAIModule::onUnitDestroy(BWAPI::Unit unit)
{
}

void CenAIModule::onUnitMorph(BWAPI::Unit unit)
{
}

void CenAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void CenAIModule::onSaveGame(std::string gameName)
{
}

void CenAIModule::onUnitComplete(BWAPI::Unit unit)
{
}



/*

        if (u->getType().isWorker())
        {
            if (u->isIdle())
            {
                if (u->isCarryingGas() || u->isCarryingMinerals())
                {
                    u->returnCargo();
                }
                else
                {
                    if (!u->gather(u->getClosestUnit(IsMineralField || IsRefinery)))
                    {
                        Broodwar << Broodwar->getLastError() << std::endl;
                    }
                }
            }

        }
        else if (u->getType() == UnitTypes::Zerg_Overlord)
        {
            if (u->isMoving())
                continue;

            static int dir = 0;

            int mapWidth = Broodwar->mapWidth();
            int mapHeight = Broodwar->mapHeight();

            switch (dir)
            {
            case 0:
            {
                Position pos = u->getPosition();
                if (pos.x < abs(mapWidth - pos.x))
                {
                    pos.x = mapWidth - 200;
                }
                else
                {
                    pos.x = 200;
                }
                u->move(pos);
                dir++;
            }
                break;
            case 1:
            {
                Position pos = u->getPosition();
                if (pos.y < abs(mapHeight - pos.y))
                {
                    pos.y = mapHeight - 200;
                }
                else
                {
                    pos.y = 200;
                }
                u->move(pos);
                dir++;
            }
                break;
            case 2:
                break;
            }

        }
        else if (u->getType().isResourceDepot())
        {
            if (scoutDestPos.isValid())
                continue;

            resourceDepot = u;

            scoutDestPos = u->getTilePosition();

            
            if (u->isIdle() && !u->train(u->getType().getRace().getWorker()))
            {
                Position pos = u->getPosition();
                Error lastErr = Broodwar->getLastError();
                Broodwar->registerEvent(
                    [pos, lastErr](Game*) {
                        Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str());
                    },
                    nullptr,
                        Broodwar->getLatencyFrames());

                UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
                static int lastChecked = 0;

                if (lastErr == Errors::Insufficient_Supply &&
                    lastChecked + 400 < Broodwar->getFrameCount() &&
                    Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
                {
                    lastChecked = Broodwar->getFrameCount();

                    Unit supplyBuilder = u->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
                        (IsIdle || IsGatheringMinerals) &&
                        IsOwned);

                    if (supplyBuilder)
                    {
                        if (supplyProviderType.isBuilding())
                        {
                            TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
                            if (targetBuildLocation)
                            {
                                Broodwar->registerEvent(
                                    [targetBuildLocation, supplyProviderType](Game*)
                                    {
                                    Broodwar->drawBoxMap(
                                            Position(targetBuildLocation),
                                            Position(targetBuildLocation + supplyProviderType.tileSize()),
                                            Colors::Blue);
                                    },
                                    nullptr,
                                        supplyProviderType.buildTime() + 100);

                                supplyBuilder->build(supplyProviderType, targetBuildLocation);
                            }
                        }
                        else
                        {
                            supplyBuilder->train(supplyProviderType);
                        }
                    }
                }
            }
        }




        for (auto& u : Broodwar->self()->getUnits())
    {
        if (!u->exists())
            continue;

        if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
            continue;

        if (u->isLoaded() || !u->isPowered() || u->isStuck())
            continue;

        if (!u->isCompleted() || u->isConstructing())
            continue;

    }

    switch (curStage)
    {
    case start:
    {
        for (auto& u : Broodwar->self()->getUnits())
        {
            if (u->getType().isWorker())
            {
                if (!u->isCarryingMinerals())
                {
                    int mineralCount = Broodwar->self()->minerals();
                    int spawningPoolMineralCost = UnitTypes::Zerg_Spawning_Pool.mineralPrice();

                    if (mineralCount >= (spawningPoolMineralCost))
                    {
                        static bool isSpawning = false;

                        if (!isSpawning)
                        {
                            std::vector<Unit> mineralUnits;
                            for (auto& unit : resourceDepot->getUnitsInRadius(320)) // Adjust the radius as needed
                            {
                                if (unit->getType().isMineralField())
                                {
                                    mineralUnits.push_back(unit);
                                }
                            }

                            // Process the mineral units
                            TilePosition midMineralPos = TilePosition(0, 0);
                            int minCount = 0;
                            for (auto& unit : mineralUnits)
                            {
                                minCount++;
                                midMineralPos += unit->getTilePosition();
                            }
                            midMineralPos /= minCount;


                            // Calculate the direction vector away from the minerals
                            auto buildPos = resourceDepot->getTilePosition() + (resourceDepot->getTilePosition() - midMineralPos)*1.1;

                            if (u->build(UnitTypes::Zerg_Spawning_Pool, buildPos)) isSpawning = true;
                        }
                    }

                }

            }
        }
    }
        break;
    case build:
    {
        for (auto& u : Broodwar->self()->getUnits())
        {
            if (u->getType() == UnitTypes::Zerg_Larva)
            {
                static bool oneDrone = false;
                if (!oneDrone && Broodwar->self()->minerals() >= UnitTypes::Zerg_Drone.mineralPrice())
                {
                    if (u->morph(UnitTypes::Zerg_Drone))
                        oneDrone = true;

                }
            }
        }
    }
        break;
    case train:
    {
        for (auto& u : Broodwar->self()->getUnits())
        {
            static int count = 0;
            if (u->getType() == UnitTypes::Zerg_Larva)
            {
                if (Broodwar->self()->minerals() >= UnitTypes::Zerg_Zergling.mineralPrice())
                {
                    // Train a Zergling from the selected Larva
                    if (u->morph(UnitTypes::Zerg_Zergling)) count++;
                }
            }

            if (count >= 3)
                curStage = trainWait;
        }
    }
        break;
    case trainWait:
    {

        int zerglingCount = 0;
        for (auto u : Broodwar->self()->getUnits())
        {
            if (!u->exists())
                continue;

            if (u->getType() == UnitTypes::Zerg_Zergling)
            {
                zerglingCount++;
            }
        }

        if (zerglingCount >= 6)
        {
            curStage = scout;


            Broodwar->setLocalSpeed(3);


            if (enemyBasePos.isValid())
            {
                curStage = attack;
            }
        }
    }

        break;

    case scout:

        for (auto& u : Broodwar->self()->getUnits())
        {
            if (!u->exists())
                continue;

            if (u->getType() == UnitTypes::Zerg_Zergling)
            {
                int diffX = abs(u->getTilePosition().x - scoutDestPos.x);
                int diffY = abs(u->getTilePosition().y - scoutDestPos.y);
                double dist = sqrt( diffX * diffX + diffY * diffY);

                Position pos;
                pos.x = scoutDestPos.x * 32;
                pos.y = scoutDestPos.y * 32;
                if (dist < 5)
                {


                    scoutDestPos = GetNextScoutPos(scoutDestPos);


                    Position posTo;
                    posTo.x = scoutDestPos.x * 32;
                    posTo.y = scoutDestPos.y * 32;

                    Broodwar->drawCircleMap(posTo, 10, BWAPI::Colors::Green);
                    Broodwar->drawLineMap(pos, posTo, BWAPI::Colors::Red);
                }
                else
                {

                    u->move(pos);
                }
            }

        }


        if (enemyBasePos.isValid())
        {
            curStage = attack;

            Broodwar << "Attack started !!!";
        }

        break;

    case attack:


        break;
    }



    if (!enemyBasePos.isValid())
    for (auto& u : Broodwar->enemy()->getUnits())
    {
        if (!u->exists())
            continue;

        if (u->getType().isResourceDepot())
        {
            enemyBasePos = u->getTilePosition();
            Broodwar << "Enemy Base is spotted!" << std::endl;
        }
    }
*/