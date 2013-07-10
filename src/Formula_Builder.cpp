#include "Formula_Builder.h"
#include <assert.h>

_formula* Formula_Builder::composite_atom(FORMULA_TYPE formula_type,
									const int predicate_id, _term* parameters)
{
    assert(formula_type == ATOM);

    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = formula_type;
    fml->predicate_id = predicate_id;
    fml->parameters   = parameters;
    return fml;
}

_formula* Formula_Builder::composite_bool(FORMULA_TYPE formula_type, 
								_formula* subformula_l, _formula* subformula_r)
{
    assert(formula_type == NEGA || formula_type == DISJ ||
		   formula_type == CONJ || formula_type == IMPL);

    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = formula_type;
    fml->subformula_l = subformula_l;
    fml->subformula_r = subformula_r;
    return fml;
}

_formula* Formula_Builder::composite_qntf(FORMULA_TYPE formula_type,
								_formula* subformula_l, const int variable_id)
{
	assert(formula_type == EXIS || formula_type == UNIV);

	_formula* fml = (_formula*)malloc(sizeof(_formula));
	assert(fml);

	fml->formula_type = formula_type;
	fml->subformula_l = subformula_l;
	fml->variable_id = variable_id;
	return fml;
}

_formula* Formula_Builder::copy_formula(const _formula *fml)
{
    assert (fml);
	
    _formula* newFormula = (_formula*)malloc(sizeof(_formula));
    assert (newFormula);

    memcpy(newFormula, fml, sizeof (_formula));
    switch (fml->formula_type)
    {
    case ATOM:
        newFormula->parameters = copy_terms(fml->parameters, 
											predicate_arity(fml->predicate_id));
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        assert ( fml->subformula_r );
        newFormula->subformula_r = copy_formula ( fml->subformula_r );
    case NEGA:
    case UNIV:
    case EXIS:
        assert ( fml->subformula_l );
        newFormula->subformula_l = copy_formula ( fml->subformula_l );
        break;
    default:
        assert (0);
    }

    return newFormula;
}
