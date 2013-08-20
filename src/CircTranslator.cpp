#include "CircTranslator.h"
#include "Vocabulary.h"
#include "NNFUtils.h"
#include "Utils.h"

CircTranslator::CircTranslator() {
    
}

CircTranslator::~CircTranslator() {
    
}
/**
 * 生成 theta__
 * @param _originalFml
 * @return 
 */
Formula CircTranslator::createFormula_1(const Formula& _originalFml) {
    map<int, string> vIntensionPredicates = Vocabulary::instance().getAllIntensionPredicates();
    Formula copyOriginalFml = _originalFml;
    copyOriginalFml.doubleNegationPredicates(vIntensionPredicates);
    return copyOriginalFml;
}
/**
 * 生成 theta~
 * @param _originalFml
 * @return 
 */
Formula CircTranslator::createFormula_2(const Formula& _originalFml) {
    _formula* fml = Utils::copyFormula(_originalFml.getFormula());
    _formula* final = Utils::_thetaReplace(m_nRid, fml);
    return Formula(final, false);
}
/**
 * 生成　^[!X](p|~p)
 * @return 
 */
_formula* CircTranslator::createP_P() {
    _formula* final = NULL;
    Formulas* atoms = Vocabulary::instance().getAtomList();
    for (FORMULAS_CONST_ITERATOR it = atoms->begin();
            it != atoms->end(); ++ it) {
        if (Vocabulary::instance().isIntensionPredicate(it->getFormula()->predicate_id)) {
            _formula* p = Utils::copyFormula(it->getFormula());
            _formula* _p = Utils::compositeByConnective(NEGA, Utils::copyFormula(p));
            _formula* f = Utils::compositeByConnective(DISJ, p, _p);
            Formula tmp = Formula(f, false);
            tmp.fixUniversalQuantifier();
            if (NULL == final) {
                final = Utils::copyFormula(tmp.getFormula());
            }
            else {
                final = Utils::compositeByConnective(CONJ, final, 
                                Utils::copyFormula(tmp.getFormula()));
            }
        }
    }
    return final;
}
/**
 * 生成 r-> ^[!X](p|~p)
 * @param _originalFml
 * @return 
 */
Formula CircTranslator::createFormula_3_1() {
    _formula* r = Utils::compositeToAtom(m_nRid, NULL);
    _formula* f = Utils::compositeByConnective(IMPL, r, 
                        createP_P());
    return Formula(f, false);
}
/**
 * 生成 ^[!X](p|~p)  -> r
 * @param _originalFml
 * @return 
 */
Formula CircTranslator::createFormula_3_2() {
    _formula* r = Utils::compositeToAtom(m_nRid, NULL);
    _formula* f = Utils::compositeByConnective(IMPL,
                        createP_P(),
                        r);
    return Formula(f, false);
}
/**
 * 生成 ^([!X](r->q_vary))
 * @param _originalFml
 * @return 
 */
Formulas CircTranslator::createFormula_4() {
    Formulas ret;
    Formulas* atoms = Vocabulary::instance().getAtomList();
    for (FORMULAS_CONST_ITERATOR it = atoms->begin();
            it != atoms->end(); ++ it) {
        if (Vocabulary::instance().isVaryPredicate(it->getFormula()->predicate_id)) {
            //q_vary
            _formula* q = Utils::copyFormula(it->getFormula());
            const char* qName = Vocabulary::instance().getNameById(q->predicate_id, PREDICATE);
            char qVaryName[64];
            sprintf(qVaryName, "%s%s", qName, VARY_PREDICATE_POSTFIX);
            int qVaryId = Vocabulary::instance().getSymbolId(qVaryName, PREDICATE); 
            q->predicate_id = qVaryId;
            _formula* qVary = q;
            //r
            _formula* r = Utils::compositeToAtom(m_nRid, NULL);
            _formula* f = Utils::compositeByConnective(IMPL, r, qVary);
            Formula tmp = Formula(f, false);
            tmp.fixUniversalQuantifier();
            
            ret.pushBack(tmp);
        }
    }
    return ret;
}
Formulas CircTranslator::transform(const Formula& _originalFml) {
//    m_nRid = Vocabulary::instance().generatePredicateR(vector<int>(), vector<int>());
    Formulas ret;
    ret.pushBack(createFormula_1(_originalFml));
    ret.pushBack(createFormula_2(_originalFml));
//    ret.pushBack(createFormula_3_1());
//    ret.pushBack(createFormula_3_2());
//    ret.pushBack(createFormula_4());
//    Vocabulary::instance().addIntensionPredicate(m_nRid);
    return ret;
}
/**
 * 并行circ转化成sm
 * @param _originalFml 原公式
 * @return Formulas* 需要手动销毁
 */
Formulas* CircTranslator::convert(const Formula& _originalFml) {
    vector<int> newPredicates;
    m_nRid = Vocabulary::instance().generatePredicateR(vector<int>(), vector<int>());
    newPredicates.push_back(m_nRid);
    map<int, string> varyPredicates = Vocabulary::instance().getAllVaryPredicates();
    for (map<int, string>::const_iterator it = varyPredicates.begin(); 
            it != varyPredicates.end(); ++ it) {
        string name = it->second + VARY_PREDICATE_POSTFIX;
        int id = Vocabulary::instance().addSymbol(name.c_str(), PREDICATE, 
                    Vocabulary::instance().getPredicateArity(it->first));
        Formula q = Vocabulary::instance().getAtom(it->first);
        q.getFormula()->predicate_id = id;
        Vocabulary::instance().addAtom(q);
        newPredicates.push_back(id);
    }     
    
    // 拆分输入
    Formulas* inputTmp = _originalFml.divideFormula();
    // 将输入转化成nnf
    inputTmp->convertToNNF(false);
    Formulas* nnfFmls = new Formulas();
    for (FORMULAS_CONST_ITERATOR it = inputTmp->begin();
            it != inputTmp->end(); ++ it) {
        // 进行 p->q   ==>  ~p|q 转换
        Formula tmp = Formula(Utils::removeImpl(Utils::copyFormula(it->getFormula())), false);
        // 转化成pnf
        tmp.convertToPNF();
        // 再次删除多余的~
        tmp.convertToNNF(false);
        nnfFmls->pushBack(tmp);
    }
    delete inputTmp;
    
    printf("ori:\n");
    nnfFmls->output(stdout);
    
    Formulas* pFmls = new Formulas();
    for (FORMULAS_CONST_ITERATOR it = nnfFmls->begin(); 
            it != nnfFmls->end(); ++ it) {
        pFmls->joinBack(transform(*it));
    }
    pFmls->pushBack(createFormula_3_1());
    pFmls->pushBack(createFormula_3_2());
    pFmls->joinBack(createFormula_4());
    delete nnfFmls;
    
    printf("circ:\n");
    pFmls->output(stdout);
    // 把新生成的谓词标记为内涵谓词
    for (unsigned int i = 0; i < newPredicates.size(); ++ i) {
        Vocabulary::instance().addIntensionPredicate(newPredicates[i]);
    }
    return pFmls;
}