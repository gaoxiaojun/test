#include <iostream>
using namespace std;

class Super
{
    public:
	void print(const Super&)
	{ cout << "Super print Super" << endl;}
};

class Sub : public Super
{
    public:
	void print(const Super&)
	{ cout << "Sub print Super" << endl; }
	void print(const Sub&)
	{ cout << "Sub print Sub" << endl; }
};

int main()
{
    Super super;
    Sub sub;
    Super sup_ref_sub = sub;

    super.print(sub);
    sup_ref_sub.print(sub);

    return 0;
}
