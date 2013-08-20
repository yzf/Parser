//#include "Optimization.h"
//#include "structs.h"
//#include "Vocabulary.h"
//#include "Utils.h"
//#include "NNFUtils.h"
//#include "SMTranslator.h"
//#include "HengZhang.h"
//#include <cstdlib>
//
//const string Optimization::ms_sZVariPostfix = "_OP";
//const string Optimization::ms_sPredicateNameS = "s_op";
//const string Optimization::ms_sPredicateNameW = "w_op";
//const string Optimization::ms_sPredicateNameT = "t_op";
//
//Optimization::Optimization() {
//    
//}
//
//Optimization::~Optimization() {
//    
//}
//
//Optimization& Optimization::instance() {
//    static Optimization theInstance;
//    return theInstance;
//}
///**
// * 记录已有变量,生成新变量,新谓词
// * @param _originalFml
// * @return Formula 不带量词的公式
// */
//Formula Optimization::preProcessing(const Formula& _originalFml) {
//    // pnf
//    Formula original = _originalFml;
//    original.convertToPNF();
//    printf("ori:\n");
//    original.output(stdout);
//    // nnf
//    original.convertToNNF(false);
//    // 清除旧数据
//    m_vTermsX.clear();
//    m_vTermsY.clear();
//    m_vTermsZ.clear();
//    m_vTermsMIN.clear();
//    // 记录全称量词
//    _formula* fml = original.getFormula();
//    while (fml->formula_type == UNIV) {
//        m_vTermsX.push_back(fml->variable_id);
//        fml = fml->subformula_l;
//    }
//    int id;
//    string name;
//    // 记录存在量词
//    while (fml->formula_type == EXIS) {
//        m_vTermsY.push_back(fml->variable_id);
//        const char* sVariName = Vocabulary::instance().getNameById(fml->variable_id, VARIABLE);
//        const char* sDoaminName = Vocabulary::instance().getVariableDomain(fml->variable_id);
//        // 如X@a,则 MIN_a
//        name = string("MIN_") + sDoaminName;
//        id = Vocabulary::instance().addSymbol(name.c_str(), VARIABLE);
//        m_vTermsMIN.push_back(id);
//        // 如X@a,则 MAX_a
//        name = string("MAX_") + sDoaminName;
//        id = Vocabulary::instance().addSymbol(name.c_str(), VARIABLE);
//        m_vTermsMAX.push_back(id);
//        // 存在量词换名生成Z,如X => X_OP
//        name = sVariName + ms_sZVariPostfix;
//        id = Vocabulary::instance().addSymbol(name.c_str(), VARIABLE);
//        Vocabulary::instance().setVariableDomain(name.c_str(),
//            Vocabulary::instance().getVariableDomain(fml->variable_id));
//        m_vTermsZ.push_back(id);
//        
//        fml = fml->subformula_l;
//    }
//    
//    // max_domian(_Y)
//    for (unsigned int i = 0; i < m_vTermsY.size(); ++ i) {
//        string name = string("max_") + Vocabulary::instance().getVariableDomain(m_vTermsY[i]);
//        _term* term_yi = (_term*)malloc(sizeof(_term));
//        term_yi->term_type = VARI;
//        term_yi->variable_id = m_vTermsY[i];
//        id = Vocabulary::instance().addSymbol(name.c_str(), PREDICATE, 1);
//        _formula* max_yi = Utils::compositeToAtom(id, term_yi);
//        Vocabulary::instance().addAtom(Formula(max_yi, false));
//    }
//    
//    // 生成s(X_,Y_)
//    id = Vocabulary::instance().addSymbol(ms_sPredicateNameS.c_str(),
//            PREDICATE, m_vTermsX.size() + m_vTermsY.size());
//    _term* term = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    Vocabulary::instance().addAtom(Formula(Utils::compositeToAtom(id, term), false));
//    // 生成succ(Y_,Z_)
//    ms_sPredicateNameSucc = "succ";
//    vector<string> domainNames;
//    for (unsigned int i = 0; i < m_vTermsY.size(); ++ i) {
//        const char* sDomainName = Vocabulary::instance().getVariableDomain(m_vTermsY[i]);
//        ms_sPredicateNameSucc += string("_") + sDomainName;
//        domainNames.push_back(string(sDomainName));
//    }
//    if (ms_sPredicateNameSucc != "succ") {
//        id = Vocabulary::instance().addSymbol(ms_sPredicateNameSucc.c_str(),
//                PREDICATE, m_vTermsY.size() + m_vTermsY.size());
//        term = Utils::combineTerms(m_vTermsY, m_vTermsZ);
//        Vocabulary::instance().addAtom(Formula(Utils::compositeToAtom(id, term), false));
//        Vocabulary::instance().ms_vDomainNames.push_back(domainNames);
//    }
//    // 生成w(X_,Y_)
//    id = Vocabulary::instance().addSymbol(ms_sPredicateNameW.c_str(),
//            PREDICATE, m_vTermsX.size() + m_vTermsY.size());
//    term = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    Vocabulary::instance().addAtom(Formula(Utils::compositeToAtom(id, term), false));
//    // 生成t(X_,Y_)
//    id = Vocabulary::instance().addSymbol(ms_sPredicateNameT.c_str(),
//            PREDICATE, m_vTermsX.size() + m_vTermsY.size());
//    term = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    Vocabulary::instance().addAtom(Formula(Utils::compositeToAtom(id, term), false));
//
//    //　生成辅助谓词r
//    id = Vocabulary::instance().addSymbol(R_NAME, PREDICATE);
//    _formula* r = Utils::compositeToAtom(id, NULL);
//    Vocabulary::instance().addAtom(Formula(r, false));
//    // 生成可变谓词的辅助谓词
//    map<int, string> varyPredicates = Vocabulary::instance().getAllVaryPredicates();
//    for (map<int, string>::const_iterator it = varyPredicates.begin(); 
//            it != varyPredicates.end(); ++ it) {
//        string name = it->second + VARY_PREDICATE_POSTFIX;
//        int id = Vocabulary::instance().addSymbol(name.c_str(), PREDICATE, 
//                    Vocabulary::instance().getPredicateArity(it->first));
//        Formula q = Vocabulary::instance().getAtom(it->first);
//        q.getFormula()->predicate_id = id;
//        Vocabulary::instance().addAtom(q);
//        m_vNewPredicatesName.push_back(name);
//    }     
//    
//    return Formula(fml, true);
//}
///**
// * __s(X_, MIN_)
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_1() {
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameS.c_str(), PREDICATE);
//    _term* term = Utils::combineTerms(m_vTermsX, m_vTermsMIN);
//    _formula* f = Utils::compositeToAtom(id, term);
//    f = Utils::compositeByConnective(NEGA, f);
//    f = Utils::compositeByConnective(NEGA, f);
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 (succ(Y_,Z_) & s(X_,Z)) | theta__(X_,Y_)   ->   s(X_,Y_)
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_2(const Formula& _originalFml) {
//    // succ(Y_,Z_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameSucc.c_str(), PREDICATE);
//    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
//    _formula* succ_y_z = Utils::compositeToAtom(id, t_y_z);
//    // s(X_,Z_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameS.c_str(), PREDICATE);
//    _term* t_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
//    _formula* s_x_z = Utils::compositeToAtom(id, t_x_z);
//    // theta__(X_,Y_)
//    map<int, string> mapIntensionPredicates = Vocabulary::instance().getAllIntensionPredicates();
//    _formula* theta__x_y = Utils::doubleNegationPredicates(
//                                Utils::copyFormula(_originalFml.getFormula()), 
//                                mapIntensionPredicates);
//    // s(X_,Y_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameS.c_str(), PREDICATE);
//    _term* t_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    _formula* s_x_y = Utils::compositeToAtom(id, t_x_y);
//    
//    _formula* f = Utils::compositeByConnective(CONJ, succ_y_z, s_x_z);
//    f = Utils::compositeByConnective(DISJ, f, theta__x_y);
//    f = Utils::compositeByConnective(IMPL, f, s_x_y);
//  
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 (succ(Y_,Z_) & w(X_,Z_)) | theta_t__(X_,Y_)  ->  w(X_,Y_)
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_3(const Formula& _originalFml) {
//    // succ(Y_,Z_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameSucc.c_str(), PREDICATE);
//    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
//    _formula* succ_y_z = Utils::compositeToAtom(id, t_y_z);
//    // w(X_,Z_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameW.c_str(), PREDICATE);
//    _term* t_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
//    _formula* w_x_z = Utils::compositeToAtom(id, t_x_z);
//    // thetaT__(X_,Y_)
//    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
//    _formula* thetaT__ = Utils::thetaT__Replace(theta);
//    // w(X_,Y_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameW.c_str(), PREDICATE);
//    _term* t_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    _formula* w_x_y = Utils::compositeToAtom(id, t_x_y);
//    
//    _formula* f = Utils::compositeByConnective(CONJ, succ_y_z, w_x_z);
//    f = Utils::compositeByConnective(DISJ, f, thetaT__);
//    f = Utils::compositeByConnective(IMPL, f, w_x_y);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 t(X_,MIN_) | _theta(X_,MIN_)
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_4(const Formula& _originalFml) {
//    // t(X_,MIN_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameT.c_str(), PREDICATE);
//    _term* term_x_min = Utils::combineTerms(m_vTermsX, m_vTermsMIN);
//    _formula* t_x_min = Utils::compositeToAtom(id, term_x_min);
//    // _theta(X_,MIN_)
//    Formula copyOri = _originalFml;
//    copyOri.replaceTerms(m_vTermsY, m_vTermsMIN);
//    _formula* theta = Utils::copyFormula(copyOri.getFormula());
//    _formula* _theta = Utils::_thetaReplace(theta);
//    
//    _formula* f = Utils::compositeByConnective(DISJ, t_x_min, _theta);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 succ(Y_,Z_)  ->  (t(X_,Y_) -> _theta(X_,Z_) | t(X_,Z_))
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_5_1(const Formula& _originalFml) {
//    // succ(Y_,Z_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameSucc.c_str(), PREDICATE);
//    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
//    _formula* succ_y_z = Utils::compositeToAtom(id, t_y_z);
//    // t_x_y
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameT.c_str(), PREDICATE);
//    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    _formula* t_x_y = Utils::compositeToAtom(id, term_x_y);
//    // _theta(X_,Z_)
//    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
//    _formula* _theta = Utils::_thetaReplace(theta);
//    // t(X_,Z_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameT.c_str(), PREDICATE);
//    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
//    _formula* t_x_z = Utils::compositeToAtom(id, term_x_z);
//    
//    _formula* f = Utils::compositeByConnective(DISJ, _theta, t_x_z);
//    f = Utils::compositeByConnective(IMPL, t_x_y, f);
//    f = Utils::compositeByConnective(IMPL, succ_y_z, f);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 succ(Y_,Z_)  ->  (_theta(X_,Z_) | t(X_,Z_)  ->  t(X_,Y_))
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_5_2(const Formula& _originalFml) {
//    // succ(Y_,Z_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameSucc.c_str(), PREDICATE);
//    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
//    _formula* succ_y_z = Utils::compositeToAtom(id, t_y_z);
//    // t_x_y
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameT.c_str(), PREDICATE);
//    _term* term_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    _formula* t_x_y = Utils::compositeToAtom(id, term_x_y);
//    // _theta(X_,Z_)
//    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
//    _formula* _theta = Utils::_thetaReplace(theta);
//    // t(X_,Z_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameT.c_str(), PREDICATE);
//    _term* term_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
//    _formula* t_x_z = Utils::compositeToAtom(id, term_x_z);
//    
//    _formula* f = Utils::compositeByConnective(DISJ, _theta, t_x_z);
//    f = Utils::compositeByConnective(IMPL, f, t_x_y);
//    f = Utils::compositeByConnective(IMPL, succ_y_z, f);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 (w(_X,_Y) & ((~w(_X,_Z) & succ(_Y,_Z)) | _max(Y)))
// * @return 
// */
//_formula* Optimization::create_6_left() {
//    // w(X_,Y_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameW.c_str(), PREDICATE);
//    _term* t_x_y = Utils::combineTerms(m_vTermsX, m_vTermsY);
//    _formula* w_x_y = Utils::compositeToAtom(id, t_x_y);
//    // ~w(X_,Z_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameW.c_str(), PREDICATE);
//    _term* t_x_z = Utils::combineTerms(m_vTermsX, m_vTermsZ);
//    _formula* w_x_z = Utils::compositeToAtom(id, t_x_z);
//    _formula* _w_x_z = Utils::compositeByConnective(NEGA, w_x_z);
//    // succ(Y_,Z_)
//    id = Vocabulary::instance().getSymbolId(ms_sPredicateNameSucc.c_str(), PREDICATE);
//    _term* t_y_z = Utils::combineTerms(m_vTermsY, m_vTermsZ);
//    _formula* succ_y_z = Utils::compositeToAtom(id, t_y_z);
//    
//    // max_domian(_Y)
//    vector<_formula*> max_ys;
//    for (unsigned int i = 0; i < m_vTermsY.size(); ++ i) {
//        string name = string("max_") + Vocabulary::instance().getVariableDomain(m_vTermsY[i]);
//        _term* term_yi = (_term*)malloc(sizeof(_term));
//        term_yi->term_type = VARI;
//        term_yi->variable_id = m_vTermsY[i];
//        id = Vocabulary::instance().getSymbolId(name.c_str(), PREDICATE);
//        _formula* max_yi = Utils::compositeToAtom(id, term_yi);
//        max_ys.push_back(max_yi);
//    }
//    _formula* max_y = NULL;
//    for (unsigned int i = 0; i < max_ys.size(); ++ i) {
//        if (NULL == max_y) {
//            max_y = max_ys[i];
//        }
//        else {
//            max_y = Utils::compositeByConnective(CONJ, max_y, max_ys[i]);
//        }
//    }
//    
//    _formula* f = Utils::compositeByConnective(CONJ, _w_x_z, succ_y_z);
//    if (NULL != max_y) {
//        f = Utils::compositeByConnective(DISJ, f, max_y);
//    }
//    f = Utils::compositeByConnective(CONJ, w_x_y, f);
//    
//    return f;
//}
///**
// * (w(_X,_Y) & ((~w(_X,_Z) & succ(_Y,_Z)) | _max(Y))) -> 
// *              ( (t(_X,_MAX) -> _theta(_X,_Y)))
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_6_1(const Formula& _originalFml) {
//    _formula* left = create_6_left();
//    // t(X_,MAX_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameT.c_str(), PREDICATE);
//    _term* term_x_max = Utils::combineTerms(m_vTermsX, m_vTermsMAX);
//    _formula* t_x_max = Utils::compositeToAtom(id, term_x_max);
//    // _theta(X_,Y_)
//    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
//    _formula* _theta = Utils::_thetaReplace(theta);
//    
//    _formula* f = Utils::compositeByConnective(IMPL, t_x_max, _theta);
//    f = Utils::compositeByConnective(IMPL, left, f);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * (w(_X,_Y) & ((~w(_X,_Z) & succ(_Y,_Z)) | _max(Y))) 
// *                              -> (_theta(_X,_Y) -> t(_X,_MAX))
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_6_2(const Formula& _originalFml) {
//    _formula* left = create_6_left();
//    // t(X_,MAX_)
//    int id = Vocabulary::instance().getSymbolId(ms_sPredicateNameT.c_str(), PREDICATE);
//    _term* term_x_max = Utils::combineTerms(m_vTermsX, m_vTermsMAX);
//    _formula* t_x_max = Utils::compositeToAtom(id, term_x_max);
//    // _theta(X_,Y_)
//    _formula* theta = Utils::copyFormula(_originalFml.getFormula());
//    _formula* _theta = Utils::_thetaReplace(theta);
//    
//    _formula* f = Utils::compositeByConnective(IMPL, _theta, t_x_max);
//    f = Utils::compositeByConnective(IMPL, left, f);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成　^[!X](p|~p)
// * @return 
// */
//_formula* Optimization::createP_P() {
//    _formula* final = NULL;
//    Formulas* atoms = Vocabulary::instance().getAtomList();
//    for (FORMULAS_CONST_ITERATOR it = atoms->begin();
//            it != atoms->end(); ++ it) {
//        if (Vocabulary::instance().isIntensionPredicate(it->getFormula()->predicate_id)) {
//            _formula* p = Utils::copyFormula(it->getFormula());
//            _formula* _p = Utils::compositeByConnective(NEGA, Utils::copyFormula(p));
//            _formula* f = Utils::compositeByConnective(DISJ, p, _p);
//            Formula tmp = Formula(f, false);
//            tmp.fixUniversalQuantifier();
//            if (NULL == final) {
//                final = Utils::copyFormula(tmp.getFormula());
//            }
//            else {
//                final = Utils::compositeByConnective(CONJ, final, 
//                                Utils::copyFormula(tmp.getFormula()));
//            }
//        }
//    }
//    return final;
//}
///**
// * 生成 r-> ^[!X](p|~p)
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_7_1() {
//    // r
//    int rId = Vocabulary::instance().getSymbolId(R_NAME, PREDICATE);
//    _formula* r = Utils::compositeToAtom(rId, NULL);
//    // ^[!X](p|~p)
//    _formula* p_p = createP_P();
//    _formula* f = Utils::compositeByConnective(IMPL, r, p_p);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 ^[!X](p|~p)  -> r
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_7_2() {
//    // r
//    int rId = Vocabulary::instance().getSymbolId(R_NAME, PREDICATE);
//    _formula* r = Utils::compositeToAtom(rId, NULL);
//    // ^[!X](p|~p)
//    _formula* p_p = createP_P();
//    _formula* f = Utils::compositeByConnective(IMPL, p_p, r);
//    
//    Formula ret = Formula(f, false);
//    return ret;
//}
///**
// * 生成 ^([!X](r->q_vary))
// * @param _originalFml
// * @return 
// */
//Formula Optimization::createFormula_8() {
//    _formula* final = NULL;
//    Formulas* atoms = Vocabulary::instance().getAtomList();
//    for (FORMULAS_CONST_ITERATOR it = atoms->begin();
//            it != atoms->end(); ++ it) {
//        if (Vocabulary::instance().isVaryPredicate(it->getFormula()->predicate_id)) {
//            //q_vary
//            _formula* q = Utils::copyFormula(it->getFormula());
//            const char* qName = Vocabulary::instance().getNameById(q->predicate_id, PREDICATE);
//            char qVaryName[64];
//            sprintf(qVaryName, "%s%s", qName, VARY_PREDICATE_POSTFIX);
//            int qVaryId = Vocabulary::instance().getSymbolId(qVaryName, PREDICATE); 
//            q->predicate_id = qVaryId;
//            _formula* qVary = q;
//            //r
//            int rId = Vocabulary::instance().getSymbolId(R_NAME, PREDICATE);
//            _formula* r = Utils::compositeToAtom(rId, NULL);
//            _formula* f = Utils::compositeByConnective(IMPL, r, qVary);
//            Formula tmp = Formula(f, false);
//            tmp.fixUniversalQuantifier();
//            
//            if (NULL == final) {
//                final = Utils::copyFormula(tmp.getFormula());
//            }
//            else {
//                final = Utils::compositeByConnective(CONJ, final, 
//                            Utils::copyFormula(tmp.getFormula()));
//            }
//        }
//    }
//    
//    Formula ret = Formula(final, false);
//    return ret;
//}
///**
// * op转化
// * @param _originalFml 原公式
// * @return Formulas* 需要手动销毁 
// */
//Formulas* Optimization::convert(const Formula& _originalFml) {
//    Formula fml = preProcessing(_originalFml);
//
//    Formulas* fmls = new Formulas();
//    fmls->pushBack(createFormula_1());
//    fmls->pushBack(createFormula_2(fml));
//    fmls->pushBack(createFormula_3(fml));
//    fmls->pushBack(createFormula_4(fml));
//    fmls->pushBack(createFormula_5_1(fml));
//    fmls->pushBack(createFormula_5_2(fml));
//    fmls->pushBack(createFormula_6_1(fml));
//    fmls->pushBack(createFormula_6_2(fml));
//    fmls->pushBack(createFormula_7_1());
//    fmls->pushBack(createFormula_7_2());
//    fmls->pushBack(createFormula_8());
//    // 把新生成的谓词标记为内涵谓词
//    // r
//    Vocabulary::instance().addIntensionPredicate(R_NAME);
//    // q_vary
//    for (unsigned int i = 0; i < m_vNewPredicatesName.size(); ++ i) {
//        Vocabulary::instance().addIntensionPredicate(m_vNewPredicatesName[i].c_str());
//    }
//    // s_op w_op t_op
//    Vocabulary::instance().addIntensionPredicate(ms_sPredicateNameS.c_str());
//    Vocabulary::instance().addIntensionPredicate(ms_sPredicateNameW.c_str());
//    Vocabulary::instance().addIntensionPredicate(ms_sPredicateNameT.c_str());
//    
//    return fmls;
//}