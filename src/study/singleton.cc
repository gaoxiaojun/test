#include <iostream>
#include <string>
using namespace std;

class Singleton
{
public:
    void setName(const string &nm)
    { name = nm; }
    string getName()
    { return name; }
    static Singleton *getInstance()
    {
	static Singleton instance;
	return &instance;
    }
private:
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
    ~Singleton() { cout << "I'm not willing to leave this place, woo" << endl; }
    string name;
};

int main()
{
    Singleton *instance = Singleton::getInstance();
    instance->setName("Hello");
    Singleton *other_instance = Singleton::getInstance();
    cout << other_instance->getName() << endl;

    other_instance->setName("World");
    cout << instance->getName() << endl;

    return 0;
}
