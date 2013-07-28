#include "Vocabulary.h"
#include "PointerSensitive.h"
#include <cstring>
#include <string.h>
#include <deque>
#include <iostream>
#include <cstdlib>
#include "utility.h"
#include <vector>

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
               delete p;
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
               delete z;
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
                    delete p;
                    
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
                    delete z;
                    
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
    
    _formula* currfml = NULL;
    for(int i = vocabulary.arities_predicate[ll->predicate_id] - 1; i >= 0; i--)
    {    
        _formula* univ = composite_qntf(UNIV, currfml, (ll->parameters+i)->variable_id);
        currfml = univ;
    }
    _formula* pointer = currfml;
    while(pointer->subformula_l != NULL)
        pointer = pointer->subformula_l;
    pointer->subformula_l = tmpl;
    
    _formula* l = currfml;
   
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
    
    char* p = strdup(vocabulary.query_name(vocabulary.index_intension_predicate[0], PREDICATE));
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
    _formula* newfml = copy_formula(fml);
    
    _formula* p = copy_formula(vocabulary.get_atom(vocabulary.index_intension_predicate[0]));
   
    //构建R(X,U)的过程。
    const int term_size = vocabulary.predicate_arity(p->predicate_id);
    _term* param_x = construct_terms(p->parameters, term_size);   //OK
    _term* param_u = construct_terms(p->parameters, term_size);   //OK
    _term* param_r = combine_terms(param_x, term_size, param_u, term_size);
    
    int r_id = vocabulary.add_symbol(vocabulary.r, PREDICATE, 2*term_size);
    _formula* R = composite_atom(ATOM, r_id, param_r);
    
    //构建p_min(U)
    char* pp = vocabulary.query_name(p->predicate_id, PREDICATE);
    char* tp = strdup(pp);
    strcat(tp, "_min");
    int p_id = vocabulary.query_symbol(tp, PREDICATE);
    _formula* PR = composite_atom(ATOM, p_id, param_u);
    
    //构建p(U)
    _formula* PU = composite_atom(ATOM, p->predicate_id, param_u);
    
    //获取gamma_new
    int gamma_id = vocabulary.query_symbol("gamma_new", PREDICATE);
    _formula* gamma = copy_formula(vocabulary.get_atom(gamma_id));
    
    //构建公式（9）
    _formula* f1 = composite_bool(NEGA, R, NULL);
    _formula* f2 = composite_bool(IMPL, PR, gamma);
    _formula* f31 = composite_bool(NEGA, PU, NULL);
    _formula* f3 = composite_bool(NEGA, f31, NULL);
    _formula* tmp_ps9 = composite_bool(DISJ, f1, f2);
    _formula* ps9 = composite_bool(DISJ, tmp_ps9, f3);
    Formula _ps9 = Formula(ps9, true);
    printf("\nps9 : ");_ps9.output(stdout);
    
    //构建公式（10） 
    _term* param_w = construct_terms(p->parameters, term_size);
    _term* param_rr = combine_terms(param_x, term_size, param_w, term_size);   
    _formula* RW = composite_atom(ATOM, r_id, param_rr);

    _formula* PRW = composite_atom(ATOM, p_id, param_w);
    _formula* PW = composite_atom(ATOM, p->predicate_id, param_w);
    
    _formula* ff2 = composite_bool(IMPL, PRW, gamma);
    _formula* ff31 = composite_bool(NEGA, PW, NULL);
    _formula* ff3 = composite_bool(NEGA, ff31, NULL);
    _formula* tmp_ps10_l = composite_bool(CONJ, RW, ff2);
    _formula* ps10_l = composite_bool(CONJ, tmp_ps10_l, ff3);
    _formula* ps10 = composite_bool(DISJ, ps10_l, gamma);
    Formula _ps10 = Formula(ps10, true);
    printf("\nps10 : ");_ps10.output(stdout);
  

    //构建公式（11）    
    //vary_id保存的是q1、q2的predicate_id
    vector<int> vary_id;
    for(int i = 0; i < vocabulary.num_predicate; i++)
    {
        if(vocabulary.is_vary_predicate(i) && !vocabulary.is_intension_predicate(i))
            vary_id.push_back(i);
    }
    
    //构造对应q1、q2参数形似的两个新的参数。
    vector<_term*> _param;
    vector<int> _param_size;
    for(int i = 0; i < vary_id.size(); i++)
    {
        _formula* t = copy_formula(vocabulary.get_atom(vary_id[i]));
        _term* param_y = construct_terms(t->parameters, vocabulary.predicate_arity(vary_id[i]));
        _param.push_back(param_y);
        _param_size.push_back(vocabulary.predicate_arity(vary_id[i]));
    }
    
    //构造整条公式中需要用到的主要ATOM
    vector<_formula*> V_xy;     //V1(X,Y), V2(X,Y)
    _formula* V_xu;     //V1(X,U)
    vector<_formula*> Q_y;      //Q1(Y), Q2(Y)
    vector<_formula*> Z_y;      //Z1(Y), Z2(Y)
    for(int i = 0; i < vocabulary.num_names_vary; i++)
    {   
        //V_xy
        char* V = strdup(vocabulary.names_vary[i]);
        int id = vocabulary.query_symbol(V, PREDICATE);
        int pvy_id;
        if(id < 0)
        {                   
            int vid = vocabulary.add_symbol(V, PREDICATE, term_size + _param_size[i]);
            
            pvy_id = vid;
   //         vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vid;
        }                   
        else                        
            pvy_id = id;
        
        _term* v_param = combine_terms(param_x, term_size, _param[i], _param_size[i]);
        _formula* Vi_xy = composite_atom(ATOM, pvy_id, v_param);
        V_xy.push_back(Vi_xy);
        
        //V_xu  这部应该放在外面判断，当p属于z时才使用。
//        char* Vu_tmp = strdup(V);
//        char* Vu = strcat(Vu_tmp, "_u");
//        int vu_id = vocabulary.query_symbol(Vu, PREDICATE);
//        int vu_vid;
//        if(vu_id < 0)
//        {
//            vu_vid = vocabulary.add_symbol(Vu, PREDICATE, 2*term_size);
////            vocabulary.index_intension_predicate[vocabulary.num_intension_predicate++] = vu_vid;
//        }
//        else
//            vu_vid = vu_id;
        
        //当读到和p对应的V时，加入该式子    
        if(vocabulary.predicate_in_vary[p->predicate_id] == i)
        {
            _term* vu_param = combine_terms(param_x, term_size, param_u, term_size);        
            V_xu = composite_atom(ATOM, pvy_id, vu_param); 
        }
        
        //Q_y
        char* qq = strdup(vocabulary.query_name(vary_id[i], PREDICATE));
        char* tq = strdup(qq);
        strcat(tq, "_vary");
        int q_id = vocabulary.query_symbol(tq, PREDICATE);
        _formula* Qi_y = composite_atom(ATOM, q_id, _param[i]);
        Q_y.push_back(Qi_y);
        
        //Z_y
        _formula* Zi_y = composite_atom(ATOM, vary_id[i], _param[i]);
        Z_y.push_back(Zi_y);       
    }
    
    //基于上述ATOM，构建公式
    vector<_formula*> res;
    int count = 0;
    for(int i = 0; i < vocabulary.num_names_vary; i++)
    {
        //part1
        _formula* mid1 = composite_bool(IMPL, Q_y[i], gamma);
        _formula* n_zy = composite_bool(NEGA, Z_y[i], NULL);
        _formula* tail1 = composite_bool(NEGA, n_zy, NULL);
        _formula* head1 = composite_bool(DISJ, V_xy[i], mid1);
        _formula* part1_ = composite_bool(DISJ, head1, tail1);
        
        //part2
        _formula* head2 = composite_bool(DISJ, V_xy[i], n_zy);
        _formula* part2_ = composite_bool(DISJ, head2, Q_y[i]);
        
        _formula* res_ = composite_bool(CONJ, part1_, part2_);
        
        //part_p
        _formula* mid_p = composite_bool(IMPL, PR, gamma);
        _formula* n_pu = composite_bool(NEGA, PU, NULL);
        _formula* nn_pu = composite_bool(NEGA, n_pu, NULL);
        _formula* head_p_1 = composite_bool(DISJ, V_xu, mid_p);
        _formula* part_p_1 = composite_bool(DISJ, head_p_1, nn_pu);
        _formula* head_p_2 = composite_bool(DISJ, V_xu, n_pu);
        _formula* part_p_2 = composite_bool(DISJ, head_p_2, PR);
        _formula* part_p_ = composite_bool(CONJ, part_p_1, part_p_2);
        
        if(vocabulary.is_vary_predicate(p->predicate_id) && count == 0)
        {
            res_ = composite_bool(CONJ, res_, part_p_);
            count++;
        }
        
        res.push_back(res_);
    }
    
    _formula* ps11_ = copy_formula(res[0]);
    for(int i = 1; i < vocabulary.num_names_vary; i++)
    {
        ps11_ = composite_bool(CONJ, ps11_, res[i]);
    }
    
    Formula _ps11 = Formula(ps11_, true);
    printf("\nps11 : ");_ps11.output(stdout);
    
    //加入量词[?X][?W][!U][!Y]
    _formula* exis_x = NULL;   
    for(int i = vocabulary.arities_predicate[p->predicate_id] - 1; i >= 0; i--)
    {
        _formula* curr = composite_qntf(EXIS, exis_x, (param_x+i)->variable_id);
        exis_x = curr;
    }
    
    _formula* exis_w = NULL;
    for(int i = vocabulary.arities_predicate[p->predicate_id] - 1; i >= 0; i--)
    {
        _formula* curr = composite_qntf(EXIS, exis_w, (param_w+i)->variable_id);
        exis_w = curr;
    }
    
    _formula* univ_u = NULL;
    for(int i = vocabulary.arities_predicate[p->predicate_id] - 1; i >= 0; i--)
    {
        _formula* curr = composite_qntf(UNIV, univ_u, (param_u+i)->variable_id);
        univ_u = curr;
    }
    
    _formula* univ_y = NULL;
    for(int j = _param.size() - 1; j >= 0; j--)
    {
        for(int i = _param_size[j] - 1; i >= 0; i--)
        {
            _formula* curr = composite_qntf(UNIV, univ_y, (_param[j]+i)->variable_id);
            univ_y = curr;
        }
    }
    
    vector<_formula*> qntf;   
    qntf.push_back(exis_w);
    qntf.push_back(univ_u);
    qntf.push_back(univ_y);
    
    _formula* pointer = exis_x;
    for(int i = 0; i < qntf.size(); i++)
    {       
        while(pointer->subformula_l != NULL)
            pointer = pointer->subformula_l;
        pointer->subformula_l = qntf[i];
    }
    
 
    _formula* final_tmp = composite_bool(CONJ, ps9, ps10);
    _formula* final_ = composite_bool(CONJ, final_tmp, ps11_);
    
    while(pointer->subformula_l != NULL)
            pointer = pointer->subformula_l;
    pointer->subformula_l = final_;
    
    _formula* result = copy_formula(exis_x);
    
    Formula _result = Formula(result, true);
    printf("\n9_0 : ");_result.output(stdout);printf("\n\n");
    
    return result;
}
 


Formulas PointerSensitive::PointerSensitive_Convert(_formula* fml)
{
    _formula* f61 =  PointerSensitive_6_1(fml);    
    _formula* f62 =  PointerSensitive_6_2(fml);
    Formula ff62 = Formula(f62, true);
    printf("\n6_2 : ");ff62.output(stdout);
        
    _formula* f71 =  PointerSensitive_7_1(fml);
    _formula* f72 =  PointerSensitive_7_2(fml);
    _formula* f8 =  PointerSensitive_8(fml);
    _formula* f9 = PointerSensitive_9(fml);
    
    Formula ff61 = Formula(f61, true);
//    Formula ff62 = Formula(f62, true);
    Formula ff71 = Formula(f71, true);
    Formula ff72 = Formula(f72, true);
    Formula ff8 = Formula(f8, true);
    Formula ff9 = Formula(f9, true);
    
   
    
    deque<Formula> df;
    df.push_back(ff61);
    df.push_back(ff62);
    df.push_back(ff71);
    df.push_back(ff72);
    df.push_back(ff8);
    
    Formulas fmls(df);
    
    return fmls;
}
