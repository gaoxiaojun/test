#ifndef ARITH_PROGRESS_H
#define ARITH_PROGRESS_H

#include "progress.h"

class ArithProgression : public Progression
{
public:
    ArithProgression(long i = 1);
protected:
    virtual long nextValue();
protected:
    long inc;
};

#endif
