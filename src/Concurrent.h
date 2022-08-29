#ifndef __CONCURRENT_H
#define __CONCURRENT_H

#include <cstdint>
#include <assert.h>

#include "stm32l476xx.h"


class Lock {

    public:

        Lock();

        Lock& operator=(const Lock &val) volatile = delete;

        bool status() const volatile;

        void lock() volatile;
        void unlock() volatile;
        bool tryLock() volatile;
    
    private:

        volatile int8_t _lock;

};


class AtomicInt32 {

    public:

        AtomicInt32();
        AtomicInt32(const int32_t &val);

        AtomicInt32& operator=(const AtomicInt32 &val) volatile = delete;

        int32_t load() const volatile;
        void store(const int32_t &val) volatile;
        bool tryStore(const int32_t &val) volatile;

        bool fetch_add(const int32_t &val) volatile;
        bool fetch_sub(const int32_t &val) volatile;

    private:

        volatile int32_t _val;

};


template<typename T>
class Atomic {

    public:

        Atomic();
        Atomic(const T &val);

        Atomic<T>& operator=(const Atomic<T> &val) volatile = delete;

        T load() volatile;
        void store(const T &val) volatile;
        
        bool tryLoad(const T* val) volatile;
        bool tryStore(const T &val) volatile;

        bool fetch_add(const T &val) volatile;
        bool fetch_sub(const T &val) volatile;

    private:

        volatile Lock lock;
        volatile uint32_t _val[sizeof(T)/4];

};



#include "Concurrent.hpp"


#endif // __CONCURRENT_H