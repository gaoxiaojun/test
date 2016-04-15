#include <iostream>
using namespace std;

class Super
{
    public:
        void print(const Super* )
	{
	    cout << "super print Super" << endl;
	}
};

class Sub : public Super
{
    public:
	void print(const Super* )
	{
	    cout << "sub print super" << endl;
	}
	void print(const Sub *)
	{
	    cout << "sub print sub" << endl;
	}
};

int main()
{
    Super *super = new Super();
    Sub *sub = new Sub();
    Super *sup_ref_sub = new Sub();

    super->print(sub);
    sup_ref_sub->print(sub);

    delete super;
    delete sub;
    delete sup_ref_sub;

    return 0;
}
