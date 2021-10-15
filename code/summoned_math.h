/* date = August 5th 2021 2:16 pm */

#ifndef SUMMONED_MATH_H
#define SUMMONED_MATH_H

F32 Lerp(F32 a, F32 t, F32 b)
{
    F32 result = (1.0f - t)*a + t*b;
    return result;
}

bool IsPowerOfTwo(uint x)
{
    bool result = (x != 0) && ((x & (x - 1)) == 0);
    return result;
}

typedef union V4
{
    struct
    {
        F32 x, y, z, w;
    };
    struct
    {
        F32 r, g, b, a;
    };
    struct
    {
        F32 x0, x1, y0, y1;
    };
    F32 e[4];
} V4;
typedef V4 Rect;


V4 V4F32(F32 x, F32 y, F32 z, F32 w)
{
    V4 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

typedef union V2
{
    struct
    {
        F32 x, y;
    };
    
    struct
    {
        F32 min, max;
    };
    
    F32 e[2];
} V2;

V2
V2F32(F32 x, F32 y)
{
    V2 result;
    result.x = x;
    result.y = y;
    return result;
}

V2
ScaleV2(F32 a, V2 b)
{
    V2 result;
    result.x = a*b.x;
    result.y = a*b.y;
    return result;
}

V2
OppositeV2(V2 a)
{
    V2 result;
    result.x = -a.x;
    result.y = -a.y;
    return result;
}

V2
AddV2(V2 a, V2 b)
{
    V2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

V2
SubtractV2(V2 a, V2 b)
{
    V2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

#endif //SUMMONED_MATH_H
