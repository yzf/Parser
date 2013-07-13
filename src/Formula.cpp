#include "Formula.h"
#include "Formulas.h"
#include "utility.h"
#include <assert.h>
#include <cstdlib>

Formula::Formula() {
    this->formula = NULL;
}

Formula::Formula(_formula* fml) {
	this->formula = fml;
}

/*Formula::Formula(Formula& FML) {
	_formula* new_formula = copy_formula(FML.formula);
	
	this->formula = new_formula;
}*/

Formula::~Formula() {
	delete_formula(this->formula);
}

bool Formula::is_child_universal(_formula* fml) {
    if(fml != NULL) {
        switch(fml->formula_type)
        {
            case ATOM:
                    return true;
            case CONJ:
            case DISJ:
            case IMPL:
                    return is_child_universal(fml->subformula_l) && is_child_universal(fml->subformula_r);
            case NEGA:
            case UNIV:
                    return is_child_universal(fml->subformula_l);
            case EXIS:
                    return false;
            default:
                    assert(0);
        }
    }

    return false;
}

bool Formula::is_universal() {
	return is_child_universal(this->formula);
}

bool Formula::compare_formula(const _formula* phi, const _formula* psi) {
    int k;

    assert(phi);
    assert(psi);

    if (phi->formula_type!=psi->formula_type) return false;

    switch (phi->formula_type)
    {
    case ATOM:
        if (phi->predicate_id!=psi->predicate_id) return false;
        k = vocabulary.predicate_arity(phi->predicate_id) - 1;
        assert(k<0||phi->parameters);
        assert(k<0||psi->parameters);
        for ( ; k>=0; k--)
        {
            if (!compare_term(phi->parameters+k,psi->parameters+k))
                return FALSE;
        }
        return true;
    case UNIV:
    case EXIS:
        if (phi->variable_id!=psi->variable_id) return false;
    case NEGA:
        assert(phi->subformula_l);
        assert(psi->subformula_l);
        return compare_formula(phi->subformula_l,psi->subformula_l);
    case CONJ:
    case DISJ:
    case IMPL:
        assert(phi->subformula_l);
        assert(psi->subformula_l);
        assert(phi->subformula_r);
        assert(psi->subformula_r);
        return (compare_formula(phi->subformula_l,psi->subformula_l)
            && compare_formula(phi->subformula_r,psi->subformula_r));
    default:
        assert(0);
    }
    return false;
}

bool Formula::compare(Formula& ffc) {
	return compare_formula(this->formula, ffc.formula);
}

void Formula::delete_formula ( _formula* fml ) {
    assert ( fml );

    switch ( fml->formula_type )
    {
    case ATOM:
        if(fml->parameters)
	delete_terms(fml->parameters, vocabulary.predicate_arity(fml->variable_id));
        break;
    case CONJ:
    case DISJ:
    case IMPL:
        assert ( fml->subformula_r );
        delete_formula(fml->subformula_r);
    case NEGA:
    case UNIV:
    case EXIS:
        assert ( fml->subformula_l );
        delete_formula(fml->subformula_l);
        break;
    default:
        assert ( 0 );
    }

    free(fml);
}

bool Formula::find_var_formula(const _formula* phi, int var_id) {
    int i;

    assert ( phi );

    switch ( phi->formula_type )
    {
    case ATOM:
        for ( i = 0; i < vocabulary.predicate_arity(phi->predicate_id); i++ )
        {
            assert ( phi->parameters );
            if ( find_var_term ( phi->parameters+i, var_id ) )
                return true;
        }
        return false;
    case NEGA:
    case UNIV:
    case EXIS:
        assert ( phi->subformula_l );
        return find_var_formula ( phi->subformula_l, var_id );
    case CONJ:
    case DISJ:
    case IMPL:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        return (find_var_formula ( phi->subformula_l, var_id )
               && find_var_formula ( phi->subformula_r, var_id ));
    default:
        assert ( 0 );
    }
    return false;
}

bool Formula::find_var(int var_id) {
    return find_var_formula(this->formula, var_id);
}

void Formula::rename_var_formula(_formula* phi, int oldv, int newv) {
    int i;
    assert ( phi );
	
	if(oldv == newv)
		return;

    switch ( phi->formula_type )
    {
    case ATOM:
        assert ( phi->parameters );
        for ( i = 0; i < vocabulary.predicate_arity(phi->predicate_id); i++ )
        {
            rename_var_term ( phi->parameters+i, oldv, newv );
        }
        break;
    case UNIV:
    case EXIS:
        if ( phi->variable_id==oldv ) phi->variable_id = newv;
    case NEGA:
        assert ( phi->subformula_l );
        rename_var_formula ( phi->subformula_l, oldv, newv );
        break;
    default:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        rename_var_formula ( phi->subformula_l, oldv, newv );
        rename_var_formula ( phi->subformula_r, oldv, newv );
    }
}

void Formula::rename_var(int oldv, int newv) {
	rename_var_formula(this->formula, oldv, newv);
}

_formula* Formula::replace_terms_formula(_formula* fml, 
			const vector<int>& exis, const vector<int>& replacements)
{
    assert(fml);
    assert(exis.size() == replacements.size());

    switch(fml->formula_type)
    {
    case ATOM:
        replace_term(fml->parameters, vocabulary.predicate_arity(fml->predicate_id), 
					exis, replacements);
        break;
    case DISJ:
    case CONJ:
    case IMPL:
        replace_terms_formula(fml->subformula_r, exis, replacements);
    case NEGA:
    case UNIV:
    case EXIS:
        replace_terms_formula(fml->subformula_l, exis, replacements);
        break;
    default:
        assert(0);
    }
	
    return fml;//formula after replace_terms, formula do not change here.
}

void Formula::replace_terms(const vector<int>& exis, const vector<int>& replacement) {
	replace_terms_formula(this->formula, exis, replacement);
}

void Formula::output_formula (FILE* out, _formula* phi)
{
    char* s_conn = NULL;
    int i;

    assert ( phi );

    switch ( phi->formula_type )
    {
    case ATOM:
        if(phi->predicate_id >= 0)
        {
            fprintf ( out, "%s(", vocabulary.query_name(phi->predicate_id, PREDICATE));
            for(i = 0; i < vocabulary.predicate_arity(phi->predicate_id); i++)
            {
                if ( i > 0 ) fprintf ( out, ", " );
                output_term ( out, phi->parameters+i );
            }
        }
        else
        {
            switch ( phi->predicate_id )
            {
			case PRED_TRUE:
				fprintf( out, "(TRUE");
				break;
			case PRED_FALSE:
				fprintf( out, "(FALSE");
				break;
            case PRED_EQUALITY:
                fprintf ( out, "(" );
                assert ( phi->parameters );
                output_term ( out, phi->parameters );
                fprintf ( out, " = " );
                output_term ( out, phi->parameters+1 );
                break;
			case PRED_MIN:
				fprintf( out, "MIN(");
				assert( phi->parameters );
				output_term( out, phi->parameters );
				break;
			case PRED_MAX:
				fprintf( out, "MAX(");
				assert( phi->parameters );
				output_term( out, phi->parameters );
				break;
			case PRED_SUCC:
                fprintf ( out, "SUCC(" );
                assert ( phi->parameters );
                output_term ( out, phi->parameters );
                fprintf ( out, "," );
                output_term ( out, phi->parameters+1 );
				break;
            default:
                assert ( 0 );
            }
        }
        fprintf ( out, ")" );
        break;
    case NEGA:
        fprintf ( out, "~ (" );
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        fprintf ( out, ")" );
        break;
    case CONJ:
        s_conn = (char*)" & ";
    case DISJ:
        if ( NULL==s_conn ) s_conn = (char*)" | ";
    case IMPL:
        if ( NULL==s_conn ) s_conn = (char*)" -> ";
        fprintf ( out, "(" );
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        fprintf ( out, "%s", s_conn );
        assert ( phi->subformula_r );
        output_formula ( out, phi->subformula_r );
        fprintf ( out, ")" );
        break;
    case UNIV:
        s_conn = (char*)"!";
    case EXIS:
        if ( NULL==s_conn ) s_conn = (char*)"?";
        fprintf(out, "[%s %s] (", s_conn, vocabulary.query_name(phi->variable_id, VARIABLE));
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        fprintf ( out, ")" );
        break;
    default:
        assert ( 0 );
    }
}

void Formula::divide_clause_formula(_formula* fml, Formulas& result) {
    Formula* new_formula;
    if(fml != NULL) {
        if(fml->formula_type == CONJ && fml->subformula_l->formula_type == CONJ ) {
            new_formula = new Formula(fml->subformula_r);  
            result.push_formula(*new_formula);
            divide_clause_formula(fml->subformula_l, result);
        }
        else {
            new_formula = new Formula(fml);
            result.push_formula(*new_formula);
        }
        
    }
}

void Formula::divide_CNF_formula(_formula* fml, Formulas& result) {
    Formula* new_formula;
    
    if(fml != NULL) {
        if(fml->formula_type == CONJ) {
            divide_CNF_formula(fml->subformula_l, result);
            divide_CNF_formula(fml->subformula_r, result);
        }
        else {
            new_formula = new Formula(fml);
            result.push_formula(*new_formula);
        }
    }
}

Formulas Formula::divide_clause() {
    Formulas* outputFormulas = new Formulas();
    
    divide_clause_formula(this->formula, *outputFormulas);
    
    return *outputFormulas;  
}       

Formulas Formula::divide_CNF() {
    Formulas* outputFormulas = new Formulas();
    
    divide_CNF_formula(this->formula, *outputFormulas);
    
    return *outputFormulas;    
}

void Formula::lower_negative(_formula* srcFormula) {
    if(srcFormula == NULL) {
        return;
    }
    
    switch (srcFormula->formula_type) {
        case NEGA: {
            _formula* subFor = srcFormula->subformula_l;
            if (subFor == NULL) {
                return;
            }
            switch (subFor->formula_type) {
                case NEGA:
                    lower_negative(subFor);
                    if(subFor->subformula_l->formula_type == NEGA) {
                        srcFormula->subformula_l = subFor->subformula_l->subformula_l;
                        free(subFor->subformula_l);
                        free(subFor);                       
                    }
                    if(srcFormula->subformula_l->subformula_l->formula_type != ATOM) {
                        lower_negative(srcFormula);
                    }
                    break;
                case CONJ:
                case DISJ:
                {
                    FORMULA_TYPE ftype = (subFor->formula_type == CONJ ? DISJ : CONJ);

                    // reconstruct the source formula
                    srcFormula->formula_type = ftype;
                    srcFormula->subformula_l = composite_bool(NEGA, subFor->subformula_l, NULL);
                    srcFormula->subformula_r = composite_bool(NEGA, subFor->subformula_r, NULL);

                    free(subFor);
                    lower_negative(srcFormula->subformula_l);
                    lower_negative(srcFormula->subformula_r);
                    break;
                }
                case UNIV:
                case EXIS:
                {
                    FORMULA_TYPE ftype = (subFor->formula_type == UNIV ? EXIS : UNIV);

                    // reconstruct the source formula
                    srcFormula->formula_type = ftype;
                    srcFormula->subformula_l = composite_bool(NEGA, subFor->subformula_l, NULL);
                    srcFormula->variable_id = subFor->variable_id;

                    free(subFor);
                    lower_negative(srcFormula->subformula_l);
                    break;
                }
                case IMPL:
                    /*free(srcFormula);
                    srcFormula = composite_bool(subFor->subformula_l,
                        composite_bool(subFor->subformula_r, NULL, NEGA), 
                        CONJ);
                    free(subFor);
                    lower_negative(srcFormula);*/
                    
                    /*subFor = composite_bool(composite_bool(subFor->subformula_l, NULL, NEGA), subFor->subformula_r, CONJ);
                    lower_negative(srcFormula);*/
                    break;
                default:
                    break;
            }
            break;
        }           
        case CONJ:
        case DISJ:
            lower_negative(srcFormula->subformula_l);
            lower_negative(srcFormula->subformula_r);
            break;
        case UNIV:
        case EXIS:
            lower_negative(srcFormula->subformula_l);
            break;
        case IMPL:
           /* if (!inSM) {
                _formula* tmpl = srcFormula->subformula_l;
                _formula* tmpr = srcFormula->subformula_r;
                free(srcFormula);
                srcFormula = composite_bool(composite_bool(tmpl, NULL, NEGA), 
                                tmpr,
                                DISJ);
                LowerNegative(srcFormula, inSM);
            }*/
            lower_negative(srcFormula->subformula_l);
            lower_negative(srcFormula->subformula_r);
            break;
        case ATOM:
            break;
        default:
            break;
    }
}

void Formula::lower_disjunction(_formula* srcFormula, int depth) {
    if(srcFormula == NULL) {
        return;
    }
    
    switch (srcFormula->formula_type)
    {
        case UNIV:
        case EXIS:
        case NEGA:
            lower_disjunction(srcFormula->subformula_l, depth);
            break;

        case CONJ:
            lower_disjunction(srcFormula->subformula_l, depth);
            lower_disjunction(srcFormula->subformula_r, depth);
            break;
        case DISJ:
            {
                lower_disjunction(srcFormula->subformula_l, depth + 1);
                lower_disjunction(srcFormula->subformula_r, depth + 1);
                
                if(depth < 4) {
                    if (srcFormula->subformula_l->formula_type != CONJ && srcFormula->subformula_r->formula_type == CONJ)
                    {
                        _formula* temp = srcFormula->subformula_l;
                        srcFormula->subformula_l = srcFormula->subformula_r;
                        srcFormula->subformula_r = temp;
                    }
                    if (srcFormula->subformula_l->formula_type == CONJ && srcFormula->subformula_r->formula_type == CONJ)
                    {
                        _formula* tmpl = srcFormula->subformula_l;
                        _formula* tmpr = srcFormula->subformula_r;

                        _formula* t1 = composite_bool(DISJ, tmpl->subformula_l, tmpr->subformula_l);
                        _formula* t2 = composite_bool(DISJ, copy_formula(tmpl->subformula_l), tmpr->subformula_r);
                        _formula* t3 = composite_bool(DISJ, tmpl->subformula_r, copy_formula(tmpr->subformula_l));
                        _formula* t4 = composite_bool(DISJ, copy_formula(tmpl->subformula_r), copy_formula(tmpr->subformula_r));

                        srcFormula->subformula_l = composite_bool(CONJ, t1, t2);
                        srcFormula->subformula_r = composite_bool(CONJ, t3, t4);

                        srcFormula->formula_type = CONJ;

                        free(tmpl);
                        free(tmpr);
                        lower_disjunction(srcFormula->subformula_l, depth + 1);
                        lower_disjunction(srcFormula->subformula_r, depth + 1);
                    }
                    else if (srcFormula->subformula_l->formula_type == CONJ && srcFormula->subformula_r->formula_type != CONJ)
                    {
                        _formula* tmpl = srcFormula->subformula_l;
                        _formula* tmpr = srcFormula->subformula_r;

                        srcFormula->formula_type = CONJ;

                        srcFormula->subformula_l = composite_bool(DISJ, tmpl->subformula_l, 
                                                                    tmpr);
                        srcFormula->subformula_r = composite_bool(DISJ, tmpl->subformula_r,
                                                                    copy_formula(tmpr));
                        free(tmpl);
                        lower_disjunction(srcFormula->subformula_l, depth + 1);
                        lower_disjunction(srcFormula->subformula_r, depth + 1);
                    }
                }
            }
            break;
        case IMPL:
            lower_disjunction(srcFormula->subformula_l, depth);
            lower_disjunction(srcFormula->subformula_r, depth);
        case ATOM:
            break;
        default:
            break;
    }
}

void Formula::convert_CNF() {
    lower_negative(this->formula);
    lower_disjunction(this->formula, 0);
}

void Formula::convert_pernex_formula(_formula* fml) {
    if(fml == NULL) {
        return;
    }
    
    switch(fml->formula_type) {
        case ATOM:
            break;
        case NEGA:
            convert_pernex_formula(fml->subformula_l);
            
            while(fml->subformula_l->formula_type == EXIS || fml->subformula_l->formula_type == UNIV) {
                FORMULA_TYPE flt = (fml->subformula_l->formula_type == UNIV) ? EXIS : UNIV;
                fml->formula_type = flt;
                fml->variable_id = fml->subformula_l->variable_id;
                fml->subformula_l->formula_type = NEGA;
                
                fml = fml->subformula_l;
            }
            break;
        case IMPL:
        case CONJ:
        case DISJ:
            
    }
}

void Formula::convert_prenex() {
    
}

void Formula::output(FILE* out) {
	output_formula(out, this->formula);
}