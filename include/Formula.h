#ifndef _FORMULA_H_
#define _FORMULA_H_

#include "Formulas.h"
#include <vector>
#include "structs.h"
#include <cstdio>
#include "utility.h"
#include <map>

using namespace std;

class Formulas;

class Formula {
private:
	_formula* formula;	
	vector<int> name_domain_id;
        bool is_prenex_formula;                
        
	bool is_child_universal(_formula*);
         void remove_from_prenex(_formula* parent, int d, _formula* tag);
        _formula* find_prenex_quanlifier(_formula* fml, int var_id);
	bool find_var_formula (_formula* phi, int var_id);
	void rename_var_formula (_formula* phi, int oldv, int newv);
        void rename_node_formula( _formula* tag);
	_formula* replace_terms_formula(_formula* fml, const vector<int>& exis, 
				const vector<int>& replacements);
	void output_formula(FILE *out, _formula* fml);
        
        void divide_clause_formula(_formula* fml, _formula* parent, Formulas& result);
        void divide_CNF_formula(_formula* fml, Formulas& result);
        
        void convert_CNF_formula(_formula* fml);
        void lower_negative(_formula* fml);
        void lower_disjunction(_formula* fml, int depth);
        
        void convert_prenex_formula(_formula* fml);
        bool is_negative_formula(const _formula* phi, 
			const int* sm_preds, int num_sp, bool negative);
        
        bool in_list ( const int* list, int len, int obj );
        _formula* double_negation_formula(_formula* phi, const int* int_preds, int num_ip);
        void get_no_quantifier_variables(map<int, bool> &flag, vector<int> &varis, _formula* fml);
public:
        int deep;       //公式在章衡转化中的深度
public:	
        Formula();
    	Formula(_formula* fml, bool copy);
        Formula(const Formula&);
        
	~Formula();

	bool is_universal();	
	bool find_var(int var_id);
	void rename_var(int oldv, int newv);
	void replace_terms(const vector<int>& exis, 
				const vector<int>& replacements);
	void output(FILE* out);
        
        Formulas divide_clause();
        Formulas divide_CNF();
        
        void convert_CNF();
	bool is_prenex();
	void convert_prenex();
        
        _formula* get_formula();
        void set_formula(_formula* f);
        Formula& operator = (const Formula& rhs);
        bool is_negative(const int* sm_preds, int num_sp, bool negative);
        void double_negation(const int* int_preds, int num_ip);
        void fix_universal_quantifier();
        bool operator == (Formula&);
};

#endif
