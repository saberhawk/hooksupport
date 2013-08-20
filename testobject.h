#pragma once

// class hook rules:
// can't usually change base classes or order
// can't change size unless fully derived
// can't reorder instance variables
// can't reorder virtual methods
// can't add new virtual methods unless fully derived
// can add new instance fields in unused space
// can add/reorder static variables
// can add/reorder instance and static methods

class TestObject
{
public:

    TestObject();

    void Show();
    void SetValue(int value) { Value = value; }

private:

    bool CanShow();

    short ShowCount; // 0
    short TestCount; // 2 - new
    int Value;       // 4

}; // sizeof == 8

void RunObjectTests();
