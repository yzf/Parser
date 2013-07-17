#include "Formula.h"
#include "Formulas.h"
#include "utility.h"
#include <assert.h>
#include <cstdlib>

Formula::Formula() {
    this->formula = NULL;
    this->is_prenex_formula = false;
}

Formula::Formula(_formula* fml, bool copy) {
    if(!copy) {
        this->formula = fml;
    }    
    else {
        _formula* f = copy_formula(fml);
        this->formula = f;
    }

    this->is_prenex_formula = false;
}

Formula::Formula(const Formula& FML) {
    _formula* new_formula = copy_formula(FML.formula);
    this->formula = new_formula;
    
 
    this->is_prenex_formula = false;
}

Formula::~Formula() {
    if (this->formula != NULL) {
        delete_formula(this->formula);
    }
    
}

_formula* Formula::get_formula()
{
    return this->formula;
}
        
void Formula::set_formula(_formula* f)
{
    this->formula = f;
}

Formula& Formula::operator = (const Formula& rhs) {
    _formula* new_formula = copy_formula(rhs.formula);
    this->formula = new_formula;
    
   
    this->is_prenex_formula = false;
    return *this;
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

void Formula::delete_formula( _formula* fml ) {
    assert ( fml );

    switch ( fml->formula_type )
    {
    case ATOM:
        if(fml->parameters)
	delete_terms(fml->parameters, vocabulary.predicate_arity(fml->predicate_id));
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

void Formula::remove_from_prenex(_formula* parent, int d, _formula* tag) {    
    _formula* sf;
    
    if(d == 0) sf = parent->subformula_l;
    else sf = parent->subformula_r;
    
    if(sf == NULL || (sf->formula_type != UNIV && sf->formula_type != EXIS)) {
        return;
    }
    
    if(sf == tag) {
        if(d == 0) parent->subformula_l = sf->subformula_l;
        else parent->subformula_r = sf->subformula_l;
        free(sf);
        remove_from_prenex(sf, 0, tag);
    }
    else {
        remove_from_prenex(sf, 0, tag);
    }
    
}

_formula* Formula::find_prenex_quanlifier(_formula* fml, int var_id) {
    if(fml == NULL || (fml->formula_type != UNIV && fml->formula_type != EXIS)) {
        return NULL;
    }
    if(fml->variable_id == var_id) {
        return fml;
    }
    else if(fml->subformula_l->formula_type == UNIV || fml->subformula_l->formula_type == EXIS) {
        return find_prenex_quanlifier(fml->subformula_l, var_id);
    }
    else {
        return NULL;
    }
}

bool Formula::find_var_formula(_formula* phi, int var_id) {
    int i;

    assert ( phi );

    switch ( phi->formula_type )
    {
    case ATOM:
        for ( i = 0; i < vocabulary.predicate_arity(phi->predicate_id); i++ )
        {
            assert ( phi->parameters );
            if ( find_var_term ( phi->parameters+i, var_id ) )
                return phi;
        }
        return NULL;
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
    return NULL;
}

bool Formula::find_var(int var_id) {
    return find_var_formula(this->formula, var_id);
}

void Formula::rename_node_formula(_formula* tag) {   
    tag->variable_id = vocabulary.add_rename_variable();
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
        if(phi->predicate_id >= 0 && vocabulary.predicate_arity(phi->predicate_id) == 0)
            fprintf ( out, "%s", vocabulary.query_name(phi->predicate_id, PREDICATE));
        else{
        if(phi->predicate_id >= 0)
        {
            fprintf ( out, "%s(", vocabulary.query_name(phi->predicate_id, PREDICATE));
            for(i = 0; i < vocabulary.predicate_arity(phi->predicate_id); i++)
            {
                if ( i > 0 ) fprintf ( out, "," );
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
                fprintf ( out, "=" );
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
        }
        break;
    case NEGA:
        fprintf ( out, "~" );
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        break;
    case CONJ:
        s_conn = (char*)"&";
    case DISJ:
        if ( NULL==s_conn ) s_conn = (char*)"|";
    case IMPL:
        if ( NULL==s_conn ) s_conn = (char*)"->";
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
        fprintf(out, "[%s%s](", s_conn, vocabulary.query_name(phi->variable_id, VARIABLE));
        assert ( phi->subformula_l );
        output_formula ( out, phi->subformula_l );
        fprintf ( out, ")" );
        break;
    default:
        assert ( 0 );
    }
}

void Formula::divide_clause_formula(_formula* fml, _formula* parent, Formulas& result) {
    if(fml != NULL) {
        if((parent == NULL || parent->formula_type == CONJ) && fml->formula_type == CONJ) {
            
            Formula new_formula = Formula(fml->subformula_r, true);  
            result.push_formula(new_formula);
            divide_clause_formula(fml->subformula_l, fml, result);   
        }
        else {
            Formula new_formula = Formula(fml, true);
            result.push_formula(new_formula);
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
            new_formula = new Formula(fml, true);
            result.push_formula(*new_formula);
        }
    }
}

Formulas Formula::divide_clause() {
    Formulas* outputFormulas = new Formulas();
    divide_clause_formula(this->formula, NULL, *outputFormulas);
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
                    subFor = srcFormula->subformula_l;
                    if(subFor->formula_type == NEGA && subFor->subformula_l->formula_type == NEGA) {
                        srcFormula->subformula_l = subFor->subformula_l->subformula_l;
                        free(subFor->subformula_l);
                        free(subFor);                       
                    }
                    if(srcFormula->subformula_l->formula_type != ATOM && srcFormula->subformula_l->subformula_l->formula_type != ATOM) {
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

                    srcFormula->formula_type = ftype;
                    srcFormula->subformula_l = composite_bool(NEGA, subFor->subformula_l, NULL);
                    srcFormula->variable_id = subFor->variable_id;

                    free(subFor);
                    lower_negative(srcFormula->subformula_l);
                    break;
                }
                case IMPL:
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

void Formula::convert_prenex_formula(_formula* fml) {
    if(fml == NULL) {
        return;
    }
    
    switch(fml->formula_type) {
        case ATOM:
            break;
        case NEGA:
            convert_prenex_formula(fml->subformula_l);
            
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
        case DISJ: {
            convert_prenex_formula(fml->subformula_l);
            convert_prenex_formula(fml->subformula_r);
            
            _formula* sub_l = fml->subformula_l;
            _formula* sub_r = fml->subformula_r;
            _formula* curr_fml = fml;                      
            
            while(sub_l->formula_type == UNIV || sub_l->formula_type == EXIS || 
                    sub_r->formula_type == UNIV || sub_r->formula_type == EXIS) {
                
                if(curr_fml->formula_type == IMPL && (sub_l->formula_type == EXIS 
                        || sub_l->formula_type == UNIV)) {
                    sub_l->formula_type = (sub_l->formula_type == UNIV) ? EXIS : UNIV;
                }
                FORMULA_TYPE priority_type = UNIV;
            
                if(sub_l->formula_type != priority_type && sub_r->formula_type != priority_type) {
                    priority_type = EXIS;
                }
                if(sub_l->formula_type == priority_type || sub_l->formula_type == sub_r->formula_type) {
                    _formula* same_variable = find_prenex_quanlifier(sub_r, sub_l->variable_id);
                    
                    if(same_variable != NULL) {
                        if((sub_l->formula_type == UNIV && curr_fml->formula_type == CONJ && same_variable->formula_type == UNIV)||
                                (sub_l->formula_type == EXIS && curr_fml->formula_type == DISJ && same_variable->formula_type == EXIS)) {
                            remove_from_prenex(curr_fml, 1, same_variable);
                        }
                        else {
                            int new_id = vocabulary.add_rename_variable();
                            rename_var_formula(sub_r, sub_l->variable_id, new_id);
                        }
                    }
                    
                    FORMULA_TYPE temp = curr_fml->formula_type;
                    sub_r = curr_fml->subformula_r;
                    curr_fml->formula_type = sub_l->formula_type;
                    curr_fml->variable_id = sub_l->variable_id;
                    sub_l->formula_type = temp;
                    sub_l->subformula_r = sub_r;
                }
                else {
                    _formula* same_variable = find_prenex_quanlifier(sub_l, sub_r->variable_id);
                    
                    if(same_variable != NULL) {
                        if((sub_r->formula_type == UNIV && curr_fml->formula_type == CONJ && same_variable->formula_type == UNIV)||
                                (sub_r->formula_type == EXIS && curr_fml->formula_type == DISJ && same_variable->formula_type == EXIS)) {
                            remove_from_prenex(curr_fml, 0, same_variable);
                        }
                        else {
                            int new_id = vocabulary.add_rename_variable();
                            rename_var_formula(sub_r, sub_l->variable_id, new_id);
                        }
                    }
                    
                    FORMULA_TYPE temp = curr_fml->formula_type;
                    sub_l = curr_fml->subformula_l;
                    _formula* r_sub_l = sub_r->subformula_l;
                    curr_fml->formula_type = sub_r->formula_type;
                    curr_fml->variable_id = sub_r->variable_id;
                    curr_fml->subformula_l = sub_r;
                    sub_r->formula_type = temp;
                    sub_r->subformula_l = sub_l;
                    sub_r->subformula_r = r_sub_l;
                }
                curr_fml = curr_fml->subformula_l;
                sub_l = curr_fml->subformula_l;
                sub_r = curr_fml->subformula_r;
            }
        }
            break;
        case UNIV:
        case EXIS:
            convert_prenex_formula(fml->subformula_l);
            break;
        default:
            break;           
    }
}

void Formula::convert_prenex() {
    convert_prenex_formula(this->formula);
    this->is_prenex_formula = true;
   
}

bool Formula::is_prenex() {
    return this->is_prenex_formula;
}

//_formula* Formula::getFormula() {
//    return this->formula;
//}

void Formula::output(FILE* out) {
    output_formula(out, this->formula);
    fprintf(out, "\n");
}

bool Formula::is_negative (const int* sm_preds, int num_sp, bool negative ) {
    return is_negative_formula(this->formula, sm_preds, num_sp, negative);
}

bool Formula::in_list ( const int* list, int len, int obj )
{
    for ( len--; len>=0; len-- )
        if ( list[len]==obj ) return true;
    return false;
}

bool Formula::is_negative_formula(const _formula* phi, 
		const int* sm_preds, int num_sp, bool negative) {
    assert ( phi );

    switch ( phi->formula_type )
    {
    case ATOM:
        if ( negative || !in_list ( sm_preds, num_sp, phi->predicate_id ) )
            return true;
        break;
    case NEGA:
        assert ( phi->subformula_l );
        return is_negative_formula( phi->subformula_l, sm_preds, num_sp, !negative );
    case CONJ:
    case DISJ:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        return (is_negative_formula( phi->subformula_l, sm_preds, num_sp, negative ) &&
               is_negative_formula( phi->subformula_r, sm_preds, num_sp, negative ));
    case IMPL:
        assert ( phi->subformula_l );
        assert ( phi->subformula_r );
        return (is_negative_formula( phi->subformula_l, sm_preds, num_sp, !negative ) &&
               is_negative_formula( phi->subformula_r, sm_preds, num_sp, negative ));
    case UNIV:
    case EXIS:
        assert ( phi->subformula_l );
        return is_negative_formula( phi->subformula_l, sm_preds, num_sp, negative );
    default:
        assert ( 0 );
    }

    return false;
}

_formula* Formula::double_negation_formula(_formula* phi, const int* int_preds, int num_ip) {
    assert(phi);

    if (is_negative_formula(phi, int_preds, num_ip, false))
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
		phi->subformula_r = double_negation_formula(phi->subformula_r,int_preds,num_ip);
    case NEGA:
    case UNIV:
    case EXIS:
		phi->subformula_l = double_negation_formula(phi->subformula_l,int_preds,num_ip);
        break;

    default:
        assert ( 0 );
    }

    return phi;
}

void Formula::double_negation(const int* int_preds, int num_ip) {
    this->formula = double_negation_formula(this->formula, int_preds, num_ip);
}
