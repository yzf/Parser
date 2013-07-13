#ifndef _FORMULA_H_
#define _FORMULA_H_
#include "Formulas.h"
#pragma once

#include <vector>
#include "structs.h"
#include <cstdio>

using namespace std;

class Formulas;

class Formula {
private:
	_formula* formula;	
	vector<int> name_domain_id;

	bool is_child_universal(_formula*);
	bool compare_formula(const _formula* phi, const _formula* psi);
	void delete_formula (_formula* fml);
	bool find_var_formula (const _formula* phi, int var_id);
	void rename_var_formula (_formula* phi, int oldv, int newv);
	_formula* replace_terms_formula(_formula* fml, const vector<int>& exis, 
				const vector<int>& replacements);
	void output_formula(FILE *out, _formula* fml);
        
        void divide_clause_formula(_formula* fml, Formulas& result);
        void divide_CNF_formula(_formula* fml, Formulas& result);
        
        void convert_CNF_formula(_formula* fml);
        void lower_negative(_formula* fml);
        void lower_disjunction(_formula* fml, int depth);
        
        void convert_pernex_formula(_formula* fml);
        
public:	
        Formula();
    	Formula(_formula* fml);
        
	~Formula();

	bool is_universal();
	bool compare(Formula& psi);	
	bool find_var(int var_id);
	void rename_var(int oldv, int newv);
	void replace_terms(const vector<int>& exis, 
				const vector<int>& replacements);
	void output(FILE* out);
        
        Formulas divide_clause();
        Formulas divide_CNF();
        
        void convert_CNF();
	
	void convert_prenex();
        _formula* getFormula() const;
};

#endif