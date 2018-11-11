#ifndef DSA_CPP_DENG_MYQUEUE_H
#define DSA_CPP_DENG_MYQUEUE_H

#include "myList.h"

template <typename T> class Queue: public List<T> {
public:
    void enqueu(T const& e) {
        this->insertAsLast(e);
    }
    T dequeue() {
        return this->remove(this->first());
    }
    T& front() {
        return this->first()->data;
    }
};

#endif //DSA_CPP_DENG_MYQUEUE_H
