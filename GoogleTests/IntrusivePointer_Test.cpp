#include "IntrusivePtr.h"
#include <gtest/gtest.h>


//Generate with OpenAI o1


class TestObject : public RefCounter
{
public:
    TestObject(int v = 0) : value(v)
    {
    }

    int value;
};


// Basic construction tests
TEST(IntrusivePtrTest, DefaultConstruct)
{
    IntrusivePtr<TestObject> p;
    EXPECT_FALSE(p);
    EXPECT_EQ(p.get(), nullptr);
}

TEST(IntrusivePtrTest, ConstructFromRaw)
{
    auto* obj = new TestObject(1);
    IntrusivePtr<TestObject> p(obj);
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 1);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, ConstructFromRawMultiple)
{
    auto* obj = new TestObject(2);
    IntrusivePtr<TestObject> p(obj);
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 2);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, CopyConstruction)
{
    auto* obj = new TestObject(3);
    IntrusivePtr<TestObject> p(obj);
    IntrusivePtr<TestObject> q(p);
    EXPECT_TRUE(q);
    EXPECT_EQ(q->value, 3);
    p.reset();
    EXPECT_TRUE(q);
    q.reset();
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, MoveConstruction)
{
    auto* obj = new TestObject(4);
    IntrusivePtr<TestObject> p(obj);
    IntrusivePtr<TestObject> q(std::move(p));
    EXPECT_TRUE(q);
    EXPECT_EQ(q->value, 4);
    EXPECT_FALSE(p);
    q.reset();
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, CopyAssignment)
{
    auto* obj = new TestObject(5);
    IntrusivePtr<TestObject> p(obj);
    IntrusivePtr<TestObject> q;
    q = p;
    EXPECT_TRUE(q);
    EXPECT_EQ(q->value, 5);
    p.reset();
    EXPECT_EQ(q->value, 5);
    q.reset();
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, MoveAssignment)
{
    auto* obj = new TestObject(6);
    IntrusivePtr<TestObject> p(obj);
    IntrusivePtr<TestObject> q;
    q = std::move(p);
    EXPECT_TRUE(q);
    EXPECT_EQ(q->value, 6);
    EXPECT_FALSE(p);
    q.reset();
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, DereferenceOperator)
{
    auto* obj = new TestObject(7);
    IntrusivePtr<TestObject> p(obj);
    EXPECT_EQ((*p).value, 7);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, ArrowOperator)
{
    auto* obj = new TestObject(8);
    IntrusivePtr<TestObject> p(obj);
    EXPECT_EQ(p->value, 8);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, BoolConversion)
{
    IntrusivePtr<TestObject> p;
    EXPECT_FALSE(p);
    p = new TestObject(9);
    EXPECT_TRUE(p);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, Reset)
{
    IntrusivePtr<TestObject> p(new TestObject(10));
    EXPECT_TRUE(p);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, Swap)
{
    IntrusivePtr<TestObject> p(new TestObject(11));
    IntrusivePtr<TestObject> q(new TestObject(12));
    p.swap(q);
    EXPECT_EQ(p->value, 12);
    EXPECT_EQ(q->value, 11);
    p.reset();
    q.reset();
}

TEST(IntrusivePtrTest, MultipleRefs)
{
    auto* obj = new TestObject(13);
    IntrusivePtr<TestObject> p(obj);
    {
        IntrusivePtr<TestObject> q(p);
        EXPECT_EQ(p->value, 13);
        EXPECT_EQ(q->value, 13);
    }
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, AssignNewRawPointer)
{
    IntrusivePtr<TestObject> p(new TestObject(14));
    p = new TestObject(15);
    EXPECT_EQ(p->value, 15);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, DoubleReset)
{
    IntrusivePtr<TestObject> p(new TestObject(16));
    p.reset();
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, SelfAssignment)
{
    IntrusivePtr<TestObject> p(new TestObject(17));
    p = p;
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 17);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, SelfMoveAssignment)
{
    IntrusivePtr<TestObject> p(new TestObject(18));
    p = std::move(p);
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 18);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, MultipleCopyAssignments)
{
    auto* obj = new TestObject(19);
    IntrusivePtr<TestObject> p(obj);
    IntrusivePtr<TestObject> q;
    IntrusivePtr<TestObject> r;
    q = p;
    r = p;
    EXPECT_EQ(q->value, 19);
    EXPECT_EQ(r->value, 19);
    p.reset();
    q.reset();
    r.reset();
}

TEST(IntrusivePtrTest, MultipleMoveAssignments)
{
    IntrusivePtr<TestObject> p(new TestObject(20));
    IntrusivePtr<TestObject> q;
    IntrusivePtr<TestObject> r;
    q = std::move(p);
    r = std::move(q);
    EXPECT_EQ(r->value, 20);
    r.reset();
    EXPECT_FALSE(r);
}

TEST(IntrusivePtrTest, ArrowOperatorMultipleTimes)
{
    IntrusivePtr<TestObject> p(new TestObject(21));
    EXPECT_EQ(p->value, 21);
    EXPECT_EQ(p->value, 21);
    p.reset();
}

TEST(IntrusivePtrTest, DereferenceMultipleTimes)
{
    IntrusivePtr<TestObject> p(new TestObject(22));
    EXPECT_EQ((*p).value, 22);
    (*p).value = 23;
    EXPECT_EQ(p->value, 23);
    p.reset();
}

TEST(IntrusivePtrTest, SwapMultipleTimes)
{
    IntrusivePtr<TestObject> p(new TestObject(24));
    IntrusivePtr<TestObject> q(new TestObject(25));
    IntrusivePtr<TestObject> r(new TestObject(26));
    p.swap(q);
    EXPECT_EQ(p->value, 25);
    q.swap(r);
    EXPECT_EQ(q->value, 26);
    p.reset();
    q.reset();
    r.reset();
}

TEST(IntrusivePtrTest, ResetAfterMove)
{
    IntrusivePtr<TestObject> p(new TestObject(27));
    IntrusivePtr<TestObject> q(std::move(p));
    q.reset();
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, CopyAfterReset)
{
    IntrusivePtr<TestObject> p(new TestObject(28));
    p.reset();
    IntrusivePtr<TestObject> q(p);
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, MoveAfterReset)
{
    IntrusivePtr<TestObject> p(new TestObject(29));
    p.reset();
    IntrusivePtr<TestObject> q(std::move(p));
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, MultipleOperations)
{
    IntrusivePtr<TestObject> p;
    p = new TestObject(30);
    EXPECT_EQ(p->value, 30);
    p.reset();
    p = new TestObject(31);
    EXPECT_EQ(p->value, 31);
    IntrusivePtr<TestObject> q(p);
    EXPECT_EQ(q->value, 31);
    p.reset();
    q.reset();
}

TEST(IntrusivePtrTest, MakeIntrusive)
{
    auto p = make_intrusive<TestObject>();
    EXPECT_TRUE(p);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, ValueCheck32)
{
    IntrusivePtr<TestObject> p(new TestObject(32));
    EXPECT_EQ(p->value, 32);
    p.reset();
}

TEST(IntrusivePtrTest, ValueCheck33)
{
    IntrusivePtr<TestObject> p(new TestObject(33));
    IntrusivePtr<TestObject> q(p);
    EXPECT_EQ(q->value, 33);
    p.reset();
    q.reset();
    EXPECT_FALSE(p);
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, ValueCheck34)
{
    IntrusivePtr<TestObject> p(new TestObject(34));
    IntrusivePtr<TestObject> q(std::move(p));
    EXPECT_TRUE(q);
    EXPECT_EQ(q->value, 34);
    q.reset();
}

TEST(IntrusivePtrTest, AssignCheck35)
{
    IntrusivePtr<TestObject> p;
    p = new TestObject(35);
    EXPECT_EQ(p->value, 35);
    p.reset();
}

TEST(IntrusivePtrTest, AssignCheck36)
{
    IntrusivePtr<TestObject> p(new TestObject(36));
    p = new TestObject(37);
    EXPECT_EQ(p->value, 37);
    p.reset();
}

TEST(IntrusivePtrTest, ResetCheck)
{
    IntrusivePtr<TestObject> p(new TestObject(38));
    EXPECT_TRUE(p);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, MultipleRefsAgain)
{
    auto* obj = new TestObject(39);
    IntrusivePtr<TestObject> p(obj);
    {
        IntrusivePtr<TestObject> q(p);
        IntrusivePtr<TestObject> r(q);
        EXPECT_EQ(r->value, 39);
    }
    EXPECT_TRUE(p);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, LargeValueCheck)
{
    IntrusivePtr<TestObject> p(new TestObject(1000));
    EXPECT_EQ(p->value, 1000);
    p.reset();
}

// Additional unique tests (no macros, more scenarios):

TEST(IntrusivePtrTest, ChainAssignment)
{
    IntrusivePtr<TestObject> p(new TestObject(2001));
    IntrusivePtr<TestObject> q(new TestObject(2002));
    IntrusivePtr<TestObject> r(new TestObject(2003));
    p = q;
    q = r;
    EXPECT_EQ(p->value, 2002);
    EXPECT_EQ(q->value, 2003);
    r.reset();
    p.reset();
    q.reset();
}

TEST(IntrusivePtrTest, AssignFromGet)
{
    IntrusivePtr<TestObject> p(new TestObject(2004));
    TestObject* raw = p.get();
    IntrusivePtr<TestObject> q;
    q = raw;
    EXPECT_EQ(q->value, 2004);
    q.reset();
    p.reset();
}

TEST(IntrusivePtrTest, ComplexResetSequence)
{
    IntrusivePtr<TestObject> p(new TestObject(2005));
    IntrusivePtr<TestObject> q(p);
    p.reset();
    EXPECT_TRUE(q);
    q.reset();
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, MultipleSwaps)
{
    IntrusivePtr<TestObject> p(new TestObject(2006));
    IntrusivePtr<TestObject> q(new TestObject(2007));
    IntrusivePtr<TestObject> r(new TestObject(2008));
    p.swap(q);
    q.swap(r);
    r.swap(p);
    EXPECT_EQ(p->value, 2006);
    EXPECT_EQ(q->value, 2008);
    EXPECT_EQ(r->value, 2007);
    p.reset();
    q.reset();
    r.reset();
}

TEST(IntrusivePtrTest, AssignNullMultipleTimes)
{
    IntrusivePtr<TestObject> p(new TestObject(2009));
    p = (TestObject*)nullptr;
    p = (TestObject*)nullptr;
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, MoveToSelfIndirect)
{
    IntrusivePtr<TestObject> p(new TestObject(2010));
    IntrusivePtr<TestObject> q(p);
    p = std::move(q);
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 2010);
    p.reset();
}

TEST(IntrusivePtrTest, CopyToSelfIndirect)
{
    IntrusivePtr<TestObject> p(new TestObject(2011));
    IntrusivePtr<TestObject> q(p);
    p = q;
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 2011);
    p.reset();
}

TEST(IntrusivePtrTest, ResetOnEmpty)
{
    IntrusivePtr<TestObject> p;
    p.reset();
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, AssignToItselfWithNull)
{
    IntrusivePtr<TestObject> p;
    p = p;
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, AssignToDifferentObjectsMultipleTimes)
{
    IntrusivePtr<TestObject> p;
    p = new TestObject(2012);
    p = new TestObject(2013);
    p = new TestObject(2014);
    EXPECT_EQ(p->value, 2014);
    p.reset();
}

TEST(IntrusivePtrTest, AssignFromFunction)
{
    auto createObj = []() { return new TestObject(2015); };
    IntrusivePtr<TestObject> p;
    p = createObj();
    EXPECT_EQ(p->value, 2015);
    p.reset();
}

TEST(IntrusivePtrTest, SwapWithItself)
{
    IntrusivePtr<TestObject> p(new TestObject(2016));
    p.swap(p);
    EXPECT_EQ(p->value, 2016);
    p.reset();
}

TEST(IntrusivePtrTest, MoveAssignmentFromEmptyToNonEmpty)
{
    IntrusivePtr<TestObject> p(new TestObject(2017));
    IntrusivePtr<TestObject> q;
    p = std::move(q);
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, CopyAssignmentFromEmptyToNonEmpty)
{
    IntrusivePtr<TestObject> p(new TestObject(2018));
    IntrusivePtr<TestObject> q;
    p = q;
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, MoveAssignmentFromNonEmptyToEmpty)
{
    IntrusivePtr<TestObject> p;
    IntrusivePtr<TestObject> q(new TestObject(2019));
    p = std::move(q);
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 2019);
    p.reset();
}

TEST(IntrusivePtrTest, ChainedCopies)
{
    IntrusivePtr<TestObject> p(new TestObject(2020));
    IntrusivePtr<TestObject> a(p);
    IntrusivePtr<TestObject> b(a);
    IntrusivePtr<TestObject> c(b);
    IntrusivePtr<TestObject> d(c);
    EXPECT_EQ(d->value, 2020);
    a.reset();
    b.reset();
    c.reset();
    d.reset();
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, ChainedMoves)
{
    IntrusivePtr<TestObject> p(new TestObject(2021));
    IntrusivePtr<TestObject> a(std::move(p));
    IntrusivePtr<TestObject> b(std::move(a));
    IntrusivePtr<TestObject> c(std::move(b));
    EXPECT_TRUE(c);
    EXPECT_EQ(c->value, 2021);
    c.reset();
}

TEST(IntrusivePtrTest, AssignRawPointerThenNull)
{
    IntrusivePtr<TestObject> p;
    p = new TestObject(2022);
    p = (TestObject*)nullptr;
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, LargeChainReset)
{
    IntrusivePtr<TestObject> p(new TestObject(2023));
    IntrusivePtr<TestObject> q(p);
    IntrusivePtr<TestObject> r(q);
    r.reset();
    q.reset();
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, AssignAndResetInLoop)
{
    for (int i = 0; i < 5; ++i)
    {
        IntrusivePtr<TestObject> p(new TestObject(i));
        EXPECT_EQ(p->value, i);
        p.reset();
        EXPECT_FALSE(p);
    }
}

TEST(IntrusivePtrTest, MoveMultipleTimes)
{
    IntrusivePtr<TestObject> p(new TestObject(2024));
    IntrusivePtr<TestObject> q(std::move(p));
    IntrusivePtr<TestObject> r(std::move(q));
    IntrusivePtr<TestObject> s(std::move(r));
    EXPECT_TRUE(s);
    EXPECT_EQ(s->value, 2024);
    s.reset();
}

TEST(IntrusivePtrTest, CopyMultipleTimes)
{
    IntrusivePtr<TestObject> p(new TestObject(2025));
    IntrusivePtr<TestObject> q(p);
    IntrusivePtr<TestObject> r(q);
    IntrusivePtr<TestObject> s(r);
    EXPECT_EQ(s->value, 2025);
    p.reset();
    q.reset();
    r.reset();
    s.reset();
}

TEST(IntrusivePtrTest, AssignNullToNonEmptyRepeatedly)
{
    IntrusivePtr<TestObject> p(new TestObject(2026));
    p = nullptr;
    p = nullptr;
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, AssignSelfRawPointer)
{
    // This is unusual but let's test assigning get() to itself
    IntrusivePtr<TestObject> p(new TestObject(2027));
    p = p.get();
    EXPECT_TRUE(p);
    EXPECT_EQ(p->value, 2027);
    p.reset();
}

TEST(IntrusivePtrTest, SwapInChain)
{
    IntrusivePtr<TestObject> p(new TestObject(1));
    IntrusivePtr<TestObject> q(new TestObject(2));
    IntrusivePtr<TestObject> r(new TestObject(3));
    p.swap(q);
    q.swap(r);
    p.swap(r);
    EXPECT_EQ(p->value, 1);
    EXPECT_EQ(q->value, 3);
    EXPECT_EQ(r->value, 2);
    p.reset();
    q.reset();
    r.reset();
}

TEST(IntrusivePtrTest, ResetMultiplePointers)
{
    IntrusivePtr<TestObject> p(new TestObject(2031));
    IntrusivePtr<TestObject> q(new TestObject(2032));
    p.reset();
    q.reset();
    EXPECT_FALSE(p);
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, MoveFromResetObject)
{
    IntrusivePtr<TestObject> p(new TestObject(2033));
    p.reset();
    IntrusivePtr<TestObject> q(std::move(p));
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, CopyFromResetObject)
{
    IntrusivePtr<TestObject> p(new TestObject(2034));
    p.reset();
    IntrusivePtr<TestObject> q(p);
    EXPECT_FALSE(q);
}

TEST(IntrusivePtrTest, AssignAfterMultipleResets)
{
    IntrusivePtr<TestObject> p;
    p = new TestObject(2035);
    EXPECT_EQ(p->value, 2035);
    p.reset();
    p = new TestObject(2036);
    EXPECT_EQ(p->value, 2036);
    p.reset();
    EXPECT_FALSE(p);
}

TEST(IntrusivePtrTest, ComplexAssignSequence)
{
    IntrusivePtr<TestObject> p(new TestObject(2037));
    IntrusivePtr<TestObject> q(new TestObject(2038));
    IntrusivePtr<TestObject> r(new TestObject(2039));
    p = q;
    q = r;
    r = p;
    EXPECT_EQ(r->value, 2038);
    p.reset();
    q.reset();
    r.reset();
}

TEST(IntrusivePtrTest, NoLeakingAfterMultipleOperations)
{
    // Just stress test
    for (int i = 0; i < 10; ++i)
    {
        IntrusivePtr<TestObject> p(new TestObject(i));
        IntrusivePtr<TestObject> q(std::move(p));
        q.reset();
    }
    SUCCEED();
}

TEST(IntrusivePtrTest, AssignReturnedRawPointer)
{
    auto fn = [] { return new TestObject(2040); };
    IntrusivePtr<TestObject> p;
    p = fn();
    EXPECT_EQ(p->value, 2040);
    p.reset();
}

TEST(IntrusivePtrTest, SwapNonEmptyWithEmpty)
{
    IntrusivePtr<TestObject> p(new TestObject(2041));
    IntrusivePtr<TestObject> q;
    p.swap(q);
    EXPECT_FALSE(p);
    EXPECT_EQ(q->value, 2041);
    q.reset();
}

TEST(IntrusivePtrTest, ManyRefsInNestedScopes)
{
    IntrusivePtr<TestObject> p(new TestObject(2042));
    {
        IntrusivePtr<TestObject> a(p);
        {
            IntrusivePtr<TestObject> b(a);
            EXPECT_EQ(b->value, 2042);
        }
        EXPECT_EQ(a->value, 2042);
    }
    EXPECT_EQ(p->value, 2042);
    p.reset();
}

TEST(IntrusivePtrTest, AssignToDifferentPointerThenReset)
{
    IntrusivePtr<TestObject> p(new TestObject(2043));
    p = new TestObject(2044);
    EXPECT_EQ(p->value, 2044);
    p.reset();
}

TEST(IntrusivePtrTest, MoveChainCheck)
{
    IntrusivePtr<TestObject> p(new TestObject(2045));
    IntrusivePtr<TestObject> q(std::move(p));
    IntrusivePtr<TestObject> r(std::move(q));
    IntrusivePtr<TestObject> s(std::move(r));
    EXPECT_FALSE(q);
    EXPECT_TRUE(s);
    EXPECT_EQ(s->value, 2045);
    s.reset();
}

TEST(IntrusivePtrTest, CopyChainCheck)
{
    IntrusivePtr<TestObject> p(new TestObject(2046));
    IntrusivePtr<TestObject> q(p);
    IntrusivePtr<TestObject> r(q);
    IntrusivePtr<TestObject> s(r);
    EXPECT_EQ(s->value, 2046);
    p.reset();
    q.reset();
    r.reset();
    s.reset();
    EXPECT_FALSE(p);
}
