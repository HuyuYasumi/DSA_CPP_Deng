#ifndef DSA_CPP_DENG_MYSTACK_H
#define DSA_CPP_DENG_MYSTACK_H

#include "myVector.h"
template <typename T> class Stack: public Vector<T> {
public:
    void push(T const& e) {
        insert(size(), e);
    }
    T pop() {
        return remove(size() - 1);
    }
    T& top() {
        return (*this)[size() - 1];
    }
};

// 十进制数n 到base 进制的转换（递归版）
void convertRecursive(Stack<char> &S, __int64 n, int base) {
    static char digit[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
            'B', 'C', 'D', 'E', 'F'};
    if(0 < n) {
        convertRecursive(S, n / base, base);
        S.push(digit[n % base]);
    }
}

// 十进制数n 到base 进制的转换（迭代版）
void convertIter(Stack<char>& S, __int64 n, int base) {
    static char digit[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
           'B', 'C', 'D', 'E', 'F'};
    while(0 < n) {
        int remainder = (int)(n % base);
        S.push(digit[remainder]);
        n /= base;
    }
}
#endif //DSA_CPP_DENG_MYSTACK_H
