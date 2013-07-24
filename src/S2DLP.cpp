#include "S2DLP.h"
#include "cstring"
#include <assert.h>

S2DLP::S2DLP() : output_file(NULL){
}

S2DLP::S2DLP(const S2DLP& rhs) {
    this->dlp_formulas = rhs.dlp_formulas;
    this->dlp_rules = rhs.dlp_rules;
    this->nega_predicates = rhs.nega_predicates;
    this->origin_formulas = rhs.origin_formulas;
    this->output_file = rhs.output_file;
    this->zhangheng_formulas = rhs.zhangheng_formulas;
}

S2DLP& S2DLP::operator = (const S2DLP& rhs) {
    this->dlp_formulas = rhs.dlp_formulas;
    this->dlp_rules = rhs.dlp_rules;
    this->nega_predicates = rhs.nega_predicates;
    this->origin_formulas = rhs.origin_formulas;
    this->output_file = rhs.output_file;
    this->zhangheng_formulas = rhs.zhangheng_formulas;
    return *this;
}

S2DLP::~S2DLP() {
    if (this->output_file != NULL) {
        fclose(this->output_file);
    }
}

S2DLP& S2DLP::instance() {
    static S2DLP the_instance;
    return the_instance;
}

void S2DLP::convert() {
    this->zhangheng_formulas = HengZhang::instance().create(this->origin_formulas);
    this->dlp_formulas = Cabalar::instance().convert_Cabalar(this->zhangheng_formulas);
    Formulas cabalar_result = this->dlp_formulas;
    while (cabalar_result.size_formulas() > 0) {
        Formula cur_fml = cabalar_result.top_formula();
        cabalar_result.pop_formula();
        Rule rule(cur_fml);
        this->dlp_rules.push_back(rule);
    }
}

void S2DLP::set_output_file(FILE*& f) {
    this->output_file = f;
}
void S2DLP::output_origin_formulas() {
    assert(this->output_file != NULL);
    fprintf(this->output_file, "origin_formulas:\n");
    this->origin_formulas.output_formulas(this->output_file);
}
void S2DLP::output_zhangheng_formulas() {    
    assert(this->output_file != NULL);
    fprintf(this->output_file, "zhangheng_formulas:\n");
    this->zhangheng_formulas.output_formulas(this->output_file);
//    this->zhangheng_formulas.output_formulas(stdout);
}
void S2DLP::output_dlp_formulas() {   
    assert(this->output_file != NULL);
    fprintf(this->output_file, "dlp_formulas:\n");
    this->dlp_formulas.output_formulas(this->output_file);
}
void S2DLP::set_origin_formulas(_formula* input) {
    Formula* origin = new Formula(input, false);
    this->origin_formulas = origin->divide_clause();
}
void S2DLP::output_addition() {
    fprintf(this->output_file, "%%MIN and MAX domain\n");
    for(int i = 0; i < vocabulary.num_names_domain; i++) {
        fprintf(this->output_file, "#domain min_%s(MIN_%s).\n", vocabulary.names_domain[i], vocabulary.names_domain[i]);
        fprintf(this->output_file, "#domain max_%s(MAX_%s).\n", vocabulary.names_domain[i], vocabulary.names_domain[i]);
    }
    
    fprintf(this->output_file, "%%Variable domain\n");
    for(int i = 0; i < vocabulary.num_variable; i++) {
        string str = vocabulary.names_variable[i];
        if (str.find("MIN") == string::npos && 
                str.find("MAX") == string::npos) {
            fprintf(this->output_file, "#domain %s(%s).\n", vocabulary.names_domain[vocabulary.variable_at_domain[i]], vocabulary.names_variable[i]);
        }
    }
    
//    fprintf(this->output_file, "%%Addition rule for not intension\n");
//    for(int i = 0; i < vocabulary.num_predicate; i++) {
//        if(!vocabulary.is_intension_predicate(i)) {
//            fprintf(this->output_file, "%s", vocabulary.names_predicate[i]);
//            int arties = vocabulary.predicate_arity(i);
//            for(int j = 0; j < arties; j++) {
//                if(j == 0) fprintf(this->output_file, "(");
//                if(j != arties - 1) fprintf(this->output_file, "%c,", 'A' + j);
//                else fprintf(this->output_file, "%c)", 'A' + j);
//            }
//            fprintf(this->output_file, "|_%s", vocabulary.names_predicate[i]);
//            for(int j = 0; j < arties; j++) {
//                if(j == 0) fprintf(this->output_file, "(");
//                if(j != arties - 1) fprintf(this->output_file, "%c,", 'A' + j);
//                else fprintf(this->output_file, "%c)", 'A' + j);
//            }
//        }
//    }
    
    fprintf(this->output_file, "\n%%Succ predicate definition\n");
    
    for(int i = 0; i < HengZhang::instance().domain_names.size(); i++) {
        addSucc(HengZhang::instance().domain_names.at(i));
    }    
}

void S2DLP::addSucc(vector<string> domains) {
    int size = domains.size();
    
    if(size == 1) {
        fprintf(this->output_file, "succ");
        for(int i = 0; i < size; i++) {
            fprintf(this->output_file, "_%s", domains.at(i).c_str());
        }
        fprintf(this->output_file, "(A1, A2):-A1==A2-1,%s(A1),%s(A2).\n", domains.at(0).c_str(), domains.at(0).c_str());
    }
    else {
        for(int i = 0; i < size; i++) {
            fprintf(this->output_file, "succ");
            for(int j = 0; j < size; j++) {
                fprintf(this->output_file, "_%s", domains.at(j).c_str());
            }
            fprintf(this->output_file, "(");
            for(int j = 0; j < size; j++) {
                if(j >= size - i) fprintf(this->output_file, "MAX_%s,", domains.at(j).c_str());
                else if(j == size - i - 1) fprintf(this->output_file, "%c1,", 'A' + j);
                else fprintf(this->output_file, "%c,", 'A' + j); 
            }
            for(int j = 0; j < size; j++) {
                if(j >= size - i) fprintf(this->output_file, "MIN_%s", domains.at(j).c_str());
                else if(j == size - i - 1) fprintf(this->output_file, "%c2", 'A' + j);
                else fprintf(this->output_file, "%c", 'A' + j);
                if(j == size - 1) fprintf(this->output_file, ")");
                else fprintf(this->output_file, ",");
            }
            
            bool exis = false;
            for(int j = 0; j < HengZhang::instance().domain_names.size(); j++) {
                if(HengZhang::instance().domain_names.at(j).size() == 1 && HengZhang::instance().domain_names.at(j).at(0) == domains[size - j - 1]) {
                    exis = true;
                }
            }
            if(!exis) {
                vector<string> d;
                d.push_back(domains[size - i - 1]);
                HengZhang::instance().domain_names.push_back(d);
            }
            fprintf(this->output_file, ":-succ_%s(%c1,%c2),", domains.at(size - i - 1).c_str(), 'A' + size - i - 1, 'A' + size - i - 1);
            
            for(int j = 0; j < size - i; j++) {
                if(j == size - i - 1)
                    fprintf(this->output_file, "%s(%c1),%s(%c2).", domains.at(j).c_str(), 'A' + j, domains.at(j).c_str(), 'A' + j);
                else
                    fprintf(this->output_file, "%s(%c),", domains.at(j).c_str(), 'A' + j);            
            }
            
            fprintf(this->output_file, "\n");
        }      
    }
}

void S2DLP::printAtom(_formula* atom, FILE* out) {
    fprintf(out, "%s", vocabulary.query_name(atom->predicate_id, PREDICATE));

    if(atom->parameters != NULL) {
        _term* ft = atom->parameters;
        int ftc = vocabulary.predicate_arity(atom->predicate_id);

        for(int i = 0; i < ftc; i++) {
            if(i == 0)
            fprintf(out, "(%s", vocabulary.query_name(ft[i].variable_id, VARIABLE));
            else 
            fprintf(out, ",%s", vocabulary.query_name(ft[i].variable_id, VARIABLE));
            if (i == ftc - 1) {
                fprintf(out, ")");
            }
        }
        
    }    
}

void S2DLP::output_asp() {
    assert(this->output_file != NULL);
    output_addition();
    
    for(vector<Rule>::iterator it = this->dlp_rules.begin(); it != this->dlp_rules.end(); it++) {
        (*it).output(this->output_file);
    }
    
    fprintf(this->output_file, "%%NEW variable define\n");
    for(vector<Formula>::iterator iter = S2DLP::instance().nega_predicates.begin(); 
            iter != S2DLP::instance().nega_predicates.end(); iter++) {
        fprintf(this->output_file, "_");
        printAtom(iter->get_formula(), this->output_file);
        fprintf(this->output_file, ":- not ");
        printAtom(iter->get_formula(), this->output_file);        
        fprintf(this->output_file, ".\n");
    }
    
    for(int i = 0 ; i < vocabulary.atom_list.size(); i++) {
        if(!vocabulary.is_intension_predicate(vocabulary.atom_list.at(i)->predicate_id)) {
            fprintf(this->output_file, "_");
            printAtom(vocabulary.atom_list.at(i), this->output_file);
            fprintf(this->output_file, " :- not ");
            printAtom(vocabulary.atom_list.at(i), this->output_file);
            fprintf(this->output_file, ".\n");
            
            printAtom(vocabulary.atom_list.at(i), this->output_file);
            fprintf(this->output_file, " | _");
            printAtom(vocabulary.atom_list.at(i), this->output_file);
            fprintf(this->output_file, ".\n");
        }
    }
}