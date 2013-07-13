#include "utility.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//terms operations
///////////////////////////////////////////////////////////////////////////////
_term* combine_terms(const _term* head, int head_size, 
					   const _term* tail, int tail_size)
{
	_term* terms = (_term*)malloc( sizeof(_term) * (head_size+tail_size) );
	
	memcpy(terms, head, sizeof(_term)*head_size);
	memcpy(terms+sizeof(_term)*head_size, tail, sizeof(_term)*tail_size);
	
	return terms;
}

_term* combine_terms(const std::vector<int>& head,
					   const std::vector<int>& tail)
{
    _term* terms = (_term*)malloc(sizeof(_term) * (head.size()+tail.size()));

    int 
	index = 0;
    for(std::vector<int>::const_iterator it = head.begin();
		it != head.end(); it++, index++)
    {
        terms[index].variable_id = head[index];
        terms[index].term_type = VARI;
    }
	index = 0;
    for(std::vector<int>::const_iterator it = tail.begin(); 
		it != tail.end(); it++, index++)
    {
        terms[index+head.size()].variable_id = tail[index];
        terms[index+head.size()].term_type = VARI;
    }
	
	return terms;
}

_term* copy_terms(const _term* terms, int size)//deep copy
{
    _term* newTerms = (_term*)malloc(sizeof(_term)*size);
    assert(newTerms);

    memcpy(newTerms, terms, sizeof(_term)*size);

    for(int i = 0; i < size; i++)
    {
        if(newTerms[i].term_type == FUNC)
        {
            newTerms[i].parameters = copy_terms(newTerms[i].parameters,
			vocabulary.function_arity(newTerms[i].function_id));
        }
    }

    return newTerms;
}

void delete_terms(_term* terms, int size)//deep delete
{
    for(int i = 0; i < size; i++)
    {
        if(terms[i].term_type == FUNC)
        {
            delete_terms(terms[i].parameters,
			vocabulary.function_arity(terms[i].function_id));
        }
    }
    free(terms);
}

bool compare_term(const _term* t, const _term* s)
{
    int k;

    assert(t);
    assert(s);

    if (t->term_type!=s->term_type) return false;

    switch (t->term_type)
    {
    case VARI:
        return (t->variable_id==s->variable_id);
    case FUNC:
        if (t->function_id==s->function_id)
        {
            k = vocabulary.function_arity(t->function_id) - 1;
            assert(k<0||t->parameters);
            assert(k<0||s->parameters);
            for ( ; k>=0; k--)
            {
                if (!compare_term(t->parameters+k,s->parameters+k))
                    return false;
            }
            return true;
        }
    default:
        assert(0);
    }
    return false;
}

bool find_var_term(const _term* t, int var_id)
{
    int i;

    assert(t);

    switch(t->term_type)
    {
    case VARI:
        return (t->variable_id==var_id);
    case FUNC:
        assert(t->parameters);
        for(i = 0; i < vocabulary.function_arity(t->function_id); i++)
        {
            if ( find_var_term(t->parameters+i, var_id) )
                return true;
        }
        break;
    default:
        assert(0);
    }
    return false;
}

void rename_var_term(_term* t, int oldv, int newv)
{
    int i;
    assert(t);

    switch(t->term_type)
    {
    case VARI:
        if (t->variable_id == oldv)
		{
			t->variable_id = newv;
		}
        break;
    case FUNC:
        assert (t->parameters);
        for(i = 0; i < vocabulary.function_arity(t->function_id); i++)
        {
            rename_var_term(t->parameters+i, oldv, newv);
        }
        break;
    default:
        assert(0);
    }
}

_term* replace_term(_term* terms, int arity, 
			const std::vector<int>& exis, const std::vector<int>& replacements)
{
    for(int i = 0; i < arity; i++)
    {
        //replacement
        if(terms[i].term_type == VARI)
        {
            for(int j = 0; j < exis.size(); j++)
            {
                if(terms[i].variable_id == exis[j])
                {
                    terms[i].variable_id = replacements[j];
                }
            }
        }
        //traversal
        else if(terms[i].term_type == FUNC)
        {
            replace_term(terms[i].parameters, 
					vocabulary.function_arity(terms[i].function_id), exis, replacements);
        }
    }
    return terms;
}

void output_term ( FILE* out, const _term* t )
{
    int i, k;

    assert (t);

    if ( VARI==t->term_type )
    {
        fprintf ( out, "%s", vocabulary.query_name(t->variable_id, VARIABLE));
    }
    else
    {
        fprintf ( out, "%s", vocabulary.query_name(t->function_id, FUNCTION));
        k = vocabulary.function_arity(t->function_id);
        if ( k > 0 )
        {
            fprintf ( out, "(" );
            for ( i=0; i<k; i++ )
            {
                if ( 0 < i ) printf ( ", " );
                output_term ( out, t->parameters+i );
            }
            fprintf ( out, ")" );
        }
    }
}
//formula operations
///////////////////////////////////////////////////////////////////////////////
_formula* composite_atom(FORMULA_TYPE formula_type,
									const int predicate_id, _term* parameters)
{
    assert(formula_type == ATOM);

    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = formula_type;
    fml->predicate_id = predicate_id;
    fml->parameters   = parameters;
    return fml;
}

_formula* composite_bool(FORMULA_TYPE formula_type, 
								_formula* subformula_l, _formula* subformula_r)
{
    assert(formula_type == NEGA || formula_type == DISJ ||
		   formula_type == CONJ || formula_type == IMPL);

    _formula* fml = (_formula*)malloc(sizeof(_formula));
    assert(fml);

    fml->formula_type = formula_type;
    fml->subformula_l = subformula_l;
    fml->subformula_r = subformula_r;
    return fml;
}

_formula* composite_qntf(FORMULA_TYPE formula_type,
								_formula* subformula_l, const int variable_id)
{
	assert(formula_type == EXIS || formula_type == UNIV);

	_formula* fml = (_formula*)malloc(sizeof(_formula));
	assert(fml);

	fml->formula_type = formula_type;
	fml->subformula_l = subformula_l;
	fml->variable_id = variable_id;
	return fml;
}
//------------------------------------------------------------------------------
_formula* copy_formula (const _formula *fml)
{
    assert (fml);
	
    _formula* newFormula = (_formula*)malloc(sizeof(_formula));
    assert (newFormula);

    memcpy(newFormula, fml, sizeof (_formula));
    switch (fml->formula_type)
    {
    case ATOM:
        newFormula->parameters = copy_terms(fml->parameters, vocabulary.predicate_arity(fml->predicate_id));
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        assert ( fml->subformula_r );
        newFormula->subformula_r = copy_formula ( fml->subformula_r );
    case NEGA:
    case UNIV:
    case EXIS:
        assert ( fml->subformula_l );
        newFormula->subformula_l = copy_formula ( fml->subformula_l );
        break;
    default:
        assert (0);
    }

    return newFormula;
}

//other
///////////////////////////////////////////////////////////////////////////////

/*void output_extraDefinition(FILE* out)//@TODO
{
	int i;
	for(i = 0; i < vocabulary.num_predicate; i++)
	{
		fprintf(out,"_%s :- not %s\n",vocabulary.names_predicate[i],
				vocabulary.names_predicate[i]);
	}
}

_formula* double_negation(_formula* phi, const int* int_preds, int num_ip)
{
    assert(phi);

    if (is_negative(phi, int_preds, num_ip, false))
    {
        return phi;
    }

    switch ( phi->formula_type )
    {
    case ATOM:
        if(in_list(int_preds, num_ip, phi->predicate_id))
        {
            phi = composite_bool ( NEGA, phi, NULL );
            phi = composite_bool ( NEGA, phi, NULL );
        }
        break;
    case CONJ:
    case DISJ:
    case IMPL:
		phi->subformula_r = double_negation(phi->subformula_r,int_preds,num_ip);
    case NEGA:
    case UNIV:
    case EXIS:
		phi->subformula_l = double_negation(phi->subformula_l,int_preds,num_ip);
        break;

    default:
        assert ( 0 );
    }

    return phi;
}

_formula* 
minimal_simu(_formula* phi, const int* int_preds, int num_ip,
		 const _formula* reff )
{
    assert(phi);

    if (num_ip <= 0)//No internal predicates, fast return.
    {
        return phi;
    }

    switch (phi->formula_type)
    {
	case ATOM:
		return phi;
	
    case NEGA:
        assert(phi->subformula_l);
        if ( ATOM == phi->subformula_l->formula_type &&
             in_list ( int_preds, num_ip, phi->subformula_l->predicate_id ) )
        {			
			phi->formula_type = IMPL;
			phi->subformula_r = copy_formula(reff);
        }
        else
        {
            phi->subformula_l = minimal_simu(phi->subformula_l, 
												int_preds, num_ip, reff );
        }
        return phi;
		
    case CONJ:
    case DISJ:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        phi->subformula_l = minimal_simu (phi->subformula_l, 
												int_preds, num_ip, reff );
        phi->subformula_r = minimal_simu (phi->subformula_r, 
												int_preds, num_ip, reff );
        return phi;
		
    case UNIV:
    case EXIS:
        assert ( phi->subformula_l );
        phi->subformula_l = minimal_simu (phi->subformula_l, 
												int_preds, num_ip, reff );
        return phi;
		
    default:
        assert(0);
    }
    return NULL;
}*/
