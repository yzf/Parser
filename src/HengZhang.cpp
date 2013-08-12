#include "HengZhang.h"
#include "cstdlib"
#include "Utils.h"

vector< vector<string> > HengZhang::ms_vDomainNames;

HengZhang::HengZhang() {
}
HengZhang::~HengZhang() {
}

HengZhang& HengZhang::instance() {
    static HengZhang theSingleton;
    return theSingleton;
}
/**
 * 对每条公式进行章衡公式转换
 * @param _originalFmls Formulas 需要进行转换的公式
 * @return 返回Formulas*，需要手动销毁
 */
Formulas* HengZhang::convert(const Formulas& _originalFmls) {
    Formulas tempFmls = _originalFmls;
    Formulas* pFinalFmls = new Formulas();
    while (! tempFmls.isEmpty()) {
        Formula curFml = tempFmls.popFront();
        curFml.fixUniversalQuantifier();
        curFml.convertToPNF();
        if (curFml.isUniversal()) {
            pFinalFmls->pushBack(curFml);
            continue;
        }
        Formulas hzFmls = transform(curFml);
        tempFmls.joinFront(hzFmls);
    }
    
    return pFinalFmls;
}
/**
 * 把量词保存到对应的vector
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::recordQuantifier(const Formula& _originalFml) {
    char sBuf[32];

    m_vTermsX.clear();
    m_vTermsY.clear();
    m_vTermsZ.clear();
    m_vTermsMIN.clear();
    m_vTermsMAX.clear();
    
    _formula* fml = _originalFml.getFormula();
    while(fml->formula_type == UNIV) {
        m_vTermsX.push_back(fml->variable_id);
        fml = fml->subformula_l;
    }
    while (fml->formula_type == EXIS) {
        m_vTermsY.push_back(fml->variable_id);
        
        const char* sDomainName = Vocabulary::instance().getVariableDomain(fml->variable_id);
        
        sprintf(sBuf, "MIN_%s", sDomainName);
        int nIdMin = Vocabulary::instance().addSymbol(sBuf, VARIABLE);
        m_vTermsMIN.push_back(nIdMin);

        sprintf(sBuf, "MAX_%s", sDomainName);
        int nIdMax = Vocabulary::instance().addSymbol(sBuf, VARIABLE);
        m_vTermsMAX.push_back(nIdMax);

        sprintf(sBuf,"NV_%d", m_nNumNV ++);
        int nIdNV = Vocabulary::instance().addSymbol(sBuf, VARIABLE);
        Vocabulary::instance().setVariableDomain(sBuf, 
                        Vocabulary::instance().getVariableDomain(nIdNV));
        m_vTermsZ.push_back(nIdNV);

        fml = fml->subformula_l;
    }
    Formula ret = Formula(fml, true);
    ret.m_nFormulaId = _originalFml.m_nFormulaId;
    return ret;
}
/**
 * 新产生的谓词 s、t为内涵谓词，succ、max为“特殊”外延谓词
 * @param _originalFml
 * @return 
 */
Formulas HengZhang::transform(const Formula& _originalFml) {
    Formula originalFml = recordQuantifier(_originalFml);
    //无存在量词
    if(m_vTermsY.size() == 0) {
        Formulas fmls;
        fmls.pushBack(_originalFml);
        return fmls;
    }
    //生成谓词s
    char sBuf[32];
    sprintf(sBuf, "s_%d", m_nNumS ++);
    m_nSymbolS = Vocabulary::instance().addSymbol(sBuf, 
                        PREDICATE, m_vTermsX.size() + m_vTermsY.size());
    Vocabulary::instance().addIntensionPredicate(sBuf);
    //保存谓词原型
    _term* term_x_y   = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* s_x_y = Utils::compositeToAtom(m_nSymbolS, term_x_y);
    Vocabulary::instance().addAtom(Formula(s_x_y, false));
    //生成谓词t
    sprintf(sBuf, "t_%d", m_nNumT ++);
    m_nSymbolT = Vocabulary::instance().addSymbol(sBuf, 
                        PREDICATE, m_vTermsX.size() + m_vTermsY.size());
    Vocabulary::instance().addIntensionPredicate(sBuf);
    //保存谓词原型
    term_x_y   = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* t_x_y = Utils::compositeToAtom(m_nSymbolT, term_x_y);
    Vocabulary::instance().addAtom(Formula(t_x_y, false));
    //生成谓词succ
    string succName = "succ";
    vector<string> domainNames;
    for (unsigned int i = 0; i < m_vTermsY.size(); ++ i) {
        const char* sDomainName = Vocabulary::instance().getVariableDomain(m_vTermsY[i]);
        succName += string("_") + sDomainName;
        domainNames.push_back(string(sDomainName));
    }
    if (Vocabulary::instance().getSymbolId(succName.c_str(), PREDICATE) == -1) {
        ms_vDomainNames.push_back(domainNames);
    }
    m_nSymbolSucc = Vocabulary::instance().addSymbol(succName.c_str(), 
                        PREDICATE, m_vTermsY.size() + m_vTermsZ.size());
    //保存谓词原型
    _term* term_y_z   = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z = Utils::compositeToAtom(m_nSymbolSucc, term_y_z);
    Vocabulary::instance().addAtom(Formula(succ_y_z, false));
    
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
Formula HengZhang::createFormula_1(const Formula& _originalFml) {
    // Add S(_X, _MIN)
    _term* term_x_min   = Utils::combineTerms(m_vTermsX, m_vTermsMIN);
    _formula* s_x_min = Utils::compositeToAtom(m_nSymbolS, term_x_min);
    _formula* s_x_y = Utils::copyFormula(s_x_min);
    Utils::replaceTerm(s_x_y->parameters, 
            m_vTermsMIN.size(), m_vTermsMIN, m_vTermsY);

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
Formula HengZhang::createFormula_2(const Formula& _originalFml) {
    //create atom formulas
    //1 (succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z = Utils::compositeToAtom(m_nSymbolSucc, term_y_z);
    //2 s(_X,_Z)
    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* s_x_z = Utils::compositeToAtom(m_nSymbolS, term_x_z);
    //3 theta__(_X,_Y)
    Formula copyOriginalFml = _originalFml;
    copyOriginalFml.doubleNegationIntensionPredicates();
    _formula* theta__ = Utils::copyFormula(copyOriginalFml.getFormula());
    //4 s(_X,_Y)
    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* s_x_y   = Utils::compositeToAtom(m_nSymbolS, term_x_y);

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
Formula HengZhang::createFormula_3(const Formula& _originalFml) {
    //create left sub-formula t(_X,_MIN)
    _term* term_x_min = Utils::combineTerms(m_vTermsX, m_vTermsMIN);
    _formula* t_x_min   = Utils::compositeToAtom(m_nSymbolT, term_x_min);
    //create right sub-formula theta(_X,_MIN)
    Formula copyOriginalFml = _originalFml;
    copyOriginalFml.replaceTerms(m_vTermsY, m_vTermsMIN);
    _formula* theta_x_min = Utils::copyFormula(copyOriginalFml.getFormula());

    //create structure
    _formula* F   = Utils::compositeByConnective(DISJ, t_x_min, theta_x_min);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}
// (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y)))
_formula* HengZhang::generateFormulaLeft_4() {
    // 1 S(_X,_Y)
    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* s_x_y = Utils::compositeToAtom(m_nSymbolS, term_x_y);
    // 2 ( ~S(_X,_Z)
    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* s_x_z = Utils::compositeToAtom(m_nSymbolS, term_x_z);
    _formula* _s_x_z = Utils::compositeByConnective(NEGA, s_x_z, NULL);
    // 3 succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z = Utils::compositeToAtom(m_nSymbolSucc, term_y_z);
    // 4 max_domian(_Y)
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
Formula HengZhang::createFormula_4_1(const Formula& _originalFml) {
    // 5 T(_X,_MAX)
    _term* term_x_max = Utils::combineTerms(m_vTermsX, m_vTermsMAX);
    _formula* t_x_max  = Utils::compositeToAtom(m_nSymbolT, term_x_max);
    // 6 theta(_X,_Y)
    _formula* theta_x_y  = Utils::copyFormula(_originalFml.getFormula());
    
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
Formula HengZhang::createFormula_4_2(const Formula& _originalFml) {
    // 5 theta(_X,_Y)
    _formula* theta_x_y  = Utils::copyFormula(_originalFml.getFormula());
    // 6 T(_X,_MAX)
    _term* term_x_max = Utils::combineTerms(m_vTermsX, m_vTermsMAX);
    _formula* t_x_max  = Utils::compositeToAtom(m_nSymbolT, term_x_max);
    
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
Formula HengZhang::createFormula_5_1(const Formula& _originalFml) {
    //1 _succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z  = Utils::compositeToAtom(m_nSymbolSucc, term_y_z);

    //2 T(_X,_Y)
    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* t_x_y  = Utils::compositeToAtom(m_nSymbolT, term_x_y);

    //3 theta(_X,_Z)
    Formula copyOriginalFml = _originalFml;
    copyOriginalFml.replaceTerms(m_vTermsY, m_vTermsZ);
    _formula* theta_x_z = Utils::copyFormula(copyOriginalFml.getFormula());

    //4 T(_X,_Z)
    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* t_x_z = Utils::compositeToAtom(m_nSymbolT, term_x_z);

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
Formula HengZhang::createFormula_5_2(const Formula& _originalFml) {
    //1 _succ(_Y,_Z)
    _term* term_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
    _formula* succ_y_z  = Utils::compositeToAtom(m_nSymbolSucc, term_y_z);

    //2 T(_X,_Y)
    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
    _formula* t_x_y  = Utils::compositeToAtom(m_nSymbolT, term_x_y);

    //3 theta(_X,_Z)
    Formula copyOriginalFml = _originalFml;
    copyOriginalFml.replaceTerms(m_vTermsY, m_vTermsZ);
    _formula* theta_x_z = Utils::copyFormula(copyOriginalFml.getFormula());

    //4 T(_X,_Z)
    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
    _formula* t_x_z = Utils::compositeToAtom(m_nSymbolT, term_x_z);

    //create structure
    _formula* left = succ_y_z;
    _formula* rl = Utils::compositeByConnective(DISJ, theta_x_z, t_x_z);
    _formula* right = Utils::compositeByConnective(IMPL, rl, t_x_y);
    _formula* F = Utils::compositeByConnective(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.m_nFormulaId = Formula::ms_nNewFormulaId ++;
    return fml;
}