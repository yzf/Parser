#include "Cabalar.h"
#include "Utils.h"
#include "CabalarUtils.h"

Cabalar::Cabalar() {
    
}

Cabalar::~Cabalar() {
    
}

Cabalar& Cabalar::instance() {
    static Cabalar theInstance;
    return theInstance;
}
/**
 * 把所有公式转化成否定标准式
 * @param _originalFmls
 * @return 
 */
Formulas Cabalar::convertToNegativeNormalForms(Formulas _originalFmls) {
    Formulas retFormulas;
    while(! _originalFmls.isEmpty()) {
        Formula fml = _originalFmls.popFront();
        Formula newFml = Formula(
                        Utils::convertToNegativeNormalForm(
                                Utils::copyFormula(fml.getFormula())), false);
        newFml.m_nFormulaId = fml.m_nFormulaId;
        retFormulas.pushBack(newFml);
    }
    
    return retFormulas;
}
/**
 * Cabalar转化
 * @param _originalFmls
 * @return 
 */
Formulas* Cabalar::convert(const Formulas& _originalFmls) {
    Formulas* pFinalFormulas = new Formulas();
    Formulas tmpFormulas = convertToNegativeNormalForms(_originalFmls);
    while (! tmpFormulas.isEmpty()) {
        Formula curFml = tmpFormulas.popFront();
        curFml.removeUniversalQuantifier();
        deque<Formula> tmpDeq;
        tmpDeq.push_back(curFml);
        
        Formulas beforeTrans(tmpDeq);
        Formulas afterTrans = CabalarUtils::transform(Utils::copyFormula(curFml.getFormula()));
        if (beforeTrans == afterTrans) {
            pFinalFormulas->pushBack(curFml);
        }
        else {
            tmpFormulas.joinFormulas(afterTrans);
        }
    }
    return pFinalFormulas;
}
