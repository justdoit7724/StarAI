#include "pch.h"
#include "MapManager.h"
#include "LogManager.h"



float GetAttWeight(Unit unit, int x, int y)
{
    auto pos = unit->getTilePosition();

    float dist = sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2));
    float hpCof = unit->getType().maxHitPoints() / (float)WEIGHT_MAX_HP;

    return std::fmax(0, std::fmin(hpCof * (1 - (dist / (hpCof * WEIGHT_MAX_DIST))), 1.0));
}



MapManager::MapManager()
{
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


        m_terrain.push_back(row);
    }
    weighMap m_att1;
    weighMap m_att2;
    weighMap m_def1;
    weighMap m_def2;
    weighMap m_total1;
    weighMap m_total2;
    for (int y = 0; y < m_WeightHeight; ++y)
    {
        std::vector<float> row(m_WeightWidth, 0);

        m_att1.push_back(row);
        m_att2.push_back(row);
        m_def1.push_back(row);
        m_def2.push_back(row);
        m_total1.push_back(row);
        m_total2.push_back(row);
    }

    m_wmAtt = std::make_unique<SwapChain<weighMap>>(m_att1, m_att2);
    m_wmDef = std::make_unique<SwapChain<weighMap>>(m_def1, m_def2);
    m_wmTotal = std::make_unique<SwapChain<weighMap>>(m_total1, m_total2);


    m_wPts = std::make_unique< SwapChain<weighPts>>(std::vector<std::pair<int, int>>(), std::vector<std::pair<int, int>>());
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
    return m_terrain[pos.y][pos.x];
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


void MapManager::Update()
{
    //SG_TIMER.Reset();

    static int updateAttStep = 0;
    switch (updateAttStep % 50)
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
        updateAttStep--;

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
            updateAttStep++;
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
        updateAttStep--;

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
            updateAttStep++;
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



    /*std::string log = std::to_string((updateAttStep)%50) + "," + std::to_string(SG_TIMER.ElapsedMilliseconds());
    SG_LOGMGR.Record("DEBUG", log);*/

    updateAttStep++;
    

    

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

            auto col = Colors::Black;

            int r=0, g=0, b=0;
            int curTerrain = GetTerrain(wPos);
            if ((curTerrain & MAP_MOVABLE) == MAP_MOVABLE)
            {
                g = 1;
            }
            if((curTerrain & MAP_BUILDABLE) == MAP_BUILDABLE)
            {
                b = 1;
            }

            if(r||g||b)
                Broodwar->drawDot(BWAPI::CoordinateType::Screen, x * 8 + 4, y * 8 + 4, Color(r, g, b));
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

