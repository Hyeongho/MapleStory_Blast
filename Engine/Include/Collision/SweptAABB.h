#pragma once

#include "../GameInfo.h"

class CSweptAABB
{
public:
    static bool AABBCheck(const Box2DInfo& Src, const Box2DInfo& Dest);
    static Box2DInfo GetSweptBroadphaseBox(const Box2DInfo& Box, const Vector2& Velocity);
    static float Sweep(const Box2DInfo& Src, const Box2DInfo& Dest, const Vector2& Velocity, Vector2& Normal);
};

