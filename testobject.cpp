#include "hooksupport.h"
#include "testobject.h"
#include "testobject_compiled.h"

const short CanShowDelay = 4;

TestObject::TestObject(): ShowCount(0), TestCount(0), Value(0)
{

}

EXTERNAL_FUNCTION
void TestObject::Show()
AT(TestObject_Compiled::Show, 0, 0)

bool TestObject::CanShow()
{
    if (++TestCount >= CanShowDelay)
    {
        TestCount = 0;
        return true;
    }

    return false;
}

EXTERNAL_FUNCTION
void RunObjectTests()
AT(RunObjectTests_Compiled, 0, 0);