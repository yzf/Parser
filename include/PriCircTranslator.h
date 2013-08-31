/* 
 * File:   PriCircTranslator.h
 * Author: yzf
 *
 * Created on August 25, 2013, 1:26 PM
 */

#ifndef PRICIRCTRANSLATOR_H
#define	PRICIRCTRANSLATOR_H

#include "Formula.h"
#include "Formulas.h"
#include <vector>

using namespace std;

class PriCircTranslator {
protected:
    vector<int> m_vSysbolR;
    vector<int> m_vNewPredicates;
private:
    Formula createFormula_1(const Formula& _originalFml);
    Formulas createFormula_2(const Formula& _originalFml);
    Formulas createFormula_3();
    Formulas createFormula_4();
protected:
    Formulas* preProcessing(const Formula& _originalFml);
    void postProcessing(Formulas* _pFmls);
    Formulas transform(const Formula& _originalFml);
public:
    PriCircTranslator();
    ~PriCircTranslator();
    Formulas* convert(const Formula& _originalFml);
};

#endif	/* PRICIRCTRANSLATOR_H */

