#pragma once

// "external application" code
// used only for easy address access

class TestObject_Compiled
{
public:

    TestObject_Compiled();
    void Show();

private:

    bool CanShow();

    short ShowCount; // 0
    int Value;       // 4

}; // sizeof = 8

extern int TestObjectCount_Compiled;
extern TestObject_Compiled* TestObjects_Compiled[16];

void RunObjectTests_Compiled();