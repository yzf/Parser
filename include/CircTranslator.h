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
public:
    static const string ms_sRName;
    static const string ms_sVaryPostfix;
private:
    Formula createFormula_1(const Formula& _originalFml);
    Formula createFormula_2(const Formula& _originalFml);
    _formula* createFormula_3_right();
    Formula createFormula_3_1();
    Formula createFormula_3_2();
    Formula createFormula_4(const Formula& _originalFml);
public:
    CircTranslator();
    virtual ~CircTranslator();
    virtual Formulas* convert(const Formula& _originalFml);
};


#endif	/* CIRCTRANSLATOR_H */

