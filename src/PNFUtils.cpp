#include "PNFUtils.h"
#include <cstdio>
#include <assert.h>
#include "Utils.h"
#include <cstdlib>
/**
 * 调用该函数前，请确保_fml已经是前束范式，并且所有变量都有量词限定
 * 因为该函数只查询前面量词部分
 * @param _fml
 * @param _nVariableId　变量Id
 * @return 
 */
bool PNFUtils::isVariableInFormula(_formula* _fml, int _nVariableId) {
    if (_fml == NULL || 
            (_fml->formula_type != UNIV && _fml->formula_type != EXIS)) {
        return false;
    }
    if (_fml->variable_id == _nVariableId) {
        return true;
    }
    return isVariableInFormula(_fml->subformula_l, _nVariableId);
}
/**
 * NEGA的量词提取操作
 * @param _fml
 */
void PNFUtils::extractQuantifier_NEGA(_formula* _fml) {
    assert(_fml);
    assert(_fml->formula_type == NEGA);
    assert(_fml->subformula_l);
    // ~[?X]p(X) => [!X]~p(X)
    // ~[!X]p(X) => [?X]~p(X)
    while (_fml->subformula_l->formula_type == EXIS || _fml->subformula_l->formula_type == UNIV) {
        FORMULA_TYPE flt = (_fml->subformula_l->formula_type == UNIV) ? EXIS : UNIV;
        _fml->formula_type = flt;
        _fml->variable_id = _fml->subformula_l->variable_id;
        _fml->subformula_l->formula_type = NEGA;
        _fml = _fml->subformula_l;
    }
}
/**
 * IMPL,CONJ,DISJ的提取操作
 * @param _fml
 */
void PNFUtils::extractQuantifier_IMPL_CONJ_DISJ(_formula* _fml) {
    assert(_fml);
    assert(_fml->formula_type == IMPL || 
            _fml->formula_type == CONJ ||
            _fml->formula_type == DISJ);
    assert(_fml->subformula_l);
    assert(_fml->subformula_r);
    
    _formula* curr_fml = _fml; 
    _formula* sub_l = curr_fml->subformula_l;
    _formula* sub_r = curr_fml->subformula_r;  

    while (sub_l->formula_type == UNIV || sub_l->formula_type == EXIS || 
            sub_r->formula_type == UNIV || sub_r->formula_type == EXIS) {
        // 蕴含体部的量词提前的预处理：存在=>全称    全称=>存在
        // [!X]p(X) -> q => [?X](p(X)->q)
        // [?X]p(X) -> q => [!X](p(X)->q)
        if (curr_fml->formula_type == IMPL && (sub_l->formula_type == EXIS 
                || sub_l->formula_type == UNIV)) {
            sub_l->formula_type = (sub_l->formula_type == UNIV) ? EXIS : UNIV;
        }

        FORMULA_TYPE priority_type = UNIV;
        if (sub_l->formula_type != priority_type && sub_r->formula_type != priority_type) {
            priority_type = EXIS;
        }
        // [!X]p(X) & q  =>  [!X](p(X) & q)
        // [?X]p(X) & q  =>  [?X](p(X) & q)
        // [!X]p(X) | q  =>  [!X](p(X) | q)
        // [?X]p(X) | q  =>  [?X](p(X) | q)
        // q -> [!X]p(X) =>  [!X](q -> p(X))
        // q -> [?X]p(X) =>  [?X](q -> p(X))
        // 其中 q 不带变元X．
        // 提取左子公式的量词
        if (sub_l->formula_type == priority_type || sub_l->formula_type == sub_r->formula_type) {
            // 特殊情况：
            // [!X]p(X) & [!X]q(X) => [!X](p(X)&q(X))
            // [?X]p(X) | [!X]q(X) => [?X](p(X)|q(X))
            if (isVariableInFormula(sub_r, sub_l->variable_id)) {
                if (sub_l->variable_id == sub_r->variable_id &&
                        ((sub_l->formula_type == UNIV && sub_r->formula_type == UNIV && curr_fml->formula_type == CONJ) ||
                        (sub_l->formula_type == EXIS && sub_r->formula_type == EXIS && curr_fml->formula_type == DISJ))) {
                    // 删除右子公式的量词限定
                    _formula* preSub_r = sub_r;
                    sub_r = sub_r->subformula_l;
                    free(preSub_r);
                }
                else {
                    int new_id = Vocabulary::instance().addRenameVariable();
                    Utils::renameFormulaVariables(sub_r, sub_l->variable_id, new_id);
                }
            }

            FORMULA_TYPE temp = curr_fml->formula_type;
            curr_fml->formula_type = sub_l->formula_type;
            curr_fml->variable_id = sub_l->variable_id;
            sub_l->formula_type = temp;
            sub_l->subformula_r = sub_r;
        }
        // 相关公式与提取左子公式相似
        // 提取右子公式的量词
        else {
            // 特殊情况：
            // [!X]p(X) & [!X]q(X) => [!X](p(X)&q(X))
            // [?X]p(X) | [!X]q(X) => [?X](p(X)|q(X))
            if (isVariableInFormula(sub_l, sub_r->variable_id)) {
                if(sub_l->variable_id == sub_r->variable_id &&
                        ((sub_l->formula_type == UNIV && sub_r->formula_type == UNIV && curr_fml->formula_type == CONJ) ||
                        (sub_l->formula_type == EXIS && sub_r->formula_type == EXIS && curr_fml->formula_type == DISJ))) {
                    // 删除左子公式的量词限定
                    _formula* preSub_l = sub_l;
                    sub_l = sub_l->subformula_l;
                    free(preSub_l);
                }
                else {
                    int new_id = Vocabulary::instance().addRenameVariable();
                    Utils::renameFormulaVariables(sub_r, sub_l->variable_id, new_id);
                }
            }

            FORMULA_TYPE temp = curr_fml->formula_type;
            curr_fml->formula_type = sub_r->formula_type;
            curr_fml->variable_id = sub_r->variable_id;
            curr_fml->subformula_l = sub_r;
            sub_r->formula_type = temp;
            sub_r->subformula_r = sub_r->subformula_l;
            sub_r->subformula_l = sub_l;
        }
        curr_fml = curr_fml->subformula_l;
        sub_l = curr_fml->subformula_l;
        sub_r = curr_fml->subformula_r;
    }
}
/**
 * 把公式_fml转化成前束范式
 * @param _fml
 */
void PNFUtils::convertToPrenex(_formula* _fml) {
    if(_fml == NULL) {
        return;
    }
    
    switch(_fml->formula_type) {
    case ATOM:
        break;
    case NEGA:
        convertToPrenex(_fml->subformula_l);
        extractQuantifier_NEGA(_fml);
        break;
    case IMPL:
    case CONJ:
    case DISJ: {
        convertToPrenex(_fml->subformula_l);
        convertToPrenex(_fml->subformula_r);
        extractQuantifier_IMPL_CONJ_DISJ(_fml);
    }
        break;
    case UNIV:
    case EXIS:
        convertToPrenex(_fml->subformula_l);
        break;
    default:
        assert(0);
        break;           
    }
}
