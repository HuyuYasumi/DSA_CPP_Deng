#ifndef DSA_CPP_DENG_MYSTACK_H
#define DSA_CPP_DENG_MYSTACK_H

//#include <locale>
#include "myVector.h"
template <typename T> class Stack: public Vector<T> {
public:
    void push(T const& e) {
        this->insert(this->size(), e);
    }
    T pop() {
        return remove(this->size() - 1);
    }
    T& top() {
        return (*this)[this->size() - 1];
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

// 删除表达式exp[lo, hi]不含括号的最长前缀、后缀
void trim(const char exp[], int& lo, int& hi) {
    while((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')'))
        ++lo;
    while((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')'))
        --hi;
}

// 切分表达式exp[lo, hi], 使exp 匹配仅当它们匹配
int divide(const char exp[], int lo, int hi) {
    int mi = lo;
    int crc = 1;
    while((0 < crc) && (++mi < hi)) {
        if(exp[mi] == ')')
            --crc;
        if(exp[mi] == '(')
            ++crc;
    }
    return mi;
}

// 检查表达式exp[lo, hi] 是否括号匹配（递归版）
bool parenIter(const char exp[], int lo, int hi) {
    trim(exp, lo, hi);
    if(lo > hi)
        return true;
    if(exp[lo] != '(')
        return false;
    if(exp[hi] != ')')
        return false;
    int mi = divide(exp, lo, hi);
    if(mi > hi)
        return false;
    return parenIter(exp, lo + 1, mi -1) && parenIter(exp, mi + 1, hi);
}

// 表达式括号匹配检查（栈与迭代），可兼顾三种括号
bool parenStack(const char exp[], int lo, int hi) {
    Stack<char> S;
    for(int i = 0; exp[i]; ++i) {
        switch(exp[i]) {
            case '(':
            case '[':
            case '{':
                S.push(exp[i]);
                break;
            case ')':
                if((S.empty()) || ('(' != S.pop()))
                    return false;
                break;
            case ']':
                if((S.empty()) || ('[' != S.pop()))
                    return false;
                break;
            case '}':
                if((S.empty()) || '{' != S.pop())
                    return false;
                break;
            default:break;
        }
    }
    return S.empty();
}

/*
 #define N_OPTR 9
typedef enum {ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE} Operator;

const char pri[N_OPTR][N_OPTR] = {
        /*        +    -    *    /    ^    !    (    )    \0 */
        /* + */  '>', '>', '<', '<', '<', '<', '<', '>', '>',
        /* - */  '>', '>', '<', '<', '<', '<', '<', '>', '>',
        /* * */  '>', '>', '>', '>', '<', '<', '<', '>', '>',
        /* / */  '>', '>', '>', '>', '<', '<', '<', '>', '>',
        /* ^ */  '>', '>', '>', '>', '>', '<', '<', '>', '>',
        /* ! */  '>', '>', '>', '>', '>', '>', ' ', '>', '>',
        /* ( */  '<', '<', '<', '<', '<', '<', '<', '=', ' ',
        /* ) */  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        /* \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '=',/*
};
/*
float evaluate(char* S, char*& RPN) {
    Stack<float> opnd;
    Stack <char> optr;
    optr.push('\0');
    while(!optr.empty()) {
        if(isdigit(*S)) {
            readNumber(S, opnd);
            append(RPN, opnd.top());
        } else {
            switch(orderBetween(optr.top(), *S)) {
                case '<':
                    optr.push(*S);
                    S++;
                    break;
                case '=':
                    optr.pop();
                    ++S;
                    break;
                case '>':
                    char op = optr.pop();
                    append(RPN, op);
                    if('!' == op) {
                        float pOpnd = opnd.pop();
                        opnd.push(calcu(op, pOpnd));
                    } else {
                        float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
                        opnd.push(calcu(pOpnd1, op,pOpnd2);
                    }
                    break;
                default:exit(-1);
            } // switch
        }
    } // while
    return opnd.pop();
}
*/
#endif //DSA_CPP_DENG_MYSTACK_H
