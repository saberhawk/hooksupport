#include <cstdio>
#include "testobject_compiled.h"

int TestObjectCount_Compiled = 0;
TestObject_Compiled* TestObjects_Compiled[16];

TestObject_Compiled::TestObject_Compiled(): ShowCount(0), Value(0)
{
}

bool TestObject_Compiled::CanShow()
{
    return false;
}

void TestObject_Compiled::Show()
{
    if (CanShow())
    {
        ++ShowCount;
        printf("The value is %d.\n", Value);
    }
}

void RunObjectTests_Compiled()
{
    for (int i = 0; i < TestObjectCount_Compiled; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            TestObjects_Compiled[i]->Show();
        }
    }
}