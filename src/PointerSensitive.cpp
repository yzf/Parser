#include "Vocabulary.h"
#include "PointerSensitive.h"
#include <cstring>

using namespace std;

PointerSensitive::PointerSensitive()
{
    
}
    
PointerSensitive::~PointerSensitive()
{

}



_formula* PointerSensitive::PointerSensitive_6_1(_formula* fml)
{
    Formula newfml = Formula(fml, true);
    
    int size = vocabulary.atom_list.size();
    int* pred;
    for(int i = 0; i < size; i++)
        pred[i] = vocabulary.atom_list.at(i)->predicate_id;
    
    newfml.double_negation(pred, size);
    return newfml.get_formula();
}


_formula* PointerSensitive::PointerSensitive_6_2(_formula* fml)
{
    _formula* newfml = copy_formula(fml);
    
    switch(newfml->formula_type){  
       case ATOM: 
           if(vocabulary.is_intension_predicate(newfml->predicate_id))
           {
               char* p = vocabulary.query_name(newfml->predicate_id, PREDICATE);
               const char* add = "_min";
               char* p_min = strcat(p, add);
               int id = vocabulary.query_symbol(p_min, PREDICATE);
               if(id < 0)
               {
                    int vid = vocabulary.add_symbol(p_min, PREDICATE, 
                            vocabulary.predicate_arity(newfml->predicate_id));
                    newfml->predicate_id = vid;
               }
               else
                   newfml->predicate_id = id;
               vocabulary.add_atom(newfml);  
           }
           
           if(vocabulary.is_vary_predicate(newfml->predicate_id))
           {
               
           }
               
           break;
           
       case NEGA:
           if(newfml->subformula_l->formula_type == ATOM)
           {
                if(vocabulary.is_intension_predicate(newfml->subformula_l->predicate_id))
                {
                    char* p = vocabulary.query_name(newfml->subformula_l->predicate_id, PREDICATE);
                    const char* add = "_min";
                    char* p_min = strcat(p, add);
                    int id = vocabulary.query_symbol(p_min, PREDICATE);
                    if(id < 0)
                    {
                         int vid = vocabulary.add_symbol(p_min, PREDICATE, 
                                 vocabulary.predicate_arity(newfml->subformula_l->predicate_id));
                         newfml->subformula_l->predicate_id = vid;
                    }
                    else
                        newfml->subformula_l->predicate_id = id;
                    vocabulary.add_atom(newfml);  
                }
               
                newfml->formula_type = IMPL;
                
                int gid = vocabulary.query_symbol("gamma_new", PREDICATE);
                if(gid < 0)
                {
                    int vvid = vocabulary.add_symbol("gamma_new", PREDICATE, 0);
                    newfml->subformula_r->formula_type = ATOM;
                    newfml->subformula_r->predicate_id = vvid;
                    newfml->subformula_r->parameters = NULL;
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
  
   return newfml; 
}
    
_formula* PointerSensitive::PointerSensitive_7_1(_formula* fml)
{
    
}
    
_formula* PointerSensitive::PointerSensitive_7_2(_formula* fml)
{
    
}


_formula* PointerSensitive::PointerSensitive_8(_formula* fml)
{
    
}

_formula* PointerSensitive::PointerSensitive_9(_formula* fml)
{
    
}
 
_formula* PointerSensitive::PointerSensitive_10(_formula* fml)
{
    
}

_formula* PointerSensitive::PointerSensitive_11(_formula* fml)
{
    
}

Formulas PointerSensitive::PointerSensitive_Convert(_formula* fml)
{
    
}
