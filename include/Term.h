#ifndef _TERM_H_
#define _TERM_H_

/* Ignoring _terms with a vector<term> to instead of it */

class Term{
private:
	TERM_TYPE term_type;
	union{
		int variable_id;
		int function_id;	
	};	
	Term* parameters;
public:
	Term* combine_terms(const Term* head, int head_size, 
					   const Term* tail, int tail_size);
	Term* combine_terms(const std::vector<int>& head,
					   const std::vector<int>& tail);
	Term* copy_terms(const Term* terms, int size);	//deep copy
	void delete_terms(_term* terms, int size);	//deep delete
	bool compare_term(const Term* t, const Term* s);
	bool find_var_term(const Term* t, int var_id);
	void rename_var_term(Term* t, int oldv, int newv);
	Term* replace_term(Term* terms, int arity, 
			const std::vector<int>& exis, const std::vector<int>& 				replacements)
	void output_term ( FILE* out, const Term* t );
	
	

};

#endif