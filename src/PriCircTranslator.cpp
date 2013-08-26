#include <cstdlib>
#include <assert.h>
#include "Vocabulary.h"
#include "PriCircTranslator.h"
#include "Utils.h"

PriCircTranslator::PriCircTranslator() {   
}

PriCircTranslator::~PriCircTranslator() {
    m_vSysbolR.clear();
    m_vNewPredicates.clear();
}

Formula PriCircTranslator::createFormula_1(const Formula& _originalFml) {
    map<int, string> vIntensionPredicates = Vocabulary::instance().getAllIntensionPredicates();
    Formula copyOriginalFml = _originalFml;
    copyOriginalFml.doubleNegationPredicates(vIntensionPredicates);
    return copyOriginalFml;
}

Formulas PriCircTranslator::createFormula_2(const Formula& _originalFml) {
    Formulas ret;
    vector<int> preMiniPredicates;
    vector<int> curMiniPredicates;
    vector<int> otherPredicates;
    vector<int> varyPredicates = Vocabulary::instance().getAllVaryPredicatesId();
    
    vector< vector<int> > mininalPredicates = Vocabulary::instance().getAllMininalPredicates();
    unsigned int priSize = mininalPredicates.size();
    for (unsigned int i = 0; i < priSize; ++ i) {
        int rId = m_vSysbolR[i];
        curMiniPredicates = mininalPredicates[i];
        otherPredicates.clear();
        for (unsigned int j = i + 1; j < priSize; ++ j) {
            otherPredicates.insert(otherPredicates.end(), 
                    mininalPredicates[j].begin(), mininalPredicates[j].end());
        }
        otherPredicates.insert(otherPredicates.end(), varyPredicates.begin(), varyPredicates.end());
        
        _formula* f = Utils::copyFormula(_originalFml.getFormula());
        f = Utils::_thetaReplace(preMiniPredicates, curMiniPredicates, otherPredicates, rId, i, f);
        ret.pushBack(Formula(f, false));
        
        preMiniPredicates.insert(preMiniPredicates.end(), curMiniPredicates.begin(), curMiniPredicates.end());
    }
    return ret;
}

Formulas PriCircTranslator::createFormula_3() {
    Formulas ret;
    vector<int> curMiniPredicates;
    vector< vector<int> > mininalPredicates = Vocabulary::instance().getAllMininalPredicates();
    unsigned int priSize = mininalPredicates.size();
    for (unsigned int i = 0; i < priSize; ++ i) {
        int rId = m_vSysbolR[i];
        _formula* p_p = NULL;
        curMiniPredicates = mininalPredicates[i];
        for (unsigned int j = 0; j < curMiniPredicates.size(); ++ j) {
            Formula atom = Vocabulary::instance().getAtom(curMiniPredicates[j]);
            _formula* p = Utils::copyFormula(atom.getFormula());
            _formula* _p = Utils::compositeByConnective(NEGA, Utils::copyFormula(p));
            _formula* f = Utils::compositeByConnective(DISJ, p, _p);
            p_p = (NULL == p_p) ? f : Utils::compositeByConnective(CONJ, p_p, f);
        }
        Formula tmp = Formula(p_p, false);
        tmp.fixUniversalQuantifier();
        p_p = Utils::copyFormula(tmp.getFormula());
        _formula* r = Utils::compositeToAtom(rId, NULL);
        _formula* final_1 = Utils::compositeByConnective(IMPL, r, p_p);
        _formula* final_2 = Utils::compositeByConnective(IMPL, Utils::copyFormula(p_p), Utils::copyFormula(r));
        ret.pushBack(Formula(final_1, false));
        ret.pushBack(Formula(final_2, false));
    }
    return ret;
}

Formulas PriCircTranslator::createFormula_4() {
    Formulas ret;
    vector<int> otherPredicates;
    vector<int> varyPredicates = Vocabulary::instance().getAllVaryPredicatesId();
    vector< vector<int> > mininalPredicates = Vocabulary::instance().getAllMininalPredicates();
    unsigned int priSize = mininalPredicates.size();
    for (unsigned int i = 0; i < priSize; ++ i) {
        int rId = m_vSysbolR[i];
        otherPredicates.clear();
        for (unsigned int j = i + 1; j < priSize; ++ j) {
            otherPredicates.insert(otherPredicates.end(), 
                    mininalPredicates[j].begin(), mininalPredicates[j].end());
        }
        otherPredicates.insert(otherPredicates.end(), varyPredicates.begin(), varyPredicates.end());
        if (otherPredicates.size() == 0) {
            continue;
        }
        _formula* f = NULL;
        for (unsigned int j = 0; j < otherPredicates.size(); ++ j) {
            Formula atom = Vocabulary::instance().getAtom(otherPredicates[j]);
            const char* oriName = Vocabulary::instance().getNameById(otherPredicates[j], PREDICATE);
            char name[64];
            sprintf(name, "%s_%d%s", oriName, i, VARY_PREDICATE_POSTFIX);
            int id = Vocabulary::instance().getSymbolId(name, PREDICATE);
            _formula* q = Utils::copyFormula(atom.getFormula());
            q->predicate_id = id;
            f = (NULL == f) ? q : Utils::compositeByConnective(CONJ, f, q);
        }
        _formula* r = Utils::compositeToAtom(rId, NULL);
        _formula* final = Utils::compositeByConnective(IMPL, r, f);
        Formula tmp = Formula(final, false);
        tmp.fixUniversalQuantifier();
        ret.pushBack(tmp);
    }
    return ret;
}

Formulas* PriCircTranslator::preProcessing(const Formula& _originalFml) {
    m_vNewPredicates.clear();
    m_vSysbolR.clear();
    
    vector< vector<int> > mininalPredicates = Vocabulary::instance().getAllMininalPredicates();
    unsigned int priSize = mininalPredicates.size();
    // 生成辅助谓词
    vector<int> otherPredicates;
    vector<int> varyPredicates = Vocabulary::instance().getAllVaryPredicatesId();
    for (unsigned int i = 0; i < priSize; ++ i) {
        // 生成 r 
        m_vSysbolR.push_back(Vocabulary::instance().generatePredicateR(vector<int>(), vector<int>()));
        
        otherPredicates.clear();
        for (unsigned int j = i + 1; j < priSize; ++ j) {
            otherPredicates.insert(otherPredicates.end(), 
                    mininalPredicates[j].begin(), mininalPredicates[j].end());
        }
        otherPredicates.insert(otherPredicates.end(), varyPredicates.begin(), varyPredicates.end());
        // 生成 q_j_vary类型
        for (unsigned int j = 0; j < otherPredicates.size(); ++ j) {
            const char* oriName = Vocabulary::instance().getNameById(otherPredicates[j], PREDICATE);
            char name[64];
            sprintf(name, "%s_%d%s", oriName, i, VARY_PREDICATE_POSTFIX);
            int id = Vocabulary::instance().addSymbol(name, PREDICATE, 
                        Vocabulary::instance().getPredicateArity(otherPredicates[j]));
            Formula atom = Vocabulary::instance().getAtom(otherPredicates[j]);
            atom.getFormula()->predicate_id = id;
            Vocabulary::instance().addAtom(atom);
            m_vNewPredicates.push_back(id);
        }
    }
    
    Formulas* fmls = _originalFml.divideFormula();
    fmls->convertToNNF(false);
    fmls->removeImpl();
    fmls->convertToPNF();
    fmls->convertToNNF(false);
    
    printf("ori:\n");
    fmls->output(stdout);
    
    return fmls;
}

void PriCircTranslator::postProcessing(Formulas* _pFmls) {
    _pFmls->joinBack(createFormula_3());
    _pFmls->joinBack(createFormula_4());
    // 把新生成的谓词标记为内涵谓词
    for (unsigned int i = 0; i < m_vSysbolR.size(); ++ i) {
        Vocabulary::instance().addIntensionPredicate(m_vSysbolR[i]);
    }
    for (unsigned int i = 0; i < m_vNewPredicates.size(); ++ i) {
        Vocabulary::instance().addIntensionPredicate(m_vNewPredicates[i]);
    }
}

Formulas PriCircTranslator::transform(const Formula& _originalFml) {
    Formulas ret;
    ret.pushBack(createFormula_1(_originalFml));
    ret.joinBack(createFormula_2(_originalFml));
    return ret;
}

Formulas* PriCircTranslator::convert(const Formula& _originalFml) {
    
    Formulas* nnfFmls = preProcessing(_originalFml);
    assert(nnfFmls);
    
    Formulas* pFmls = new Formulas();
    for (FORMULAS_CONST_ITERATOR it = nnfFmls->begin();
            it != nnfFmls->end(); ++ it) {
        pFmls->joinBack(transform(*it));
    }
    delete nnfFmls;
    postProcessing(pFmls);
    
    printf("pri circ:\n");
    pFmls->output(stdout);
    
    return pFmls;
}
