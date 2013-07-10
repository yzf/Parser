#include "Formula.h"

class Formula_Builder{
private:

public:
	Formula* composite_atom(FORMULA_TYPE formula_type,
			const int predicate_id, Term* parameters);
	Formula* composite_bool(FORMULA_TYPE formula_type, 
				Formula* subformula_l, Formula* subformula_r);
	Formula* composite_qntf(FORMULA_TYPE formula_type,
				Formula* subformula_l, const int variable_id);
};
