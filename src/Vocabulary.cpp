#include "Vocabulary.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include "utility.h"
#include <iostream>


Vocabulary::Vocabulary() {
    memset(this->names_variable, 0, sizeof(char));
    memset(this->variable_at_domain, 0, sizeof(int));
    memset(this->arities_function, 0, sizeof(int));
    memset(this->names_function, 0, sizeof(char));
    memset(this->arities_predicate, 0, sizeof(int));
    memset(this->names_predicate, 0, sizeof(char));
    memset(this->index_intension_predicate, -1, sizeof(int));
    memset(this->names_domain, 0, sizeof(char));
    memset(this->predicate_in_vary, -1, sizeof(int)*MAX_NUM_PREDICATE);
    
    this->num_variable = 0;
    this->num_function = 0;
    this->num_predicate = 0;	
    this->num_new_variable = 0;
    this->num_new_predicate = 0;
    this->num_intension_predicate = 0;
    this->num_names_domain = 0;
    this->newNexName = 0;
}

Vocabulary::~Vocabulary() {
    int i;

    for (i = 0; i < this->num_variable; i++) {
        free(this->names_variable[i]);
    }
    for (i = 0; i < this->num_function; i++) {
        free(this->names_function[i]);
    }
    for (i = 0; i < this->num_predicate; i++) {
        free(this->names_predicate[i]);
    }
}

int Vocabulary::add_symbol ( const char* name, SYMBOL_TYPE type, int arity )
{
	// return value: 1. >=0 - id; 2. -1 - too many symbols; 3. -2 - type error
    char* s;
    int id;
    assert (name);
    
    if((id = query_symbol(name, type)) < 0) {
        s = (char*)malloc( (strlen(name) + 1) *sizeof(char) );
        strcpy (s, name);

        switch (type)
        {
            case VARIABLE:
                if (this->num_variable >= MAX_NUM_VARIABLE) break;
                this->names_variable[this->num_variable] = s;
                return (this->num_variable )++;
            case FUNCTION:
                if (this->num_variable >= MAX_NUM_FUNCTION) break;
                this->names_function[this->num_function] = s;
                this->arities_function[this->num_function] = arity;
                return (this->num_function)++;
            case PREDICATE:
                if (this->num_variable >= MAX_NUM_PREDICATE) break;
                this->names_predicate[this->num_predicate] = s;
                this->arities_predicate[this->num_predicate] = arity;
                return (this->num_predicate)++;
        default:
            assert ( 0 );
            return -2;
        }
    }
    
    return id;
}

int Vocabulary::add_rename_variable(const char* prefix) {
    char name_buf[512];
    sprintf(name_buf, "%s%i", prefix, this->newNexName++);
    
    while(query_symbol(name_buf,VARIABLE) >= 0) {
        this->newNexName++;
    }
    
    return add_symbol(name_buf,VARIABLE,0);
}

int Vocabulary::query_symbol ( const char* name, SYMBOL_TYPE type )
{
	//return value: 1. >=0 id; 2. -1 no such symbol; 3. -2 type error
    int i, n;
    char **ps;

    assert(name);

    switch(type)
    {
        case VARIABLE:
            n = this->num_variable;
            ps = this->names_variable;
            break;
        case FUNCTION:
            n = this->num_function;
            ps = this->names_function;
            break;
        case PREDICATE:
            n = this->num_predicate;
            ps = this->names_predicate;
            break;
        case DOMAIN:
            n = this->num_names_domain;
            ps = this->names_domain;
            break;
        case VARY:
            n = this->num_names_vary;
            ps = this->names_vary;
            break;
    default:
        assert ( 0 );
        return -2;
    }

    for(i=0; i<n; i++)
    {
        if ( 0 == strcmp(name, ps[i]) )
		{
            return i;
		}
    }

    return -1;
}

bool Vocabulary::is_intension_predicate(int var_id) {
    for(int i = 0; i < this->num_intension_predicate; i++) {
        if(this->index_intension_predicate[i] == var_id) {
            return true;
        }
    }
    return false;
}

bool Vocabulary::is_vary_predicate(int id)
{
    if(this->predicate_in_vary[id] > -1)
        return true;
    else
        return false;
}

int Vocabulary::set_intension_predicate(const char* name) {
    int id = query_symbol(name, PREDICATE);
    
    this->index_intension_predicate[this->num_intension_predicate] = id;
    
    return (this->num_intension_predicate)++;
}

void Vocabulary::set_domain(char* variable, char* domain) {
    int id = query_symbol(domain, DOMAIN);
    int vid;
    
    if(id == -1) {
        this->names_domain[this->num_names_domain] = domain;
        id = this->num_names_domain++;
    }
    if((vid = query_symbol(variable, VARIABLE)) != -1) {
        this->variable_at_domain[vid] = id;
    }
}

void Vocabulary::set_vary(char* predicate, char* vary) {
    int id = query_symbol(vary, VARY);
    int vid;
    
    if(id == -1) {
        this->names_vary[this->num_names_vary] = vary;
        id = this->num_names_vary++;
    }
    if((vid = query_symbol(predicate, PREDICATE)) != -1) {
        this->predicate_in_vary[vid] = id;
    }
}

int Vocabulary::predicate_arity(int id)
{
    if(id < 0)
    {
        return spec_pred_arities[-id-1];
    }
    else
    {
        assert(this->arities_predicate);
        assert(id < this->num_predicate);
        return this->arities_predicate[id];
    }
}

int Vocabulary::function_arity(int id)
{
    assert(this->arities_function);
    assert(id >= 0 && id < this->num_function);
    return this->arities_function[id];
}

char* Vocabulary::query_name ( int id, SYMBOL_TYPE type )
{
    switch ( type )
    {
    case VARIABLE:
        assert ( 0 <= id && id < this->num_variable );
        assert ( this->names_variable[id] );
        return this->names_variable[id];

    case FUNCTION:
        assert ( 0 <= id && id < this->num_function );
        assert ( this->names_function[id] );
        return this->names_function[id];

    case PREDICATE:
        assert ( 0 <= id && id < this->num_predicate );
        assert ( this->names_predicate[id] );
        return this->names_predicate[id];

    default:
        assert ( 0 );
    }

    return NULL;
}

void Vocabulary::add_atom(_formula* atom) {
    bool exis = false;
    
    for(int i = 0; i < atom_list.size(); i++) {
        if(atom_list.at(i)->predicate_id == atom->predicate_id) {
            exis = true;
        }
    }
    
    if(!exis) {
        atom_list.push_back(atom);
    }
}

_formula* Vocabulary::get_atom(int predicate_id) {
    for(int i = 0; i < atom_list.size(); i++) {
        if(atom_list.at(i)->predicate_id == predicate_id) {
            return copy_formula(atom_list.at(i));
        }
    }
    
    return NULL;
}

void Vocabulary::dump_vocabulary(FILE* out) {
    int n;
    
    fprintf(out, "variable:\n");
    for(n = 0; n < num_variable;n++) {
        fprintf(out, "%s ", names_variable[n]);
    }
    
    fprintf(out, "\nfunction:\n");
    for(n = 0; n < num_function; n++) {
        fprintf(out, "%s ", names_function[n]);
    }
    
    fprintf(out, "\npredicate:\n");
    for(n = 0; n < num_predicate; n++) {
        fprintf(out, "%s ", names_predicate[n]);
    }
    
    fprintf(out, "\nintension predicate:\n");
    for(n = 0; n < num_intension_predicate; n++) {
        fprintf(out, "%s ", names_predicate[index_intension_predicate[n]]);
    }
    
    fprintf(out, "\ndomain:\n");    
    for(n = 0; n < num_variable; n++) {
        fprintf(out, "%s at %s", names_variable[n], names_domain[variable_at_domain[n]]);
        if(n != num_variable - 1) {
            fprintf(out, ", ");
        }
    }
    
    fprintf(out, "\nvary:\n");    
    for(n = 0; n < num_predicate; n++) {
        fprintf(out, "%s in %s", names_predicate[n], names_vary[predicate_in_vary[n]]);
        if(n != num_predicate - 1) {
            fprintf(out, ", ");
        }
    }
    
    fprintf(out, "\natom\n");
    for(n = 0; n < atom_list.size(); n++) {
        fprintf(out, "%s", query_name(atom_list.at(n)->predicate_id, PREDICATE));
    }
    
    fprintf(out, "ri:%s", r);
    fprintf(out, "\n");
}