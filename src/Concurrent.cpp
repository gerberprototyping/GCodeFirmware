#include "Concurrent.h"




/***********************************************************
 * Semaphore
 **********************************************************/

Semaphore::Semaphore()
    : count(0)
{}


Semaphore::Semaphore(const bool state) {
    count = state ? 1 : 0;
}


Semaphore::Semaphore(const int32_t val)
    : count(val)
{}


void Semaphore::set() volatile {
    count = 1;
    __DMB();                                // Data memory barrier
}


void Semaphore::clear() volatile {
    count = 0;
    __DMB();                                // Data memory barrier
}


bool Semaphore::give() volatile {
    bool success = false;
    int32_t x = __LDREXW(&count);           // Load exclusive
    __DMB();                                // Data memory barrier
    if (0 == x) {
        x = 1;
        if (0 == __STREXW(x, &count)) {     // Attempt store exclusive
            success = true;
        }
    } else {
        __CLREX();                          // Clear exclusive access lock
    }
    __DMB();                                // Data memory barrier
    return success;
}


bool Semaphore::take() volatile {
    bool success = false;
    int32_t x = __LDREXW(&count);           // Load exclusive
    __DMB();                                // Data memory barrier
    if (x > 0) {
        x = 0;
        if (0 == __STREXW(x, &count)) {      // Attempt store exclusive
            success = true;
        }
    } else {
        __CLREX();                          // Clear exclusive access lock
    }
    __DMB();
    return success;
}


bool Semaphore::increment() volatile {
    bool success = false;
    int32_t x = __LDREXW(&count);           // Load exclusive
    __DMB();                                // Data memory barrier
    if (x >= 0 && (++x) > 0) {
        if (0 == __STREXW(x, &count)) {     // Attempt store exclusive
            success = true;
        }
    } else {
        __CLREX();                          // Clear exclusive access lock
    }
    __DMB();                                // Data memory barrier
    return success;
}


bool Semaphore::decrement() volatile {
    bool success = false;
    int32_t x = __LDREXW(&count);           // Load exclusive
    __DMB();                                // Data memory barrier
    if (x > 0) {
        x--;
        if (0 == __STREXW(x, &count)) {     // Attempt store exclusive
            success = true;
        }
    } else {
        __CLREX();                          // Clear exclusive access lock
    }
    __DMB();                                // Data memory barrier
    return success;
}


bool Semaphore::check() const volatile {
    return count > 0;
}


int32_t Semaphore::getCount() const volatile {
    return count;
}




/***********************************************************
 * Lock
 **********************************************************/

Lock::Lock()
    : Semaphore()
{
    // Intentionally left blank
}

void Lock::lock() volatile {
    while(!tryLock());
}

void Lock::unlock() volatile {
    while(!tryUnlock());
}

bool Lock::tryLock() volatile {
    return give();
}

bool Lock::tryUnlock() volatile {
    return take();
}




/***********************************************************
 * AtomicInt32
 **********************************************************/

AtomicInt32::AtomicInt32()
    : Semaphore()
{
    // Intentionally left blank
}

AtomicInt32::AtomicInt32(const int32_t val)
    : Semaphore(val)
{
    // Intentionally left blank
}

int32_t AtomicInt32::load() volatile {
    __DMB();
    int32_t x = __LDREXW(&count);       // Load exclusive
    __CLREX();                          // Clear exclusive access lock
    __DMB();
    return x;
}

void AtomicInt32::store(int32_t val) volatile {
    __DMB();
    while (0 != __STREXW(val, &count));
    __CLREX();
    __DMB();
}




/***********************************************************
 * AtomicInt64
 **********************************************************/

AtomicInt64::AtomicInt64() {
    // Intentionally left blank
}

AtomicInt64::AtomicInt64(const int64_t val) {
    _val[0].store(val >> 32);
    _val[1].store(val);
}

int64_t AtomicInt64::load() volatile {
    int64_t x = _val[0].load() << 32;
    x |= _val[1].load();
    return x;
}

void AtomicInt64::store(const int64_t val) volatile {
    _val[0].store(val >> 32);
    _val[1].store(val);
}

