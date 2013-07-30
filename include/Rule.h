/* 
 * File:   Rule.h
 * Author: yzf
 *
 * Created on July 30, 2013, 7:59 AM
 */

#ifndef RULE_H
#define	RULE_H

#include <cstdio>
#include "Formula.h"
#include "Formulas.h"

class Formula;
class Formulas;

class Rule {
private:
    Formulas* m_pHeadFormulas;
    Formulas* m_pBodyFormulas;
private:
    void divideHead(const _formula* _head);
    void divideBody(const _formula* _body);
    void convertFormulaToRule(const _formula* _fml);
public:
    Rule(const Formula& _fml);
    Rule(const Rule& _rhs);
    ~Rule();
    Rule& operator = (const Rule& _rhs);
    void output(FILE* _out);
    bool isUseless() const;
    void aspModify();
};

#endif	/* RULE_H */

