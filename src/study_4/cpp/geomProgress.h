#ifndef GEOM_PROGRESS_H
#define GEOM_PROGRESS_H

#include "progress.h"

class GeomProgression : public Progression
{
public:
    GeomProgression(long b = 2);
protected:
    virtual long nextValue();
protected:
    long base;
};

#endif
