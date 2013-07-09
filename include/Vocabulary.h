class Vocabulary{
private:
	int num_variable;
	char* names_variable[MAX_NUM_VARIABLE];
	
	int num_function;
	int arities_function[MAX_NUM_FUNCTION];
	char* names_function[MAX_NUM_FUNCTION];
	
	int num_predicate;
	int arities_predicate[MAX_NUM_PREDICATE];
	char* names_predicate[MAX_NUM_PREDICATE];
	
	int num_stable_predicate;
	int index_stable_predicate[MAX_NUM_PREDICATE];

	int num_new_variable;
	int num_new_predicate;

	int num_intension_predicate;
        int index_intension_predicate[MAX_NUM_PREDICATE];
        
        int num_names_domain;
        char* names_domain[MAX_NUM_DOMAIN];

public:
	void init_vocabulary();
	void destory_vocabulary();
	int add_symbol ( const char* name, SYMBOL_TYPE type, int arity );
	int query_symbol ( const char* name, SYMBOL_TYPE type );
	int predicate_arity(int id);
	int function_arity ( int id );
	char* query_name ( int id, SYMBOL_TYPE type );
	
};
