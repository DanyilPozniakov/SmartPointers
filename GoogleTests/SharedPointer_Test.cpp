#include <SharedPointer.h>
#include <gtest/gtest.h>


//Create by Danyil Pozniakov




TEST(SharedPointerTest, DefaultConstruct)
{
    SharedPointer<int> p;
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);
}

TEST(SharedPointerTest, ConstructFromRaw)
{
    auto* obj = new int(1);
    SharedPointer<int> p(obj);
    EXPECT_TRUE(p);
    EXPECT_EQ(*p, 1);
}

TEST(SharedPointerTest, CopyConstruction)
{
    auto* obj = new int(2);
    SharedPointer<int> p(obj);
    SharedPointer<int> p2(p);
    EXPECT_TRUE(p);
    EXPECT_TRUE(p2);
    EXPECT_EQ(*p, 2);
    EXPECT_EQ(*p2, 2);
}

TEST(SharedPointerTest, MoveConstruct)
{
    auto* obj = new int(3);
    SharedPointer<int> p1(obj);
    SharedPointer<int> p2(std::move(p1));

    EXPECT_FALSE(p1);
    EXPECT_TRUE(p2);

    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*p2.get(), 3);
}

TEST(SharedPointerTest, CopyAssigment)
{
    auto* obj = new int(4);
    SharedPointer<int> p1(obj);
    SharedPointer<int> p2;

    p2 = p1;

    EXPECT_TRUE(p1);
    EXPECT_TRUE(p2);

    EXPECT_EQ(*p1.get(), 4);
    EXPECT_EQ(*p2.get(), 4);
}

TEST(SharedPointerTest, MoveAssigment)
{
    auto* obj = new int(3);
    SharedPointer<int> p1(obj);
    SharedPointer<int> p2;

    p2 = std::move(p1);

    EXPECT_FALSE(p1);
    EXPECT_TRUE(p2);

    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*p2.get(), 3);
}

TEST(SharedPointerTest, CopyOperator)
{
    auto* obj = new int(5);
    SharedPointer<int> a(obj);
    SharedPointer<int> b;
    b = a;
    EXPECT_TRUE(a);
    EXPECT_TRUE(b);

    EXPECT_EQ(*a, *b);
}

TEST(SharedPointerTest, MoveOperator)
{
    auto* obj = new int(5);
    SharedPointer<int> a(obj);
    SharedPointer<int> b;
    b = std::move(a);
    EXPECT_FALSE(a);
    EXPECT_TRUE(b);

    EXPECT_EQ(*b, 5);
}

TEST(SharedPointerTest, OperatorArrow)
{
    std::string* obj = new std::string("Hello");
    SharedPointer<std::string> p(obj);
    EXPECT_EQ(p->at(0), 'H');
}

TEST(SharedPointerTest, OperatorStar)
{
    int* obj = new int(10);
    SharedPointer<int> p(obj);
    EXPECT_EQ(*p, 10);
}

TEST(SharedPointerTest, OperatorBool)
{
    SharedPointer<int> p;
    EXPECT_FALSE(p);
    p = SharedPointer<int>(new int(10));
    EXPECT_TRUE(p);
}

TEST(SharedPointerTest, OperatorSubscript)
{
    int* arr = new int[5]{1, 2, 3, 4, 5};
    SharedPointer<int> p(arr);
    EXPECT_EQ(p[2], 3);
}

TEST(SharedPointerTest, UseCount)
{
    int* obj = new int(5);
    SharedPointer<int> a(obj);
    SharedPointer<int> b(a);
    EXPECT_EQ(a.use_count(), 2);
    EXPECT_EQ(b.use_count(), 2);
}

TEST(SharedPointerTest, unique)
{
    int* obj = new int(5);
    SharedPointer<int> a(obj);
    SharedPointer<int> b(a);
    EXPECT_FALSE(a.unique());
    b.reset();
    EXPECT_TRUE(a.unique());
}

TEST(SharedPointerTest, PointerComparison)
{
    int* obj = new int(5);
    SharedPointer<int> a(obj);
    SharedPointer<int> b(a);
    SharedPointer<int> c(new int(5));
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(SharedPointerTest, Reset)
{
    int* obj = new int(5);
    SharedPointer<int> a(obj);
    a.reset();
    EXPECT_FALSE(a);
}

TEST(SharedPointerTest, Get)
{
    int* obj = new int(5);
    SharedPointer<int> a(obj);
    EXPECT_EQ(a.get(), obj);
}

TEST(SharedPointerTest, AccsessObject)
{
    int* obj = new int(5);
    SharedPointer<int> a(obj);
    EXPECT_EQ(*a.get(), 5);
    EXPECT_EQ(*a, 5);

    std::string* str = new std::string("Hello");
    SharedPointer<std::string> b(str);
    EXPECT_EQ(b->at(0), 'H');
}

TEST(SharedPointerTest, WeakPointer)
{
    int* obj = new int(5);
    SharedPointer<int> a(obj);

    if (WeakPointer<int> w(a); !w.expired())
    {
        EXPECT_TRUE(w);
    }
    else
    {
        EXPECT_TRUE(false);
    }
}
