#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdlib.h>

#include "color.h"

#define MESSAGE_RING_SIZE 128

typedef struct Message
{
    char* text;
    Color color;
} Message;

typedef struct MessageRing
{
    Message* pRead;
    Message* pWrite;
    Message buffer[MESSAGE_RING_SIZE];
} MessageRing;

Message* ReadNext(MessageRing* mRing);
void WriteNext(MessageRing* mRing, Message msg);

#endif