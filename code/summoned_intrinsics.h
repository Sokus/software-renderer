/* date = August 5th 2021 2:14 pm */

#ifndef SUMMONED_INTRINSICS_H
#define SUMMONED_INTRINSICS_H

#include <math.h>

F32 RoundF32(F32 value)
{
    F32 result = roundf(value);
    return result;
}

F32 FloorF32(F32 value)
{
    F32 result = floorf(value);
    return result;
}

F32 Sin(F32 angle)
{
    F32 result = sinf(angle);
    return result;
}

F32 Cos(F32 angle)
{
    F32 result = cosf(angle);
    return result;
}

F32 Tan(F32 angle)
{
    F32 result = tanf(angle);
    return result;
}

#endif //SUMMONED_INTRINSICS_H
