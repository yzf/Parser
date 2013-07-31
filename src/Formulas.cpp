#include "Formulas.h"


Formulas::Formulas() {
}

Formulas::Formulas(const deque<Formula>& _formulas) {
    this->m_dequeFormulas = _formulas;
}

Formulas::Formulas(const Formulas& _rhs) {
    this->m_dequeFormulas = _rhs.m_dequeFormulas;
}

Formulas::~Formulas() {
    this->m_dequeFormulas.clear();
}

unsigned int Formulas::size() const {
    return this->m_dequeFormulas.size();
}

bool Formulas::isEmpty() const {
    return this->m_dequeFormulas.empty();
}

void Formulas::pushBack(const Formula& _tail) {
    this->m_dequeFormulas.push_back(_tail);
}
void Formulas::pushFront(const Formula& _front) {
    this->m_dequeFormulas.push_front(_front);
}
Formula Formulas::popBack() {
    Formula ret = this->m_dequeFormulas.back();
    this->m_dequeFormulas.pop_back();
    return ret;
}
Formula Formulas::popFront() {
    Formula ret = this->m_dequeFormulas.front();
    this->m_dequeFormulas.pop_front();
    return ret;
}
Formula Formulas::front() {
    return this->m_dequeFormulas.front();
}
Formula Formulas::back() {
    return this->m_dequeFormulas.back();
}
/**
 * 连接公式数组 如 {a,b,c}{d,e} -> {a,b,c,d,e}
 * @param _tail
 */
void Formulas::joinFormulas(const Formulas& _tail) {
    Formulas tmpFmls = _tail;
    while (! tmpFmls.isEmpty()) {
        pushBack(tmpFmls.popFront());
    }
}

bool Formulas::operator == (const Formulas& _rhs) const {
    deque<Formula>::const_iterator it1;
    deque<Formula>::const_iterator it2;
    if (this->m_dequeFormulas.size() != _rhs.m_dequeFormulas.size()) {
        return false;
    }
    for (it1 = this->m_dequeFormulas.begin(), it2 = _rhs.m_dequeFormulas.begin(); 
            it1 != this->m_dequeFormulas.end() && it2 != _rhs.m_dequeFormulas.end(); 
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

void Formulas::output(FILE* _out) const {
    for (deque<Formula>::const_iterator it = this->m_dequeFormulas.begin();
            it != this->m_dequeFormulas.end(); ++ it) {
        it->output(_out);
    }
}

FORMULAS_ITERATOR Formulas::begin() {
    return this->m_dequeFormulas.begin();
}
FORMULAS_ITERATOR Formulas::end() {
    return this->m_dequeFormulas.end();
}

FORMULAS_ITERATOR Formulas::erase(FORMULAS_ITERATOR _it) {
    return this->m_dequeFormulas.erase(_it);
}