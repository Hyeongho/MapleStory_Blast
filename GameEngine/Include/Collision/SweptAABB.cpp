#include "SweptAABB.h"
#include "../Component/ColliderBox2D.h"
#include <algorithm>
#include <limits>

CSweptAABB::Result::Result()
    : Collision(false), EntryTime(1.f), ExitTime(1.f), Normal(0.f, 0.f),
    RelativeMove(0.f, 0.f), SrcContact(0.f, 0.f), DestContact(0.f, 0.f)
{
}

bool CSweptAABB::CollisionBox2DToBox2D(CColliderBox2D* Src, CColliderBox2D* Dest,
    CollisionResult& SrcResult, CollisionResult& DestResult)
{
    if (!Src || !Dest)
    {
        return false;
    }

    Box2DInfo SrcPrev = Src->HasPrevInfo() ? Src->GetPrevInfo() : Src->GetInfo();
    Box2DInfo DestPrev = Dest->HasPrevInfo() ? Dest->GetPrevInfo() : Dest->GetInfo();
    Box2DInfo SrcCurr = Src->GetInfo();
    Box2DInfo DestCurr = Dest->GetInfo();

    Vector2 SrcMove = SrcCurr.Center - SrcPrev.Center;
    Vector2 DestMove = DestCurr.Center - DestPrev.Center;

    if (SrcMove.Length() == 0.f && DestMove.Length() == 0.f)
    {
        return false;
    }

    Result SweepResult;

    if (!Compute(SrcPrev, DestPrev, SrcMove, DestMove, SweepResult))
    {
        return false;
    }

    SweepResult.SrcContact = SrcPrev.Center + SrcMove * SweepResult.EntryTime;
    SweepResult.DestContact = DestPrev.Center + DestMove * SweepResult.EntryTime;

    SrcResult.Src = Src;
    SrcResult.Dest = Dest;
    SrcResult.HitPoint = Vector3(SweepResult.SrcContact.x, SweepResult.SrcContact.y, 0.f);

    DestResult.Src = Dest;
    DestResult.Dest = Src;
    DestResult.HitPoint = Vector3(SweepResult.DestContact.x, SweepResult.DestContact.y, 0.f);

    return true;
}

bool CSweptAABB::Compute(const Box2DInfo& SrcPrev, const Box2DInfo& DestPrev,
    const Vector2& SrcMove, const Vector2& DestMove, Result& OutResult)
{
    Vector2 RelativeMove = SrcMove - DestMove;

    OutResult.RelativeMove = RelativeMove;

    if (RelativeMove.x == 0.f && RelativeMove.y == 0.f)
    {
        return false;
    }

    auto AxisOverlap = [](float MinA, float MaxA, float MinB, float MaxB)
    {
        return !(MaxA < MinB || MinA > MaxB);
    };

    if (RelativeMove.x == 0.f && !AxisOverlap(SrcPrev.Min.x, SrcPrev.Max.x, DestPrev.Min.x, DestPrev.Max.x))
    {
        return false;
    }

    if (RelativeMove.y == 0.f && !AxisOverlap(SrcPrev.Min.y, SrcPrev.Max.y, DestPrev.Min.y, DestPrev.Max.y))
    {
        return false;
    }

    float xEntry = 0.f;
    float xExit = 0.f;
    float yEntry = 0.f;
    float yExit = 0.f;

    if (RelativeMove.x > 0.f)
    {
        xEntry = DestPrev.Min.x - SrcPrev.Max.x;
        xExit = DestPrev.Max.x - SrcPrev.Min.x;
    }
    else
    {
        xEntry = DestPrev.Max.x - SrcPrev.Min.x;
        xExit = DestPrev.Min.x - SrcPrev.Max.x;
    }

    if (RelativeMove.y > 0.f)
    {
        yEntry = DestPrev.Min.y - SrcPrev.Max.y;
        yExit = DestPrev.Max.y - SrcPrev.Min.y;
    }
    else
    {
        yEntry = DestPrev.Max.y - SrcPrev.Min.y;
        yExit = DestPrev.Min.y - SrcPrev.Max.y;
    }

    if (RelativeMove.x == 0.f)
    {
        xEntry = -std::numeric_limits<float>::infinity();
        xExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        xEntry /= RelativeMove.x;
        xExit /= RelativeMove.x;
    }

    if (RelativeMove.y == 0.f)
    {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        yEntry /= RelativeMove.y;
        yExit /= RelativeMove.y;
    }

    float EntryTime = std::max(xEntry, yEntry);
    float ExitTime = std::min(xExit, yExit);

    if (EntryTime > ExitTime)
    {
        return false;
    }

    if (ExitTime < 0.f)
    {
        return false;
    }

    if (EntryTime < 0.f)
    {
        EntryTime = 0.f;
    }

    if (EntryTime > 1.f)
    {
        return false;
    }

    OutResult.EntryTime = EntryTime;
    OutResult.ExitTime = ExitTime;
    OutResult.Collision = true;

    if (xEntry > yEntry)
    {
        OutResult.Normal = (RelativeMove.x < 0.f) ? Vector2(1.f, 0.f) : Vector2(-1.f, 0.f);
    }
    else
    {
        OutResult.Normal = (RelativeMove.y < 0.f) ? Vector2(0.f, 1.f) : Vector2(0.f, -1.f);
    }

    return true;
}
