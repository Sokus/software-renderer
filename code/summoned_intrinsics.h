/* date = August 5th 2021 2:14 pm */

#ifndef SUMMONED_INTRINSICS_H
#define SUMMONED_INTRINSICS_H

#include <math.h>

I32
RoundF32ToI32(F32 value)
{
    I32 result = (I32)roundf(value);
    return result;
}

U32
RoundF32ToU32(F32 value)
{
    U32 result = (U32)roundf(value);
    return result;
}


#endif //SUMMONED_INTRINSICS_H
