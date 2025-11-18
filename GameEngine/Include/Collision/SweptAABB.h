#pragma once

#include "../GameInfo.h"

class CSweptAABB
{
public:
    struct Result
    {
        Result();

        bool Collision;
        float EntryTime;
        float ExitTime;
        Vector2 Normal;
        Vector2 RelativeMove;
        Vector2 SrcContact;
        Vector2 DestContact;
    };

public:
    static bool CollisionBox2DToBox2D(class CColliderBox2D* Src, class CColliderBox2D* Dest,
        CollisionResult& SrcResult, CollisionResult& DestResult);

private:
    static bool Compute(const Box2DInfo& SrcPrev, const Box2DInfo& DestPrev,
        const Vector2& SrcMove, const Vector2& DestMove, Result& OutResult);
};
