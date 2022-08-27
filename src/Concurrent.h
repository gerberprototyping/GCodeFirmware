#ifndef __CONCURRENT_H
#define __CONCURRENT_H


#include <cstdint>

#include "stm32l476xx.h"




class Semaphore {

    public:

        Semaphore();
        Semaphore(const bool state);
        Semaphore(const int32_t val);

        Semaphore& operator=(const Semaphore &S) = delete;
        Semaphore& operator=(const Semaphore &S) volatile = delete;

        void set() volatile;
        void clear() volatile;

        bool give() volatile;
        bool take() volatile;

        bool increment() volatile;
        bool decrement() volatile;

        bool check() const volatile;
        int32_t getCount() const volatile;

    protected:
    
        volatile int32_t count;

};


class Lock: protected Semaphore {

    public:

        Lock();

        void lock() volatile;
        void unlock() volatile;
        bool tryLock() volatile;
        bool tryUnlock() volatile;

};


class AtomicInt32: protected Semaphore {

    public:

        AtomicInt32();
        AtomicInt32(const int32_t val);

        int32_t load() volatile;
        void store(int32_t val) volatile;

};


class AtomicInt64 {

    public:

        AtomicInt64();
        AtomicInt64(const int64_t val);

        AtomicInt64& operator=(const AtomicInt64 &S) = delete;
        AtomicInt64& operator=(const AtomicInt64 &S) volatile = delete;

        int64_t load() volatile;
        void store(const int64_t val) volatile;

    private:

        volatile AtomicInt32 _val[2];

};


template<typename T>
class Atomic {

    public:
    
        Atomic();
        Atomic(const T &val);

        Atomic& operator=(const Atomic &S) = delete;
        Atomic& operator=(const Atomic &S) volatile = delete;

        T load() volatile;
        void store(const T &val) volatile;

    private:

        volatile Lock lock;
        volatile T _val;

};



#include "Concurrent.hpp"


#endif
