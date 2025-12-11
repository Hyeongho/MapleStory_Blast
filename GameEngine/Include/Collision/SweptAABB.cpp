#include "SweptAABB.h"

bool CSweptAABB::AABBCheck(const Box2DInfo& Src, const Box2DInfo& Dest)
{
    return !(Src.Max.x < Dest.Min.x || Src.Min.x > Dest.Max.x || Src.Max.y < Dest.Min.y || Src.Min.y > Dest.Max.y);
}

Box2DInfo CSweptAABB::GetSweptBroadphaseBox(const Box2DInfo& Box, const Vector2& Velocity)
{
    Box2DInfo Result = Box;

    if (Velocity.x > 0.f)
    {
        Result.Max.x += Velocity.x;
    }

    else
    {
        Result.Min.x += Velocity.x;
    }

    if (Velocity.y > 0.f)
    {
        Result.Max.y += Velocity.y;
    }

    else
    {
        Result.Min.y += Velocity.y;
    }

    Result.Center.x = (Result.Min.x + Result.Max.x) * 0.5f;
    Result.Center.y = (Result.Min.y + Result.Max.y) * 0.5f;

    Result.Length.x = (Result.Max.x - Result.Min.x) * 0.5f;
    Result.Length.y = (Result.Max.y - Result.Min.y) * 0.5f;

    // 축은 기존 값을 그대로 사용 (축 회전과의 호환 유지)
    Result.Axis[0] = Box.Axis[0];
    Result.Axis[1] = Box.Axis[1];

    return Result;
}

float CSweptAABB::Sweep(const Box2DInfo& Src, const Box2DInfo& Dest, const Vector2& Velocity, Vector2& Normal)
{
    // 기본 AABB와 브로드페이즈 박스를 먼저 체크하여 기존 충돌 흐름과 호환
    if (AABBCheck(Src, Dest))
    {
        Normal = Vector2(0.f, 0.f);
        return 0.f;
    }

    Box2DInfo BroadPhase = GetSweptBroadphaseBox(Src, Velocity);

    if (!AABBCheck(BroadPhase, Dest))
    {
        Normal = Vector2(0.f, 0.f);
        return 1.f;
    }

    float xInvEntry, xInvExit;
    float yInvEntry, yInvExit;

    if (Velocity.x > 0.f)
    {
        xInvEntry = Dest.Min.x - Src.Max.x;
        xInvExit = Dest.Max.x - Src.Min.x;
    }
    else
    {
        xInvEntry = Dest.Max.x - Src.Min.x;
        xInvExit = Dest.Min.x - Src.Max.x;
    }

    if (Velocity.y > 0.f)
    {
        yInvEntry = Dest.Min.y - Src.Max.y;
        yInvExit = Dest.Max.y - Src.Min.y;
    }
    else
    {
        yInvEntry = Dest.Max.y - Src.Min.y;
        yInvExit = Dest.Min.y - Src.Max.y;
    }

    float xEntry = Velocity.x == 0.f ? -FLT_MAX : xInvEntry / Velocity.x;
    float xExit = Velocity.x == 0.f ? FLT_MAX : xInvExit / Velocity.x;

    float yEntry = Velocity.y == 0.f ? -FLT_MAX : yInvEntry / Velocity.y;
    float yExit = Velocity.y == 0.f ? FLT_MAX : yInvExit / Velocity.y;

    float entryTime = xEntry > yEntry ? xEntry : yEntry;
    float exitTime = xExit < yExit ? xExit : yExit;

    if (entryTime > exitTime || (xEntry < 0.f && yEntry < 0.f) || entryTime > 1.f)
    {
        Normal = Vector2(0.f, 0.f);
        return 1.f;
    }

    if (xEntry > yEntry)
    {
        Normal = Vector2((Velocity.x < 0.f) ? 1.f : -1.f, 0.f);
    }

    else
    {
        Normal = Vector2(0.f, (Velocity.y < 0.f) ? 1.f : -1.f);
    }

    return entryTime;
}
