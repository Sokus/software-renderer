#include "message.h"

Message* ReadNext(MessageRing* mRing)
{
    Message* pR = mRing->pRead;
    Message* msg = NULL;
    if(pR && pR->text)
    {
        msg = pR;
        pR++;
        if(pR >= mRing->buffer+MESSAGE_RING_SIZE)
            pR = mRing->buffer;
        mRing->pRead = pR;
    }
    return msg;
}

void WriteNext(MessageRing* mRing, Message msg)
{
    Message* pW = mRing->pWrite;
    if(pW)
    {
        free(pW->text);

        pW->text = msg.text;
        pW->color = msg.color;

        pW++;
        if(pW >= mRing->buffer+MESSAGE_RING_SIZE)
            pW = mRing->buffer;
        mRing->pWrite = pW;
    }
}
