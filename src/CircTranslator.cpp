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
    map<int, string> vIntensionPredicates = Vocabulary::instance().getAllPredicates();
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
    _formula* final = Utils::_thetaReplace(fml);
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
    int rId = Vocabulary::instance().getSymbolId(R_NAME, PREDICATE);
    _formula* r = Utils::compositeToAtom(rId, NULL);
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
    int rId = Vocabulary::instance().getSymbolId(R_NAME, PREDICATE);
    _formula* r = Utils::compositeToAtom(rId, NULL);
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
Formula CircTranslator::createFormula_4() {
    _formula* final = NULL;
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
            int rId = Vocabulary::instance().getSymbolId(R_NAME, PREDICATE);
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
    // 生成新谓词
    //　r
    int rId = Vocabulary::instance().addSymbol(R_NAME, PREDICATE);
    _formula* r = Utils::compositeToAtom(rId, NULL);
    Vocabulary::instance().addAtom(Formula(r, false));
    // 生成可变谓词的辅助谓词
    map<int, string> varyPredicates = Vocabulary::instance().getAllVaryPredicates();
    vector<string> newPredicatesName;
    for (map<int, string>::const_iterator it = varyPredicates.begin(); 
            it != varyPredicates.end(); ++ it) {
        string name = it->second + VARY_PREDICATE_POSTFIX;
        int id = Vocabulary::instance().addSymbol(name.c_str(), PREDICATE, 
                    Vocabulary::instance().getPredicateArity(it->first));
        Formula q = Vocabulary::instance().getAtom(it->first);
        q.getFormula()->predicate_id = id;
        Vocabulary::instance().addAtom(q);
        newPredicatesName.push_back(name);
    }     
    
    Formulas* pFmls = new Formulas();
    Formula nnfFml = NNFUtils::convertToNegativeNormalForm(_originalFml, false);
    pFmls->pushBack(createFormula_1(nnfFml));
    pFmls->pushBack(createFormula_2(nnfFml));
    pFmls->pushBack(createFormula_3_1());
    pFmls->pushBack(createFormula_3_2());
    pFmls->pushBack(createFormula_4());
    // 把新生成的谓词标记为内涵谓词
    Vocabulary::instance().addIntensionPredicate(R_NAME);
    for (unsigned int i = 0; i < newPredicatesName.size(); ++ i) {
        Vocabulary::instance().addIntensionPredicate(newPredicatesName[i].c_str());
    }
    return pFmls;
}