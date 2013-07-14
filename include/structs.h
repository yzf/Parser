#ifndef _STRUCTS_H_
#define	_STRUCTS_H_

//Pre-definition
////////////////////////////////////////////////////////////////////////////////
#define MAX_NUM_PREDICATE   100
#define MAX_NUM_DOMAIN      10
#define MAX_NUM_FUNCTION	100
#define MAX_NUM_VARIABLE	100
#define MAX_LEN_PARAMETER	10
#define MAX_NUM_ATOM		200
#define MAX_LEN_CLAUSE		(MAX_NUM_ATOM*2)

#define PRED_TRUE			-1
#define PRED_FALSE			-2
#define PRED_EQUALITY		-3
#define PRED_MIN			-4
#define PRED_MAX			-5
#define PRED_SUCC			-6

const int spec_pred_arities[] =
{
    0,			//PRED_TRUE
    0,			//PRED_FALSE
    2,			//PRED_EQUALITY
    1,			//PRED_MIN
    1,			//PRED_MAX
    2,			//PRED_SUCC
};

// Type definition
////////////////////////////////////////////////////////////////////////////////
typedef enum {
	FALSE = 0,
	TRUE  = 1,
} BOOL;

typedef enum {
	VARI = 0,
	FUNC,
} TERM_TYPE;

typedef enum {
	VARIABLE = 0,
	FUNCTION,
	PREDICATE,
        DOMAIN
} SYMBOL_TYPE;

typedef enum {
	ATOM = 0,	//atom
	NEGA,		//negation
	CONJ,		//conjunction
	DISJ,		//disjunction
	IMPL,		//implication
	UNIV,		//universal
	EXIS,		//existential
} FORMULA_TYPE;

// Structures
////////////////////////////////////////////////////////////////////////////////
typedef struct __term {
	TERM_TYPE term_type;

	union {
		int variable_id;			//VARI
		int function_id;			//FUNC
	};

	__term* parameters;		//FUNC
} _term;

typedef struct __terms {			// only be used in lex&yacc
	_term terms[MAX_LEN_PARAMETER];
	int num_term;
} _terms;

typedef struct __formula {
	FORMULA_TYPE formula_type;

	union {
		struct __formula* subformula_l;   //NEGA,CONJ,DISJ,IMPL,UNIV,EXIS
		int predicate_id;                 //ATOM
	};

	union {
		struct __formula* subformula_r;   //CONJ,DISJ,IMPL
		int variable_id;                  //UNIV,EXIS
		struct __term* parameters;        //ATOM
	};
} _formula;

////////////////////////////////////////////////////////////////////////////////
/*typedef struct __rule {
    _formulas* head;
	_formulas* body;
}_rule;

typedef struct __rules {
	_rule* curr_rule;
	struct __rules* remained_rules;
} _rules;

typedef struct __dlp_rules
{
	char head[MAX_LEN_CLAUSE];
	char body[MAX_LEN_CLAUSE];
	struct __dlp_rules* remained_rules;
} _dlp_rules;

typedef struct __dlp 
{
	int num_atom;
	_formula* atoms[MAX_NUM_ATOM];	
	_dlp_rules* rules;
} _dlp;
*/
#endif



