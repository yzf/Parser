#ifndef _FORMULA_FUILDER_H_
#define _FORMULA_FUILDER_H_

#include "Formula.h"

class Formula_Builder{
public:
	static _formula* composite_atom(FORMULA_TYPE formula_type,
			const int predicate_id, Term* parameters);
	static _formula* composite_bool(FORMULA_TYPE formula_type, 
				_formula* subformula_l, Formula* subformula_r);
	static _formula* composite_qntf(FORMULA_TYPE formula_type,
				_formula* subformula_l, const int variable_id);
	static _formula* copy_formula(const _formula* fml);
};

#endif