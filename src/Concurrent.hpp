#include "Concurrent.h"


/***********************************************************
 * Atomic
 **********************************************************/

template<typename T>
Atomic<T>::Atomic() {
    static_assert(!(sizeof(T)%4), "Atomic<T> template requires template type to be word aligned");
}

template<typename T>
Atomic<T>::Atomic(const T &val) {
    static_assert(!(sizeof(T)%4), "Atomic<T> template requires template type to be word aligned");
    store(val);
}


template<typename T>
T Atomic<T>::load()  volatile {
    T x;
    while(!tryLoad(&x));
    return x;
}

template<typename T>
void Atomic<T>::store(const T &val) volatile {
    while(!tryStore(val));
}

template<typename T>
bool Atomic<T>::tryLoad(const T* val) volatile {
    if (!lock.tryLock()) {
        return false;
    }
    uint32_t* ptr = (uint32_t*) val;
    for (uint32_t i=0; i<sizeof(T)/4; i++) {
        ptr[i] = this->_val[i];
    }
    lock.unlock();
    return true;
}

template<typename T>
bool Atomic<T>::tryStore(const T &val) volatile {
    if (!lock.tryLock()) {
        return false;
    }
    uint32_t* ptr = (uint32_t*) &val;
    for (uint32_t i=0; i<sizeof(T)/4; i++) {
        this->_val[i] = ptr[i];
    }
    lock.unlock();
    return true;
}

template<typename T>
bool Atomic<T>::fetch_add(const T &val) volatile {
    if (!lock.tryLock()) {
        return false;
    }
    T tmp;
    uint32_t* ptr = (uint32_t*) &tmp;
    for (uint32_t i=0; i<sizeof(T)/4; i++) {
        ptr[i] = this->_val[i];
    }
    tmp = tmp + val;
    for (uint32_t i=0; i<sizeof(T)/4; i++) {
        this->_val[i] = ptr[i];
    }
    lock.unlock();
    return true;
}

template<typename T>
bool Atomic<T>::fetch_sub(const T &val) volatile {
    if (!lock.tryLock()) {
        return false;
    }
    T tmp;
    uint32_t* ptr = &tmp;
    for (uint32_t i=0; i<sizeof(T)/4; i++) {
        ptr[i] = this->_val[i];
    }
    tmp = tmp + val;
    for (uint32_t i=0; i<sizeof(T)/4; i++) {
        this->_val[i] = ptr[i];
    }
    lock.unlock();
    return true;
}
