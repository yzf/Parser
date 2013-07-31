#include "Vocabulary.h"
#include "PointerSensitive.h"
#include <cstring>
#include <string.h>
#include <deque>
#include <iostream>
#include <cstdlib>
#include "Utils.h"
#include <vector>
#include "Formulas.h"
#include "Formula.h"

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
    
//    int size = vocabulary.atom_list.size();
    int size = Vocabulary::instance().getAtomList()->size();
    
    int* pred = new int[size];
//    for(int i = 0; i < size; i++)
//        pred[i] = vocabulary.atom_list.at(i)->predicate_id;
//    cout << "pred : ";
    int i = 0;
    Formulas* tmp = Vocabulary::instance().getAtomList();
    for(FORMULAS_ITERATOR iter = tmp->begin(); 
            iter != tmp->end(); iter++)
    {
        pred[i] = iter->getFormula()->predicate_id;
//        cout << pred[i] << " ";
        i++;       
    }
      
    newfml.doubleNegationPredicates(pred, size);
  
    printf("\n6_1 : ");newfml.output(stdout);
    delete pred;
    return Utils::copyFormula(newfml.getFormula());
}


_formula* PointerSensitive::PointerSensitive_6_2(_formula* fml)
{
    _formula* newfml = Utils::copyFormula(fml);//copy_formula(fml);
    
    switch(newfml->formula_type){  
       case ATOM:
           //内涵谓词P的处理
//           if(vocabulary.is_intension_predicate(newfml->predicate_id))
           if(Vocabulary::instance().isIntensionPredicate(newfml->predicate_id))    
           {
               const char* predicateName = Vocabulary::instance().getNameById(newfml->predicate_id, PREDICATE);
               char* p_min = new char[strlen(predicateName) + 5];
               strcpy(p_min, predicateName);
               strcat(p_min, "_min");

               int vid;
//               int id = vocabulary.query_symbol(p_min, PREDICATE);
               int id = Vocabulary::instance().getSymbolId(p_min, PREDICATE);
               if(id < 0)
               {
//                    vid = vocabulary.add_symbol(p_min, PREDICATE, 
//                            vocabulary.predicate_arity(newfml->predicate_id));   
                   vid = Vocabulary::instance().addSymbol(p_min, PREDICATE,
                          Vocabulary::instance().getPredicateArity(newfml->predicate_id));
                    newfml->predicate_id = vid;
                    //新加入的都是内涵谓词
//                    vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
                    Vocabulary::instance().addIntensionPredicate(p_min);
               }
               else
               {
                   newfml->predicate_id = id;
               }
                
//               vocabulary.add_atom(newfml);
               Formula _newfml_ = Formula(newfml, true);
               Vocabulary::instance().addAtom(_newfml_);
               delete p_min;
           }
           //Z系列谓词的处理，若p在z中，还是写成p_min，不用写成p_vary
//           if(vocabulary.is_vary_predicate(newfml->predicate_id) && 
//                   !vocabulary.is_intension_predicate(newfml->predicate_id))
           if(Vocabulary::instance().isVaryPredicate(newfml->predicate_id) && 
                   !Vocabulary::instance().isIntensionPredicate(newfml->predicate_id))
           {
               const char* predicateName = Vocabulary::instance().getNameById(newfml->predicate_id, PREDICATE);
               char* z_vary = new char[strlen(predicateName) + 6];
               strcpy(z_vary, predicateName);
               strcat(z_vary, "_vary");
               
//               int id = vocabulary.query_symbol(z_vary, PREDICATE);
               int id = Vocabulary::instance().getSymbolId(z_vary, PREDICATE);
               if(id < 0)
               {
//                    int vid = vocabulary.add_symbol(z_vary, PREDICATE, 
//                            vocabulary.predicate_arity(newfml->predicate_id));
                    int vid = Vocabulary::instance().addSymbol(z_vary, PREDICATE,
                            Vocabulary::instance().getPredicateArity(newfml->predicate_id));
                    newfml->predicate_id = vid;
//                    vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
                    Vocabulary::instance().addIntensionPredicate(z_vary);
               }
               else
                   newfml->predicate_id = id;
                   
//               vocabulary.add_atom(newfml);
               Formula _newfml_ = Formula(newfml, true);
               Vocabulary::instance().addAtom(_newfml_);
               delete z_vary;
           }   
           break;
           
       case NEGA:
           if(newfml->subformula_l->formula_type == ATOM)
           {
               //内涵谓词P时
//                if(vocabulary.is_intension_predicate(newfml->subformula_l->predicate_id))
                if(Vocabulary::instance().isIntensionPredicate(newfml->subformula_l->predicate_id))
                {
                    const char* predicateName = Vocabulary::instance().getNameById(newfml->subformula_l->predicate_id, PREDICATE);
                    char* p_min = new char[strlen(predicateName) + 5];
                    strcpy(p_min, predicateName);
                    strcat(p_min, "_min");
                    
                    
//                    int id = vocabulary.query_symbol(p_min, PREDICATE);
                    int id = Vocabulary::instance().getSymbolId(p_min, PREDICATE);
                    if(id < 0)
                    {
//                         int vid = vocabulary.add_symbol(p_min, PREDICATE, 
//                                 vocabulary.predicate_arity(newfml->subformula_l->predicate_id));
                         int vid = Vocabulary::instance().addSymbol(p_min, PREDICATE,
                                 Vocabulary::instance().getPredicateArity(newfml->subformula_l->predicate_id));
                         newfml->subformula_l->predicate_id = vid;
//                         vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
                         Vocabulary::instance().addIntensionPredicate(p_min);
                    }
                    else
                        newfml->subformula_l->predicate_id = id;
//                    vocabulary.add_atom(newfml->subformula_l);
//                    Vocabulary::instance().addAtom(newfml->subformula_l);
                    Formula _newfml_ = Formula(newfml->subformula_l, true);
                    Vocabulary::instance().addAtom(_newfml_);
//                    delete p;
                    
                    newfml->formula_type = IMPL;
                
//                    int gid = vocabulary.query_symbol("gamma_new", PREDICATE);
                    int gid = Vocabulary::instance().getSymbolId("gamma_new", PREDICATE);
                    newfml->subformula_r = (_formula*)malloc(sizeof(_formula));
                    newfml->subformula_r->formula_type = ATOM;    
                    newfml->subformula_r->parameters = NULL;
                    if(gid < 0)
                    {
//                        int vvid = vocabulary.add_symbol("gamma_new", PREDICATE, 0); 
                        int vvid = Vocabulary::instance().addSymbol("gamma_new", PREDICATE, 0);
                        newfml->subformula_r->predicate_id = vvid;
//                        vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vvid;
                        Vocabulary::instance().addIntensionPredicate("gamma_new");
                    }
                    else                  
                        newfml->subformula_r->predicate_id = gid;
//                    vocabulary.add_atom(newfml->subformula_r);
//                    Vocabulary::instance().addAtom(newfml->subformula_r);
                     Formula _newfml_0 = Formula(newfml->subformula_r, true);
                    Vocabulary::instance().addAtom(_newfml_0);
                    delete p_min;
                }
                
                //是谓词Z时
//                if(vocabulary.is_vary_predicate(newfml->subformula_l->predicate_id) &&
//                        !vocabulary.is_intension_predicate(newfml->subformula_l->predicate_id))
                if(Vocabulary::instance().isVaryPredicate(newfml->subformula_l->predicate_id) && 
                   !Vocabulary::instance().isIntensionPredicate(newfml->subformula_l->predicate_id))
                {     
                    const char* zz = Vocabulary::instance().getNameById(newfml->subformula_l->predicate_id, PREDICATE);
                    char* z_vary = new char[strlen(zz) + 6];
                    strcpy(z_vary, zz);
                    strcat(z_vary, "_vary");
                    
//                    int id = vocabulary.query_symbol(z_vary, PREDICATE);
                    int id = Vocabulary::instance().getSymbolId(z_vary, PREDICATE);
                    if(id < 0)
                    {
//                         int vid = vocabulary.add_symbol(z_vary, PREDICATE, 
//                                 vocabulary.predicate_arity(newfml->subformula_l->predicate_id));
                        int vid = Vocabulary::instance().addSymbol(z_vary, PREDICATE,
                            Vocabulary::instance().getPredicateArity(newfml->subformula_l->predicate_id));
                         newfml->subformula_l->predicate_id = vid;
//                         vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
                         Vocabulary::instance().addIntensionPredicate(z_vary);
                    }
                    else
                        newfml->subformula_l->predicate_id = id;
//                    vocabulary.add_atom(newfml->subformula_l);
//                    Vocabulary::instance().addAtom(newfml->subformula_l);
                     Formula _newfml_ = Formula(newfml->subformula_l, true);
                    Vocabulary::instance().addAtom(_newfml_);
//                    delete z;
                    
                    newfml->formula_type = IMPL;
                
//                    int gid = vocabulary.query_symbol("gamma_new", PREDICATE);
                    int gid = Vocabulary::instance().getSymbolId("gamma_new", PREDICATE);
                    newfml->subformula_r = (_formula*)malloc(sizeof(_formula));
                    newfml->subformula_r->formula_type = ATOM;    
                    newfml->subformula_r->parameters = NULL;
                    if(gid < 0)
                    {
//                        int vvid = vocabulary.add_symbol("gamma_new", PREDICATE, 0);
                        int vvid = Vocabulary::instance().addSymbol("gamma_new", PREDICATE, 0);
                        newfml->subformula_r->predicate_id = vvid;
//                        vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vvid;
                        Vocabulary::instance().addIntensionPredicate("gamma_new");
                    }
                    else                  
                        newfml->subformula_r->predicate_id = gid;
//                    vocabulary.add_atom(newfml->subformula_r);
//                    Vocabulary::instance().addAtom(newfml->subformula_r);
                    Formula _newfml_0 = Formula(newfml->subformula_r, true);
                    Vocabulary::instance().addAtom(_newfml_0);
                    delete z_vary;
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
   
//   printf("\n6_2 : ");Utils::outputFormula(stdout, newfml);
   return newfml; 
}
    
_formula* PointerSensitive::PointerSensitive_7_1(_formula* fml)
{
    int id, vid;
    _formula* l;
    
//    if((id = vocabulary.query_symbol("gamma_new", PREDICATE)) < 0)
    if((id = Vocabulary::instance().getSymbolId("gamma_new", PREDICATE)) < 0)
    {
//       vid  = vocabulary.add_symbol("gamma_new", PREDICATE, 0); 
        vid = Vocabulary::instance().addSymbol("gamma_new", PREDICATE, 0);
        l = Utils::compositeToAtom(vid, NULL);
        Vocabulary::instance().addIntensionPredicate("gamma_new");
        Formula _newfml_ = Formula(l, true);
        Vocabulary::instance().addAtom(_newfml_);
    }
    else
    {
         
         l = Utils::copyFormula(Vocabulary::instance().getAtom(id).getFormula());
    }
    
    const int ip_id = Vocabulary::instance().getIntensionPredicateId(0);
//    _formula* rl = Vocabulary::instance().getAtom(ip_id);
    _formula* rl = Utils::copyFormula(Vocabulary::instance().getAtom(ip_id).getFormula());
    _formula* rr = Utils::compositeByConnective(NEGA, Utils::copyFormula(rl), NULL);
    _formula* r = Utils::compositeByConnective(DISJ, rl, rr);
    _formula* result = Utils::compositeByConnective(IMPL, l ,r);
   
     printf("\n7_1 : ");Utils::outputFormula(stdout, result);
    return result;  
}
  

_formula* PointerSensitive::PointerSensitive_7_2(_formula* fml)
{
    int id, vid;
    _formula* r;   
    
//    if((id = vocabulary.query_symbol("gamma_new", PREDICATE)) < 0)
    if((id = Vocabulary::instance().getSymbolId("gamma_new", PREDICATE)) < 0)
    {
//       vid  = vocabulary.add_symbol("gamma_new", PREDICATE, 0); 
        vid = Vocabulary::instance().addSymbol("gamma_new", PREDICATE, 0);
       r = Utils::compositeToAtom(vid, NULL);
       Vocabulary::instance().addIntensionPredicate("gamma_new");
       Formula r_ = Formula(r, true);
       Vocabulary::instance().addAtom(r_);
    }
    else
    {
        r = Utils::copyFormula(Vocabulary::instance().getAtom(id).getFormula());
    }
      
//    _formula* ll = vocabulary.get_atom(vocabulary.index_intension_predicate[0]);
    int ip_id = Vocabulary::instance().getIntensionPredicateId(0);
    _formula* ll = Utils::copyFormula(Vocabulary::instance().getAtom(ip_id).getFormula());
    _formula* lr = Utils::compositeByConnective(NEGA, Utils::copyFormula(ll), NULL);
    _formula* tmpl = Utils::compositeByConnective(DISJ, ll, lr);
    
    _formula* currfml = NULL;
//    for(int i = vocabulary.arities_predicate[ll->predicate_id] - 1; i >= 0; i--)
    for(int i = Vocabulary::instance().getPredicateArity(ll->predicate_id) - 1;
            i >= 0; i--)
    {    
        _formula* univ = Utils::compositeByQuantifier(UNIV, currfml, (ll->parameters+i)->variable_id);
        currfml = univ;
    }
    _formula* pointer = currfml;
    while(pointer->subformula_l != NULL)
        pointer = pointer->subformula_l;
    pointer->subformula_l = tmpl;
    
    _formula* l = currfml;
   
    _formula* result = Utils::compositeByConnective(IMPL, l ,r);
    
     printf("\n7_2 : ");Utils::outputFormula(stdout, result);
    return result;   
}


_formula* PointerSensitive::PointerSensitive_8(_formula* fml)
{
    int id, vid;
    _formula* gamma;   
    
//    if((id = vocabulary.query_symbol("gamma_new", PREDICATE)) < 0)
    if((id = Vocabulary::instance().getSymbolId("gamma_new", PREDICATE)) < 0)
    {
//       vid  = vocabulary.add_symbol("gamma_new", PREDICATE, 0); 
        vid = Vocabulary::instance().addSymbol("gamma_new", PREDICATE, 0);
        Vocabulary::instance().addIntensionPredicate("gamma_new");
        gamma = Utils::compositeToAtom(vid, NULL);
        Vocabulary::instance().addAtom(Formula(gamma, true));
    }
    else
    {
        gamma = Utils::copyFormula(Vocabulary::instance().getAtom(id).getFormula());
    }
    
//    char* p = strdup(vocabulary.query_name(vocabulary.index_intension_predicate[0], PREDICATE));
    int ip_id = Vocabulary::instance().getIntensionPredicateId(0);    
    const char* name = Vocabulary::instance().getNameById(ip_id,PREDICATE);
    char* p_min = new char[strlen(name) + 9];
    strcpy(p_min, name);
    strcat(p_min, "_min");
    
    int iid = Vocabulary::instance().getSymbolId(p_min, PREDICATE);
    _formula* pr = Utils::copyFormula(Vocabulary::instance().getAtom(iid).getFormula());
    
    _formula* result = Utils::compositeByConnective(IMPL, gamma, pr);
       
    Formulas* tmp = Vocabulary::instance().getAtomList();
    for(FORMULAS_ITERATOR iter = tmp->begin(); 
            iter != tmp->end(); iter++)
    {
        int id = iter->getFormula()->predicate_id;
        const char* pn = Vocabulary::instance().getNameById(id, PREDICATE);
        if(strstr(pn, "_vary") != NULL)
        {
            _formula* q = Utils::copyFormula(Vocabulary::instance().getAtom(id).getFormula());
            _formula* rightpart = Utils::compositeByConnective(IMPL, 
                    Utils::copyFormula(gamma), q);
            result = Utils::compositeByConnective(CONJ, result, rightpart);
        }    
    }
    delete p_min;
    printf("\n8   : ");Utils::outputFormula(stdout, result);
    return result;
}


_formula* PointerSensitive::PointerSensitive_9(_formula* fml)
{   
//    _formula* newfml = Utils::copyFormula(fml);
    
//    _formula* p = copy_formula(vocabulary.get_atom(vocabulary.index_intension_predicate[0]));
    _formula* p = Utils::copyFormula(Vocabulary::instance().getAtom(
                        Vocabulary::instance().getIntensionPredicateId(0)).getFormula());
    
    //构建R(X,U)的过程。
//    const int term_size = vocabulary.predicate_arity(p->predicate_id);
    const int term_size = Vocabulary::instance().getPredicateArity(p->predicate_id);
    _term* param_x = Utils::constructTerms(p->parameters, term_size);   //OK
    _term* param_u = Utils::constructTerms(p->parameters, term_size);   //OK
    _term* param_r = Utils::combineTerms(param_x, term_size, param_u, term_size);
    
    int r_id = Vocabulary::instance().addSymbol(Vocabulary::instance().getR(), PREDICATE, 2*term_size);
    _formula* R = Utils::compositeToAtom(r_id, param_r);
    
    //构建p_min(U)
    const char* pp = Vocabulary::instance().getNameById(p->predicate_id, PREDICATE);
    char* tp = new char[strlen(pp) + 5];
    strcpy(tp, pp);
    strcat(tp, "_min");
    
    int p_id = Vocabulary::instance().getSymbolId(tp, PREDICATE);
    _formula* PR = Utils::compositeToAtom(p_id, param_u);
    
    //构建p(U)
    _formula* PU = Utils::compositeToAtom(p->predicate_id, param_u);
    
    //获取gamma_new
    int gamma_id = Vocabulary::instance().getSymbolId("gamma_new", PREDICATE);
    _formula* gamma = Utils::copyFormula(Vocabulary::instance().getAtom(gamma_id).getFormula());
    
    //构建公式（9）
    _formula* f1 = Utils::compositeByConnective(NEGA, R, NULL);
    _formula* f2 = Utils::compositeByConnective(IMPL, PR, gamma);
    _formula* f31 = Utils::compositeByConnective(NEGA, PU, NULL);
    _formula* f3 = Utils::compositeByConnective(NEGA, f31, NULL);
    _formula* tmp_ps9 = Utils::compositeByConnective(DISJ, f1, f2);
    _formula* ps9 = Utils::compositeByConnective(DISJ, tmp_ps9, f3);
    
     printf("\nps9 : ");Utils::outputFormula(stdout, ps9);
    
    //构建公式（10） 
    _term* param_w = Utils::constructTerms(p->parameters, term_size);
    _term* param_rr = Utils::combineTerms(param_x, term_size, param_w, term_size);   
    _formula* RW = Utils::compositeToAtom(r_id, param_rr);

    _formula* PRW = Utils::compositeToAtom(p_id, param_w);
    _formula* PW = Utils::compositeToAtom(p->predicate_id, param_w);
    
    _formula* ff2 = Utils::compositeByConnective(IMPL, PRW, 
                            Utils::copyFormula(gamma));
    _formula* ff31 = Utils::compositeByConnective(NEGA, PW, NULL);
    _formula* ff3 = Utils::compositeByConnective(NEGA, ff31, NULL);
    _formula* tmp_ps10_l = Utils::compositeByConnective(CONJ, RW, ff2);
    _formula* ps10_l = Utils::compositeByConnective(CONJ, tmp_ps10_l, ff3);
    _formula* ps10 = Utils::compositeByConnective(DISJ, ps10_l, 
                                Utils::copyFormula(gamma));
    printf("\nps10 : ");Utils::outputFormula(stdout, ps10);
    
    //构建公式（11）    
    //vary_id保存的是q1、q2的predicate_id
    vector<int> vary_id;

    for(int i = 0; i < Vocabulary::instance().getNumPredicate(); i++)
    {
        if(Vocabulary::instance().isVaryPredicate(i) &&
               !Vocabulary::instance().isIntensionPredicate(i))
            vary_id.push_back(i);
    }
    
    //构造对应q1、q2参数形似的两个新的参数。
    vector<_term*> _param;
    vector<int> _param_size;
    for(unsigned int i = 0; i < vary_id.size(); i++)
    {
//        _formula* t = copy_formula(vocabulary.get_atom(vary_id[i]));
        _formula* t = Utils::copyFormula(Vocabulary::instance().getAtom(vary_id[i]).getFormula());
        _term* param_y = Utils::constructTerms(t->parameters, 
                        Vocabulary::instance().getPredicateArity(vary_id[i]));
        _param.push_back(param_y);
        _param_size.push_back(Vocabulary::instance().getPredicateArity(vary_id[i]));
    }
    
    vector<_formula*> res;
    
    //构造整条公式中需要用到的主要ATOM及对应的公式
    vector<_formula*> V_xy;     //Vi(X,Y)
    vector<_formula*> Q_y;      //Qi(Y)
    vector<_formula*> Z_y;      //Zi(Y)  
    for(unsigned int i = 0; i < vary_id.size(); i++)
    {       
        _formula* res_ = NULL;
        //V_xy
//        char* V = strdup(vocabulary.names_vary[vocabulary.predicate_in_vary[vary_id[i]]]);
//        int id = vocabulary.query_symbol(V, PREDICATE);
        char* V = strdup(Vocabulary::instance().getNameById(
                        Vocabulary::instance().getVaryWithPredicate(vary_id[i]),VARY));
        int id = Vocabulary::instance().getSymbolId(V, PREDICATE);
        int pvy_id;
        if(id < 0)
        {                   
//            int vid = vocabulary.add_symbol(V, PREDICATE, term_size + _param_size[i]);
            int vid = Vocabulary::instance().addSymbol(V, PREDICATE, term_size + _param_size[i]);
            pvy_id = vid;
   //         vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
        }                   
        else                        
            pvy_id = id;
        
        _term* v_param = Utils::combineTerms(param_x, term_size, _param[i], _param_size[i]);
        _formula* Vi_xy = Utils::compositeToAtom(pvy_id, v_param);
        V_xy.push_back(Vi_xy);
                       
        //Q_y
        char* qq = strdup(Vocabulary::instance().getNameById(vary_id[i], PREDICATE));
        char* tq = strdup(qq);
        strcat(tq, "_vary");
        int q_id = Vocabulary::instance().getSymbolId(tq, PREDICATE);
        _formula* Qi_y = Utils::compositeToAtom(q_id, _param[i]);
        Q_y.push_back(Qi_y);
        
        //Z_y
        _formula* Zi_y = Utils::compositeToAtom(vary_id[i], _param[i]);
        Z_y.push_back(Zi_y);
              
        ////////////////
        //part1
        _formula* mid1 = Utils::compositeByConnective(IMPL, Q_y[i], 
                                Utils::copyFormula(gamma));
        _formula* n_zy = Utils::compositeByConnective(NEGA, Z_y[i], NULL);
        _formula* tail1 = Utils::compositeByConnective(NEGA, n_zy, NULL);
        _formula* head1 = Utils::compositeByConnective(DISJ, V_xy[i], mid1);
        _formula* part1_ = Utils::compositeByConnective(DISJ, head1, tail1);

        //part2
        _formula* head2 = Utils::compositeByConnective(DISJ, V_xy[i], n_zy);
        _formula* part2_ = Utils::compositeByConnective(DISJ, head2, Q_y[i]);

        res_ = Utils::compositeByConnective(CONJ, part1_, part2_);
        res.push_back(res_);
    }
    
    //当读到和p对应的V时，加入该式子  
    _formula* V_xu = NULL;     //V1(X,U)
    _term* vu_param = NULL;
    if(Vocabulary::instance().isVaryPredicate(p->predicate_id))
    {
//        char* Vp = strdup(vocabulary.names_vary[vocabulary.predicate_in_vary[p->predicate_id]]);
        char* Vp = strdup(Vocabulary::instance().getNameById(
                        Vocabulary::instance().getVaryWithPredicate(p->predicate_id),VARY));
        int pid = Vocabulary::instance().getSymbolId(Vp, PREDICATE);
        int pvu_id;
        if(pid < 0)
        {                   
            int pvid = Vocabulary::instance().addSymbol(Vp, PREDICATE, 2*term_size);

             pvu_id = pvid;
      //         vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
        }                   
        else                        
             pvu_id = pid;
      
        vu_param = Utils::combineTerms(param_x, term_size, param_u, term_size);        
        V_xu = Utils::compositeToAtom(pvu_id, vu_param);
        
        _formula* mid_p = Utils::compositeByConnective(IMPL, Utils::copyFormula(PR), 
                                Utils::copyFormula(gamma));
        _formula* n_pu = Utils::compositeByConnective(NEGA, Utils::copyFormula(PU), NULL);
        _formula* nn_pu = Utils::compositeByConnective(NEGA, n_pu, NULL);
        _formula* head_p_1 = Utils::compositeByConnective(DISJ, V_xu, mid_p);
        _formula* part_p_1 = Utils::compositeByConnective(DISJ, head_p_1, nn_pu);
        _formula* head_p_2 = Utils::compositeByConnective(DISJ, V_xu, n_pu);
        _formula* part_p_2 = Utils::compositeByConnective(DISJ, head_p_2, Utils::copyFormula(PR));
        _formula* part_p_ = Utils::compositeByConnective(CONJ, part_p_1, part_p_2);
      
        res.push_back(part_p_);
    }

        
    _formula* ps11_ = NULL;
    if(res.size() > 0)
        ps11_ = Utils::copyFormula(res[0]);
    for(unsigned int i = 1; i < res.size(); i++)
    {
        ps11_ = Utils::compositeByConnective(CONJ, ps11_, res[i]);
    }
    
    if(ps11_ != NULL)
    {
        Formula _ps11 = Formula(Utils::copyFormula(ps11_), false);
        printf("\nps11 : ");_ps11.output(stdout);
    }
    else
        printf("\nps11 is null!\n");
    
    //加入量词[?X][?W][!U][!Y]
    _formula* exis_x = NULL;   
    for(int i = Vocabulary::instance().getPredicateArity(p->predicate_id) - 1; 
            i >= 0; i--)
    {
        _formula* curr = Utils::compositeByQuantifier(EXIS, exis_x, (param_x+i)->variable_id);
        exis_x = curr;
    }
    
    _formula* exis_w = NULL;
    for(int i = Vocabulary::instance().getPredicateArity(p->predicate_id) - 1; i >= 0; i--)
    {
        _formula* curr = Utils::compositeByQuantifier(EXIS, exis_w, (param_w+i)->variable_id);
        exis_w = curr;
    }
    
    _formula* univ_u = NULL;
    for(int i = Vocabulary::instance().getPredicateArity(p->predicate_id) - 1; i >= 0; i--)
    {
        _formula* curr = Utils::compositeByQuantifier(UNIV, univ_u, (param_u+i)->variable_id);
        univ_u = curr;
    }
    
    _formula* univ_y = NULL;
    for(int j = _param.size() - 1; j >= 0; j--)
    {
        for(int i = _param_size[j] - 1; i >= 0; i--)
        {
            _formula* curr = Utils::compositeByQuantifier(UNIV, univ_y, (_param[j]+i)->variable_id);
            univ_y = curr;
        }
    }
    
    vector<_formula*> qntf;   
    qntf.push_back(exis_w);
    qntf.push_back(univ_u);
    qntf.push_back(univ_y);
    
    _formula* pointer = exis_x;
    for(unsigned int i = 0; i < qntf.size(); i++)
    {       
        if(qntf[i] != NULL)
        {
            while(pointer->subformula_l != NULL)
                pointer = pointer->subformula_l;
            pointer->subformula_l = qntf[i];
        }
    }
    
    _formula* final_tmp = Utils::compositeByConnective(CONJ, ps9, ps10);
    _formula* final_;
    if(ps11_ != NULL)
        final_ = Utils::compositeByConnective(CONJ, final_tmp, ps11_);
    else
        final_ = final_tmp;
    
    while(pointer->subformula_l != NULL)
            pointer = pointer->subformula_l;
    pointer->subformula_l = final_;
    
    _formula* result = Utils::copyFormula(exis_x);
    
//    Formula _result = Formula(Utils::copyFormula(result), false);
//    printf("\n9_0 : ");_result.output(stdout);printf("\n\n");
    printf("\n9  : ");Utils::outputFormula(stdout, result);
    
    return result;
}
 


Formulas* PointerSensitive::PointerSensitive_Convert(_formula* fml)
{
    
    _formula* f61 =  PointerSensitive_6_1(fml);    
    _formula* f62 =  PointerSensitive_6_2(fml);
    Formula ff62 = Formula(f62, false);
    printf("\n6_2 : ");ff62.output(stdout);
        
    _formula* f71 =  PointerSensitive_7_1(fml);
    _formula* f72 =  PointerSensitive_7_2(fml);
    _formula* f8 =  PointerSensitive_8(fml);
    _formula* f9 = PointerSensitive_9(fml);
//    Utils::outputFormula(stdout, f9);
//    printf("sdf\n");
    Formula ff61 = Formula(f61, false);
//    Formula ff62 = Formula(f62, true);
    Formula ff71 = Formula(f71, false);
    Formula ff72 = Formula(f72, false);
    Formula ff8 = Formula(f8, false);
    Formula ff9 = Formula(f9, false);
    
    
    
    deque<Formula> df;
    df.push_back(ff61);
    df.push_back(ff62);
    df.push_back(ff71);
    df.push_back(ff72);
    df.push_back(ff8);
    df.push_back(ff9);
    
    Formulas* fmls = new Formulas(df);
    
    printf("\n\nThe total formulas : \n");fmls->output(stdout);
    
    
    return fmls;
}
