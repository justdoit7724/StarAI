#pragma once

#include <vector>
#include <functional>
#include <set>

/*
* //usage
auto rd = SG_SITU.GetUnit(true, UnitTypes::Zerg_Hatchery);
auto rd2 = SG_SITU.GetUnit(false, UnitTypes::Protoss_Nexus);



static bool done = false;
static AStar::CoordinateList path;
if (rd && rd2 && !done)
{
    AStar::Generator generator;
    // Set 2d map size.
    int tWidth = SG_MAP.GetWidth() / 4;
    int tHeight = SG_MAP.GetHeight() / 4;
    generator.setWorldSize({ tWidth, tHeight });
    // You can use a few heuristics : manhattan, euclidean or octagonal.
    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(false);

    for (int y = 0; y < tHeight; ++y)
    {
        for (int x = 0; x < tWidth; ++x)
        {
            auto terr = SG_MAP.GetTerrain(WalkPosition(x * 4, y * 4));
            if(terr & MAP_MOVABLE)
        }
    }

    SG_MAP.GetTerrain()
    generator.addCollision(Vec2i coordinates_);

    // This method returns vector of coordinates from target to source.
    auto path = generator.findPath({ rd->getTilePosition().x * 4, rd->getTilePosition().y * 4 }, { rd2->getTilePosition().x * 4, rd2->getTilePosition().y * 4 });

    for (auto& coordinate : path) {

        SG_DEBUGMGR.DrawBox(TilePosition(coordinate.x, coordinate.y), 32, 32, Colors::Orange);
    }
}
*/

namespace AStar
{
    struct Vec2i
    {
        int x, y;

        bool operator == (const Vec2i& coordinates_);
        friend Vec2i operator + (const AStar::Vec2i& left_, const AStar::Vec2i& right_) {
            return{ left_.x + right_.x, left_.y + right_.y };
        }
    };

    using uint = unsigned int;
    using HeuristicFunction = std::function<uint(Vec2i, Vec2i)>;
    using CoordinateList = std::vector<Vec2i>;

    struct Node
    {
        uint G, H;
        Vec2i coordinates;
        Node* parent;

        Node(Vec2i coord_, Node* parent_ = nullptr);
        uint getScore();
    };

    using NodeSet = std::vector<Node*>;

    class Generator
    {
        bool detectCollision(Vec2i coordinates_);
        Node* findNodeOnList(NodeSet& nodes_, Vec2i coordinates_);
        void releaseNodes(NodeSet& nodes_);

    public:
        Generator();
        void setWorldSize(Vec2i worldSize_);
        void setDiagonalMovement(bool enable_);
        void setHeuristic(HeuristicFunction heuristic_);
        CoordinateList findPath(Vec2i source_, Vec2i target_);
        void addCollision(Vec2i coordinates_);
        void removeCollision(Vec2i coordinates_);
        void clearCollisions();

    private:
        HeuristicFunction heuristic;
        CoordinateList direction, walls;
        Vec2i worldSize;
        uint directions;
    };

    class Heuristic
    {
        static Vec2i getDelta(Vec2i source_, Vec2i target_);

    public:
        static uint manhattan(Vec2i source_, Vec2i target_);
        static uint euclidean(Vec2i source_, Vec2i target_);
        static uint octagonal(Vec2i source_, Vec2i target_);
    };
}
