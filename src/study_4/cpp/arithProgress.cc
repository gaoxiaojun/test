#include "arithProgress.h"

ArithProgress::ArithProgress(long i)
    : Progress(), inc(i)
{
}

long ArithProgress::nextValue()
{
    cur += inc;
    return cur;
}
