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
private:
    int m_nRid;
private:
    Formula createFormula_1(const Formula& _originalFml);
    Formula createFormula_2(const Formula& _originalFml);
    _formula* createP_P();
    Formula createFormula_3_1();
    Formula createFormula_3_2();
    Formulas createFormula_4();
    Formulas transform(const Formula& _originalFml);
public:
    CircTranslator();
    virtual ~CircTranslator();
    virtual Formulas* convert(const Formula& _originalFml);
};


#endif	/* CIRCTRANSLATOR_H */

