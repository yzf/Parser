#include "HengZhang.h"
#include "cstdlib"
#include "Utils.h"

HengZhang::HengZhang() {
}

HengZhang::HengZhang(const HengZhang& _rhs) {
}
HengZhang& HengZhang::operator = (const HengZhang& _rhs) {
    return *this;
}
HengZhang::~HengZhang() {
}

HengZhang& HengZhang::instance() {
    static HengZhang theSingleton;
    return theSingleton;
}
/**
 * 把量词保存到对应的vector
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::recordQuantifier(Formula _originalFml) {
    char strBuf[32];

    terms_X.clear();
    terms_Y.clear();
    terms_Z.clear();
    terms_MIN.clear();
    terms_MAX.clear();
    
    _formula* fml = _originalFml.getFormula();
    while(fml->formula_type == UNIV) {
        terms_X.push_back(fml->variable_id);
        fml = fml->subformula_l;
    }
    while (fml->formula_type == EXIS) {
        terms_Y.push_back(fml->variable_id);
        
        const char* domainName = Vocabulary::instance().getVariableDomain(fml->variable_id);
        
        sprintf(strBuf, "MIN_%s", domainName);
        int idMin = Vocabulary::instance().addSymbol(strBuf, VARIABLE);
        terms_MIN.push_back(idMin);

        sprintf(strBuf, "MAX_%s", domainName);
        int idMax = Vocabulary::instance().addSymbol(strBuf, VARIABLE);
        terms_MAX.push_back(idMax);

        sprintf(strBuf,"NV_%d", this->num_NV ++);
        int idNV = Vocabulary::instance().addSymbol(strBuf, VARIABLE);
        terms_Z.push_back(idNV);

        fml = fml->subformula_l;
    }
    Formula ret = Formula(fml, true);
    ret.m_nFormulaId = _originalFml.m_nFormulaId;
    return ret;
}
/**
 * 对每条公式进行章衡公式转换
 * @param _originalFmls
 * @return 
 */
Formulas HengZhang::create(Formulas _originalFmls) {
    Formulas tempFmls = _originalFmls;
    Formulas finalFmls;
    while (! tempFmls.isEmpty()) {
        Formula curFml = tempFmls.popFront();
        curFml.convertToPrenex();
        if (curFml.isUniversal()) {
            finalFmls.pushBack(curFml);
            continue;
        }
        curFml.fixUniversalQuantifier();
        Formulas hzFmls = transform(curFml);
        tempFmls.joinFormulas(hzFmls);
    }
    
    return finalFmls;
}

Formulas HengZhang::transform(Formula _originalFml) {
    Formula originalFml = recordQuantifier(_originalFml);

    if(terms_Y.size() == 0) {
        Formulas fmls;
        fmls.pushBack(_originalFml);
        return fmls;
    }
    
    char nameBuf[32];
    sprintf(nameBuf, "s_%d", num_s ++);
    symbol_s = Vocabulary::instance().addSymbol(nameBuf, 
                        PREDICATE, terms_X.size() + terms_Y.size());
    Vocabulary::instance().addIntensionPredicate(nameBuf);
 
    sprintf(nameBuf, "t_%d", num_t ++);
    symbol_t = Vocabulary::instance().addSymbol(nameBuf, 
                        PREDICATE, terms_X.size() + terms_Y.size());
    Vocabulary::instance().addIntensionPredicate(nameBuf);
 
    string succName = "succ";
    vector<string> domain_name;
    for (int i = 0; i < terms_Y.size(); ++ i) {
        succName += string("_") + 
                Vocabulary::instance().getVariableDomain(terms_Y[i]);
    }
    symbol_succ = Vocabulary::instance().addSymbol(succName.c_str(), 
                        PREDICATE, terms_Y.size() + terms_Z.size());
    
    Formulas fmls;
    fmls.pushBack(createFormula_1(originalFml));
    fmls.pushBack(createFormula_2(originalFml));
    fmls.pushBack(createFormula_3(originalFml));
    fmls.pushBack(createFormula_4_1(originalFml));
    fmls.pushBack(createFormula_4_2(originalFml));
    fmls.pushBack(createFormula_5_1(originalFml));
    fmls.pushBack(createFormula_5_2(originalFml));
    return fmls;
}
/**
 * 章衡量词消去公式一    ~~s(_X,_MIN)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_1(Formula _originalFml) {
    // Add S(_X, _MIN)
    _term* term_x_min   = Utils::combineTerms(terms_X, terms_MIN);
    _formula* s_x_min = Utils::compositeToAtom(symbol_s, term_x_min);
    _formula* s_x_y = Utils::copyFormula(s_x_min);
    Utils::replaceTerm(s_x_y->parameters, 
            terms_MIN.size(), terms_MIN, terms_Y);

    // create structure
    _formula* F  = Utils::compositeByConnective(NEGA, s_x_min, NULL);
    F  = Utils::compositeByConnective(NEGA, F,  NULL);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}
/**
 * 章衡量词消去公式二    ((succ(_Y,_Z) & s(_X,_Z)) | theta__(_X,_Y)) -> s(_X,_Y)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_2(Formula _originalFml) {
    //create atom formulas
    //1 (succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(terms_Y, terms_Z);
    _formula* succ_y_z = Utils::compositeToAtom(symbol_succ, term_y_z);
    //2 s(_X,_Z)
    _term* term_x_z = Utils::combineTerms(terms_X, terms_Z);
    _formula* s_x_z = Utils::compositeToAtom(symbol_s, term_x_z);
    //3 theta__(_X,_Y)
    _originalFml.doubleNegationIntensionPredicates();
    _formula* theta__ = Utils::copyFormula(_originalFml.getFormula());
    //4 s(_X,_Y)
    _term* term_x_y = Utils::combineTerms(terms_X, terms_Y);
    _formula* s_x_y   = Utils::compositeToAtom(symbol_s, term_x_y);

    //create structure
    _formula* ll = Utils::compositeByConnective(CONJ, succ_y_z, s_x_z);
    _formula* l = Utils::compositeByConnective(DISJ, ll, theta__);
    _formula* r = s_x_y;
    _formula* F = Utils::compositeByConnective(IMPL, l, r);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}
/**
 * 章衡量词消去公式三    t(_X,_MIN) | theta(_X,_MIN)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_3(Formula _originalFml) {
    //create left sub-formula t(_X,_MIN)
    _term* term_x_min = Utils::combineTerms(terms_X, terms_MIN);
    _formula* t_x_min   = Utils::compositeToAtom(symbol_t, term_x_min);
    //create right sub-formula theta(_X,_MIN)
    _originalFml.replaceTerms(terms_Y, terms_MIN);
    _formula* theta_x_min = Utils::copyFormula(_originalFml.getFormula());

    //create structure
    _formula* F   = Utils::compositeByConnective(DISJ, t_x_min, theta_x_min);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}
// (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y)))
_formula* HengZhang::generateFormulaLeft_4() {
    // 1 S(_X,_Y)
    _term* term_x_y = Utils::combineTerms(terms_X, terms_Y);
    _formula* s_x_y = Utils::compositeToAtom(symbol_s, term_x_y);
    // 2 ( ~S(_X,_Z)
    _term* term_x_z = Utils::combineTerms(terms_X, terms_Z);
    _formula* s_x_z = Utils::compositeToAtom(symbol_s, term_x_z);
    _formula* _s_x_z = Utils::compositeByConnective(NEGA, s_x_z, NULL);
    // 3 succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(terms_Y, terms_Z);
    _formula* succ_y_z = Utils::compositeToAtom(symbol_succ, term_y_z);
    // 4 max_domian(_Y)
    vector<Formula> max_ys;
    for (int i = 0; i < terms_Y.size(); ++ i) {
        string name = string("max_") + Vocabulary::instance().getVariableDomain(terms_Y[i]);
        _term* term_yi = (_term*)malloc(sizeof(_term));
        term_yi->term_type = VARI;
        term_yi->variable_id = terms_Y[i];
        int pre_id = Vocabulary::instance().addSymbol(name.c_str(), PREDICATE, 1);
        _formula* max_yi = Utils::compositeToAtom(pre_id, term_yi);
        max_ys.push_back(Formula(max_yi, false));
    }
    _formula* max_y = Utils::copyFormula(max_ys[0].getFormula());
    for (int i = 1; i < max_ys.size(); ++ i) {
        max_y = Utils::compositeByConnective(CONJ, max_y, Utils::copyFormula(max_ys[i].getFormula()));
    }
    _formula* lrl = Utils::compositeByConnective(CONJ, _s_x_z, succ_y_z);
    _formula* lr = Utils::compositeByConnective(DISJ, lrl, max_y);
    _formula* l = Utils::compositeByConnective(CONJ, s_x_y, lr);
    return l;
}
/**
 * 章衡量词消去公式四_1    (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y))) 
 *                              -> (t(_X,_MAX) -> theta(_X,_Y)) 
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_4_1(Formula original_fml) {
    // 5 T(_X,_MAX)
    _term* term_x_max = Utils::combineTerms(terms_X, terms_MAX);
    _formula* t_x_max  = Utils::compositeToAtom(symbol_t, term_x_max);
    // 6 theta(_X,_Y)
    _formula* theta_x_y  = Utils::copyFormula(original_fml.getFormula());
    
    _formula* left = generateFormulaLeft_4();
    _formula* right = Utils::compositeByConnective(IMPL, t_x_max, theta_x_y);
    
    _formula* F = Utils::compositeByConnective(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}
/**
 * 章衡量词消去公式四_2    (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y))) 
 *                              -> (theta(_X,_Y) -> t(_X,_MAX)) 
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_4_2(Formula _originalFml) {
    // 5 theta(_X,_Y)
    _formula* theta_x_y  = Utils::copyFormula(_originalFml.getFormula());
    // 6 T(_X,_MAX)
    _term* term_x_max = Utils::combineTerms(terms_X, terms_MAX);
    _formula* t_x_max  = Utils::compositeToAtom(symbol_t, term_x_max);
    
    _formula* left = generateFormulaLeft_4();
    _formula* right = Utils::compositeByConnective(IMPL, theta_x_y, t_x_max);
    
    _formula* F = Utils::compositeByConnective(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}
/**
 * 章衡量词消去公式五 1    succ(_Y,_Z) -> (t(_X,_Y) -> (theta(_X,_Z) | t(_X,_Z)))
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_5_1(Formula _originalFml) {
    //1 _succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(terms_Y, terms_Z);
    _formula* succ_y_z  = Utils::compositeToAtom(symbol_succ, term_y_z);

    //2 T(_X,_Y)
    _term* term_x_y = Utils::combineTerms(terms_X, terms_Y);
    _formula* t_x_y  = Utils::compositeToAtom(symbol_t, term_x_y);

    //3 theta(_X,_Z)
    _originalFml.replaceTerms(terms_Y, terms_Z);
    _formula* theta_x_z = Utils::copyFormula(_originalFml.getFormula());

    //4 T(_X,_Z)
    _term* term_x_z = Utils::combineTerms(terms_X, terms_Z);
    _formula* t_x_z = Utils::compositeToAtom(symbol_t, term_x_z);

    //create structure
    _formula* left = succ_y_z;
    _formula* rr = Utils::compositeByConnective(DISJ, theta_x_z, t_x_z);
    _formula* right = Utils::compositeByConnective(IMPL, t_x_y, rr);
    _formula* F = Utils::compositeByConnective(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}
/**
 * 章衡量词消去公式五 2   succ(_Y,_Z) -> ((theta(_X,_Z) | t(_X,_Z)) -> t(_X,_Y)) 
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_5_2(Formula _originalFml) {
    //1 _succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(terms_Y, terms_Z);
    _formula* succ_y_z  = Utils::compositeToAtom(symbol_succ, term_y_z);

    //2 T(_X,_Y)
    _term* term_x_y = Utils::combineTerms(terms_X, terms_Y);
    _formula* t_x_y  = Utils::compositeToAtom(symbol_t, term_x_y);

    //3 theta(_X,_Z)
    _originalFml.replaceTerms(terms_Y, terms_Z);
    _formula* theta_x_z = Utils::copyFormula(_originalFml.getFormula());

    //4 T(_X,_Z)
    _term* term_x_z = Utils::combineTerms(terms_X, terms_Z);
    _formula* t_x_z = Utils::compositeToAtom(symbol_t, term_x_z);

    //create structure
    _formula* left = succ_y_z;
    _formula* rl = Utils::compositeByConnective(DISJ, theta_x_z, t_x_z);
    _formula* right = Utils::compositeByConnective(IMPL, rl, t_x_y);
    _formula* F = Utils::compositeByConnective(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}