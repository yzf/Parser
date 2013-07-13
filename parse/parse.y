%{
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "structs.h"
#include "Vocabulary.h"
#include "utility.h"
#include "S2DLP.h"
#include "Formula.h"
#include "Formulas.h"

extern "C" {
	void yyerror(const char *s);
	extern int yylex(void);
}

extern Vocabulary vocabulary;
extern int context_flag;
extern gformula; 

int id;
char str_buf[512];

void yyerror(const char* s)
{
	printf("Parser error: %s\n", s);
}

%}

%union {
	char* s;
	struct __formula* f;
	struct __term* t;
        struct __terms* ts;
}

%token <s> S_VARI
%token <s> S_PRED
%token <s> S_FUNC
%token <s> S_NEGA
%token <s> S_CONJ
%token <s> S_DISJ
%token <s> S_IMPL
%token <s> S_UNIV
%token <s> S_EXIS
%token <s> PERIOD
%token <s> LPAREN
%token <s> RPAREN
%token <s> LBRACKET
%token <s> RBRACKET
%token <s> COMMA
%token <s> EQUAL
%token <s> MARCO
%token <s> LBRACE
%token <s> RBRACE
%token <s> SEMICO
%token <s> AT
%token <s> LL
%token <s> RR

%type <f>  s2dlp formulas formula atom
%type <t>  term 
%type <ts> terms

%left S_IMPL
%left S_CONJ S_DISJ
%right S_NEGA S_EXIS S_UNIV

%%
s2dlp 
	: formulas intensionP domain_section{
            assert($1);
            printf("root\n");
            gformula = $1;
	}
;

intensionP
        : LBRACE inten_preds RBRACE {
        
        }
        |
;
inten_preds
        : inten_preds COMMA intent_pred {
            
        } 
        | intent_pred {
            
        }
;
intent_pred
        : S_PRED {           
            vocabulary.set_intension_predicate($1);

            context_flag = 0;
        }
;
domain_section
        : LL domains RR {

        }
        |
;
domains
        : domains SEMICO domain {
        
        }
        | domain {
            
        }
;
domain
        : S_VARI AT S_PRED {
            vocabulary.set_domain($1, $3);
            context_flag = 0;
        }
;

formulas
	: formulas formula PERIOD {
		printf("formulas recursive\n");
		$$ = composite_bool(CONJ, $1, $2);
	}
	| formula PERIOD {
		printf("formulas single\n");
		$$ = $1;
	}
;
	
formula
	: formula S_CONJ formula {
		printf("formula conj\n");
		assert($1);
		assert($3);
		$$ = composite_bool(CONJ, $1, $3);
	}
	| formula S_DISJ formula {
		printf("formula disj\n");
		assert($1);
		assert($3);
		$$ = composite_bool(DISJ, $1, $3);
	}
	| formula S_IMPL formula {
		printf("formula impl\n");
		assert($1);
		assert($3);
		$$ = composite_bool(IMPL, $1, $3);
	}
	| S_NEGA formula {
		printf("formula nega\n");
		assert($2);
		$$ = composite_bool(NEGA, $2, NULL);
	}
	| LBRACKET S_UNIV S_VARI RBRACKET formula {
		printf("formula univ\n");
		assert($3);
		assert($5);
		
		if ((id = vocabulary.query_symbol($3, VARIABLE)) < 0) {
			id = vocabulary.add_symbol($3, VARIABLE, 0);
		}
	    $$ = composite_qntf(UNIV, $5, id);
	    
	    //free($3);
	}
	| LBRACKET S_EXIS S_VARI RBRACKET formula {
		printf("formula exis\n");
		assert($3);
		assert($5);
		if ((id = vocabulary.query_symbol($3, VARIABLE)) < 0) {
			id = vocabulary.add_symbol($3, VARIABLE, 0);
		}
	    $$ = composite_qntf(EXIS, $5, id);
	    
	    //free($2);
	}
	| atom {
		printf("formula atom\n");
		$$ = $1;
	}
	| LPAREN formula RPAREN {
		printf("formula paren\n");
		$$ = $2;
	}
;

atom
	: S_PRED LPAREN terms RPAREN  {
		printf("atom terms\n");
		assert($1);
		assert($3);
		
		$$ = (_formula*)malloc(sizeof(_formula));

		if ((id = vocabulary.query_symbol($1, PREDICATE)) < 0) {
			id = vocabulary.add_symbol($1, PREDICATE, $3->num_term);
		} 
		else if (vocabulary.predicate_arity(id) != $3->num_term)
		{
			sprintf(str_buf, "the predicate \"%s\" has too many definitions!", $1);
			yyerror(str_buf);
			exit(-1);
		}
		
		printf("atom terms: id ok\n");

		$$->formula_type = ATOM;
		$$->predicate_id = id;
		$$->parameters   = (_term*)malloc(sizeof(_term)*$3->num_term);
		memcpy($$->parameters, $3->terms, sizeof(_term)*$3->num_term);
		
		free($3);
		//free($1);

		context_flag = 0;
	}
	| S_PRED {
		printf("atom no terms\n");
		assert($1);
		
		$$ = (_formula*)malloc(sizeof(_formula));

		if(strcmp($1, "true") == 0)
		{
			id = PRED_TRUE;
		}
		else if(strcmp($1, "false") == 0)
		{
			id = PRED_FALSE;
		}
		else if ((id = vocabulary.query_symbol($1, PREDICATE)) < 0) {
			id = vocabulary.add_symbol($1, PREDICATE, 0);
		} 
		else if (vocabulary.predicate_arity(id) != 0)
		{
			sprintf(str_buf, "the predicate \"%s\" has too many definitions!", $1);
			yyerror(str_buf);
			exit(-1);
		}
	
		$$->formula_type = ATOM;
		$$->predicate_id = id;
		$$->parameters   = NULL;

		context_flag = 0;
	}
;
	
terms
	: terms COMMA term {
		printf("terms comma\n");
		assert($1);
		assert($3);
		
		$$ = $1;

		if ($$->num_term+1 < MAX_LEN_PARAMETER) {	
			memcpy($$->terms+$$->num_term, $3, sizeof(_term));
		 
			$$->num_term ++;
		}
		else {
			sprintf(str_buf, "too many parameters!");
			yyerror(str_buf);
			exit(-1);
		}
		
		free($3);
	}
	| term {
		printf("terms single\n");
		$$ = (_terms*)malloc(sizeof(_terms));
		
		assert($1);
		
		memcpy(&($$->terms), $1, sizeof(_term));
		 
		$$->num_term = 1;
	}
;

term
	: S_VARI {
		printf("term var\n");
		assert($1);
		
		$$ = (_term*)malloc(sizeof(_term));
		$$->term_type = VARI;
                
                id = vocabulary.add_symbol($1, VARIABLE, 0);

		$$->variable_id = id;
		//free($1);
	}
	| S_FUNC {
		assert($1);
		printf("term cons %s\n",$1);
		$$ = (_term*)malloc(sizeof(_term));
		$$->term_type = FUNC;

		if ((vocabulary.query_symbol($1, FUNCTION)) < 0) {
			id = vocabulary.add_symbol($1, FUNCTION, 0);
		}
		else if (vocabulary.function_arity(id) != 0)
		{
			sprintf(str_buf, "the function \"%s\" has too many definitions!", $1);
			yyerror(str_buf);
			exit(-1);
		}
		
		$$->function_id = id;
		$$->parameters  = NULL;
		
		//free($1);
	}
	| S_FUNC LPAREN terms RPAREN {
		printf("term func\n");
		assert($1);
		assert($3);
		
		$$ = (_term*)malloc(sizeof(_term));
		$$->term_type = FUNC;

		if ((id = vocabulary.query_symbol($1, FUNCTION)) < 0) {
			id = vocabulary.add_symbol($1, FUNCTION, $3->num_term);
		}
		else if (vocabulary.function_arity(id) != $3->num_term)
		{
			sprintf(str_buf, "the function \"%s\" has too many definitions!", $1);
			yyerror(str_buf);
			exit(-1);
		}
		
		$$->function_id = id;
		$$->parameters = (_term*)malloc(sizeof(_term)*$3->num_term);
		memcpy($$->parameters, $3->terms, sizeof(_term)*$3->num_term);
		
		free($3); 
		//free($1);
	}
;
%%
