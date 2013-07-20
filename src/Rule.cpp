#include "Rule.h"
#include "Formula.h"
#include "Vocabulary.h"
#include "utility.h"
#include "S2DLP.h"
#include <cstdlib>

Rule::Rule(Formula cabalar) {
    convert_formula_rule(cabalar.get_formula());
    asp_modify();
}

Rule::~Rule() {
      
}

Rule::Rule(const Rule& rhs) {
    this->head = rhs.head;
    this->body = rhs.body;
    this->nega_atoms = rhs.nega_atoms;
}

Rule& Rule::operator = (const Rule& rhs) {
    this->head = rhs.head;
    this->body = rhs.body;
    this->nega_atoms = rhs.nega_atoms;
}

void Rule::divide_body(_formula* body) {
    if(body->formula_type != CONJ) {
        this->body.push_back(Formula(body, true));
    }
    else {
        divide_body(body->subformula_l);
        divide_body(body->subformula_r);
    }
}

void Rule::divide_head(_formula* head) {
    if(head->formula_type != DISJ) {
        this->head.push_back(Formula(head, true));
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
    for(vector<Formula>::iterator iter = head.begin(); iter != head.end(); iter++) {
        _formula* head_part = iter->get_formula();
        _formula* cur = head_part;
        
        while(cur->formula_type != ATOM) {
            cur = cur->subformula_l;
        }
        if(head_part->formula_type == NEGA || (!vocabulary.is_intension_predicate(cur->predicate_id) && cur->predicate_id > 0)) {
            _formula* new_head_part = composite_bool(NEGA, copy_formula(head_part), NULL);
            head.erase(iter);            
            body.push_back(Formula(new_head_part, false));
            iter--;
        }               
    }
    
    for(vector<Formula>::iterator iter = body.begin(); iter != body.end(); iter++) {
        _formula* body_part = iter->get_formula();
        _formula* cur = body_part;
        
        while(cur->formula_type != ATOM) {
            cur = cur->subformula_l;
        }
        
       /* if(!vocabulary.is_intension_predicate(cur->predicate_id)) {
            while(body_part->formula_type == NEGA && body_part->subformula_l->formula_type != ATOM) {
                body_part = body_part->subformula_l->subformula_l;
            }
           // iter->set_formula(copy_formula(body_part));
        }
        else {*/
            while(body_part->formula_type == NEGA && body_part->subformula_l->formula_type != ATOM 
                && body_part->subformula_l->subformula_l->formula_type != ATOM) {
                _formula* sub = body_part->subformula_l;
                if(sub->formula_type == NEGA && sub->subformula_l->formula_type == NEGA) {
                    body_part->subformula_l = sub->subformula_l->subformula_l;
                    free(sub);
                    free(sub->subformula_l);
                }
            }
       // }
    }
}

void Rule::output(FILE* out) {
    for(vector<Formula>::iterator iter = head.begin(); iter != head.end(); iter++) {
        _formula* head_part = iter->get_formula();
        
        if(head_part->formula_type == ATOM && head_part->predicate_id != PRED_FALSE 
                && head_part->predicate_id != PRED_TRUE) {
            printAtom(head_part, out);       
            if(iter != head.end() - 1 && (iter+1)->get_formula()->predicate_id != PRED_FALSE &&
                (iter+1)->get_formula()->predicate_id != PRED_TRUE) fprintf(out, "|");
        }        
    }
    
    bool body_begin = true;
    
    for(vector<Formula>::iterator iter = body.begin(); iter != body.end(); iter++) {
        _formula* body_part = iter->get_formula();
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
                    bool exis = false;
                    for(vector<Formula>::iterator it = S2DLP::instance().nega_predicates.begin(); 
                            it != S2DLP::instance().nega_predicates.end();it++) {
                        if((it->get_formula())->predicate_id == body_part->subformula_l->predicate_id)
                            exis = true;
                    }
                    if(!exis) {
                        S2DLP::instance().nega_predicates.push_back(Formula(body_part->subformula_l, true));  
                    }
                    fprintf(out, "_");
                }
            }            
            printAtom(cur, out);
            if(iter != body.end() - 1 && (iter+1)->get_formula()->predicate_id != PRED_FALSE &&
                (iter+1)->get_formula()->predicate_id != PRED_TRUE) 
                fprintf(out, ",");            
        }
        fflush(out);
    }  
    
    fprintf(out, ".\n");    
    fflush(out);
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
    for(vector<Formula>::iterator hit = head.begin(); hit != head.end(); hit++) {
        for(vector<Formula>::iterator bit = body.begin(); bit != body.end(); bit++) {
            if(compare_formula(hit->get_formula(), bit->get_formula())) {
                return true;
            }
        }
    }
    
    return false;
}