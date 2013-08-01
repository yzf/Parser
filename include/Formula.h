/* 
 * File:   Formula.h
 * Author: yzf
 *
 * Created on July 28, 2013, 11:21 AM
 */

#ifndef FORMULA_H
#define	FORMULA_H

#include <cstdio>
#include "structs.h"
#include "Formulas.h"
#include <vector>

using namespace std;

class Formulas;
/*
 * 公式类
 */
class Formula {
private:
    _formula* m_pFormula;//公式
public:
    int m_nFormulaId;//公式id
    static int ms_nNewFormulaId;//下一个新公式的id
public:
    Formula();
    Formula(const Formula& _rhs);
    Formula(_formula* _fml, bool _bIsCopy);
    ~Formula();
    Formula& operator = (const Formula& _rhs);
    bool operator == (const Formula& _rhs) const;
    bool operator != (const Formula& _rhs) const;
    
    _formula* getFormula() const;
    void setFormula(_formula* _newFormula);
    bool isUniversal() const;
    void convertToPrenex();
    void fixUniversalQuantifier();
    void removeUniversalQuantifier();
    void output(FILE* _out) const;
    void doubleNegationIntensionPredicates();
    void doubleNegationPredicates(int* _pPredicateIds, int _nSize);
    void replaceTerms(const vector<int>& _originals, 
				const vector<int>& _replacements);
    Formulas* divideFormula() const;
};



#endif	/* FORMULA_H */

