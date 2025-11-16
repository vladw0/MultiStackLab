#include "gtest/gtest.h"
#include "Stack.h"
#include "MultiStack.h"

TEST(MultiStackBasic, PushPop) {
    TMultiStack<int> ms(9, 3);
    ms.Push(1, 0);
    ms.Push(2, 0);
    ms.Push(10, 1);
    ms.Push(100, 2);
    EXPECT_EQ(ms.Pop(0), 2);
    EXPECT_EQ(ms.Pop(0), 1);
    EXPECT_EQ(ms.Pop(1), 10);
    EXPECT_EQ(ms.Pop(2), 100);
}

TEST(MultiStackRepackLocalShift, LocalShift) {
    TMultiStack<int> ms(5, 3);
    ms.Push(1,0); ms.Push(2,0);
    ms.Push(10,1); ms.Push(11,1);
    ms.Push(100,2);
    EXPECT_FALSE(ms.IsEmpty(2));
}

TEST(MultiStackIterator, IterAll) {
    TMultiStack<int> ms(6,2);
    ms.Push(1,0); ms.Push(2,1); ms.Push(3,0);
    int sum = 0;
    for (auto it = ms.beginAll(); it != ms.endAll(); ++it) sum += *it;
    EXPECT_GT(sum, 0);
}
