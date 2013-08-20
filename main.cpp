#include <cstdio>

#include "hooksupport.h"
#include "testobject.h"
#include "testobject_compiled.h"

REF_DEF(int, TestObjectCount, (intptr_t)&TestObjectCount_Compiled, 0, 0);
REF_ARR_DEF(TestObject*, TestObjects, 16, (intptr_t)&TestObjects_Compiled, 0, 0);

// referenced data is available after static initialization
// external functions are always available

int DetermineHookTarget()
{
    // usually called *once* during static initialization
    // original: compare data at address in process to known data at that address
    // returns 0, 1, or 2 (client, server, leveledit)
    return 0;
}

void ApplyHooks()
{
    //HookAsJump(new_function, client, server, address)
    HookAsJump(TestObject::CanShow, TestObject_Compiled::CanShow, 0, 0);
}

int main()
{
    ApplyHooks();

    for (int i = 0; i < 16; ++i)
    {
        TestObjects[i] = new TestObject();
        TestObjects[i]->SetValue(i * 2);
    }
    TestObjectCount = 16;

    RunObjectTests();

    for (int i = 0; i < TestObjectCount; ++i)
    {
        delete TestObjects[i];
    }
    TestObjectCount = 0;
}