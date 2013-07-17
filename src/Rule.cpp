#include "Rule.h"
#include "Formula.h"
#include "Vocabulary.h"
#include "utility.h"
#include <cstdlib>

Rule::Rule(Formula cabalar) {
    convert_formula_rule(cabalar.get_formula());
    asp_modify();
}

Rule::~Rule() {
    
}

void Rule::divide_body(_formula* body) {
    if(body->formula_type != CONJ) {
        _formula* body_part = copy_formula(body);
        this->body.push_back(body_part);
    }
    else {
        divide_body(body->subformula_l);
        divide_body(body->subformula_r);
    }
}

void Rule::divide_head(_formula* head) {
    if(head->formula_type != DISJ) {
        _formula* head_part = copy_formula(head);
        this->head.push_back(head_part);
    }
    else {
        divide_head(head->subformula_l);
        divide_head(head->subformula_r);
    }    
}

void Rule::convert_formula_rule(_formula* cabalar) {
    if(cabalar == NULL) {
        return;
    }
    
    switch(cabalar->formula_type) {
        case IMPL:
            divide_head(cabalar->subformula_r);
            divide_body(cabalar->subformula_l);
            break;
        case NEGA:
        case DISJ:
        case ATOM:
            divide_head(cabalar);
            break;
        default:
            break;
    }
}

void Rule::asp_modify() {
    for(vector<_formula*>::iterator iter = head.begin(); iter != head.end(); iter++) {
        _formula* head_part = *iter;
        _formula* cur = head_part;
        
        while(cur->formula_type != ATOM) {
            cur = cur->subformula_l;
        }
        if(head_part->formula_type == NEGA || !vocabulary.is_intension_predicate(cur->predicate_id)) {
            head_part = composite_bool(NEGA, head_part, NULL);
            head.erase(iter);            
            body.push_back(head_part);
            iter--;
        }               
    }
    
    for(vector<_formula*>::iterator iter = body.begin(); iter != body.end(); iter++) {
        _formula* body_part = *iter;
        _formula* cur = body_part;
        
        while(cur->formula_type == NEGA && cur->subformula_l->formula_type != ATOM 
                && cur->subformula_l->subformula_l->formula_type != ATOM) {
            _formula* sub = cur->subformula_l;
            if(sub->formula_type == NEGA && sub->subformula_l->formula_type == NEGA) {
                cur->subformula_l = sub->subformula_l->subformula_l;
                free(sub);
                free(sub->subformula_l);
            }
        }
    }
}

void Rule::output(FILE* out) {
    for(vector<_formula*>::iterator iter = head.begin(); iter != head.end(); iter++) {
        _formula* head_part = *iter;
        
        if(head_part->formula_type == ATOM && head_part->predicate_id != PRED_FALSE 
                && head_part->predicate_id != PRED_TRUE) {
            printAtom(head_part, out);
            
            if(iter != head.end() - 1) fprintf(out, "|");
        }               
    }
    
    bool body_begin = true;
    
    for(vector<_formula*>::iterator iter = body.begin(); iter != body.end(); iter++) {
        _formula* body_part = *iter;
        _formula* cur = body_part;
        
        while(cur->formula_type != ATOM) {
            cur = cur->subformula_l;
        }
        
        if(cur->predicate_id != PRED_TRUE && cur->predicate_id != PRED_FALSE) {
            if(body_begin) {
                fprintf(out, ":-");
                body_begin = false;
            }
            
            if(body_part->formula_type == NEGA) {
                fprintf(out, "not ");
                body_part = body_part->subformula_l;
                if(body_part->formula_type == NEGA) {
                    nega_atoms.push_back(body_part->subformula_l);
                    fprintf(out, "_");
                }
            }            
            printAtom(cur, out);
            if(iter != body.end() - 1) fprintf(out, ", "); 
        }                      
    }  
    
    fprintf(out, ".\n"); 
    
    for(vector<_formula*>::iterator iter = nega_atoms.begin(); iter != nega_atoms.end(); iter++) {
        fprintf(out, ":-");
        printAtom(*iter, out);
        fprintf(out, ", _");
        printAtom(*iter, out);
        fprintf(out, ".\n");
    }    
}

void Rule::printAtom(_formula* atom, FILE* out) {
    fprintf(out, "%s", vocabulary.query_name(atom->predicate_id, PREDICATE));

    if(atom->parameters != NULL) {
        _term* ft = atom->parameters;
        int ftc = vocabulary.predicate_arity(atom->predicate_id);

        for(int i = 0; i < ftc; i++) {
            if(i == 0)
            fprintf(out, "(%s", vocabulary.query_name(ft[i].variable_id, VARIABLE));
            else 
            fprintf(out, ",%s", vocabulary.query_name(ft[i].variable_id, VARIABLE));                       
        }
        fprintf(out, ")");
    }    
}

bool Rule::isUseless() {
    for(vector<_formula*>::iterator hit = head.begin(); hit != head.end(); hit++) {
        for(vector<_formula*>::iterator bit = body.begin(); bit != body.end(); bit++) {
            if(compare_formula(*hit, *bit)) {
                return true;
            }
        }
    }
    
    return false;
}