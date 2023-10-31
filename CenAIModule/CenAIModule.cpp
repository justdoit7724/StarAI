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


    auto rd = SG_SITU.UnitsInRange(true, TilePosition(32, 32), UnitTypes::Zerg_Hatchery);
    SG_SITU.Init(rd[0]);

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
    SG_SITU.DisplayMinDif(Position(15,20));
    SG_SITU.DisplayPL(Position(15, 80));
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
    SG_SITU.UpdatePL(unit->getPlayer() == Broodwar->self(), unit->getType());

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


