#include "Rule.h"
#include "Utils.h"
#include "S2DLP.h"
#include <assert.h>
#include <cstdlib>

Rule::Rule(const Formula& _fml) {
    this->m_pHeadFormulas = new Formulas();
    this->m_pBodyFormulas = new Formulas();
    convertFormulaToRule(_fml.getFormula());
}
Rule::Rule(const Rule& _rhs) {
    this->m_pHeadFormulas = new Formulas(*(_rhs.m_pHeadFormulas));
    this->m_pBodyFormulas = new Formulas(*(_rhs.m_pBodyFormulas));
}
Rule::~Rule() {
    if (this->m_pHeadFormulas != NULL) {
        delete this->m_pHeadFormulas;
        this->m_pHeadFormulas = NULL;
    }
    if (this->m_pBodyFormulas != NULL) {
        delete this->m_pBodyFormulas;
        this->m_pBodyFormulas = NULL;
    }
}
Rule& Rule::operator = (const Rule& _rhs) {
    this->m_pHeadFormulas = new Formulas(*(_rhs.m_pHeadFormulas));
    this->m_pBodyFormulas = new Formulas(*(_rhs.m_pBodyFormulas));
    return *this;
}
/**
 * 输出规则
 * @param _out
 */
void Rule::output(FILE* _out) const {
    //输出规则头部
    for (FORMULAS_ITERATOR it = m_pHeadFormulas->begin(); 
            it != m_pHeadFormulas->end(); ++ it) {
        _formula* headPart = it->getFormula();
        
        if (headPart->formula_type == ATOM && headPart->predicate_id != PRED_FALSE 
                && headPart->predicate_id != PRED_TRUE) {
            Utils::printAtom(headPart, _out);       
            if (it != m_pHeadFormulas->end() - 1 && 
                    (it+1)->getFormula()->predicate_id != PRED_FALSE &&
                        (it+1)->getFormula()->predicate_id != PRED_TRUE) {
                fprintf(_out, "|");
            }
        }        
    }
    //输出规则体部
    bool bodyBegin = true;
    
    for (FORMULAS_ITERATOR it = m_pBodyFormulas->begin(); 
            it != m_pBodyFormulas->end(); ++ it) {
        _formula* bodyPart = it->getFormula();
        _formula* cur = bodyPart;
        
        while (cur->formula_type != ATOM) {
            cur = cur->subformula_l;
        }                
        
        if (cur->predicate_id != PRED_TRUE && cur->predicate_id != PRED_FALSE) {            
            if (bodyBegin) {
                fprintf(_out, ":-");
                bodyBegin = false;
            }
            
            if (bodyPart->formula_type == NEGA) {
                fprintf(_out, "not ");
                bodyPart = bodyPart->subformula_l;
                if (bodyPart->formula_type == NEGA) {
                    bool exis = false;
                    for (FORMULAS_ITERATOR it_2 = S2DLP::instance().getNegaPredicates()->begin();
                            it_2 != S2DLP::instance().getNegaPredicates()->end(); ++ it_2) {
                        if (it_2->getFormula()->predicate_id == bodyPart->subformula_l->predicate_id) {
                            exis = true;
                        }
                    }
                    if (! exis) {
                        Formula newNegaPredicate = Vocabulary::instance()
                                        .getAtom(bodyPart->subformula_l->predicate_id); 
                        assert(newNegaPredicate.getFormula());
                        S2DLP::instance().addNegaPredicates(newNegaPredicate);
                    }
                    fprintf(_out, "_");
                }
            }            
            Utils::printAtom(cur, _out);
            if(it != m_pBodyFormulas->end() - 1 && (it+1)->getFormula()->predicate_id != PRED_FALSE &&
                (it+1)->getFormula()->predicate_id != PRED_TRUE) {
                fprintf(_out, ",");            
            }
        }
    }  
    if (m_pHeadFormulas->size() + m_pBodyFormulas->size() > 0) {
        fprintf(_out, ".\n");
    }
}
/**
 * 判断该规则是否多余
 * @return 
 */
bool Rule::isUseless() const {
    for (FORMULAS_ITERATOR headIt = m_pHeadFormulas->begin(); 
            headIt != m_pHeadFormulas->end(); ++ headIt) {
        for (FORMULAS_ITERATOR bodyIt = m_pBodyFormulas->begin();
                bodyIt != m_pBodyFormulas->end(); ++ bodyIt) {
            if (*headIt == *bodyIt) {
                return true;
            }
        }
    }
    return false;
}
/**
 * 生成规则的头部
 * @param _head
 */
void Rule::divideHead(const _formula* _head) {
    if (_head->formula_type != DISJ) {
        _formula* newHead = Utils::copyFormula(_head);
        this->m_pHeadFormulas->pushBack(Formula(newHead, false));
    }
    else {
        divideHead(_head->subformula_l);
        divideHead(_head->subformula_r);
    }
}
/**
 * 生成规则的体部
 * @param _body
 */
void Rule::divideBody(const _formula* _body) {
    if (_body->formula_type != CONJ) {
        _formula* newBody = Utils::copyFormula(_body);
        this->m_pBodyFormulas->pushBack(Formula(newBody, false));
    }
    else {
        divideBody(_body->subformula_l);
        divideBody(_body->subformula_r);
    }
}
/**
 * 把公式转化成规则
 * @param _fml
 */
void Rule::convertFormulaToRule(const _formula* _fml) {
    if (_fml == NULL) {
        return;
    }
    switch (_fml->formula_type) {
    case IMPL:
        divideHead(_fml->subformula_r);
        divideBody(_fml->subformula_l);
        break;
    case NEGA:
    case DISJ:
    case ATOM:
        divideHead(_fml);
        break;
    default:
        break;
    }
    aspModify();
}
/**
 * 处理多余的非
 */
void Rule::aspModify() {
    //把带非的，出现在头部的外延谓词，加上非，放到体部
    for (FORMULAS_ITERATOR iter = m_pHeadFormulas->begin(); 
            iter != m_pHeadFormulas->end(); ) {
        _formula* headPart = iter->getFormula();
        _formula* cur = headPart;
        while (cur->formula_type != ATOM) {
            cur = cur->subformula_l;
        }
        
        bool flag = false;
        if (headPart->formula_type == NEGA || 
                (cur->predicate_id >= 0 && 
                        ! Vocabulary::instance()
                                .isIntensionPredicate(cur->predicate_id))) {
            _formula* newHeadPart = Utils::compositeByConnective(NEGA, 
                                        Utils::copyFormula(headPart), NULL);
            flag = true;
            iter = m_pHeadFormulas->erase(iter);
            m_pBodyFormulas->pushBack(Formula(newHeadPart, false));
        }               
        if (! flag) {
            ++ iter;
        }
    }
    
    //体部消除多余的非
    for (FORMULAS_ITERATOR iter = m_pBodyFormulas->begin(); 
            iter != m_pBodyFormulas->end(); ++ iter) {
        _formula* bodyPart = iter->getFormula();
        _formula* cur = bodyPart;
        
        while (cur->formula_type != ATOM) {
            cur = cur->subformula_l;
        }
        //外延谓词  ～～fml => fml
        if (! Vocabulary::instance().isIntensionPredicate(cur->predicate_id)
                && cur->predicate_id >=0 &&
                ! Vocabulary::instance().isSuccOrMax(cur->predicate_id)) {
            while (bodyPart->formula_type == NEGA && bodyPart->subformula_l->formula_type == NEGA) {
                bodyPart = bodyPart->subformula_l->subformula_l;
            }
            iter->setFormula(Utils::copyFormula(bodyPart));
        }
        //内涵谓词要 ~~~fml => ~fml
        else {
            while(bodyPart->formula_type == NEGA && bodyPart->subformula_l->formula_type == NEGA 
                        && bodyPart->subformula_l->subformula_l->formula_type == NEGA) {
                bodyPart = bodyPart->subformula_l->subformula_l;
            }
            iter->setFormula(Utils::copyFormula(bodyPart));
        }
    }
}