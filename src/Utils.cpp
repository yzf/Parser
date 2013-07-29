#include "Utils.h"
#include <assert.h>
#include <cstdlib>
#include <cstring>
/**
 * 输出term
 * @param _out
 * @param _t
 */
void Utils::outputTerm(FILE* _out, const _term* _t) {
    assert (_t);

    if (VARI==_t->term_type) {
        fprintf(_out, "%s", Vocabulary::instance().getNameById(_t->variable_id, VARIABLE));
        fflush(_out);
    }
    else {
        fprintf(_out, "%s", Vocabulary::instance().getNameById(_t->function_id, FUNCTION));
        int k = Vocabulary::instance().getFunctionArity(_t->function_id);
        if (k > 0) {
            fprintf ( _out, "(" );
            for (int i = 0; i < k; ++ i) {
                if (0 < i) {
                    printf(", ");
                }
                outputTerm(_out, _t->parameters+i);
            }
            fprintf(_out, ")");
        }
    }
}
/**
 * 替换参数
 * @param _t
 * @param _arity
 * @param _originals
 * @param _replacements
 */
void Utils::replaceTerm(_term* _ts, int _arity, const vector<int>& _originals, 
                        const vector<int>& _replacements) {
    for(int i = 0; i < _arity; i++) {
        //replacement
        if(_ts[i].term_type == VARI) {
            for(unsigned int j = 0; j < _originals.size(); j++) {
                if(_ts[i].variable_id == _originals[j]) {
                    _ts[i].variable_id = _replacements[j];
                }
            }
        }
        //traversal
        else if(_ts[i].term_type == FUNC) {
            replaceTerm(_ts[i].parameters, 
                        Vocabulary::instance().getFunctionArity(_ts[i].function_id), 
                        _originals, _replacements);
        }
    }
}
/**
 * 连接参数
 * @param _head
 * @param _tail
 * @return 
 */
_term* Utils::combineTerms(const vector<int>& _head, const vector<int>& _tail) {
    _term* terms = (_term*)malloc(sizeof(_term) * (_head.size() + _tail.size()));
    int index = 0;
    for(std::vector<int>::const_iterator it = _head.begin();
		it != _head.end(); it++, index++) {
        terms[index].variable_id = _head[index];
        terms[index].term_type = VARI;
    }
    index = 0;
    for(std::vector<int>::const_iterator it = _tail.begin(); 
		it != _tail.end(); it++, index++) {
        terms[index+_head.size()].variable_id = _tail[index];
        terms[index+_head.size()].term_type = VARI;
    }
    return terms;
}
/**
 * 比较两个term是否相同
 * @param _lhs
 * @param _rhs
 * @return 
 */
bool Utils::compareTerm(const _term* _lhs, const _term* _rhs) {
    assert(_lhs);
    assert(_rhs);

    if (_lhs->term_type != _rhs->term_type) {
        return false;
    }

    int k;
    switch (_lhs->term_type)
    {
    case VARI:
        return (_lhs->variable_id == _rhs->variable_id);
    case FUNC:
        if (_lhs->function_id == _rhs->function_id) {
            k = Vocabulary::instance().getFunctionArity(_lhs->function_id) - 1;
            assert(k < 0 || _lhs->parameters);
            assert(k < 0 || _rhs->parameters);
            for ( ; k >= 0; -- k) {
                if (! compareTerm(_lhs->parameters + k, _rhs->parameters + k)) {
                    return false;
                }
            }
            return true;
        }
    default:
        assert(0);
    }
    return false;
}
/**
 * 拷贝参数
 * @param _terms 待拷贝的参数
 * @param _size  参数个数
 * @return 拷贝结果
 */
_term* Utils::copyTerms(const _term* _ts, int _size) {
    _term* newTerms = (_term*)malloc(sizeof(_term)*_size);
    assert(newTerms);

    memcpy(newTerms, _ts, sizeof(_term)*_size);

    for(int i = 0; i < _size; ++ i) {
        if(_ts[i].term_type == FUNC) {
            newTerms[i].parameters = copyTerms(_ts[i].parameters,
			Vocabulary::instance().getFunctionArity(_ts[i].function_id));
        }
    }

    return newTerms;
}
/**
 * 销毁term指针
 * @param _ts term指针
 * @param _size 元素个数
 */
void Utils::deleteTerms(_term* _ts, int _size) {
    for(int i = 0; i < _size; ++ i) {
        if(_ts[i].term_type == FUNC) {
            deleteTerms(_ts[i].parameters,
                    Vocabulary::instance().getFunctionArity(_ts[i].function_id));
        }
    }
    free(_ts);
}
/**
 * 重命名参数
 * @param _t 
 * @param _oldVariableId
 * @param _newVariableId
 */
void Utils::renameTermVariables(_term* _t, int _oldVariableId, int _newVariableId) {
    int i;
    assert(_t);

    switch(_t->term_type)
    {
    case VARI:
        if (_t->variable_id == _oldVariableId) {
            _t->variable_id = _newVariableId;
        }
        break;
    case FUNC:
        assert(_t->parameters);
        for(i = 0; i < Vocabulary::instance().getFunctionArity(_t->function_id); ++ i) {
            renameTermVariables(_t->parameters+i, _oldVariableId, _newVariableId);
        }
        break;
    default:
        assert(0);
    }
}
/**
 * 输出公式
 * @param _out 文件标识
 * @param _fml 公式
 */
void Utils::outputFormula(FILE* _out, const _formula* _fml) {
    assert(_fml);
    
    char* s_conn = NULL;
    switch (_fml->formula_type) {
    case ATOM:
        if (_fml->predicate_id >= 0 && Vocabulary::instance().getPredicateArity(_fml->predicate_id) == 0) {
            fprintf(_out, "%s", Vocabulary::instance().getNameById(_fml->predicate_id, PREDICATE));
        }
        else{
            if (_fml->predicate_id >= 0) {
                fprintf(_out, "%s(", Vocabulary::instance().getNameById(_fml->predicate_id, PREDICATE));
                for(int i = 0; i < Vocabulary::instance().getPredicateArity(_fml->predicate_id); ++ i) {
                    if (i > 0) {
                        fprintf(_out, ",");
                    }
                    outputTerm(_out, _fml->parameters+i);
                }
            }
            else {
                switch (_fml->predicate_id) {
                case PRED_TRUE:
                    fprintf(_out, "(TRUE");
                    break;
                case PRED_FALSE:
                    fprintf(_out, "(FALSE");
                    break;
                case PRED_EQUALITY:
                    fprintf(_out, "(");
                    assert(_fml->parameters);
                    outputTerm(_out, _fml->parameters);
                    fprintf(_out, "=");
                    outputTerm(_out, _fml->parameters+1);
                    break;
                case PRED_MIN:
                    fprintf(_out, "MIN(");
                    assert(_fml->parameters);
                    outputTerm(_out, _fml->parameters);
                    break;
                case PRED_MAX:
                    fprintf(_out, "MAX(");
                    assert(_fml->parameters);
                    outputTerm(_out, _fml->parameters);
                    break;
                case PRED_SUCC:
                    fprintf(_out, "SUCC(");
                    assert(_fml->parameters);
                    outputTerm(_out, _fml->parameters);
                    fprintf(_out, ",");
                    outputTerm(_out, _fml->parameters+1);
                    break;
                default:
                    assert ( 0 );
                }
            }
            fprintf(_out, ")");
        }
        break;
    case NEGA:
        fprintf(_out, "~");
        assert(_fml->subformula_l);
        outputFormula(_out, _fml->subformula_l);
        break;
    case CONJ:
        s_conn = (char*)"&";
    case DISJ:
        if ( NULL==s_conn ) {
            s_conn = (char*)"|";
        }
    case IMPL:
        if ( NULL==s_conn ) {
            s_conn = (char*)"->";
        }
        fprintf(_out, "(");
        assert(_fml->subformula_l);
        outputFormula(_out, _fml->subformula_l);
        fprintf(_out, "%s", s_conn);
        assert(_fml->subformula_r);
        outputFormula(_out, _fml->subformula_r);
        fprintf(_out, ")");
        break;
    case UNIV:
        s_conn = (char*)"!";
    case EXIS:
        if ( NULL==s_conn ) {
            s_conn = (char*)"?";
        }
        fprintf(_out, "[%s%s](", s_conn, Vocabulary::instance().getNameById(_fml->variable_id, VARIABLE));
        assert(_fml->subformula_l);
        outputFormula(_out, _fml->subformula_l);
        fprintf ( _out, ")" );
        break;
    default:
        assert ( 0 );
    }
}
/**
 * 比较两条公式是否相同
 * @param _lhs
 * @param _rhs
 * @return 
 */
bool Utils::compareFormula(const _formula* _lhs, const _formula* _rhs) {
    assert(_lhs);
    assert(_rhs);

    if (_lhs->formula_type!=_rhs->formula_type) {
        return false;
    }
    int k;
    switch (_lhs->formula_type) {
    case ATOM:
        if (_lhs->predicate_id!=_rhs->predicate_id) {
            return false;
        }
        k = Vocabulary::instance().getPredicateArity(_lhs->predicate_id) - 1;
        assert(k < 0 || _lhs->parameters);
        assert(k < 0 || _rhs->parameters);
        for ( ; k >= 0; -- k) {
            if (! compareTerm(_lhs->parameters + k,_rhs->parameters + k)) {
                return FALSE;
            }
        }
        return true;
    case UNIV:
    case EXIS:
        if (_lhs->variable_id!=_rhs->variable_id) {
            return false;
        }
    case NEGA:
        assert(_lhs->subformula_l);
        assert(_rhs->subformula_l);
        return compareFormula(_lhs->subformula_l,_rhs->subformula_l);
    case CONJ:
    case DISJ:
    case IMPL:
        assert(_lhs->subformula_l);
        assert(_rhs->subformula_l);
        assert(_lhs->subformula_r);
        assert(_rhs->subformula_r);
        return (compareFormula(_lhs->subformula_l,_rhs->subformula_l)
            && compareFormula(_lhs->subformula_r,_rhs->subformula_r));
    default:
        assert(0);
    }
    return false;
}
/**
 * 拷贝公式
 * @param _fml
 * @return 
 */
_formula* Utils::copyFormula(const _formula* _fml) {
    if (_fml == NULL) {
        return NULL;
    }
	
    _formula* newFormula = (_formula*)malloc(sizeof(_formula));
    assert (newFormula);

    memcpy(newFormula, _fml, sizeof(_formula));
    switch (_fml->formula_type)
    {
    case ATOM:
        newFormula->parameters = copyTerms(_fml->parameters, 
                Vocabulary::instance().getPredicateArity(_fml->predicate_id));
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        assert(_fml->subformula_r);
        newFormula->subformula_r = copyFormula( _fml->subformula_r);
    case NEGA:
    case UNIV:
    case EXIS:
        assert(_fml->subformula_l);
        newFormula->subformula_l = copyFormula(_fml->subformula_l);
        break;
    default:
        assert (0);
    }

    return newFormula;
}
/**
 * 销毁公式
 * @param _fml
 */
void Utils::deleteFormula(_formula* _fml) {
    assert(_fml);

    switch (_fml->formula_type)
    {
    case ATOM:
        if(_fml->parameters) {
            deleteTerms(_fml->parameters, 
                    Vocabulary::instance().getPredicateArity(_fml->predicate_id));
        }
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        assert(_fml->subformula_r);
        deleteFormula(_fml->subformula_r);
    case NEGA:
    case UNIV:
    case EXIS:
        assert(_fml->subformula_l);
        deleteFormula(_fml->subformula_l);
        break;
    default:
        assert ( 0 );
    }

    free(_fml);
}
/**
 * 判断公式是否含有存在量词
 * @param _fml
 * @return 
 */
bool Utils::isUniversal(_formula* _fml) {
    if(_fml != NULL) {
        switch(_fml->formula_type) {
        case ATOM:
            return true;
        case CONJ:
        case DISJ:
        case IMPL:
            return isUniversal(_fml->subformula_l) && isUniversal(_fml->subformula_r);
        case NEGA:
        case UNIV:
            return isUniversal(_fml->subformula_l);
        case EXIS:
            return false;
        default:
            assert(0);
        }
    }

    return false;
}
/**
 * 在公式_fml中寻找_variableId，如存在，则返回_fml，不存在，则返回NULL
 * @param _fml
 * @param _variableId
 * @return 
 */
_formula* Utils::findPrenexQuanlifier(_formula* _fml, int _variableId) {
    if(_fml == NULL || (_fml->formula_type != UNIV && _fml->formula_type != EXIS)) {
        return NULL;
    }
    if(_fml->variable_id == _variableId) {
        return _fml;
    }
    else if(_fml->subformula_l->formula_type == UNIV || _fml->subformula_l->formula_type == EXIS) {
        return findPrenexQuanlifier(_fml->subformula_l, _variableId);
    }
    else {
        return NULL;
    }
}
/**
 * 删除多余的量词
 * @param _parent
 * @param _d 0表示左子树， 1表示右子树
 * @param _tag
 */
void Utils::removeFromPrenex(_formula* _parent, int _d, _formula* _tag) {
    _formula* sf;
    
    if (_d == 0) {
        sf = _parent->subformula_l;
    }
    else {
        sf = _parent->subformula_r;
    }
    
    if (sf == NULL || (sf->formula_type != UNIV && sf->formula_type != EXIS)) {
        return;
    }
    
    if (sf == _tag) {
        if(_d == 0) {
            _parent->subformula_l = sf->subformula_l;
        }
        else {
            _parent->subformula_r = sf->subformula_l;
        }
        free(sf);
        removeFromPrenex(sf, 0, _tag);
    }
    else {
        removeFromPrenex(sf, 0, _tag);
    }
}
/**
 * 把需要重命名的变量重命名
 * @param _fml 公式
 * @param _oldVariableId 需要重命名的变量id
 * @param _newVariableId 新变量id
 */
void Utils::renameFormulaVariables(_formula* _fml, int _oldVariableId, int _newVariableId) {
    assert(_fml);
	
    if(_oldVariableId == _newVariableId) {
        return;
    }

    switch (_fml->formula_type)
    {
    case ATOM:
        assert(_fml->parameters);
        for (int i = 0; i < Vocabulary::instance().getPredicateArity(_fml->predicate_id); ++ i) {
            renameTermVariables(_fml->parameters+i, _oldVariableId, _newVariableId);
        }
        break;
    case UNIV:
    case EXIS:
        if (_fml->variable_id==_oldVariableId) {
            _fml->variable_id = _newVariableId;
        }
    case NEGA:
        assert(_fml->subformula_l);
        renameFormulaVariables(_fml->subformula_l, _oldVariableId, _newVariableId);
        break;
    default:
        assert(_fml->subformula_l);
        assert(_fml->subformula_r);
        renameFormulaVariables(_fml->subformula_l, _oldVariableId, _newVariableId);
        renameFormulaVariables(_fml->subformula_r, _oldVariableId, _newVariableId);
    }
}
/**
 * 把公式转化成前束范式
 * @param _fml
 */
void Utils::convertToPrenex(_formula* _fml) {
    if(_fml == NULL) {
        return;
    }
    
    switch(_fml->formula_type) {
    case ATOM:
        break;
    case NEGA:
        convertToPrenex(_fml->subformula_l);
        while(_fml->subformula_l->formula_type == EXIS || _fml->subformula_l->formula_type == UNIV) {
            FORMULA_TYPE flt = (_fml->subformula_l->formula_type == UNIV) ? EXIS : UNIV;
            _fml->formula_type = flt;
            _fml->variable_id = _fml->subformula_l->variable_id;
            _fml->subformula_l->formula_type = NEGA;
            _fml = _fml->subformula_l;
        }
        break;
    case IMPL:
    case CONJ:
    case DISJ: {
        convertToPrenex(_fml->subformula_l);
        convertToPrenex(_fml->subformula_r);

        _formula* sub_l = _fml->subformula_l;
        _formula* sub_r = _fml->subformula_r;
        _formula* curr_fml = _fml;                      

        while(sub_l->formula_type == UNIV || sub_l->formula_type == EXIS || 
                sub_r->formula_type == UNIV || sub_r->formula_type == EXIS) {

            if(curr_fml->formula_type == IMPL && (sub_l->formula_type == EXIS 
                    || sub_l->formula_type == UNIV)) {
                sub_l->formula_type = (sub_l->formula_type == UNIV) ? EXIS : UNIV;
            }
            FORMULA_TYPE priority_type = UNIV;

            if(sub_l->formula_type != priority_type && sub_r->formula_type != priority_type) {
                priority_type = EXIS;
            }
            if(sub_l->formula_type == priority_type || sub_l->formula_type == sub_r->formula_type) {
                _formula* same_variable = findPrenexQuanlifier(sub_r, sub_l->variable_id);

                if(same_variable != NULL) {
                    if((sub_l->formula_type == UNIV && curr_fml->formula_type == CONJ && same_variable->formula_type == UNIV)||
                            (sub_l->formula_type == EXIS && curr_fml->formula_type == DISJ && same_variable->formula_type == EXIS)) {
                        removeFromPrenex(curr_fml, 1, same_variable);
                    }
                    else {
                        int new_id = Vocabulary::instance().addRenameVariable();
                        renameFormulaVariables(sub_r, sub_l->variable_id, new_id);
                    }
                }

                FORMULA_TYPE temp = curr_fml->formula_type;
                sub_r = curr_fml->subformula_r;
                curr_fml->formula_type = sub_l->formula_type;
                curr_fml->variable_id = sub_l->variable_id;
                sub_l->formula_type = temp;
                sub_l->subformula_r = sub_r;
            }
            else {
                _formula* same_variable = findPrenexQuanlifier(sub_l, sub_r->variable_id);

                if(same_variable != NULL) {
                    if((sub_r->formula_type == UNIV && curr_fml->formula_type == CONJ && same_variable->formula_type == UNIV)||
                            (sub_r->formula_type == EXIS && curr_fml->formula_type == DISJ && same_variable->formula_type == EXIS)) {
                        removeFromPrenex(curr_fml, 0, same_variable);
                    }
                    else {
                        int new_id = Vocabulary::instance().addRenameVariable();
                        renameFormulaVariables(sub_r, sub_l->variable_id, new_id);
                    }
                }

                FORMULA_TYPE temp = curr_fml->formula_type;
                sub_l = curr_fml->subformula_l;
                _formula* r_sub_l = sub_r->subformula_l;
                curr_fml->formula_type = sub_r->formula_type;
                curr_fml->variable_id = sub_r->variable_id;
                curr_fml->subformula_l = sub_r;
                sub_r->formula_type = temp;
                sub_r->subformula_l = sub_l;
                sub_r->subformula_r = r_sub_l;
            }
            curr_fml = curr_fml->subformula_l;
            sub_l = curr_fml->subformula_l;
            sub_r = curr_fml->subformula_r;
        }
    }
        break;
    case UNIV:
    case EXIS:
        convertToPrenex(_fml->subformula_l);
        break;
    default:
        break;           
    }
}
/**
 * 获取公式中没有量词限定的变量
 * @param _flag 标记
 * @param _varis 存放结果
 * @param _fml 公式
 */
void Utils::getNoQuantifierVariables(map<int, bool>& _flag, vector<int>& _varis, _formula* _fml) {
    assert(_fml);
    switch (_fml->formula_type) {
    case NEGA:
        break;
    case UNIV:
    case EXIS:
        _flag[_fml->variable_id] = true;
        getNoQuantifierVariables(_flag, _varis, _fml->subformula_l);
        break;
    case IMPL:
    case CONJ:
    case DISJ:
        getNoQuantifierVariables(_flag, _varis, _fml->subformula_l);
        getNoQuantifierVariables(_flag, _varis, _fml->subformula_r);
        break;
    case ATOM:
        for(int i = 0; i < Vocabulary::instance().getPredicateArity(_fml->predicate_id); ++ i) {
            _term* term = _fml->parameters + i;
            if (! _flag[term->variable_id]) {
                _flag[term->variable_id] = true;
                _varis.push_back(-(term->variable_id));
            }
        }
        break;
    default:
        assert(0);
    }
}
bool Utils::isNegativeFormula(_formula* _fml, bool _negative) {
    assert(_fml);

    switch (_fml->formula_type)
    {
    case ATOM:
        if (_negative || ! Vocabulary::instance().isIntensionPredicate(_fml->predicate_id ))
            return true;
        break;
    case NEGA:
        assert(_fml->subformula_l);
        return isNegativeFormula(_fml->subformula_l, !_negative);
    case CONJ:
    case DISJ:
        assert(_fml->subformula_l);
        assert(_fml->subformula_r);
        return (isNegativeFormula(_fml->subformula_l, _negative) &&
               isNegativeFormula(_fml->subformula_r, _negative));
    case IMPL:
        assert(_fml->subformula_l);
        assert(_fml->subformula_r);
        return (isNegativeFormula(_fml->subformula_l, !_negative) &&
               isNegativeFormula(_fml->subformula_r, _negative ));
    case UNIV:
    case EXIS:
        assert(_fml->subformula_l);
        return isNegativeFormula(_fml->subformula_l, _negative);
    default:
        assert(0);
    }

    return false;
}
/**
 * 在公式所有不带非的内涵谓词前添加非非
 * @param _fml
 * @return 
 */
_formula* Utils::doubleNegationIntensionPredicates(_formula* _fml) {
    assert(_fml);

    if (isNegativeFormula(_fml, false)) {
        return _fml;
    }

    switch (_fml->formula_type)
    {
    case ATOM:
        if(Vocabulary::instance().isIntensionPredicate(_fml->predicate_id)) {
            _fml = compositeByConnective(NEGA, _fml, NULL);
            _fml = compositeByConnective(NEGA, _fml, NULL);
        }
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        _fml->subformula_r = doubleNegationIntensionPredicates(_fml->subformula_r);
    case NEGA:
    case UNIV:
    case EXIS:
        _fml->subformula_l = doubleNegationIntensionPredicates(_fml->subformula_l);
        break;
    default:
        assert(0);
    }

    return _fml;
}
/**
 * 把公式中的参数_originals替换成_replacements，一一对应
 * @param _fml
 * @param _originals
 * @param _replacements
 * @return 
 */
void Utils::replaceFormulaTerms(_formula* _fml, 
                                const vector<int>& _originals, 
				const vector<int>& _replacements) {
    assert(_fml);
    assert(_originals.size() == _replacements.size());

    switch(_fml->formula_type) {
    case ATOM:
        replaceTerm(_fml->parameters, 
                Vocabulary::instance().getPredicateArity(_fml->predicate_id), 
                _originals, _replacements);
        break;
    case DISJ:
    case CONJ:
    case IMPL:
        replaceFormulaTerms(_fml->subformula_r, _originals, _replacements);
    case NEGA:
    case UNIV:
    case EXIS:
        replaceFormulaTerms(_fml->subformula_l, _originals, _replacements);
        break;
    default:
        assert(0);
    }
}
/**
 * 拆分公式
 * @param _fml
 * @param _parent
 * @param _result
 */
void Utils::divideFormula(_formula* _fml, _formula* _parent, Formulas* _result) {
    if(_fml != NULL) {
        if((_parent == NULL || _parent->formula_type == CONJ) && _fml->formula_type == CONJ) {
            Formula new_formula = Formula(_fml->subformula_r, true);  
            _result->pushBack(new_formula);
            divideFormula(_fml->subformula_l, _fml, _result);   
        }
        else {
            Formula new_formula = Formula(_fml, true);
            _result->pushBack(new_formula);
        }
        
    }
}
/**
 * ~TRUE => FALSE
 * @param _originalFml
 * @return 
 */
_formula* Utils::negativeNormalForm_1(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == NEGA);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->predicate_id == PRED_TRUE);
    _formula* ret = compositeToAtom(PRED_FALSE, NULL);
    deleteFormula(_originalFml);
    return ret;
}
/**
 * ~FALSE => TRUE
 * @param _originalFml
 * @return 
 */
_formula* Utils::negativeNormalForm_2(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == NEGA);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->predicate_id == PRED_FALSE);
    _formula* ret = compositeToAtom(PRED_TRUE, NULL);
    deleteFormula(_originalFml);
    return ret;
}
/**
 * ~~~fml => ~fml
 * @param _originalFml
 * @return 
 */
_formula* Utils::negativeNormalForm_3(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == NEGA);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == NEGA);
    assert(_originalFml->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_l->formula_type == NEGA);
    assert(_originalFml->subformula_l->subformula_l->subformula_l);
    _formula* ret = convertToNegativeNormalForm(_originalFml->subformula_l->subformula_l);
    free(_originalFml->subformula_l);
    free(_originalFml);
    return ret;
}
/**
 * ~(F ^ G) => ~F v ~G
 * @param _originalFml
 * @return 
 */
_formula* Utils::negativeNormalForm_4(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == NEGA);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == CONJ);
    assert(_originalFml->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_r);
    _formula* f = _originalFml->subformula_l->subformula_l;
    _formula* g = _originalFml->subformula_l->subformula_r;
    _formula* _f = convertToNegativeNormalForm(compositeByConnective(NEGA, f, NULL));
    _formula* _g = convertToNegativeNormalForm(compositeByConnective(NEGA, g, NULL));
    _formula* ret = compositeByConnective(DISJ, _f, _g);
    free(_originalFml->subformula_l);// ^
    free(_originalFml);// ~
    return ret;
}
/**
 * ~(F v G) => ~F ^ ~G
 * @param _originalFml
 * @return 
 */
_formula* Utils::negativeNormalForm_5(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == NEGA);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == DISJ);
    assert(_originalFml->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_r);
    _formula* f = _originalFml->subformula_l->subformula_l;
    _formula* g = _originalFml->subformula_l->subformula_r;
    _formula* _f = convertToNegativeNormalForm(compositeByConnective(NEGA, f, NULL));
    _formula* _g = convertToNegativeNormalForm(compositeByConnective(NEGA, g, NULL));
    _formula* ret = compositeByConnective(CONJ, _f, _g);
    free(_originalFml->subformula_l);// v
    free(_originalFml);// ~
    return ret;
}
/**
 * ~(F -> G) => ~~F ^ ~G
 * @param _originalFml
 * @return 
 */
_formula* Utils::negativeNormalForm_6(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == NEGA);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == IMPL);
    assert(_originalFml->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_r);
    _formula* f = _originalFml->subformula_l->subformula_l;
    _formula* g = _originalFml->subformula_l->subformula_r;
    _formula* _f = convertToNegativeNormalForm(compositeByConnective(NEGA, f, NULL));
    _formula* __f = convertToNegativeNormalForm(compositeByConnective(NEGA, _f, NULL));
    _formula* _g = convertToNegativeNormalForm(compositeByConnective(NEGA, g, NULL));
    _formula* ret = compositeByConnective(CONJ, __f, _g);
    free(_originalFml->subformula_l);// ->
    free(_originalFml);// ~
    return ret;
}
/**
 * 把公式转化成否定标准式
 * @param _originalFml
 * @return 
 */
_formula* Utils::convertToNegativeNormalForm(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == ATOM || _originalFml->formula_type == NEGA ||
               _originalFml->formula_type == DISJ || _originalFml->formula_type == CONJ ||
               _originalFml->formula_type == IMPL || _originalFml->formula_type == UNIV);

    //Cabalar. (2005)
    if(_originalFml->formula_type == ATOM) {
        //do nothing
    }
	
    else if(_originalFml->formula_type == NEGA) {
        _formula* subformula_l = _originalFml->subformula_l;

        if(subformula_l->formula_type == ATOM) {
            //N1
            if(subformula_l->predicate_id == PRED_TRUE) {
                return negativeNormalForm_1(_originalFml);
            }
            //N2
            if(subformula_l->predicate_id == PRED_FALSE) {
                return negativeNormalForm_2(_originalFml);
            }
        }
        
        if(subformula_l->formula_type == NEGA) {
            //N3
            if(subformula_l->subformula_l->formula_type == NEGA) {
                return negativeNormalForm_3(_originalFml);
            }
        }
        //N4
        if(subformula_l->formula_type == CONJ) {
            return negativeNormalForm_4(_originalFml);
        }
        //N5
        if(subformula_l->formula_type == DISJ) {
            return negativeNormalForm_5(_originalFml);
        }
        //N6
        if(subformula_l->formula_type == IMPL) {
            return negativeNormalForm_6(_originalFml);
        }
    }
    
    else if (_originalFml->formula_type == UNIV) {
        _originalFml->subformula_l = convertToNegativeNormalForm(_originalFml->subformula_l);
    }
    
    else {
        _originalFml->subformula_l = convertToNegativeNormalForm(_originalFml->subformula_l);
	_originalFml->subformula_r = convertToNegativeNormalForm(_originalFml->subformula_r);
    }
    
    return _originalFml;
}
/**
 * 使用联接词(非、析取、合取、蕴含)生成公式
 * @param _formulaType 联接词
 * @param _subformulaL 左公式
 * @param _subformulaR 右公式
 * @return 新公式
 */
_formula* Utils::compositeByConnective(FORMULA_TYPE _formulaType, 
                    _formula* _subformulaL, _formula* _subformulaR) {
    assert(_formulaType == NEGA || _formulaType == DISJ ||
		   _formulaType == CONJ || _formulaType == IMPL);

    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = _formulaType;
    fml->subformula_l = _subformulaL;
    fml->subformula_r = _subformulaR;
    return fml;
}
/**
 * 在公式前添加量词
 * @param _formulaType 量词类型
 * @param _subformulaL 公式
 * @param _variableId  量词对应的变量Id
 * @return 新公式
 */
_formula* Utils::compositeByQuantifier(FORMULA_TYPE _formulaType, 
                         _formula* _subformulaL, int _variableId) {
    assert(_formulaType == EXIS || _formulaType == UNIV);
    
    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = _formulaType;
    fml->subformula_l = _subformulaL;
    fml->variable_id = _variableId;
    return fml;
}
/**
 * 生成atom
 * @param _formulaType
 * @param _predicateId
 * @param _parameters
 * @return 
 */
_formula* Utils::compositeToAtom(int _predicateId, _term* _parameters) {
    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = ATOM;
    fml->predicate_id = _predicateId;
    fml->parameters   = _parameters;
    return fml;
}