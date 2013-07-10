#include <vector>

class Formula {
private:
	FORMULA_TYPE formula_type;
	union{
		Formula* subformula_l;
		int predicate_id;
	};
	union{
		Formula* subformula_r;
		int variable_id;
		Term* parameters;	
	};
		
	vector<int> name_domain_id;
public:
	
	Formula* copy_formula (const Formula *fml);
	bool compare_formula(const Formula* phi, const Formula* psi);
	void delete_formula ( Formula* fml );
	bool find_var_formula ( const Formula* phi, int var_id );
	void rename_var_formula ( Formula* phi, int oldv, int newv );
	Formula* replace_terms(Formula* fml, const std::vector<int>& exis, 
				const std::vector<int>& replacements);
	void output_formula(FILE* out, const Formula* phi);
	
	Formula* convert_prenex(Formula* fml);
};
