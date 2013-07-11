#include <deque>
#include "Formula.h"

using namespace std;

class Formulas{
private:
	deque<Formula> _formulas;
	
public:
        Formulas();
        ~Formulas();
        Formulas(deque<Formula> _formula);
        
        deque<Formula> get_formulas();
        void set_formulas(deque<Formula>& fs);
        
	void join_formulas(Formulas tail);
	void push_formula(Formula tail);
	void pop_formula ();//DO NOT DELETE formula
	Formula  top_formula ();
	void copy_formulas(const Formulas sigma);
	void	delete_formulas();
	int	size_formulas();

	void output_formulas(FILE* out);

};