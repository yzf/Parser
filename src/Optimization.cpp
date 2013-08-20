#include "Optimization.h"
#include "Utils.h"
#include <cstdlib>
#include <assert.h>

Optimization::Optimization() : m_nSymbolW(0) {
}

Optimization::~Optimization() {
}
Formulas Optimization::transform(const Formula& _originalFml) {
    Formula fml = preProcessing(_originalFml);
    Formulas fmls;
    fmls.pushBack(createFormula_1());
    fmls.pushBack(createFormula_2(fml));
    fmls.pushBack(createFormula_3(fml));
    fmls.pushBack(createFormula_4(fml));
    fmls.pushBack(createFormula_5_1(fml));
    fmls.pushBack(createFormula_5_2(fml));
    fmls.pushBack(createFormula_6_1(fml));
    fmls.pushBack(createFormula_6_2(fml));
    m_vNewPredicates.push_back(m_nSymbolS);
    m_vNewPredicates.push_back(m_nSymbolW);
    m_vNewPredicates.push_back(m_nSymbolT);
    return fmls;
}
Formula Optimization::createFormula_1() {
    return HengZhang::create__S();
}
Formula Optimization::createFormula_2(const Formula& _originalFml) {
    return HengZhang::createSuccAndSOrTheta__ImplS(_originalFml);
}
Formula Optimization::createFormula_3(const Formula& _originalFml) {
    // succ(Y_,Z_)
    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z = Utils::compositeToAtom(m_nSymbolS, t_y_z);
    // w(X_,Z_)
    _term* t_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* w_x_z = Utils::compositeToAtom(m_nSymbolW, t_x_z);
    // thetaT__(X_,Y_)
    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
    _formula* thetaT__ = Utils::thetaT__Replace(theta);
    // w(X_,Y_)
    _term* t_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* w_x_y = Utils::compositeToAtom(m_nSymbolW, t_x_y);
    
    _formula* f = Utils::compositeByConnective(CONJ, succ_y_z, w_x_z);
    f = Utils::compositeByConnective(DISJ, f, thetaT__);
    f = Utils::compositeByConnective(IMPL, f, w_x_y);
    
    Formula ret = Formula(f, false);
    return ret;
}
/**
 * 生成 t(X_,MIN_) | _theta(X_,MIN_)
 * @param _originalFml
 * @return 
 */
Formula Optimization::createFormula_4(const Formula& _originalFml) {
    // t(X_,MIN_)
    _term* term_x_min = Utils::combineTerms(m_vTermsX, m_vTermsMIN);
    _formula* t_x_min = Utils::compositeToAtom(m_nSymbolT, term_x_min);
    // _theta(X_,MIN_)
    Formula copyOri = _originalFml;
    copyOri.replaceTerms(m_vTermsY, m_vTermsMIN);
    _formula* theta = Utils::copyFormula(copyOri.getFormula());
    _formula* _theta = Utils::_thetaReplace(m_nSymbolR, theta);
    
    _formula* f = Utils::compositeByConnective(DISJ, t_x_min, _theta);
    
    Formula ret = Formula(f, false);
    return ret;
}
/**
 * 生成 succ(Y_,Z_)  ->  (t(X_,Y_) -> _theta(X_,Z_) | t(X_,Z_))
 * @param _originalFml
 * @return 
 */
Formula Optimization::createFormula_5_1(const Formula& _originalFml) {
    // succ(Y_,Z_)
    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z = Utils::compositeToAtom(m_nSymbolSucc, t_y_z);
    // t_x_y
    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* t_x_y = Utils::compositeToAtom(m_nSymbolT, term_x_y);
    // _theta(X_,Z_)
    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
    _formula* _theta = Utils::_thetaReplace(m_nSymbolR, theta);
    // t(X_,Z_)
    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* t_x_z = Utils::compositeToAtom(m_nSymbolT, term_x_z);
    
    _formula* f = Utils::compositeByConnective(DISJ, _theta, t_x_z);
    f = Utils::compositeByConnective(IMPL, t_x_y, f);
    f = Utils::compositeByConnective(IMPL, succ_y_z, f);
    
    Formula ret = Formula(f, false);
    return ret;
}
/**
 * 生成 succ(Y_,Z_)  ->  (_theta(X_,Z_) | t(X_,Z_)  ->  t(X_,Y_))
 * @param _originalFml
 * @return 
 */
Formula Optimization::createFormula_5_2(const Formula& _originalFml) {
    // succ(Y_,Z_)
    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z = Utils::compositeToAtom(m_nSymbolSucc, t_y_z);
    // t_x_y
    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* t_x_y = Utils::compositeToAtom(m_nSymbolT, term_x_y);
    // _theta(X_,Z_)
    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
    _formula* _theta = Utils::_thetaReplace(m_nSymbolR, theta);
    // t(X_,Z_)
    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* t_x_z = Utils::compositeToAtom(m_nSymbolT, term_x_z);
    
    _formula* f = Utils::compositeByConnective(DISJ, _theta, t_x_z);
    f = Utils::compositeByConnective(IMPL, f, t_x_y);
    f = Utils::compositeByConnective(IMPL, succ_y_z, f);
    
    Formula ret = Formula(f, false);
    return ret;
}
/**
 * 生成 (w(_X,_Y) & ((~w(_X,_Z) & succ(_Y,_Z)) | _max(Y)))
 * @return 
 */
_formula* Optimization::create_6_left() {
    // w(X_,Y_)
    _term* t_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* w_x_y = Utils::compositeToAtom(m_nSymbolW, t_x_y);
    // ~w(X_,Z_)
    _term* t_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* w_x_z = Utils::compositeToAtom(m_nSymbolW, t_x_z);
    _formula* _w_x_z = Utils::compositeByConnective(NEGA, w_x_z);
    // succ(Y_,Z_)
    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z = Utils::compositeToAtom(m_nSymbolSucc, t_y_z);
    
    // max_domian(_Y)
    vector<Formula> max_ys;
    for (unsigned int i = 0; i < m_vTermsY.size(); ++ i) {
        string name = string("max_") + Vocabulary::instance().getVariableDomain(m_vTermsY[i]);
        _term* term_yi = (_term*)malloc(sizeof(_term));
        term_yi->term_type = VARI;
        term_yi->variable_id = m_vTermsY[i];
        int pre_id = Vocabulary::instance().addSymbol(name.c_str(), PREDICATE, 1);
        _formula* max_yi = Utils::compositeToAtom(pre_id, term_yi);
        Formula f = Formula(max_yi, false);
        max_ys.push_back(f);
        Vocabulary::instance().addAtom(f);
    }
    _formula* max_y = Utils::copyFormula(max_ys[0].getFormula());
    for (unsigned int i = 1; i < max_ys.size(); ++ i) {
        max_y = Utils::compositeByConnective(CONJ, max_y, Utils::copyFormula(max_ys[i].getFormula()));
    }
    
    _formula* f = Utils::compositeByConnective(CONJ, _w_x_z, succ_y_z);
    if (NULL != max_y) {
        f = Utils::compositeByConnective(DISJ, f, max_y);
    }
    f = Utils::compositeByConnective(CONJ, w_x_y, f);
    
    return f;
}
/**
 * (w(_X,_Y) & ((~w(_X,_Z) & succ(_Y,_Z)) | _max(Y))) -> 
 *              ( (t(_X,_MAX) -> _theta(_X,_Y)))
 * @param _originalFml
 * @return 
 */
Formula Optimization::createFormula_6_1(const Formula& _originalFml) {
    _formula* left = create_6_left();
    // t(X_,MAX_)
    _term* term_x_max = Utils::combineTerms(m_vTermsX, m_vTermsMAX);
    _formula* t_x_max = Utils::compositeToAtom(m_nSymbolT, term_x_max);
    // _theta(X_,Y_)
    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
    _formula* _theta = Utils::_thetaReplace(m_nSymbolR, theta);
    
    _formula* f = Utils::compositeByConnective(IMPL, t_x_max, _theta);
    f = Utils::compositeByConnective(IMPL, left, f);
    
    Formula ret = Formula(f, false);
    return ret;
}
/**
 * (w(_X,_Y) & ((~w(_X,_Z) & succ(_Y,_Z)) | _max(Y))) 
 *                              -> (_theta(_X,_Y) -> t(_X,_MAX))
 * @param _originalFml
 * @return 
 */
Formula Optimization::createFormula_6_2(const Formula& _originalFml) {
    _formula* left = create_6_left();
    // t(X_,MAX_)
    _term* term_x_max = Utils::combineTerms(m_vTermsX, m_vTermsMAX);
    _formula* t_x_max = Utils::compositeToAtom(m_nSymbolT, term_x_max);
    // _theta(X_,Y_)
    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
    _formula* _theta = Utils::_thetaReplace(m_nSymbolR, theta);
    
    _formula* f = Utils::compositeByConnective(IMPL, _theta, t_x_max);
    f = Utils::compositeByConnective(IMPL, left, f);
    
    Formula ret = Formula(f, false);
    return ret;
}
Formula Optimization::preProcessing(const Formula& _originalFml) {
    Formula fml = HengZhang::preProcessing(_originalFml);
    m_nSymbolW = Vocabulary::instance().generatePredicateW(m_vTermsX, m_vTermsY);
    return fml;
}
Formulas* Optimization::convert(const Formula& _originalFml) {
    Formulas* nnfFmls = CircTranslator::preProcessing(_originalFml);
    assert(nnfFmls);
    
    Formulas* pFmls = new Formulas();
    for (FORMULAS_CONST_ITERATOR it = nnfFmls->begin(); 
            it != nnfFmls->end(); ++ it) {
        if (it->isUniversal()) {
            pFmls->joinBack(CircTranslator::transform(*it));
        }
        else {
            pFmls->joinBack(transform(*it));
        }
    }
    delete nnfFmls;
    
    CircTranslator::postProcessing(pFmls);
    
    printf("op:\n");
    pFmls->output(stdout);
    
    return pFmls;
}