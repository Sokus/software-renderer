/* date = August 5th 2021 2:16 pm */

#ifndef SUMMONED_MATH_H
#define SUMMONED_MATH_H


typedef union V2
{
    struct { float x, y; };
    float e[2];
} V2;

typedef union V3
{
    struct { float x, y, z; };
    float e[3];
} V3;

typedef union V4
{
    struct { float x, y, z, w; };
    float e[4];
} V4;

#define DEFINE_V_FUNCTIONS(n) \
V##n V##n##_Add(V##n const a, V##n const b) \
{ \
V##n r; \
for(int i=0; i<n; ++i) \
r.e[i] = a.e[i] + b.e[i]; \
return r; \
} \
V##n V##n##_Sub(V##n const a, V##n const b) \
{ \
V##n r; \
for(int i=0; i<n; ++i) \
r.e[i] = a.e[i] - b.e[i]; \
return r; \
} \

DEFINE_V_FUNCTIONS(2)
DEFINE_V_FUNCTIONS(4)

F32 Lerp(F32 a, F32 t, F32 b)
{
    F32 result = (1.0f - t)*a + t*b;
    return result;
}

int SignOf(int value)
{
    return (0 < value) - (value < 0);
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
    struct
    {
        F32 pitch, yaw, roll, unnamed_rotation;
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

V4 AddV4(V4 a, V4 b)
{
    V4 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

V4 SubtractV4(V4 a, V4 b)
{
    V4 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

V4 ScaleV4(F32 a, V4 b)
{
    V4 result;
    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;
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

V2 V2F32(F32 x, F32 y)
{
    V2 result;
    result.x = x;
    result.y = y;
    return result;
}

V2 ScaleV2(F32 a, V2 b)
{
    V2 result;
    result.x = a*b.x;
    result.y = a*b.y;
    return result;
}

V2 OppositeV2(V2 a)
{
    V2 result;
    result.x = -a.x;
    result.y = -a.y;
    return result;
}

V2 AddV2(V2 a, V2 b)
{
    V2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

V2 SubtractV2(V2 a, V2 b)
{
    V2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

F32 MagnitudeSq(V2 a)
{
    F32 result = (a.x * a.x) + (a.y * a.y);
    return result;
}

#endif //SUMMONED_MATH_H
