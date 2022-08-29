#include "Concurrent.h"


/***********************************************************
 * Lock
 **********************************************************/

Lock::Lock() {
    _lock = 0;
    __DMB();                                // Data memory barrier
}

void Lock::lock() volatile {
    while(!tryLock());
}

void Lock::unlock() volatile {
    _lock = 0;
    __DMB();                                // Data memory barrier
}

bool Lock::status() const volatile {
    __DMB();                                // Data memory barrier
    int8_t x = _lock;
    return 0 != x;
}

bool Lock::tryLock() volatile {
    bool success = false;
    __DMB();                                // Data memory barrier
    int8_t x = __LDREXB(&_lock);            // Load exclusive
    if (0 == x) {
        if (0 == __STREXB(1, &_lock)) {     // Attempt store exclusive
            success = true;
        }
    } else {
        __CLREX();                          // Clear exclusive access lock
    }
    __DMB();                                // Data memory barrier
    return success;
}




/***********************************************************
 * AtomicInt32
 **********************************************************/

AtomicInt32::AtomicInt32() {
    this->_val = 0;
    __DMB();                                // Data memory barrier
}

AtomicInt32::AtomicInt32(const int32_t &val) {
    this->_val = val;
    __DMB();                                // Data memory barrier
}

int32_t AtomicInt32::load() const volatile {
    __DMB();                                // Data memory barrier
    int32_t x = this->_val;
    return x;
}

void AtomicInt32::store( const int32_t &val) volatile {
    while(!tryStore(val));
}

bool AtomicInt32::tryStore(const int32_t &val) volatile {
    bool success = false;
    __DMB();                                // Data memory barrier
    if (0 == __STREXW(0, &this->_val)) {    // Attempt store exclusive
        success = true;
    }
    __DMB();                                // Data memory barrier
    return success;
}

bool AtomicInt32::fetch_add(const int32_t &val) volatile {
    bool success = false;
    while (!success) {
        __DMB();                                    // Data memory barrier
        int32_t x = __LDREXW(&_val);                // Load exclusive
        if (0 == __STREXW(x + val, &this->_val)) {  // Attempt store exclusive
            success = true;
        }
        __DMB();                                    // Data memory barrier
    }
    return success;
}

bool AtomicInt32::fetch_sub(const int32_t &val) volatile {
    bool success = false;
    while (!success) {
        __DMB();                                    // Data memory barrier
        int32_t x = __LDREXW(&_val);                // Load exclusive
        if (0 == __STREXW(x - val, &this->_val)) {  // Attempt store exclusive
            success = true;
        }
        __DMB();                                    // Data memory barrier
    }
    return success;
}
