#include <assert.h>
#include "hooksupport.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static HANDLE process = GetCurrentProcess();

class VirtualProtectScope
{
    intptr_t address;
    uint32_t size;
    DWORD protection;

public:

    VirtualProtectScope(intptr_t address, size_t size): address(address), size(size)
    {
        assert(address);
        BOOL result = VirtualProtectEx(process, (void*)address, size, PAGE_EXECUTE_READWRITE, &protection);
        assert(result);
    }

    ~VirtualProtectScope()
    {
        BOOL result = VirtualProtectEx(process, (void*)address, size, protection, &protection);
        assert(result);
        FlushInstructionCache(process, (void*)address, size);
    }

};

intptr_t __stdcall ResolveHookTarget(intptr_t a, intptr_t b, intptr_t c)
{
    static int target = DetermineHookTarget();
    switch (target)
    {
    case 0: return a;
    case 1: return b;
    case 2: return c;
    default:
        assert("Unreachable" && false);
        __assume(0);
        return 0;
    }
}

static const int jump_op_size = 5;
static const char jump_opcode = '\xE9';
static const int call_op_size = 5;
static const char call_opcode = '\xE8';
static const char nop_opcode = '\x90';
static const char break_opcode = '\xCC';

void __stdcall EmitJump(intptr_t location, intptr_t target)
{
    VirtualProtectScope protect(location, jump_op_size);

    *(char*)location = jump_opcode;
    *(intptr_t*)(location + 1) = target - (location + jump_op_size);
}

void __stdcall EmitCall(intptr_t location, intptr_t target)
{
    VirtualProtectScope protect(location, call_op_size);

    *(char*)location = call_opcode;
    *(intptr_t*)(location + 1) = target - (location + call_op_size);
}

void __stdcall EmitNop(intptr_t location, int count)
{
    VirtualProtectScope protect(location, count);

    intptr_t location_end = location + count;
    while (location < location_end) *(char*)location++ = nop_opcode;
}

void __stdcall EmitBreakpoint(intptr_t location, int count)
{
    VirtualProtectScope protect(location, count);

    intptr_t location_end = location + count;
    while (location < location_end) *(char*)location++ = break_opcode;
}

intptr_t __stdcall AttachAT(intptr_t location, intptr_t target)
{
    EmitJump(location, target);
    return target;
}
