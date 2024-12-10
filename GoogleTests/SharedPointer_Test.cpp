#include <SharedPointer.h>
#include <gtest/gtest.h>




TEST(SharedPointerTest, DefaultConstruct) {
    SharedPointer<int> p;
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);
}

TEST(SharedPointerTest, ConstructFromRaw) {
    auto* obj = new int(1);
    SharedPointer<int> p(obj);
    EXPECT_TRUE(p);
    EXPECT_EQ(*p, 1);
}

TEST(SharedPointerTest, CopyConstruction) {
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
    EXPECT_EQ(*p2.get(),3);
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
    EXPECT_EQ(*p2.get(),4);
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
    EXPECT_EQ(*p2.get(),3);
}


