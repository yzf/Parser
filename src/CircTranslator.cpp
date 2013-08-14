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
    vector<int> vIntensionPredicates = Vocabulary::instance().getAllIntensionPredicates();
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
    _formula* final = Utils::circReplace(fml);
    return Formula(final, false);
}
/**
 * 生成　^[!X](p|~p)
 * @return 
 */
_formula* CircTranslator::createFormula_3_right() {
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
    int rId = Vocabulary::instance().addSymbol("r_new", PREDICATE, 0);
    Vocabulary::instance().addIntensionPredicate("r_new");
    _formula* r = Utils::compositeToAtom(rId, NULL);
    _formula* f = Utils::compositeByConnective(IMPL, r, 
                        createFormula_3_right());
    return Formula(f, false);
}
/**
 * 生成 ^[!X](p|~p)  -> r
 * @param _originalFml
 * @return 
 */
Formula CircTranslator::createFormula_3_2() {
    int rId = Vocabulary::instance().addSymbol("r_new", PREDICATE, 0);
    Vocabulary::instance().addIntensionPredicate("r_new");
    _formula* r = Utils::compositeToAtom(rId, NULL);
    _formula* f = Utils::compositeByConnective(IMPL,
                        createFormula_3_right(),
                        r);
    return Formula(f, false);
}
/**
 * 生成 ^([!X](r->q_vary))
 * @param _originalFml
 * @return 
 */
Formula CircTranslator::createFormula_4(const Formula& _originalFml) {
    _formula* final = NULL;
    Formulas* atoms = Vocabulary::instance().getAtomList();
    for (FORMULAS_CONST_ITERATOR it = atoms->begin();
            it != atoms->end(); ++ it) {
        if (Vocabulary::instance().isVaryPredicate(it->getFormula()->predicate_id)) {
            //q_vary
            _formula* q = Utils::copyFormula(it->getFormula());
            const char* qName = Vocabulary::instance().getNameById(q->predicate_id, PREDICATE);
            char qVaryName[64];
            sprintf(qVaryName, "%s_vary", qName);
            int qVaryId = Vocabulary::instance().addSymbol(qVaryName, PREDICATE, 
                            Vocabulary::instance().getPredicateArity(q->predicate_id));   
            Vocabulary::instance().addIntensionPredicate(qVaryName);
            q->predicate_id = qVaryId;
            _formula* qVary = q;
            //r
            int rId = Vocabulary::instance().addSymbol("r_new", PREDICATE, 0);
            Vocabulary::instance().addIntensionPredicate("r_new");
            _formula* r = Utils::compositeToAtom(rId, NULL);
            _formula* f = Utils::compositeByConnective(IMPL, r, qVary);
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
    return Formula(final, false);
}
/**
 * 并行circ转化成sm
 * @param _originalFml 原公式
 * @return Formulas* 需要手动销毁
 */
Formulas* CircTranslator::convert(const Formula& _originalFml) {
    Formulas* pFmls = new Formulas();
    Formula nnfFml = NNFUtils::convertToNegativeNormalForm(_originalFml);
    pFmls->pushBack(createFormula_1(nnfFml));
    pFmls->pushBack(createFormula_2(nnfFml));
    pFmls->pushBack(createFormula_3_1());
    pFmls->pushBack(createFormula_3_2());
    pFmls->pushBack(createFormula_4(nnfFml));
    
    return pFmls;
}