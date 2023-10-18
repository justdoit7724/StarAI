// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#include <BWAPI.h>
using namespace BWAPI;

#include <assert.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>
#include <queue>

#define WEIGHT_MAX_HP 1200
#define WEIGHT_MAX_DIST 30
#define WEIGHT_PT_MAX_COUNT 5
#define WEIGHT_PT_MAX_DIST 180
#define WEIGHT_COUNT    1
#define MAP_MOVABLE 1
#define MAP_BUILDABLE 2
#define MAP_UNIT 4

inline float Clamp(float from, float to, float v)
{
    assert(from <= to);

    float f = (v - from) / (to - from);

    return std::fminf(to, std::fmaxf(from, f));
}
inline void GetColorIntensity(float c, int& r, int& g, int& b)
{
    c = Clamp(0, 1, c);

    float cG = 0;
    float cR = 0;
    float cB = 0;
    if (c < 0.5)
    {
        cG = c / 0.5;
        cR = 1.0 - cG;

    }
    else
    {
        cB = (c-0.5) / 0.5;
        cG = 1.0 - cB;
    }

    r = cR * 255;
    g = cG * 255;
    b = cB * 255;

}

inline float DistSqrt(int x1, int y1, int x2, int y2)
{
    return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

inline void Rotate(float originX, float originY, float endX, float endY, float rad, float& newX, float& newY)
{
    float m11 = cosf(rad);
    float m21 = sinf(rad);
    float m12 = -sinf(rad);
    float m22 = cosf(rad);

    float relX = endX - originX;
    float relY = endY - originY;

    float rotX = relX * m11 + relY * m12;
    float rotY = relX * m21 + relY * m22;

    newX = rotX + originX;
    newY = rotY + originY;
}

#endif //PCH_H
