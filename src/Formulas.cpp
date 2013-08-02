#include "Formulas.h"


Formulas::Formulas() {
}

Formulas::Formulas(const deque<Formula>& _formulas) {
    m_dequeFormulas = _formulas;
}

Formulas::Formulas(const Formulas& _rhs) {
    m_dequeFormulas = _rhs.m_dequeFormulas;
}

Formulas::~Formulas() {
    m_dequeFormulas.clear();
}

unsigned int Formulas::size() const {
    return m_dequeFormulas.size();
}

bool Formulas::isEmpty() const {
    return m_dequeFormulas.empty();
}

void Formulas::pushBack(const Formula& _tail) {
    m_dequeFormulas.push_back(_tail);
}
void Formulas::pushFront(const Formula& _front) {
    m_dequeFormulas.push_front(_front);
}
Formula Formulas::popBack() {
    Formula ret = m_dequeFormulas.back();
    m_dequeFormulas.pop_back();
    return ret;
}
Formula Formulas::popFront() {
    Formula ret = m_dequeFormulas.front();
    m_dequeFormulas.pop_front();
    return ret;
}
Formula Formulas::front() {
    return m_dequeFormulas.front();
}
Formula Formulas::back() {
    return m_dequeFormulas.back();
}
/**
 * 连接公式数组 如 {a,b,c}{d,e} -> {a,b,c,d,e}
 * 注意：{d,e} -> {}
 * @param _tail
 */
void Formulas::joinFormulas(const Formulas& _tail) {
    for (FORMULAS_CONST_ITERATOR it = _tail.begin(); 
            it != _tail.end(); ++ it) {
        pushBack(*it);
    }
}
/**
 * 公式组相等的条件是所有公式对应相等，包括位置
 * @param _rhs
 * @return 
 */
bool Formulas::operator == (const Formulas& _rhs) const {
    FORMULAS_CONST_ITERATOR it1;
    FORMULAS_CONST_ITERATOR it2;
    if (m_dequeFormulas.size() != _rhs.m_dequeFormulas.size()) {
        return false;
    }
    for (it1 = m_dequeFormulas.begin(), it2 = _rhs.m_dequeFormulas.begin(); 
            it1 != m_dequeFormulas.end() && it2 != _rhs.m_dequeFormulas.end(); 
            ++ it1, ++ it2) {
        if (*it1 != *it2) {
            return false;
        }
    }
    return true;
}

bool Formulas::operator != (const Formulas& _rhs) const {
    return ! (*this == _rhs);
}
Formula& Formulas::operator [] (int _id) {
    return m_dequeFormulas.at(_id);
}
/**
 * 输出公式组
 * @param _out
 */
void Formulas::output(FILE* _out) const {
    for (FORMULAS_CONST_ITERATOR it = m_dequeFormulas.begin();
            it != m_dequeFormulas.end(); ++ it) {
        it->output(_out);
    }
}
/**
 * 获取指向第一个元素的迭代器
 * @return FORMULAS_ITERATOR
 */
FORMULAS_ITERATOR Formulas::begin() {
    return m_dequeFormulas.begin();
}
/**
 * 获取指向结束处的迭代器
 * @return FORMULAS_ITERATOR
 */
FORMULAS_ITERATOR Formulas::end() {
    return m_dequeFormulas.end();
}
FORMULAS_CONST_ITERATOR Formulas::begin() const {
    return m_dequeFormulas.begin();
}
FORMULAS_CONST_ITERATOR Formulas::end() const {
    return m_dequeFormulas.end();
}
/**
 * 删除迭代器_it指向的元素，返回下一个有效元素
 * @param _it
 * @return FORMULAS_ITERATOR
 */
FORMULAS_ITERATOR Formulas::erase(FORMULAS_ITERATOR _it) {
    return m_dequeFormulas.erase(_it);
}