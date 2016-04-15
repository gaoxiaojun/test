#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	vector<string> score = {"F", "D", "C", "B", "A", "A++"};

	for (int grade=0; grade<=100; ++grade)
	{
		string lettergrade;
		if (grade < 60)
			lettergrade = score[0];
		else
		{
			lettergrade = score[(grade-50)/10];
			if (grade != 100)
			{
				if (grade % 10 > 7)
					lettergrade += '+';
				else if (grade % 10 < 3)
					lettergrade += '-';
			}
		}
		cout << grade << " " << lettergrade << endl;
	}

	return 0;
}
