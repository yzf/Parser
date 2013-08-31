#include "CircTranslator.h"
#include "Vocabulary.h"
#include "NNFUtils.h"
#include "Utils.h"
#include <assert.h>

CircTranslator::CircTranslator() : m_nSymbolR(0) {
    
}

CircTranslator::~CircTranslator() {
    
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
    _formula* final = Utils::_thetaReplace(m_nSymbolR, fml);
    return Formula(final, false);
}
/**
 * 生成 r-> ^[!X](p|~p)
 * @param _originalFml
 * @return 
 */
Formulas CircTranslator::createFormula_3() {
    return createREqualP_P();
}
/**
 * 生成 ^([!X](r->q_vary))
 * @param _originalFml
 * @return 
 */
Formulas CircTranslator::createFormula_4() {
    return createRImplQVary();
}
/**
 * r-> ^[!X](p|~p)   & 
 * ^[!X](p|~p) -> r
 * @return 
 */
Formulas CircTranslator::createREqualP_P() {
    Formulas ret;
    _formula* r = Utils::compositeToAtom(m_nSymbolR, NULL);
    // r-> ^[!X](p|~p)
    _formula* f_1 = Utils::compositeByConnective(IMPL, r, 
                        createP_P());
    // ^[!X](p|~p) -> r
    _formula* f_2 = Utils::compositeByConnective(IMPL, createP_P(), 
                        Utils::copyFormula(r));
    ret.pushBack(Formula(f_1, false));
    ret.pushBack(Formula(f_2, false));
    return ret;
}
/**
 * 生成 ^([!X](r->q_vary))
 * @return 
 */
Formulas CircTranslator::createRImplQVary() {
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
            _formula* r = Utils::compositeToAtom(m_nSymbolR, NULL);
            _formula* f = Utils::compositeByConnective(IMPL, r, qVary);
            Formula tmp = Formula(f, false);
            tmp.fixUniversalQuantifier();
            
            ret.pushBack(tmp);
        }
    }
    return ret;
}
Formulas* CircTranslator::preProcessing(const Formula& _originalFml) {
    m_vNewPredicates.clear();
    // 生成辅助谓词
    m_nSymbolR = Vocabulary::instance().generatePredicateR(vector<int>(), vector<int>());
    m_vNewPredicates.push_back(m_nSymbolR);
    map<int, string> varyPredicates = Vocabulary::instance().getAllVaryPredicates();
    for (map<int, string>::const_iterator it = varyPredicates.begin(); 
            it != varyPredicates.end(); ++ it) {
        string name = it->second + VARY_PREDICATE_POSTFIX;
        int id = Vocabulary::instance().addSymbol(name.c_str(), PREDICATE, 
                    Vocabulary::instance().getPredicateArity(it->first));
        Formula q = Vocabulary::instance().getAtom(it->first);
        q.getFormula()->predicate_id = id;
        Vocabulary::instance().addAtom(q);
        m_vNewPredicates.push_back(id);
    }     
    // 拆分输入
    Formulas* pFmls = _originalFml.divideFormula();
    // 将输入转化成nnf，经典逻辑
    pFmls->convertToNNF(false);
    // 消除蕴含
    pFmls->removeImpl();
    pFmls->convertToPNF();
    pFmls->convertToNNF(false);
    
    printf("ori:\n");
    pFmls->output(stdout);
    
    return pFmls;
}
void CircTranslator::postProcessing(Formulas* _pFmls) {
    assert(_pFmls);
    _pFmls->joinBackUnique(createFormula_3());
    _pFmls->joinBackUnique(createFormula_4());
    // 把新生成的谓词标记为内涵谓词
    for (unsigned int i = 0; i < m_vNewPredicates.size(); ++ i) {
        Vocabulary::instance().addIntensionPredicate(m_vNewPredicates[i]);
    }
}
Formulas CircTranslator::transform(const Formula& _originalFml) {
    Formulas ret;
    ret.pushBack(createFormula_1(_originalFml));
    ret.pushBack(createFormula_2(_originalFml));
    return ret;
}
/**
 * 并行circ转化成sm
 * @param _originalFml 原公式
 * @return Formulas* 需要手动销毁
 */
Formulas* CircTranslator::convert(const Formula& _originalFml) {
    
    Formulas* nnfFmls = preProcessing(_originalFml);
    assert(nnfFmls);
    
    Formulas* pFmls = new Formulas();
    for (FORMULAS_CONST_ITERATOR it = nnfFmls->begin(); 
            it != nnfFmls->end(); ++ it) {
        pFmls->joinBackUnique(transform(*it));
    }
    delete nnfFmls;
    
    postProcessing(pFmls);
    
    printf("circ:\n");
    pFmls->output(stdout);
    
    return pFmls;
}