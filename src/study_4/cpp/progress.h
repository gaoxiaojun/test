#ifndef PROGRESS_H
#define PROGRESS_H

class Progression
{
public:
    Progression(long f = 0)
	: first(f), cur(f) {}
    virtual ~Progression() {}
    void printProgression(int n);
protected:
    virtual long firstValue();
    virtual long nextValue();
protected:
    long first;
    long cur;
};

#endif
