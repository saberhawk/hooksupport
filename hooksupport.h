#pragma once

#include <cstdint>

int DetermineHookTarget();
intptr_t __stdcall ResolveHookTarget(intptr_t a, intptr_t b, intptr_t c);
void __stdcall EmitJump(intptr_t location, intptr_t target);
void __stdcall EmitCall(intptr_t location, intptr_t target);
void __stdcall EmitPointer(intptr_t location, intptr_t value);
void __stdcall EmitNop(intptr_t location, int count);
void __stdcall EmitBreakpoint(intptr_t location, int count);
intptr_t __stdcall AttachAT(intptr_t location, intptr_t target);

#define ResolveHook_(a, b, c)       \
__asm {                             \
    __asm push c                    \
    __asm push b                    \
    __asm push a                    \
    __asm call ResolveHookTarget    \
    __asm push eax                  \
}

#define HookAsJump(function, a, b, c)   \
__asm {                                 \
    __asm push function                 \
    ResolveHook_(a, b, c)               \
    __asm call EmitJump                 \
}

#define HookAsCall(function, a, b, c)   \
__asm {                                 \
    __asm push function                 \
    ResolveHook_(a, b, c)               \
    __asm call EmitCall                 \
}

#define HookPointer(function, a, b, c)  \
__asm {                                 \
    __asm push function                 \
    ResolveHook(a, b, c)                \
    __asm call EmitPointer              \
}

#define HookNop(count, a, b, c) \
__asm {                         \
    __asm push count            \
    ResolveHook_(a, b, c)       \
    __asm call EmitNop          \
}

#define HookHazard(count, a, b, c)  \
__asm {                             \
    __asm push count                \
    ResolveHook_(a, b, c)           \
    __asm call EmitBreakpoint       \
}

#define EXTERNAL_FUNCTION  __declspec(naked)
#define AT(a, b, c)                 \
{ __asm {                           \
    __asm ATPatchStart:             \
    __asm push ecx                  \
    ResolveHook_(a, b, c)           \
    __asm push offset ATPatchStart  \
    __asm call AttachAT             \
    __asm pop ecx                   \
    __asm jmp eax                   \
}}

#define REF_DECL(type, name) type & name
#define REF_DEF(type, name, a, b, c) type & name = *(type*)ResolveHookTarget(a, b, c)

#define REF_ARR_DECL(type, name, size) RefArrayHelper<type, size> & name
#define REF_ARR_DEF(type, name, size, a, b, c) RefArrayHelper<type, size> & name = *(RefArrayHelper<type, size>*)ResolveHookTarget(a, b, c)

template <typename T, const int size>
struct RefArrayHelper
{
    operator T* () { return (T*)this; }
    operator const T* () const { return (T*)this; };
private:
    char padding[size * sizeof(T)];
};
