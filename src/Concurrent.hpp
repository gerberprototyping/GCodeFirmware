#include "Concurrent.h"


/***********************************************************
 * Atomic
 **********************************************************/

template<typename T>
Atomic<T>::Atomic() {
    // Intentionally left blank
}

template<typename T>
Atomic<T>::Atomic(const T &val) {
    _val = val;
}

template<typename T>
T Atomic<T>::load() volatile {
    lock.lock();
    T x = _val;
    lock.unlock();
    return x;
}

template<typename T>
void Atomic<T>::store(const T &val) volatile {
    lock.lock();
    _val = val;
    lock.unlock();
}