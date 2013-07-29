#include "CabalarUtils.h"
#include "Utils.h"
#include <cstdlib>
#include <assert.h>
/**
 * TRUE | fml => TRUE
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::disjLeftTrue(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == DISJ);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == ATOM);
    assert(_originalFml->subformula_l->predicate_id == PRED_TRUE);
    assert(_originalFml->subformula_r);
    _formula* ret = _originalFml->subformula_l;
    Utils::deleteFormula(_originalFml->subformula_r);
    free(_originalFml);
    return ret;
}
/**
 * FALSE | fml => fml
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::disjLeftFalse(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == DISJ);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == ATOM);
    assert(_originalFml->subformula_l->predicate_id == PRED_FALSE);
    assert(_originalFml->subformula_r);
    _formula* ret = _originalFml->subformula_r;
    Utils::deleteFormula(_originalFml->subformula_l);
    free(_originalFml);
    return ret;
}
/**
 * TRUE & fml => fml
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::conjLeftTrue(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == CONJ);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == ATOM);
    assert(_originalFml->subformula_l->predicate_id == PRED_TRUE);
    assert(_originalFml->subformula_r);
    _formula* ret = _originalFml->subformula_r;
    Utils::deleteFormula(_originalFml->subformula_l);
    free(_originalFml);
    return ret;
}
/**
 * FALSE & fml => FALSE
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::conjLeftFalse(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == CONJ);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == ATOM);
    assert(_originalFml->subformula_l->predicate_id == PRED_FALSE);
    assert(_originalFml->subformula_r);
    _formula* ret = _originalFml->subformula_l;
    Utils::deleteFormula(_originalFml->subformula_r);
    free(_originalFml);
    return ret;
}
/**
 * TRUE -> fml  =>  fml
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::implLeftTrue(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == ATOM);
    assert(_originalFml->subformula_l->predicate_id == PRED_TRUE);
    assert(_originalFml->subformula_r);
    _formula* ret = _originalFml->subformula_r;
    Utils::deleteFormula(_originalFml->subformula_l);
    free(_originalFml);
    return ret;
}
/**
 * FALSE -> fml  =>  TRUE
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::implLeftFalse(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == ATOM);
    assert(_originalFml->subformula_l->predicate_id == PRED_FALSE);
    assert(_originalFml->subformula_r);
    _formula* ret = Utils::compositeToAtom(PRED_TRUE, NULL);
    Utils::deleteFormula(_originalFml);
    return ret;
}
/**
 * fml -> TRUE => TRUE
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::implRightTrue(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_r);
    assert(_originalFml->subformula_r->formula_type == ATOM);
    assert(_originalFml->subformula_r->predicate_id == PRED_TRUE);
    _formula* ret = Utils::compositeToAtom(PRED_TRUE, NULL);
    Utils::deleteFormula(_originalFml);
    return ret;
}
/**
 * fml -> FALSE => ~fml
 * @param _originalFml
 * @return 
 */
_formula* CabalarUtils::implRightFalse(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_r);
    assert(_originalFml->subformula_r->formula_type == ATOM);
    assert(_originalFml->subformula_r->predicate_id == PRED_FALSE);
    _formula* ret = Utils::compositeByConnective(NEGA, _originalFml->subformula_l, NULL);
    ret = Utils::convertToNegativeNormalForm(ret);
    Utils::deleteFormula(_originalFml->subformula_r);
    free(_originalFml);
    return ret;
}
/**
 * TRUE ^ F -> G  =>  F->G
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::leftRule_1(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == CONJ);
    assert(_originalFml->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_l->predicate_id == PRED_TRUE);
    assert(_originalFml->subformula_l->subformula_r);
    
    _formula* f = _originalFml->subformula_l->subformula_r;
    _formula* g = _originalFml->subformula_r;
    _formula* fg = Utils::compositeByConnective(IMPL, f, g);
    
    Utils::deleteFormula(_originalFml->subformula_l->subformula_l);//TRUE
    free(_originalFml->subformula_l);//^
    free(_originalFml);// ->
    
    Formulas ret;
    ret.pushBack(Formula(fg, false));
    return ret;
}
/**
 * FALSE ^ F -> G => {NULL}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::leftRule_2(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == CONJ);
    assert(_originalFml->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_l->predicate_id == PRED_FALSE);
    assert(_originalFml->subformula_l->subformula_r);
    
    Utils::deleteFormula(_originalFml);//_originalFml
    
    Formulas ret;
    return ret;
}
/**
 * --F ^ G -> H ----> {G -> -F v H}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::leftRule_3(_formula* _originalFml) {
    assert(_originalFml);
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);
    assert(_originalFml->subformula_l->formula_type == CONJ);
    assert(_originalFml->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_l->formula_type == NEGA);
    assert(_originalFml->subformula_l->subformula_l->subformula_l);
    assert(_originalFml->subformula_l->subformula_l->subformula_l->formula_type == NEGA);
    assert(_originalFml->subformula_l->subformula_l->subformula_l->subformula_l);//f
    assert(_originalFml->subformula_l->subformula_r);//g
    assert(_originalFml->subformula_r);//h
    
    _formula* _f = _originalFml->subformula_l->subformula_l->subformula_l;
    _formula* g = _originalFml->subformula_l->subformula_r;
    _formula* h = _originalFml->subformula_r;
    _formula* _fh = Utils::compositeByConnective(DISJ, _f, h);
    _formula* g_fh = Utils::compositeByConnective(IMPL, g, _fh);
    
    free(_originalFml->subformula_l->subformula_l);// ~
    free(_originalFml->subformula_l); // ^
    free(_originalFml);// ->
    
    Formulas ret;
    ret.pushBack(Formula(g_fh, false));
    return ret;
}
/**
 * (F v G) ^ H -> K ----> { F ^ H -> K
 *                          G ^ H -> K}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::leftRule_4(_formula* _originalFml) {
    assert(_originalFml);//->
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);//^
    assert(_originalFml->subformula_l->formula_type == CONJ);
    assert(_originalFml->subformula_l->subformula_l);//v
    assert(_originalFml->subformula_l->subformula_l->formula_type == DISJ);
    assert(_originalFml->subformula_l->subformula_l->subformula_l);//f
    assert(_originalFml->subformula_l->subformula_l->subformula_r);//g
    assert(_originalFml->subformula_l->subformula_r);//h
    assert(_originalFml->subformula_r);//k
    
    _formula* f = _originalFml->subformula_l->subformula_l->subformula_l;
    _formula* g = _originalFml->subformula_l->subformula_l->subformula_r;
    _formula* h = _originalFml->subformula_l->subformula_r;
    _formula* h_2 = Utils::copyFormula(h);
    _formula* k = _originalFml->subformula_r;
    _formula* k_2 = Utils::copyFormula(k);
    _formula* fml_1 = Utils::compositeByConnective(IMPL, Utils::compositeByConnective(CONJ, f, h), k);
    _formula* fml_2 = Utils::compositeByConnective(IMPL, Utils::compositeByConnective(CONJ, g, h_2), k_2);
    
    free(_originalFml->subformula_l->subformula_l);
    free(_originalFml->subformula_l);
    free(_originalFml);
    
    Formulas ret;
    ret.pushBack(Formula(fml_1, false));
    ret.pushBack(Formula(fml_2, false));
    return ret;
}
/**
 * (F -> G) ^ H -> K  ----> { -F ^ H -> K
//                             G ^ H -> K
//                             H -> F v -G v K}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::leftRule_5(_formula* _originalFml) {
    assert(_originalFml);//->
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);//^
    assert(_originalFml->subformula_l->formula_type == CONJ);
    assert(_originalFml->subformula_l->subformula_l);//->
    assert(_originalFml->subformula_l->subformula_l->formula_type == IMPL);
    assert(_originalFml->subformula_l->subformula_l->subformula_l);//f
    assert(_originalFml->subformula_l->subformula_l->subformula_r);//g
    assert(_originalFml->subformula_l->subformula_r);//h
    assert(_originalFml->subformula_r);//k
    
    _formula* f = _originalFml->subformula_l->subformula_l->subformula_l;
    _formula* _f = Utils::convertToNegativeNormalForm(
                        Utils::compositeByConnective(NEGA, 
                            Utils::copyFormula(f), NULL));
    _formula* g = _originalFml->subformula_l->subformula_l->subformula_r;
    _formula* _g = Utils::convertToNegativeNormalForm(
                        Utils::compositeByConnective(NEGA, 
                            Utils::copyFormula(g), NULL));
    _formula* h = _originalFml->subformula_l->subformula_r;
    _formula* h_2 = Utils::copyFormula(h);
    _formula* h_3 = Utils::copyFormula(h);
    _formula* k = _originalFml->subformula_r;
    _formula* k_2 = Utils::copyFormula(k);
    _formula* k_3 = Utils::copyFormula(k);
    // -F ^ H -> K
    _formula* fml_1 = Utils::compositeByConnective(IMPL, 
                        Utils::compositeByConnective(CONJ, _f, h), k);
    //G ^ H -> K
    _formula* fml_2 = Utils::compositeByConnective(IMPL, 
                        Utils::compositeByConnective(CONJ, g, h_2), k_2);
    //H -> F v -G v K
    _formula* fml_3 = Utils::compositeByConnective(IMPL, h_3, 
                        Utils::compositeByConnective(DISJ, 
                                Utils::compositeByConnective(DISJ, f, _g), k_3));
    
    free(_originalFml->subformula_l->subformula_l);
    free(_originalFml->subformula_l);
    free(_originalFml);
    
    Formulas ret;
    ret.pushBack(Formula(fml_1, false));
    ret.pushBack(Formula(fml_2, false));
    ret.pushBack(Formula(fml_3, false));
    return ret;
}
/**
 * F-> FALSE v G  => {F->G}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::rightRule_1(_formula* _originalFml) {
    assert(_originalFml);//->
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_r);//v
    assert(_originalFml->subformula_r->formula_type == DISJ);
    assert(_originalFml->subformula_l);//f
    assert(_originalFml->subformula_r->subformula_l);//FALSE
    assert(_originalFml->subformula_r->subformula_l->predicate_id == PRED_FALSE);
    assert(_originalFml->subformula_r->subformula_r);//g
    
    _formula* f = _originalFml->subformula_l;
    _formula* g = _originalFml->subformula_r->subformula_r;
    _formula* fml = Utils::compositeByConnective(IMPL, f, g);
    
    Utils::deleteFormula(_originalFml->subformula_r->subformula_l);//FALSE
    free(_originalFml->subformula_r);//v
    free(_originalFml);
    
    Formulas ret;
    ret.pushBack(Formula(fml, false));
    return ret;
}
/**
 * F-> TRUE v G  => {}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::rightRule_2(_formula* _originalFml) {
    assert(_originalFml);//->
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_r);//v
    assert(_originalFml->subformula_r->formula_type == DISJ);
    assert(_originalFml->subformula_l);//f
    assert(_originalFml->subformula_r->subformula_l);//TRUE
    assert(_originalFml->subformula_r->subformula_l->predicate_id == PRED_TRUE);
    assert(_originalFml->subformula_r->subformula_r);//g
    
    Utils::deleteFormula(_originalFml);
    
    Formulas ret;
    return ret;
}
/**
 * F-> ~~G v H => {~G ^ F -> H}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::rightRule_3(_formula* _originalFml) {
    assert(_originalFml);//->
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_r);//v
    assert(_originalFml->subformula_r->formula_type == DISJ);
    assert(_originalFml->subformula_l);//f
    assert(_originalFml->subformula_r->subformula_l);//~
    assert(_originalFml->subformula_r->subformula_l->formula_type == NEGA);
    assert(_originalFml->subformula_r->subformula_l->subformula_l);//~
    assert(_originalFml->subformula_r->subformula_l->subformula_l->formula_type == NEGA);
    assert(_originalFml->subformula_r->subformula_l->subformula_l->subformula_l);//g
    assert(_originalFml->subformula_r->subformula_r);//H
    
    _formula* _g = _originalFml->subformula_r->subformula_l->subformula_l;
    _formula* f = _originalFml->subformula_l;
    _formula* h = _originalFml->subformula_r->subformula_r;
    _formula* fml = Utils::compositeByConnective(IMPL, 
                        Utils::compositeByConnective(CONJ, _g, f), h);
    
    free(_originalFml->subformula_r->subformula_l);//~
    free(_originalFml->subformula_r);//v
    free(_originalFml);//->

    Formulas ret;
    ret.pushBack(Formula(fml, false));
    return ret;
}
/**
 * F-> (G ^ H) v K  => {F -> G v K
 *                      F -> H v K}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::rightRule_4(_formula* _originalFml) {
    assert(_originalFml);//->
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);//f
    assert(_originalFml->subformula_r);//v
    assert(_originalFml->subformula_r->formula_type == DISJ);
    assert(_originalFml->subformula_r->subformula_l);//^
    assert(_originalFml->subformula_r->subformula_l->formula_type == CONJ);
    assert(_originalFml->subformula_r->subformula_l->subformula_l);//g
    assert(_originalFml->subformula_r->subformula_l->subformula_r);//h
    assert(_originalFml->subformula_r->subformula_r);//k
    
    _formula* f_1 = _originalFml->subformula_l;
    _formula* f_2 = Utils::copyFormula(f_1);
    _formula* g = _originalFml->subformula_r->subformula_l->subformula_l;
    _formula* k_1 = _originalFml->subformula_r->subformula_r;
    _formula* k_2 = Utils::copyFormula(k_1);
    _formula* h = _originalFml->subformula_r->subformula_l->subformula_r;
    _formula* fml_1 = Utils::compositeByConnective(IMPL, f_1,
                        Utils::compositeByConnective(DISJ, g, k_1));
    _formula* fml_2 = Utils::compositeByConnective(IMPL, f_2,
                        Utils::compositeByConnective(DISJ, h, k_2));
     
    free(_originalFml->subformula_r->subformula_l);
    free(_originalFml->subformula_r);
    free(_originalFml);
    
    Formulas ret;
    ret.pushBack(Formula(fml_1, false));
    ret.pushBack(Formula(fml_2, false));
    return ret;
}
/**
 * F-> (G -> H) v K  => {G ^ F -> H v K
 *                      ~H ^ F -> ~G v K}
 * @param _originalFml
 * @return 
 */
Formulas CabalarUtils::rightRule_5(_formula* _originalFml) {
    assert(_originalFml);//->
    assert(_originalFml->formula_type == IMPL);
    assert(_originalFml->subformula_l);//f
    assert(_originalFml->subformula_r);//v
    assert(_originalFml->subformula_r->formula_type == DISJ);
    assert(_originalFml->subformula_r->subformula_l);//->
    assert(_originalFml->subformula_r->subformula_l->formula_type == IMPL);
    assert(_originalFml->subformula_r->subformula_l->subformula_l);//g
    assert(_originalFml->subformula_r->subformula_l->subformula_r);//h
    assert(_originalFml->subformula_r->subformula_r);//k
    
    _formula* f = _originalFml->subformula_l;
    _formula* f_2 = Utils::copyFormula(f);
    _formula* g = _originalFml->subformula_r->subformula_l->subformula_l;
    _formula* _g = Utils::convertToNegativeNormalForm(
                        Utils::compositeByConnective(NEGA, 
                            Utils::copyFormula(g), NULL));
    _formula* h = _originalFml->subformula_r->subformula_l->subformula_r;
    _formula* _h = Utils::convertToNegativeNormalForm(
                        Utils::compositeByConnective(NEGA, 
                            Utils::copyFormula(h), NULL));
    _formula* k = _originalFml->subformula_r->subformula_r;
    _formula* k_2 = Utils::copyFormula(k);
    //G ^ F -> H v K
    _formula* fml_1 = Utils::compositeByConnective(IMPL, 
                        Utils::compositeByConnective(CONJ, g, f),
                        Utils::compositeByConnective(DISJ, h, k));
    //~H ^ F -> ~G v K
    _formula* fml_2 = Utils::compositeByConnective(IMPL, 
                        Utils::compositeByConnective(CONJ, _h, f_2), 
                        Utils::compositeByConnective(DISJ, _g, k_2));

    free(_originalFml->subformula_r->subformula_l);
    free(_originalFml->subformula_r);
    free(_originalFml);
    
    Formulas ret;
    ret.pushBack(Formula(fml_1, false));
    ret.pushBack(Formula(fml_2, false));
    return ret;
}
/**
 * find sub-formula different from (input)>commutativeSymbol,
 * and swap it to the left side of (input)>root.
 * example:
 *              CONJ                               CONJ
 *       ------------------                   ----------------
 *       |                |                   |              |
 *    ----------         CONJ2     ====>     DISJ       -----------
 *  DISJ      CONJ1                                   CONJ2      CONJ1
 * @param _root
 * @param _cur
 * @param _commutativeSymbol
 * @return 0 - no need to swap, 1- need swap, 2- swap finish
 */
int CabalarUtils::commutaiveTrans(_formula* _root, _formula* _cur, 
                        FORMULA_TYPE _commutativeSymbol) {
    assert(_root);
    assert(_cur);
    assert(_commutativeSymbol == DISJ || _commutativeSymbol == CONJ);

    static bool isLeftSide;
    int value_l;
    int value_r;
    _formula* temp;

    // terminals
    if (_cur->formula_type != _commutativeSymbol) {
        if (_cur->formula_type == ATOM &&
                _cur->predicate_id != PRED_TRUE && 
                _cur->predicate_id != PRED_FALSE) {
            return 0;//normal predicates
        }
        if (_cur->formula_type == NEGA &&
                _cur->subformula_l->formula_type != NEGA) {
            return 0;//normal predicates
        }
        if (_cur->formula_type == UNIV || _cur->formula_type == EXIS) {
            return 0;//normal predicates
        }
        return 1;// predicates need to swap
    }

    // left sub-formula
    if (_cur == _root) { 
        isLeftSide = true; 
    }
    value_l = commutaiveTrans(_root,_cur->subformula_l,_commutativeSymbol);

    if (value_l == 2) { 
        return 2; 
    }
    if (value_l == 1) {
        if(isLeftSide)
        {
            temp = _cur->subformula_l;
            _cur->subformula_l = _root->subformula_r;

            _root->subformula_r = _root->subformula_l;
            _root->subformula_l = temp;
        }
        else
        {
            temp = _cur->subformula_l;
            _cur->subformula_l = _root->subformula_l;

            _root->subformula_l = temp;
        }
        return 2;
    }

    // right sub-formula
    if(_cur == _root) { 
        isLeftSide = false; 
    }
    value_r = commutaiveTrans(_root,_cur->subformula_r,_commutativeSymbol);

    if(value_r == 2) { 
        return 2; 
    }
    if(value_r == 1) {
        if(isLeftSide) {
            temp = _cur->subformula_r;
            _cur->subformula_r = _root->subformula_r;

            _root->subformula_r = _root->subformula_l;
            _root->subformula_l = temp;
        }
        else {
            temp = _cur->subformula_r;
            _cur->subformula_r = _root->subformula_l;

            _root->subformula_l = temp;
        }
        return 2;
    }

    return 0;
}
/**
 * 
 * @param _originalFml 在调用该函数前申请的空间，这里会处理这些空间的销毁
 * @return 
 */
Formulas CabalarUtils::transform(_formula* _originalFml) {
    assert(_originalFml);

    while(_originalFml->formula_type == UNIV) {
        _formula* pre = _originalFml;
        _originalFml = _originalFml->subformula_l;
        free(pre);
    }
    
    // Cabalar. (2005) Left/Right side rules
    if(_originalFml->formula_type == IMPL) {
        _formula* subformula_l = _originalFml->subformula_l;
        _formula* subformula_r = _originalFml->subformula_r;

        commutaiveTrans(subformula_l,subformula_l,CONJ);
        commutaiveTrans(subformula_r,subformula_r,DISJ);
		
	_formula* CD_l = subformula_l->subformula_l;
	_formula* CD_r = subformula_r->subformula_l;
        
        if(subformula_l->formula_type == CONJ) {
            if(CD_l->formula_type == ATOM) {
                //L1
                if(CD_l->predicate_id == PRED_TRUE) {
                    return leftRule_1(_originalFml);
                }
                //L2
                if(CD_l->predicate_id == PRED_FALSE) {
                    return leftRule_2(_originalFml);
                }
            }
            //L3
	    if(CD_l->formula_type == NEGA && 
                    CD_l->subformula_l->formula_type == NEGA) {
		return leftRule_3(_originalFml);
	    }
            //L4
            if(CD_l->formula_type == DISJ) {
                return leftRule_4(_originalFml);
            }
            //L5
            if(CD_l->formula_type == IMPL) {
                return leftRule_5(_originalFml);
            }
        }//end of Cabalar Left Translation (full pattern)
		
        // Cabalar. (2005) Right side rules
        if(subformula_r->formula_type == DISJ) {
            if(CD_r->formula_type == ATOM) {
                //R1
                if(CD_r->predicate_id == PRED_FALSE) {
                    return rightRule_1(_originalFml);
                }
                //R2
                if(CD_r->predicate_id == PRED_TRUE) {
                    return rightRule_2(_originalFml);
                }
            }
            //R3
	    if(CD_r->formula_type == NEGA &&
                    CD_r->subformula_l->formula_type == NEGA) {
		return rightRule_3(_originalFml);
	    }
            //R4
            if(CD_r->formula_type == CONJ) {
                return rightRule_4(_originalFml);
            }
            //R5
            if(CD_r->formula_type == IMPL) {
                return rightRule_5(_originalFml);
            }
        }//end of Cabalar Right Translation(full pattern)

		
	//start of Commutative missing Translation
	if(subformula_l->formula_type == ATOM) {
            //L1 TRUE->G ------> G
	    if(subformula_l->predicate_id == PRED_TRUE) {
                //return push_formulas(Cabalar_ILT(_originalFml),NULL); 
                //// convert_rules() does this.
                //// may also product problem like Cabalar_IRF()
	    }
	    //L2 FALSE->G ------> ALWAYS BE TRUE
	    if(subformula_l->predicate_id == PRED_FALSE) {
                _formula* temp1 = implLeftFalse(_originalFml);
                Formula temp2(temp1, false);
                Formulas ffs;
                ffs.pushBack(temp2);
                return ffs;
            //  return push_formulas(Cabalar_ILF(_originalFml),NULL);
	    }
	}
        //L3
        if(subformula_l->formula_type == NEGA &&
                subformula_l->subformula_l->formula_type == NEGA) {
            _originalFml->subformula_l = Utils::compositeByConnective(CONJ,subformula_l,NULL);
            _originalFml->subformula_l
                            ->subformula_r = Utils::compositeToAtom(PRED_TRUE,NULL);
            return leftRule_3(_originalFml);
        }
        //L4
        if(subformula_l->formula_type == DISJ) {
            _originalFml->subformula_l = Utils::compositeByConnective(CONJ,subformula_l,NULL);
            _originalFml->subformula_l
                            ->subformula_r = Utils::compositeToAtom(PRED_TRUE,NULL);
            return leftRule_4(_originalFml);
        }
        //L5
        if(subformula_l->formula_type == IMPL) {
            _originalFml->subformula_l = Utils::compositeByConnective(CONJ,subformula_l,NULL);
            _originalFml->subformula_l
                            ->subformula_r = Utils::compositeToAtom(PRED_TRUE,NULL);
            return leftRule_5(_originalFml);
        }
		
        //R1
        if(subformula_r->formula_type == ATOM) {
            //R1 F->FALSE -------->~F
            if(subformula_r->predicate_id == PRED_FALSE) {
                //return push_formulas(Cabalar_IRF(_originalFml),NULL); 
                //// convert_rules() does this. 
                //// also leads s to ~~s.
            }
            //R2 F->TRUE --------->ALWAYS BE TRUE
            if(subformula_r->predicate_id == PRED_TRUE) {
                _formula* temp1 = implRightTrue(_originalFml);
                Formula temp2(temp1, false);
                Formulas ffs;
                ffs.pushBack(temp2);
                return ffs;
            //  return push_formulas(Cabalar_IRT(_originalFml),NULL);
            }
        }
        //R3
        if(subformula_r->formula_type == NEGA &&
                subformula_r->subformula_l->formula_type == NEGA) {
            _originalFml->subformula_r = Utils::compositeByConnective(DISJ,subformula_r,NULL);
            _originalFml->subformula_r
                            ->subformula_r = Utils::compositeToAtom(PRED_FALSE,NULL);
            return rightRule_3(_originalFml);
        }
        //R4
        if(subformula_r->formula_type == CONJ) {
            _originalFml->subformula_r = Utils::compositeByConnective(DISJ,subformula_r,NULL);
            _originalFml->subformula_r
                            ->subformula_r = Utils::compositeToAtom(PRED_FALSE,NULL);
            return rightRule_4(_originalFml);
        }
        //R5
        if(subformula_r->formula_type == IMPL) {
            _originalFml->subformula_r = Utils::compositeByConnective(DISJ,subformula_r,NULL);
	    _originalFml->subformula_r->subformula_r = Utils::compositeToAtom(PRED_FALSE,NULL);
            return rightRule_5(_originalFml);
        }
    }
	
    //start of imply missing
    if(_originalFml->formula_type == CONJ) {
	commutaiveTrans(_originalFml,_originalFml, CONJ);
	_formula* CD_o = _originalFml->subformula_l;
		
	//L1
	if(CD_o->formula_type == ATOM && 
                CD_o->predicate_id == PRED_TRUE) {
            _formula* temp1 = conjLeftTrue(_originalFml);
            Formula temp2(temp1, false);
            Formulas ffs;
            ffs.pushBack(temp2);
            return ffs;
        //  return push_formulas(Cabalar_CLT(_originalFml),NULL);
        }
        //L2
        if(CD_o->formula_type == ATOM && 
                CD_o->predicate_id == PRED_FALSE) {
            _formula* temp1 = conjLeftFalse(_originalFml);
            Formula temp2(temp1, false);
            Formulas ffs;
            ffs.pushBack(temp2);
            return ffs;
        //  return push_formulas(Cabalar_CLF(_originalFml),NULL);
        }
		//Note: L3,L4,L5 lead to recursive translation
    }
	
    if(_originalFml->formula_type == DISJ) {
        commutaiveTrans(_originalFml,_originalFml, DISJ);
        _formula* CD_o = _originalFml->subformula_l;
		
        //R1
        if(CD_o->formula_type == ATOM &&
                CD_o->predicate_id == PRED_FALSE) {
            _formula* temp1 = disjLeftFalse(_originalFml);
            Formula temp2(temp1, false);
            Formulas ffs;
            ffs.pushBack(temp2);
            return ffs;
        //	return push_formulas(Cabalar_DLF(_originalFml),NULL);
        }
        //R2
        if(CD_o->formula_type == ATOM &&
                CD_o->predicate_id == PRED_TRUE) {
            _formula* temp1 = disjLeftTrue(_originalFml);
            Formula temp2(temp1, false);
            Formulas ffs;
            ffs.pushBack(temp2);
            return ffs;
        //  return push_formulas(Cabalar_DLT(_originalFml),NULL);
        }
        //R3
        if(CD_o->formula_type == NEGA && 
                CD_o->subformula_l->formula_type == NEGA) {
            _originalFml = Utils::compositeByConnective(IMPL,NULL,_originalFml);
            _originalFml->subformula_l = Utils::compositeToAtom(PRED_TRUE,NULL);
            return rightRule_3(_originalFml);
        }
        //R4
        if(CD_o->formula_type == CONJ) {
            _originalFml = Utils::compositeByConnective(IMPL,NULL,_originalFml);
            _originalFml->subformula_l = Utils::compositeToAtom(PRED_TRUE,NULL);
            return rightRule_4(_originalFml);
        }
        //R5
        if(CD_o->formula_type == IMPL) {
            _originalFml = Utils::compositeByConnective(IMPL,NULL,_originalFml);
            _originalFml->subformula_l = Utils::compositeToAtom(PRED_TRUE,NULL);
            return rightRule_5(_originalFml);
        }
    }//end of _originalFml->formula_type == DISJ
	
    
    Formula f_result(_originalFml, false);
    Formulas fs_result;
    fs_result.pushBack(f_result);
    return fs_result;
}
