#include "pch.h"
#include "MapManager.h"
#include "LogManager.h"
#include "SituationManager.h"
#include "DebugManager.h"


float GetAttWeight(Unit unit, int x, int y)
{
    auto pos = unit->getTilePosition();

    float dist = sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2));
    float hpCof = unit->getType().maxHitPoints() / (float)WEIGHT_MAX_HP;

    return std::fmax(0, std::fmin(hpCof * (1 - (dist / (hpCof * WEIGHT_MAX_DIST))), 1.0));
}



MapManager::MapManager()
    :m_isFoundExpansion(false)
{
    TurnOffDisplay();

    auto mapW = Broodwar->mapWidth();
    auto mapH = Broodwar->mapHeight();

    m_WeightWidth = mapW;
    m_WeightHeight = mapH;
    m_Width = mapW * 4;
    m_height = mapH * 4;

    auto mapName = Broodwar->mapFileName();
    while (mapName.find('.') != std::string::npos)mapName.pop_back();
    //std::string mapPath = "C:\\justd\\OneDrive\\Desktop\\Starcraft\\" + mapName + ".csv";
    std::string mapPath = __FILE__ "\\..\\";
    mapPath += mapName + ".csv";

    basicMap terr1;
    basicMap terr2;
    for (int y = 0; y < m_height; ++y)
    {
        std::vector<int> row;

        for (int x = 0; x < m_Width; ++x)
        {
            TilePosition curTilePos = TilePosition(x / 4, y / 4);
            WalkPosition curWalkPos = WalkPosition(x, y);
            bool isBuildable = Broodwar->isBuildable(curTilePos);
            bool isMovable = Broodwar->isWalkable(curWalkPos);

            int groundData = 0;
            groundData += isMovable ? MAP_MOVABLE : 0;
            groundData += isBuildable ? MAP_BUILDABLE : 0;

            row.push_back(groundData);
        }


        terr1.push_back(row);
        terr2.push_back(row);
    }
    weighMap att1;
    weighMap att2;
    weighMap def1;
    weighMap def2;
    weighMap total1;
    weighMap total2;
    basicMap state1;
    basicMap state2;
    for (int y = 0; y < m_WeightHeight; ++y)
    {
        std::vector<float> row(m_WeightWidth, 0);
        std::vector<int> rowInt(m_WeightWidth, 0);

        att1.push_back(row);
        att2.push_back(row);
        def1.push_back(row);
        def2.push_back(row);
        total1.push_back(row);
        total2.push_back(row);

        state1.push_back(rowInt);
        state2.push_back(rowInt);
    }

    m_wmAtt = std::make_unique<SwapChain<weighMap>>(att1, att2);
    m_wmDef = std::make_unique<SwapChain<weighMap>>(def1, def2);
    m_wmTotal = std::make_unique<SwapChain<weighMap>>(total1, total2);
    m_terrain = std::make_unique<SwapChain<basicMap>>(terr1, terr2);

    m_wPts = std::make_unique< SwapChain<weighPts>>(std::vector<std::pair<int, int>>(), std::vector<std::pair<int, int>>());
    m_state = std::make_unique< SwapChain<basicMap>>(state1, state2);

    m_generator = new AStar::Generator();
}

MapManager::~MapManager()
{
    delete m_generator;
}


int MapManager::GetWidth()
{
    return m_Width;
}
int MapManager::GetHeight()
{
    return m_height;
}
int MapManager::GetWeightWidth()
{
    return m_WeightWidth;
}
int MapManager::GetWeightHeight()
{
    return m_WeightHeight;
}
int MapManager::GetTerrain(WalkPosition pos)
{
    return m_terrain->Get()[pos.y][pos.x];
}
float MapManager::GetAtt(TilePosition pos)
{
    return m_wmAtt->Get()[pos.y][pos.x];
}
float MapManager::GetDef(TilePosition pos)
{
    return m_wmDef->Get()[pos.y][pos.x];
}
float MapManager::GetTotalWeight(TilePosition pos)
{
    return m_wmTotal->Get()[pos.y][pos.x];
}

std::vector<TilePosition> MapManager::GetPts() const
{
    std::vector< TilePosition> ret;

    for (int i= 0; i< m_wPts->Get().size(); ++i)
    {
        ret.push_back({ m_wPts->Get()[i].first,m_wPts->Get()[i].second });
    }

    return ret;
}

bool MapManager::IsValidPos(TilePosition tpos)
{
    return IsValidPos(Position(tpos));
}

bool MapManager::IsValidPos(Position pos)
{
    int pxWidth = m_Width * 8;
    int pxHeight = m_height * 8;

    return !((pos.x <= 0 && pos.y <= 0) || pos.x >= pxWidth || pos.y >= pxHeight);
}


void MapManager::Update()
{
    //weight pt update
    static long long updateStep = 0;
    switch (updateStep % 50)
    {
    case 0:
        for (int y = 0; y < m_WeightHeight; ++y)
        {
            for (int x = 0; x < m_WeightWidth; ++x)
            {
                (*m_wmAtt->Work())[y][x] = 0;
            }
        }

        break;

    case 1:

        m_enemies.clear();
        for (auto& u : Broodwar->enemy()->getUnits())
        {
            if (!u->exists())
                continue;

            m_enemies.push_back(u);
        }

        break;
    case 3:
    {
        updateStep--;

        for (int i = 0; i < 2 && m_enemies.size()>0; ++i)
        {
            auto unit = (Unit)m_enemies.back(); m_enemies.pop_back();

            if (!unit->exists())
                continue;

            auto pos = unit->getTilePosition();

            for (int x = -WEIGHT_MAX_DIST; x <= WEIGHT_MAX_DIST; ++x)
            {
                for (int y = -WEIGHT_MAX_DIST; y <= WEIGHT_MAX_DIST; ++y)
                {
                    int wx = pos.x + x;
                    int wy = pos.y + y;

                    if (wx < 0 || wy < 0 ||
                        wx >= m_WeightWidth || wy >= m_WeightHeight)
                        continue;

                    (*m_wmAtt->Work())[wy][wx] += GetAttWeight(unit, wx, wy);

                }
            }
        }

        if (m_enemies.empty())
        {
            updateStep++;
        }
    }

    break;
    case 4:
    {
        for (int y = 0; y < m_WeightHeight; ++y)
        {
            for (int x = 0; x < m_WeightWidth; ++x)
            {
                (*m_wmTotal->Work())[y][x] = 0;
            }
        }
    }
    break;
    case 19:
    {
        for (int y = 0; y < m_WeightHeight / 2; ++y)
        {
            for (int x = 0; x < m_WeightWidth; ++x)
            {
                (*m_wmTotal->Work())[y][x] += (*m_wmAtt->Work())[y][x];
            }
        }
    }
    break;
    case 20:
    {
        for (int y = m_WeightHeight / 2; y < m_WeightHeight; ++y)
        {
            for (int x = 0; x < m_WeightWidth; ++x)
            {
                (*m_wmTotal->Work())[y][x] += (*m_wmAtt->Work())[y][x];
            }
        }
    }
    break;
    case 22:
    {
        m_sorter.clear();


        for (int y = 0; y < m_WeightHeight; ++y)
        {
            if (y % 3 != 0)
                continue;

            for (int x = 0; x < m_WeightWidth; ++x)
            {
                float value = (*m_wmTotal->Work())[y][x];
                if (value > 0.01)
                    m_sorter[value] = x + y * m_WeightWidth;
            }
        }
    }
    break;
    case 23:
    {
        for (int y = 0; y < m_WeightHeight; ++y)
        {
            if (y % 3 != 1)
                continue;
            for (int x = 0; x < m_WeightWidth; ++x)
            {
                float value = (*m_wmTotal->Work())[y][x];
                if (value > 0.01)
                    m_sorter[value] = x + y * m_WeightWidth;
            }
        }
    }
    break;
    case 24:
    {
        for (int y = 0; y < m_WeightHeight; ++y)
        {
            if (y % 3 != 2)
                continue;

            for (int x = 0; x < m_WeightWidth; ++x)
            {
                float value = (*m_wmTotal->Work())[y][x];
                if (value > 0.01)
                    m_sorter[value] = x + y * m_WeightWidth;
            }
        }


        m_wPts->Work()->clear();
    }
    break;
    case 29:
    {
        updateStep--;

        for (int i = 0; i < 300 && m_sorter.size() && m_wPts->Work()->size() < WEIGHT_PT_MAX_COUNT; ++i)
        {
            float curValue = m_sorter.rbegin()->first;
            int curIndex = m_sorter.rbegin()->second;
            m_sorter.erase(--m_sorter.end());
            int curX = curIndex % m_WeightWidth;
            int curY = curIndex / m_WeightWidth;

            bool isolated = true;
            for (int i = 0; i < m_wPts->Work()->size(); ++i)
            {
                int checkX = (*m_wPts->Work())[i].first;
                int checkY = (*m_wPts->Work())[i].second;

                float distThreshold = (curValue / (float)WEIGHT_COUNT) * WEIGHT_PT_MAX_DIST;
                float sqrtThresh = pow(distThreshold, 2);

                if (DistSqrt(curX, curY, checkX, checkY) <= sqrtThresh)
                {
                    isolated = false;
                    break;
                }
            }
            if (isolated)
            {
                m_wPts->Work()->push_back({ curX, curY });
            }

        }

        if (m_sorter.empty() || m_wPts->Work()->size() >= WEIGHT_PT_MAX_COUNT)
        {
            updateStep++;
        }

    }
    break;
    case 30:
    {
        m_wmAtt->Swap();
        m_wmDef->Swap();
        m_wmTotal->Swap();
        m_wPts->Swap();
    }
    break;
    }

    updateStep++;


    static long long terrainStep = 0;
    switch (terrainStep++%4)
    {
    case 0:
        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_Width; ++x)
            {
                (*m_terrain->Work())[y][x] = (*m_terrain->Work())[y][x] & (~MAP_UNIT);
            }
        }

        break;
    case 1:
    {
        for (auto u : Broodwar->getAllUnits())
        {
            m_tmpUnits.push_back(u);
        }
    }
    break;

    case 2:
    {
        for (int i = 0; i < 30 && m_tmpUnits.size(); ++i)
        {
            Unit u = m_tmpUnits[m_tmpUnits.size() - 1];
            m_tmpUnits.pop_back();

            if (!u->exists())
                continue;
            if (u->isFlying())
                continue;

            int whWidth = u->getType().tileWidth() * 4 / 2;
            int whHeight = u->getType().tileHeight() * 4 / 2;
            Position pos = u->getPosition();
            WalkPosition wPos(pos);



            for (int y = -whHeight; y < whHeight; ++y)
            {
                for (int x = -whWidth; x < whWidth; ++x)
                {
                    int my = wPos.y + y;
                    int mx = wPos.x + x;

                    if (my < 0 || mx < 0 || mx >= m_Width || my >= m_height)
                        continue;

                    (*m_terrain->Work())[my][mx] |= MAP_UNIT;
                }
            }
        }

        if (!m_tmpUnits.empty())
        {
            terrainStep--;
        }
    }
    break;
    case 3:
        m_terrain->Swap();
        break;
    }
    

    //location update
    static long long mapFindStep = 0;
    switch (mapFindStep++)
    {
    case 0:
    {
        auto rdList = SG_SITU.UnitsInRange(true, TilePosition(0, 0), UnitTypes::Zerg_Hatchery);
        if (rdList.empty())
        {
            mapFindStep--;
            break;
        }
        m_myRD = rdList[0];
        std::vector<Unit> emList;
        switch (Broodwar->enemy()->getRace())
        {
        case Races::Protoss:
            emList = SG_SITU.UnitsInRange(false, TilePosition(0, 0), UnitTypes::Protoss_Nexus);
            break;
        case Races::Zerg:
            emList = SG_SITU.UnitsInRange(false, TilePosition(0, 0), UnitTypes::Zerg_Hatchery);
            break;
        case Races::Terran:
            emList = SG_SITU.UnitsInRange(false, TilePosition(0, 0), UnitTypes::Terran_Command_Center);
            break;
        }
        if (emList.empty())
        {
            mapFindStep--;
            break;
        }
        m_emRD = emList[0];
    }
    break;
    case 1:
    {
        // Set 2d map size.
        int tWidth = SG_MAP.GetWeightWidth();
        int tHeight = SG_MAP.GetWeightHeight();
        m_generator->setWorldSize({ tWidth, tHeight });
        // You can use a few heuristics : manhattan, euclidean or octagonal.
        m_generator->setHeuristic(AStar::Heuristic::euclidean);
        m_generator->setDiagonalMovement(false);

        for (int y = 0; y < tHeight; ++y)
        {
            for (int x = 0; x < tWidth; ++x)
            {
                auto terr = SG_MAP.GetTerrain(WalkPosition(x * 4 + 1, y * 4 + 1));
                if ((terr & MAP_MOVABLE) != MAP_MOVABLE)
                    m_generator->addCollision({ x,y });
            }
        }
    }
    break;
    case 2:
    {
        m_path = m_generator->findPath({ m_myRD->getTilePosition().x+1, m_myRD->getTilePosition().y+1 }, { m_emRD->getTilePosition().x+1, m_emRD->getTilePosition().y+1});

        if (m_path.empty())
        {
            mapFindStep--;
        }

    }
    break;
    case 3:
    {
        auto selPos = m_path[m_path.size() - 25];
        Position pos = Position(selPos.x * 32 + 32, selPos.y * 32 + 32);
        auto coreMins = SG_SITU.GetMineralsNear(m_myRD->getPosition(), 300);
        auto mins = SG_SITU.GetMineralsNear(pos, 500);

        SG_DEBUGMGR.DrawCircle(pos, 32, Colors::Green);
        

        Unit pivotMineral = nullptr;
        for (int i = 0; i < mins.size();)
        {
            bool isExist = false;
            for (auto cm : coreMins)
            {
                if (cm == mins[i])
                {
                    isExist = true;
                    break;
                }
            }
            if (isExist)
            {
                mins.erase(mins.begin() + i);
            }
            else
            {
                pivotMineral = mins[i];
                break;
            }
        }
        if (!pivotMineral)
        {

            mapFindStep--;
            break;

        }

        m_expGas = SG_SITU.GetGasNear(pivotMineral->getPosition(), 400);
        if (!m_expGas)
        {
            mapFindStep--;
            break;
        }

        m_expMinerals = SG_SITU.GetMineralsNear(pivotMineral->getPosition(), 500);



        for (auto m : m_expMinerals)
        {
            SG_DEBUGMGR.DrawBox(m->getPosition(), 64, 32, Colors::Cyan);
        }
        SG_DEBUGMGR.DrawBox(m_expGas->getPosition(), 96, 64, Colors::Green);

        
    }
    break;
    case 4:
    {
        Position center = Position(0, 0);

        for (auto m : m_expMinerals)
        {
            center += m->getPosition();
        }
        center += m_expGas->getPosition();
        center /= (m_expMinerals.size() + 1);

        SG_DEBUGMGR.DrawCircle(center, 16, Colors::Blue);

        for (int y = -7; y <= 7; ++y)
        {
            for (int x = -7; x <= 7; ++x)
            {
                TilePosition tp = TilePosition(center.x/32, center.y/32);
                tp.x += x;
                tp.y += y;

                //3by3 check
                if (SG_SITU.IsBuildable(tp, 4, 3,false))
                {
                    Position dp = Position(tp);
                    dp.x += 2 * 32;
                    dp.y += 1.5 * 32;

                    m_expPts.push_back(dp);
                }

            }
        }
    }
    break;
    case 5:
    {
        for (int i = 0; i < m_expPts.size();)
        {
            bool isDelete = false;

            for (const auto& m : m_expMinerals)
            {
                if (m_expPts[i].getDistance(m->getPosition()) < (32*5.5))
                {
                    isDelete = true;
                    break;
                }
            }
            if (!isDelete && m_expPts[i].getDistance(m_expGas->getPosition()) < (32*7))
            {
                isDelete = true;
            }
            if (isDelete)
                m_expPts.erase(m_expPts.begin() + i);
            else
                i++;
        }

    }
    break; 
    case 6:
    {
        double minScore = DBL_MAX;
        int selID = -1;
        for (int i = 0; i < m_expPts.size(); ++i)
        {
            double curScore = 0;
            for (const auto& m : m_expMinerals)
            {
                curScore += m_expPts[i].getDistance(m->getPosition());
            }
            curScore += m_expPts[i].getDistance(m_expGas->getPosition())*2;
            
            if (curScore < minScore)
            {
                minScore = curScore;
                selID = i;
            }
        }

        m_expPt = m_expPts[selID];
    }
    break;

    }
   

    if(isDisplayAtt)
        DisplayAtt();
    if(isDisplayTerrain)
        DisplayTerrain();
    if (isDisplayPts)
        DisplayPts();
}

void MapManager::DisplayAtt()
{
    auto srnPos = Broodwar->getScreenPosition();

    RECT screenRect;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &screenRect);

     for (int y = 0; y < screenRect.bottom / 32 - 1; ++y)
    {
        for (int x = 0; x < screenRect.right / 32 - 1; ++x)
        {

            TilePosition pos = TilePosition(srnPos);
            pos.x += x;
            pos.y += y;

            if (pos.x < 0 || pos.y < 0 ||
                pos.x >= m_WeightWidth || pos.y >= m_WeightHeight)
                continue;

            auto col = Colors::Black;

            int r, g, b;
            GetColorIntensity(GetAtt(pos), r, g, b);

            Broodwar->drawBox(BWAPI::CoordinateType::Screen, x * 32, y * 32, x * 32+32, y * 32+32, Color(r, g, b));
        }
    }
}

void MapManager::DisplayDef()
{
    auto srnPos = Broodwar->getScreenPosition();

    RECT screenRect;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &screenRect);

    for (int y = 0; y < screenRect.bottom / 8 - 1; ++y)
    {
        for (int x = 0; x < screenRect.right / 8 - 1; ++x)
        {

            WalkPosition pos = WalkPosition(srnPos);
            pos.x += x;
            pos.y += y;

            if (0 > pos.x || 0 > pos.y)
                continue;

            if(SG_SITU.IsBuildable(TilePosition(pos),1,1))
                Broodwar->drawBox(BWAPI::CoordinateType::Screen, x * 8, y * 8, x * 8 + 8, y * 8 + 8, Color(100, 100, 100));
        }
    }

    
}

void MapManager::DisplayTerrain()
{
    auto srnPos = Broodwar->getScreenPosition();

    RECT screenRect;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &screenRect);

     for (int y = 0; y < screenRect.bottom / 8 - 1; ++y)
    {
        for (int x = 0; x < screenRect.right / 8 - 1; ++x)
        {

            WalkPosition wPos = WalkPosition(srnPos);
            wPos.x += x;
            wPos.y += y;

            if (wPos.x < 0 || wPos.y < 0 ||
                wPos.x >= m_Width || wPos.y >= m_height)
                continue;

            float cof = 0;
            int curTerrain = GetTerrain(wPos);
            const float inc = 0.3333;
            if ((curTerrain & MAP_MOVABLE) == MAP_MOVABLE)
            {
                cof += inc;
            }
            if((curTerrain & MAP_BUILDABLE) == MAP_BUILDABLE)
            {
                cof += inc;
            }
            if ((curTerrain & MAP_UNIT) == MAP_UNIT)
            {
                cof += inc;
            }


            int r = 0, g = 0, b = 0;
            GetColorIntensity(cof, r, g, b);

            if(r||g||b)
                Broodwar->drawBox(BWAPI::CoordinateType::Screen, x * 8, y * 8, x*8+8, y*8+8, Color(r, g, b));
        }
    }
}

void MapManager::DisplayTotal()
{
    auto srnPos = Broodwar->getScreenPosition();

    RECT screenRect;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &screenRect);

    for (int y = 0; y < screenRect.bottom / 32 - 1; ++y)
    {
        for (int x = 0; x < screenRect.right / 32 - 1; ++x)
        {

            TilePosition pos = TilePosition(srnPos);
            pos.x += x;
            pos.y += y;

            if (pos.x < 0 || pos.y < 0 ||
                pos.x >= m_WeightWidth || pos.y >= m_WeightHeight)
                continue;

            auto col = Colors::Black;

            int r, g, b;
            GetColorIntensity(GetTotalWeight(pos), r, g, b);

            Broodwar->drawBox(BWAPI::CoordinateType::Screen, x * 32, y * 32, x * 32 + 32, y * 32 + 32, Color(r, g, b));
        }
    }
}

void MapManager::DisplayPts()
{
    auto scnPos = Broodwar->getScreenPosition();
    for (int i = 0; i < m_wPts->Get().size(); i++)
    {
        TilePosition pos;
        pos.x = m_wPts->Get()[i].first;
        pos.y = m_wPts->Get()[i].second;

        Position rpPos;
        rpPos.x = pos.x * 32 - scnPos.x;
        rpPos.y = pos.y * 32 - scnPos.y;

        if (pos.x < 0 || pos.y < 0 ||
            pos.x >= m_Width || pos.y >= m_height)
            continue;

        int r, g, b;
        GetColorIntensity(GetTotalWeight(pos), r, g, b);

        float val = m_wmTotal->Get()[pos.y][pos.x];
        float distThreshold = (val / (float)WEIGHT_COUNT) * WEIGHT_PT_MAX_DIST;

        Broodwar->drawCircle(BWAPI::CoordinateType::Screen, rpPos.x, rpPos.y, distThreshold, Color(r, g, b));

    }
}

void MapManager::DisplayBuildableCheck()
{
    auto srnPos = Broodwar->getScreenPosition();

    RECT screenRect;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &screenRect);

    for (int y = 0; y < screenRect.bottom / 32 - 1; ++y)
    {
        for (int x = 0; x < screenRect.right / 32 - 1; ++x)
        {

            TilePosition pos = TilePosition(srnPos);
            pos.x += x;
            pos.y += y;

            if (pos.x < 0 || pos.y < 0 ||
                pos.x >= m_WeightWidth || pos.y >= m_WeightHeight)
                continue;

            auto col = Colors::Black;

            if(SG_SITU.IsBuildable(pos, 3, 3))
                Broodwar->drawBox(BWAPI::CoordinateType::Screen, x * 32, y * 32, x * 32 + 32, y * 32 + 32, Colors::Green);
            else
                Broodwar->drawBox(BWAPI::CoordinateType::Screen, x * 32, y * 32, x * 32 + 32, y * 32 + 32, Colors::Red);

        }
    }
}

void MapManager::TurnOffDisplay()
{
    isDisplayAtt=false;
    isDisplayDef = false;
    isDisplayTerrain = false;
    isDisplayTotal = false;
    isDisplayPts = false;
}

Position MapManager::GetExpPt()
{
    return m_expPt;
}

bool MapManager::IsExpOccupied()
{
    return SG_SITU.UnitsInRange(true, TilePosition(m_expPt), UnitTypes::Zerg_Hatchery, 150).size();
}

bool MapManager::GetAddExpPt(Position& pos)
{
    auto center = (m_expPt + SG_SITU.GetBase()->getPosition()) / 2;

    if (SG_SITU.GetOpenPositionNear(center, pos,4,3, 100))
        return true;
    if (SG_SITU.GetOpenPositionNear(center, pos, 4, 3, 200))
        return true;
    if (SG_SITU.GetOpenPositionNear(center, pos, 4, 3, 300))
        return true;

    return false;
}

