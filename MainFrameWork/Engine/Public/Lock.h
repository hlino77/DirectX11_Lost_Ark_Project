#pragma once
#include <atomic>
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL Lock
{
    enum : unsigned __int32
    {
        ACQUIRE_TIMEOUT_TICK = 10000,
        MAX_SPIN_COUNT = 5000,
        WRITE_THREAD_MASK = 0xFFFF'0000,
        READ_COUNT_MASK = 0x0000'FFFF,
        EMPTY_FLAG = 0x0000'0000
    };

public:
    void WriteLock(const char* name);
    void WriteUnlock(const char* name);
    void ReadLock(const char* name);
    void ReadUnlock(const char* name);

private:
    std::atomic<unsigned __int32> _lockFlag = EMPTY_FLAG;
    unsigned __int16 _writeCount = 0;
};

class ENGINE_DLL ReadLockGuard
{
public:
    ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
    ~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
    Lock& _lock;
    const char* _name;
};

class ENGINE_DLL WriteLockGuard
{
public:
    WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
    ~WriteLockGuard() { _lock.WriteUnlock(_name); }

private:
    Lock& _lock;
    const char* _name;
};

END