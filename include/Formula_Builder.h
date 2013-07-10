#include "Formula.h"

class Formula_Builder{
public:
	_formula* composite_atom(FORMULA_TYPE formula_type,
			const int predicate_id, Term* parameters);
	_formula* composite_bool(FORMULA_TYPE formula_type, 
				_formula* subformula_l, Formula* subformula_r);
	_formula* composite_qntf(FORMULA_TYPE formula_type,
				_formula* subformula_l, const int variable_id);
	_formula* copy_formula(const _formula* fml);
};