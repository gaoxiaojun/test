#include <iostream>
using namespace std;
class Test
{
public:
	Test(){ cout << "construction!"<< endl;}
	~Test() {cout << "destruction!" << endl;}
	int test();

private:
	int nii;
};
int Test::test()
{
	cout << nii <<endl;
	return 10;
}
typedef int(Test::*entry_t)();
int main()
{
	Test *p = new Test();
	delete p;
	p = NULL;
	entry_t shit = &Test::test;
	cout << shit<< endl;
	shit = 0;
	cout << p->test() << endl;
	return 0;
}
