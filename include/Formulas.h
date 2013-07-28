/* 
 * File:   Formulas.h
 * Author: yzf
 *
 * Created on July 28, 2013, 2:19 PM
 */

#ifndef FORMULAS_H
#define	FORMULAS_H

#include "Formula.h"
#include <deque>

using namespace std;

class Formula;

class Formulas {
private:
    deque<Formula> m_dequeFormulas;
    
public:
    Formulas();
    Formulas(deque<Formula> _formulas);
    
    unsigned int size() const;
    bool isEmpty() const;
    void pushBack(const Formula& _tail);
    void pushFront(const Formula& _front);
    Formula popBack();
    Formula popFront();
    Formula front();
    Formula back();
    void joinFormulas(const Formulas& _tail);
    bool operator == (const Formulas& _rhs) const;
    bool operator != (const Formulas& _rhs) const;
    void output(FILE* _out) const;
};



#endif	/* FORMULAS_H */

