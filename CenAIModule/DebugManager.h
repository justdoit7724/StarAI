#pragma once
#include "Singleton.h"

#define SG_DEBUGMGR DebugManager::Instance()
class DebugManager :
    public Singleton<DebugManager>
{
public:
    void DrawTextFix(int x,int y, std::string str);
    void DrawTextScn(Position pos, std::string str);
    void DrawCircle(Position pos, float radius, Color col);
    void DrawBox(TilePosition pos, float w, float h, Color col);
    void DrawBox(Position pos, float w, float h, Color col);
    void DrawLine(TilePosition p1, TilePosition p2, Color col);
    void DrawUnit(Unit unit, Color col);
};

