#include <IntrusivePtr.h>

class A : public RefCounter
{
public:
    A(int v = 0) : a(v) {}

    int a = 0;
};



int main()
{
    A* a = new A(10);

    IntrusivePtr<A> p(a);


    return 0;
}

