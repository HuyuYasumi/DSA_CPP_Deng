#ifndef DSA_CPP_DENG_MYQUEUE_H
#define DSA_CPP_DENG_MYQUEUE_H

#include "myList.h"

template <typename T> class Queue: public List<T> {
public:
    void enqueue(T const &e) {
        this->insertAsLast(e);
    }
    T dequeue() {
        return this->remove(this->first());
    }
    T& front() {
        return this->first()->data;
    }
};

/* 模拟银行业务 */
struct Customer {
    int window;
    unsigned int time;
};

int bestWindow(Queue<Customer> windows[], int nWin) {
    int minSize = windows[0].size(), optiWin = 0;
    for(int i = 1; i < nWin; ++i) {
        if(minSize > windows[i].size()) {
            minSize = windows[i].size();
            optiWin = i;
        }
    }
    return optiWin;
}

void simulate(int nWin, int servTime) {    //按指定窗口数、服务总时间模拟银行业务
    Queue<Customer>* windows = new Queue<Customer>[nWin];
    for(int now = 0; now < servTime; ++now) {
        if(rand() % (1 + nWin)) {
            Customer* c = new Customer();
            c->time = (unsigned int)1 + rand() % 98;
            c->window = bestWindow(windows, nWin);
            windows[c->window].enqueue(*c);
        }
        for(int i = 0; i < nWin; i++) {
            if(!windows[i].empty()) {
                if(--windows[i].front().time <= 0)
                    windows[i].dequeue();
            }
        }
    }
    delete [] windows;
}

/* 结束模拟银行业务 */

#endif //DSA_CPP_DENG_MYQUEUE_H
