#include <deque>
#include "Formula.h"

class Formulas{
private:
	deque<Formula> _formulas;
	
public:
	Formulas* join_formulas(Formulas* head, Formulas* tail);
	Formulas* push_formulas(Formula* fml, Formulas* tail);
	Formulas* pop_formulas (Formulas* fmls);//DO NOT DELETE formula
	Formula*  top_formulas (Formulas* fmls);
	Formulas* copy_formulas(const Formulas* sigma);
	void	delete_formulas(Formulas* sigma);
	int	size_formulas(Formulas* fmls);

	void output_formulas(FILE* out, const Formulas* fmls);

};
