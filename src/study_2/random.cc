#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main()
{
	vector<int> ivec;
	for (int i=0; i<52; ++i)
		ivec.push_back(i);

	srand(time(NULL));
	random_shuffle(ivec.begin(), ivec.end());
	for (vector<int>::iterator it=ivec.begin();
			it!=ivec.end(); ++it)
		cout << *it << " ";
	cout << endl;

	return 0;
}
