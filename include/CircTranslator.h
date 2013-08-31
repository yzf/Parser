/* 
 * File:   CircTranslator.h
 * Author: yzf
 *
 * Created on August 14, 2013, 9:29 PM
 */

#ifndef CIRCTRANSLATOR_H
#define	CIRCTRANSLATOR_H

#include "structs.h"
#include "Formula.h"
#include "Formulas.h"
#include <string>

class Formula;
class Formulas;

class CircTranslator {
protected:
    int m_nSymbolR;
    vector<int> m_vNewPredicates;
private:
    _formula* createP_P();
    Formula createFormula_1(const Formula& _originalFml);
    Formula createFormula_2(const Formula& _originalFml);
    Formulas createFormula_3();
    Formulas createFormula_4();
protected:
    Formulas* preProcessing(const Formula& _originalFml);
    void postProcessing(Formulas* _pFmls);
    Formulas transform(const Formula& _originalFml);
    Formulas createREqualP_P();
    Formulas createRImplQVary();
public:
    CircTranslator();
    virtual ~CircTranslator();
    Formulas* convert(const Formula& _originalFml);
};


#endif	/* CIRCTRANSLATOR_H */

