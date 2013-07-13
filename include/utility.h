#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "structs.h"
#include <vector>
#include "Vocabulary.h"
#include "cstdio"

extern Vocabulary vocabulary;
extern int context_flag;

//terms operations
///////////////////////////////////////////////////////////////////////////////
_term* combine_terms(const _term* head, int head_size,
					   const _term* tail, int tail_size);//VARI terms ONLY
_term* combine_terms(const std::vector<int>& head, 
					   const std::vector<int>& tail);//VARI terms ONLY
_term*   copy_terms (const _term* terms, int size);
void   delete_terms (_term* terms, int size);
bool   compare_term (const _term* t, const _term* s);

bool find_var_term(const _term* t, int var_id);
void rename_var_term(_term* t, int oldv, int newv);

_term* replace_term (_term* terms, int arity, 
			const std::vector<int>& exis, const std::vector<int>& replacements);

void output_term(FILE* out, const _term* t);

//formula operations
///////////////////////////////////////////////////////////////////////////////
_formula* composite_atom(FORMULA_TYPE formula_type, 
							int predicate_id, _term* parameters);	 //ATOM
_formula* composite_qntf(FORMULA_TYPE formula_type, 
							_formula* subformula_l, int variable_id);//EXIS,UNIV
_formula* composite_bool(FORMULA_TYPE formula_type,
							_formula* subformula_l, _formula* subformula_r);
_formula* copy_formula(const _formula *fml);

//formula transforms
///////////////////////////////////////////////////////////////////////////////
void output_extraDefinition(FILE* out);

_formula* double_negation(_formula* phi, const int* int_preds, int num_ip);
_formula* minimal_simu(_formula* phi, const int* int_preds, int num_ip,
						 const _formula* reff);
#endif