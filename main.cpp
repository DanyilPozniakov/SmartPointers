#include <iostream>
#include <IntrusivePtr.h>

#include <memory>


class A : public RefCounter
{
public:
    A(int v = 0) : a(v) {}

    int a = 0;
};



int main()
{
    A* a = new A(10);


    IntrusivePtr<A> b(a);
    std::cout << b.get()->a << std::endl;
    return 0;
}

