#include <iostream>
#include "progress.h"
using namespace std;

void Progression::printProgression(int n)
{
    cout << firstValue();
    for (int i=2; i<=n; ++i)
	cout << ' ' << nextValue();
    cout << endl;
}

long Progression::firstValue()
{
    cur = first;
    return cur;
}

long Progression::nextValue()
{
    return ++cur;
}


