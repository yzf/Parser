#include "Cabalar.h"
#include "Utils.h"
#include "CabalarUtils.h"
#include "NNFUtils.h"

Cabalar::Cabalar() {
    
}

Cabalar::~Cabalar() {
    
}

Cabalar& Cabalar::instance() {
    static Cabalar theInstance;
    return theInstance;
}
/**
 * Cabalar转化
 * @param _originalFmls
 * @return Formulas*　需要手动delete
 */
Formulas* Cabalar::convert(const Formulas& _originalFmls) {
    Formulas* pFinalFormulas = new Formulas();
    Formulas* pTmpFormulas = NNFUtils::convertToNegativeNormalForms(_originalFmls);
    while (! pTmpFormulas->isEmpty()) {
        Formula curFml = pTmpFormulas->popFront();
        curFml.removeUniversalQuantifier();
        
        Formulas* pBforeTrans = new Formulas();
        pBforeTrans->pushBack(curFml);
        Formulas* pAfterTrans = CabalarUtils::transform(Utils::copyFormula(curFml.getFormula()));
        if (*pBforeTrans == *pAfterTrans) {
            pFinalFormulas->pushBack(curFml);
        }
        else {
            pTmpFormulas->joinFront(*pAfterTrans);
        }
        delete pBforeTrans;
        delete pAfterTrans;
    }
    delete pTmpFormulas;
    return pFinalFormulas;
}
