#include "Vocabulary.h"
#include "PointerSensitive.h"
#include <cstring>
#include <string.h>
#include <deque>
#include <iostream>
#include <cstdlib>
#include "utility.h"

using namespace std;

PointerSensitive::PointerSensitive()
{
    
}
    
PointerSensitive::~PointerSensitive()
{

}

PointerSensitive& PointerSensitive::instance() {
    static PointerSensitive the_instance;
    return the_instance;
}


_formula* PointerSensitive::PointerSensitive_6_1(_formula* fml)
{
    Formula newfml = Formula(fml, true);
    
    int size = vocabulary.atom_list.size();

    int* pred = new int[size];
    for(int i = 0; i < size; i++)
        pred[i] = vocabulary.atom_list.at(i)->predicate_id;

    newfml.double_negation(pred, size);
  
    printf("\n6_1 : ");newfml.output(stdout);
    delete pred;
    return newfml.get_formula();
}


_formula* PointerSensitive::PointerSensitive_6_2(_formula* fml)
{
    _formula* newfml = copy_formula(fml);
    
    switch(newfml->formula_type){  
       case ATOM:
           //内涵谓词P的处理
           if(vocabulary.is_intension_predicate(newfml->predicate_id))
           {
               char* pp = vocabulary.query_name(newfml->predicate_id, PREDICATE);
               char* p = new char[strlen(pp) + 1];
               strcpy(p, pp);
               const char* add = "_min";
               char* p_min = strcat(p, add);
              
               int vid;
               int id = vocabulary.query_symbol(p_min, PREDICATE);
               if(id < 0)
               {
                    vid = vocabulary.add_symbol(p_min, PREDICATE, 
                            vocabulary.predicate_arity(newfml->predicate_id));     
                    newfml->predicate_id = vid;
                    //新加入的都是内涵谓词
                    vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
               }
               else
               {
                   newfml->predicate_id = id;
               }
                
               vocabulary.add_atom(newfml);
           }
           //Z系列谓词的处理，若p在z中，还是写成p_min，不用写成p_vary
           if(vocabulary.is_vary_predicate(newfml->predicate_id) && 
                   !vocabulary.is_intension_predicate(newfml->predicate_id))
           {
               char* zz = vocabulary.query_name(newfml->predicate_id, PREDICATE);
               char* z = new char[strlen(zz) + 1];
               strcpy(z, zz);
               const char* add = "_vary";
               char* z_vary = strcat(z, add);
               int id = vocabulary.query_symbol(z_vary, PREDICATE);
               if(id < 0)
               {
                    int vid = vocabulary.add_symbol(z_vary, PREDICATE, 
                            vocabulary.predicate_arity(newfml->predicate_id));
                    newfml->predicate_id = vid;
                    vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
               }
               else
                   newfml->predicate_id = id;
                   
               vocabulary.add_atom(newfml);
              
           }   
           break;
           
       case NEGA:
           if(newfml->subformula_l->formula_type == ATOM)
           {
               //内涵谓词P时
                if(vocabulary.is_intension_predicate(newfml->subformula_l->predicate_id))
                {
                    char* pp = vocabulary.query_name(newfml->subformula_l->predicate_id, PREDICATE);
                    char* p = new char[strlen(pp) + 1];
                    strcpy(p, pp);
                    const char* add = "_min";
                    char* p_min = strcat(p, add);
                    int id = vocabulary.query_symbol(p_min, PREDICATE);
                    if(id < 0)
                    {
                         int vid = vocabulary.add_symbol(p_min, PREDICATE, 
                                 vocabulary.predicate_arity(newfml->subformula_l->predicate_id));
                         newfml->subformula_l->predicate_id = vid;
                         vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
                    }
                    else
                        newfml->subformula_l->predicate_id = id;
                    vocabulary.add_atom(newfml->subformula_l);
                    
                    newfml->formula_type = IMPL;
                
                    int gid = vocabulary.query_symbol("gamma_new", PREDICATE);
                    newfml->subformula_r = (_formula*)malloc(sizeof(_formula));
                    newfml->subformula_r->formula_type = ATOM;    
                    newfml->subformula_r->parameters = NULL;
                    if(gid < 0)
                    {
                        int vvid = vocabulary.add_symbol("gamma_new", PREDICATE, 0);     
                        newfml->subformula_r->predicate_id = vvid;
                        vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vvid;
                    }
                    else                  
                        newfml->subformula_r->predicate_id = gid;
                    vocabulary.add_atom(newfml->subformula_r);
                    
                }
                
                //是谓词Z时
                if(vocabulary.is_vary_predicate(newfml->subformula_l->predicate_id) &&
                        !vocabulary.is_intension_predicate(newfml->subformula_l->predicate_id))
                {
                    char* zz = vocabulary.query_name(newfml->subformula_l->predicate_id, PREDICATE);
                    char* z = new char[strlen(zz) + 1];
                    strcpy(z, zz);
                    const char* add = "_vary";
                    char* z_vary = strcat(z, add);
                    int id = vocabulary.query_symbol(z_vary, PREDICATE);
                    if(id < 0)
                    {
                         int vid = vocabulary.add_symbol(z_vary, PREDICATE, 
                                 vocabulary.predicate_arity(newfml->subformula_l->predicate_id));
                         newfml->subformula_l->predicate_id = vid;
                         vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
                    }
                    else
                        newfml->subformula_l->predicate_id = id;
                    vocabulary.add_atom(newfml->subformula_l);
                    
                    newfml->formula_type = IMPL;
                
                    int gid = vocabulary.query_symbol("gamma_new", PREDICATE);
                    newfml->subformula_r = (_formula*)malloc(sizeof(_formula));
                    newfml->subformula_r->formula_type = ATOM;    
                    newfml->subformula_r->parameters = NULL;
                    if(gid < 0)
                    {
                        int vvid = vocabulary.add_symbol("gamma_new", PREDICATE, 0);     
                        newfml->subformula_r->predicate_id = vvid;
                        vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vvid;
                    }
                    else                  
                        newfml->subformula_r->predicate_id = gid;
                    vocabulary.add_atom(newfml->subformula_r);
                }
                             
           }
           
           else
               newfml->subformula_l = PointerSensitive_6_2(newfml->subformula_l);
           break;
       
       case UNIV:
       case EXIS:
           newfml->subformula_l = PointerSensitive_6_2(newfml->subformula_l);
           break;
           
       case CONJ:
       case DISJ:
       case IMPL:
           newfml->subformula_l = PointerSensitive_6_2(newfml->subformula_l);
           newfml->subformula_r = PointerSensitive_6_2(newfml->subformula_r);
           break;   
                       
   }
  
    
   
//   Formula NEWFML = Formula(newfml, true);
//   printf("\n6_2 : ");NEWFML.output(stdout);
   return newfml; 
}
    
_formula* PointerSensitive::PointerSensitive_7_1(_formula* fml)
{
    int id, vid;
    _formula* l = (_formula*)malloc(sizeof(_formula));;
    
    if((id = vocabulary.query_symbol("gamma_new", PREDICATE)) < 0)
    {
       vid  = vocabulary.add_symbol("gamma_new", PREDICATE, 0); 
       l->formula_type = ATOM;
       l->predicate_id = vid;
       l = composite_atom(ATOM, vid, NULL);
       vocabulary.add_atom(l);
    }
    else
       l =  vocabulary.get_atom(id);
    
    _formula* rl = vocabulary.get_atom(vocabulary.index_intension_predicate[0]);
    _formula* rr = composite_bool(NEGA, copy_formula(rl), NULL);
    _formula* r = composite_bool(DISJ, rl, rr);
    _formula* result = composite_bool(IMPL, l ,r);
    
    Formula res = Formula(result, true);
    printf("\n7_1 : ");res.output(stdout);
    return result;  
}
  

_formula* PointerSensitive::PointerSensitive_7_2(_formula* fml)
{
    int id, vid;
    _formula* r = (_formula*)malloc(sizeof(_formula));   
    
    if((id = vocabulary.query_symbol("gamma_new", PREDICATE)) < 0)
    {
       vid  = vocabulary.add_symbol("gamma_new", PREDICATE, 0); 
       r->formula_type = ATOM;
       r->predicate_id = vid;
       r = composite_atom(ATOM, vid, NULL);
       vocabulary.add_atom(r);
    }
    else
       r =  vocabulary.get_atom(id);
      
    _formula* ll = vocabulary.get_atom(vocabulary.index_intension_predicate[0]);
    _formula* lr = composite_bool(NEGA, ll, NULL);
    _formula* tmpl = composite_bool(DISJ, ll, lr);
    
    _formula* wholeuniv = (_formula*)malloc(sizeof(_formula));
    wholeuniv->formula_type = UNIV;
    wholeuniv->variable_id = (ll->parameters+0)->variable_id;
    for(int i = 1; i < vocabulary.arities_predicate[ll->predicate_id]; i++)
    {    
        _formula* univ;
        univ->formula_type = UNIV;
        univ->variable_id = (ll->parameters+i)->variable_id;
        wholeuniv->subformula_l = univ;
    }
    wholeuniv->subformula_l = tmpl;
    
    _formula* l = wholeuniv;
   
    _formula* result = composite_bool(IMPL, l ,r);
    
    Formula res = Formula(result, true);
    printf("\n7_2 : ");res.output(stdout);
    return result;   
}


_formula* PointerSensitive::PointerSensitive_8(_formula* fml)
{
    int id, vid;
    _formula* gamma = (_formula*)malloc(sizeof(_formula));   
    
    if((id = vocabulary.query_symbol("gamma_new", PREDICATE)) < 0)
    {
       vid  = vocabulary.add_symbol("gamma_new", PREDICATE, 0); 
       gamma->formula_type = ATOM;
       gamma->predicate_id = vid;
       gamma = composite_atom(ATOM, vid, NULL);
    }
    else
       gamma =  vocabulary.get_atom(id);
    
    char* p = vocabulary.query_name(vocabulary.index_intension_predicate[0], PREDICATE);
    char* p_min = strcat(p, "_min");
    int iid = vocabulary.query_symbol(p_min, PREDICATE);
    _formula* pr = vocabulary.get_atom(iid);
    
    _formula* result = composite_bool(IMPL, gamma, pr);
            
    for(int i = 0; i < vocabulary.atom_list.size(); i++)
    {       
        int id = vocabulary.atom_list.at(i)->predicate_id;
        char* pn = vocabulary.query_name(id, PREDICATE);
        if(strstr(pn, "_vary") != NULL)
        {
           _formula* q = vocabulary.get_atom(id);
           _formula* rightpart = composite_bool(IMPL, gamma, q);
           result = composite_bool(CONJ, result, rightpart);
        }
    }
    
    Formula res = Formula(result, true);
    printf("\n8_0 : ");res.output(stdout);printf("\n");
    return result;
}


_formula* PointerSensitive::PointerSensitive_9(_formula* fml)
{
    
}
 


Formulas PointerSensitive::PointerSensitive_Convert(_formula* fml)
{
    _formula* f61 =  PointerSensitive_6_1(fml);
    _formula* f62 =  PointerSensitive_6_2(fml);
    Formula ff62 = Formula(f62, true);
    printf("\n6_2 : ");ff62.output(stdout);
    
//    for(int i = 0; i < vocabulary.atom_list.size(); i++)
//    {
//        
//        Formula temp = Formula(vocabulary.atom_list.at(i), true);
//        temp.output(stdout);
//    }
        
    _formula* f71 =  PointerSensitive_7_1(fml);
    _formula* f72 =  PointerSensitive_7_2(fml);
    _formula* f8 =  PointerSensitive_8(fml);
    
    
    Formula ff61 = Formula(f61, true);
//    Formula ff62 = Formula(f62, true);
    Formula ff71 = Formula(f71, true);
    Formula ff72 = Formula(f72, true);
    Formula ff8 = Formula(f8, true);
    
    
   
    
    deque<Formula> df;
    df.push_back(ff61);
    df.push_back(ff62);
    df.push_back(ff71);
    df.push_back(ff72);
    df.push_back(ff8);
    
    Formulas fmls(df);
    
    return fmls;
}
