#include <vector>

using namespace std;

class Formula {
private:
	_formula* formula;	
	vector<int> name_domain_id;

	bool is_child_universal(_formula*);
	bool compare_formula(const _formula* phi, const _formula* psi);
	void delete_formula (_formula* fml);
	bool find_var_formula (const _formula* phi, int var_id);
	void rename_var_formula (_formula* phi, int oldv, int newv);
	Formula replace_terms_formula(_formula* fml, const vector<int>& exis, 
				const vector<int>& replacements);
	void output_formula(FILE* out, _formula* fml);
        
        void divide_clause_formula(_formula* fml);
        void divide_CNF_formula(_formula* fml);
        
        void convert_CNF_formula(_formula* fml);
        void lower_nagative(_formula* fml);
        void lower_disjunction(_formula* fml);
        
public:		
	Formula(const Formula);
	Formula(const _formula* fml);
	~Formula();

	bool is_universal();
	bool compare(const Formula psi);	
	bool find_var(int var_id);
	void rename_var(int oldv, int newv);
	Formula replace_terms(const vector<int>& exis, 
				const vector<int>& replacements);
	void output(FILE* out);
        
        Formulas divide_clause();
        Formulas divide_CNF();
        
        void convert_CNF();
	
	void convert_prenex(Formula fml);
};