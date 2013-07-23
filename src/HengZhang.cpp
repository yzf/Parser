#include "HengZhang.h"
#include "utility.h"
#include "S2DLP.h"
#include <stdlib.h>

//#define DEBUG

extern Vocabulary vocabulary;

HengZhang::HengZhang() {
}

HengZhang::HengZhang(const HengZhang& rhs) {
}
HengZhang& HengZhang::operator = (const HengZhang& rhs) {
    return *this;
}
HengZhang::~HengZhang() {
}

HengZhang& HengZhang::instance() {
    static HengZhang theSingleton;
    return theSingleton;
}
/**
 * 
 * @param name
 * @param type
 * @param arity
 * @return 
 */
int HengZhang::add_symbol(const char* name, SYMBOL_TYPE type, int arity) {
    return vocabulary.add_symbol(name,type,arity);
}
/**
 * 把量词保存到对应的vector
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::record_quantifier(Formula original_fml) {
    char str_buf[512];
    _formula* fml_temp;

    terms_X.clear();
    terms_Y.clear();
    terms_Z.clear();
    terms_MIN.clear();
    terms_MAX.clear();
    
    _formula* fml = original_fml.get_formula();
    while(fml->formula_type == UNIV)
    {
        terms_X.push_back(fml->variable_id);
        fml_temp = fml;
        fml = fml->subformula_l;
    }
    while (fml->formula_type == EXIS) 
    {
        terms_Y.push_back(fml->variable_id);
            
        char* domain = vocabulary.names_domain[vocabulary.variable_at_domain[fml->variable_id]];
        string name = string("MIN_") + domain;
        int id_min = add_symbol(name.c_str(), VARIABLE, 0);

        terms_MIN.push_back(id_min);

        name = string("MAX_") + domain;
        int id_max = add_symbol(name.c_str(), VARIABLE, 0);
        terms_MAX.push_back(id_max);

        sprintf(str_buf,"NV_%d", this->num_NV ++);
        terms_Z.push_back(add_symbol(str_buf, VARIABLE, 0));

        fml_temp = fml;
        fml = fml->subformula_l;
    }
    Formula ret = Formula(fml, true);
    ret.formula_id = original_fml.formula_id;
    return ret;
}

void HengZhang::save_succ_name(string succ_name, vector<string> domain_name) {
    //保存succ名字
    bool flag = true;
    for (vector<string>::iterator it = this->succ_names.begin();
            it != this->succ_names.end(); it ++) {
        if (*it == succ_name) {
            flag = false;
            break;
        }
    }
    if (flag) {
        this->succ_names.push_back(succ_name);
        this->domain_names.push_back(domain_name);
    }
}

Formulas HengZhang::create(Formulas fmls) {

    S2DLP::instance().formula_tree.root = TreeNode(Formula());
    vector<TreeNode> children;
    Formula tmp_fml;
    Formulas tmp = fmls;
    while (tmp.size_formulas() > 0) {
        tmp_fml = tmp.top_formula();
        children.push_back(TreeNode(tmp_fml));
        tmp.pop_formula();
    }
    S2DLP::instance().formula_tree.insert_node_children(children, 
            S2DLP::instance().formula_tree.root.formula.formula_id);
    
    Formulas temp_fmls = fmls;
    Formulas final_fmls;
    
    while (temp_fmls.size_formulas() != 0) {
        Formula cur_fml = temp_fmls.top_formula();
        temp_fmls.pop_formula();
        cur_fml.convert_prenex();
        if (cur_fml.is_universal()) {
            final_fmls.push_formula(cur_fml);
        }
        else {
            cur_fml.fix_universal_quantifier();
            Formulas hz_result = transform(cur_fml);
            temp_fmls.join_formulas(hz_result);
            
            tmp = hz_result;
            children.clear();
            while (tmp.size_formulas() > 0) {
                tmp_fml = tmp.top_formula();
                children.push_back(TreeNode(tmp_fml));
                tmp.pop_formula();
            }
            S2DLP::instance().formula_tree.insert_node_children(children, 
                                                cur_fml.formula_id);
#ifdef DEBUG
            printf("prenex:\n");
            cur_fml.output(stdout);
            printf("hengzhang:\n");
            hz_result.output_formulas(stdout);
#endif
        }
    }

    return final_fmls;
}

Formulas HengZhang::transform(Formula fml) {
    Formula originalFml = record_quantifier(fml);

    if(terms_Y.size() == 0)
    {
        Formulas fmls;
        fmls.push_formula(fml);
        return fmls;
    }
    char name_buf[512];
    sprintf(name_buf, "s_%d", num_s ++);
    symbol_s = add_symbol(name_buf, PREDICATE, terms_X.size()+terms_Y.size());
    vocabulary.set_intension_predicate(name_buf);
 
    sprintf(name_buf, "t_%d", num_t ++);
    symbol_t = add_symbol(name_buf, PREDICATE, terms_X.size()+terms_Y.size());
    vocabulary.set_intension_predicate(name_buf);
 
    string succ_name = "succ";
    vector<string> domain_name;
    for (int i = 0; i < terms_Y.size(); ++ i) {
        succ_name += string("_") + vocabulary.names_domain[vocabulary.variable_at_domain[terms_Y[i]]];
        domain_name.push_back(vocabulary.names_domain[vocabulary.variable_at_domain[terms_Y[i]]]);
    }
    symbol_succ = add_symbol(succ_name.c_str(), PREDICATE, terms_Y.size()+terms_Z.size());
//    vocabulary.set_intension_predicate(succ_name.c_str());
    save_succ_name(succ_name, domain_name);
    
    Formulas fmls;
    fmls.push_formula(create_formula_1(originalFml));
    fmls.push_formula(create_formula_2(originalFml));
    fmls.push_formula(create_formula_3(originalFml));
//    fmls.push_formula(create_formula_4_old(originalFml));
//    fmls.push_formula(create_formula_4(originalFml));
    fmls.push_formula(create_formula_4_1(originalFml));
    fmls.push_formula(create_formula_4_2(originalFml));
//    fmls.push_formula(create_formula_5(originalFml));
    fmls.push_formula(create_formula_5_1(originalFml));
    fmls.push_formula(create_formula_5_2(originalFml));
    return fmls;
}
/**
 * 章衡量词消去公式一    ~~s(_X,_MIN)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_1(Formula original_fml) {
    // Add S(_X, _MIN)
    _term* term_x_min   = combine_terms(terms_X, terms_MIN);
    _formula* s_x_min = composite_atom(ATOM, symbol_s, term_x_min);

    // create structure
    _formula* F  = composite_bool(NEGA, s_x_min, NULL);
    F  = composite_bool(NEGA, F,  NULL);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式二    ((succ(_Y,_Z) & s(_X,_Z)) | theta__(_X,_Y)) -> s(_X,_Y)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_2(Formula original_fml) {
    //create atom formulas
    //1 (succ(_Y,_Z)
    _term* term_y_z = combine_terms(terms_Y, terms_Z);
    _formula* succ_y_z = composite_atom(ATOM, symbol_succ, term_y_z);

    //2 s(_X,_Z)
    _term* term_x_z = combine_terms(terms_X, terms_Z);
    _formula* s_x_z = composite_atom(ATOM, symbol_s, term_x_z);

    //3 theta__(_X,_Y)
    original_fml.double_negation(vocabulary.index_intension_predicate,
            vocabulary.num_intension_predicate);
    _formula* theta__ = copy_formula(original_fml.get_formula());


    //4 s(_X,_Y)
    _term* term_x_y = combine_terms(terms_X, terms_Y);
    _formula* s_x_y   = composite_atom(ATOM, symbol_s, term_x_y);

    //create structure
    _formula* ll = composite_bool(CONJ, succ_y_z, s_x_z);
    _formula* l = composite_bool(DISJ, ll, theta__);
    _formula* r = s_x_y;
    _formula* F = composite_bool(IMPL, l, r);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式三    t(_X,_MIN) | theta(_X,_MIN)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_3(Formula original_fml) {
    //create left sub-formula t(_X,_MIN)
    _term* term_x_min = combine_terms(terms_X, terms_MIN);
    _formula* t_x_min   = composite_atom(ATOM, symbol_t, term_x_min);

    //create right sub-formula theta(_X,_MIN)
    original_fml.replace_terms(terms_Y, terms_MIN);
    _formula* theta_x_min = copy_formula(original_fml.get_formula());

    //create structure
    _formula* F   = composite_bool(DISJ, t_x_min, theta_x_min);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡消去量词公式四 论文公式：
 *      (~( succ(_Y,_Z) & s(_X,_Z)) & s(_X,_Y)) -> 
 *              ( (t(_X,_MAX) -> theta(_X,_Y)) & 
 *                      (theta(_X,_Y) -> t(_X,_MAX)) )
 * @param original_fml
 * @return 
 */
Formula HengZhang::create_formula_4_old(Formula original_fml) {
    _term* term_y_z = combine_terms(terms_Y, terms_Z);
    _formula* succ_y_z = composite_atom(ATOM, symbol_succ, term_y_z);
    
    _term* term_x_z = combine_terms(terms_X, terms_Z);
    _formula* s_x_z = composite_atom(ATOM, symbol_s, term_x_z);
    
    _term* term_x_y = combine_terms(terms_X, terms_Y);
    _formula* s_x_y = composite_atom(ATOM, symbol_s, term_x_y);
    
    _formula* ll = composite_bool(CONJ, succ_y_z, s_x_z);
    ll = composite_bool(NEGA, ll, NULL);
    _formula* left = composite_bool(CONJ, ll, s_x_y);
    
    _term* term_x_max = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max = composite_atom(ATOM, symbol_t, term_x_max);
    _formula* theta_x_y = copy_formula(original_fml.get_formula());
    _formula* theta_x_y_2 = copy_formula(original_fml.get_formula());
    _term* term_x_max_2 = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max_2 = composite_atom(ATOM, symbol_t, term_x_max_2);
    
    _formula* rl = composite_bool(IMPL, t_x_max, theta_x_y);
    _formula* rr = composite_bool(IMPL, theta_x_y_2, t_x_max_2);
    _formula* right = composite_bool(CONJ, rl, rr);
    
    _formula* F = composite_bool(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml; 
}
// (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y)))
_formula* HengZhang::generate_formula_4_left() {
    // 1 S(_X,_Y)
    _term* term_x_y = combine_terms(terms_X, terms_Y);
    _formula* s_x_y = composite_atom(ATOM, symbol_s, term_x_y);
    // 2 ( ~S(_X,_Z)
    _term* term_x_z = combine_terms(terms_X, terms_Z);
    _formula* s_x_z = composite_atom(ATOM, symbol_s, term_x_z);
    _formula* _s_x_z = composite_bool(NEGA, s_x_z, NULL);
    // 3 succ(_Y,_Z)
    _term* term_y_z = combine_terms(terms_Y, terms_Z);
    _formula* succ_y_z = composite_atom(ATOM, symbol_succ, term_y_z);
    // 4 max_domian(_Y)
    vector<Formula> max_ys;
    for (int i = 0; i < terms_Y.size(); ++ i) {
        string name = string("max_") + vocabulary.names_domain[vocabulary.variable_at_domain[terms_Y[i]]];
        _term* term_yi = (_term*)malloc(sizeof(_term));
        term_yi->term_type = VARI;
        term_yi->variable_id = terms_Y[i];
        int pre_id = add_symbol(name.c_str(), PREDICATE, 1);
        _formula* max_yi = composite_atom(ATOM, pre_id, term_yi);
        max_ys.push_back(Formula(max_yi, false));
//        vocabulary.set_intension_predicate(name.c_str());
    }
    _formula* max_y = copy_formula(max_ys[0].get_formula());
    for (int i = 1; i < max_ys.size(); ++ i) {
        max_y = composite_bool(CONJ, max_y, copy_formula(max_ys[i].get_formula()));
    }
    _formula* lrl = composite_bool(CONJ, _s_x_z, succ_y_z);
    _formula* lr = composite_bool(DISJ, lrl, max_y);
    _formula* l = composite_bool(CONJ, s_x_y, lr);
    return l;
}
/**
 * 章衡量词消去公式四   (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y))) 
 *                      -> ( (t(_X,_MAX) -> theta(_X,_Y)) & 
 *                              (theta(_X,_Y) -> t(_X,_MAX)) )
 *    _max(Y) = max_domain1(Y1) ^ max_domain2(Y2) ^ ... ^ max_domainN(YN)
 * @param original_fml
 * @return 
 */
Formula HengZhang::create_formula_4(Formula original_fml) {
    // 5 T(_X,_MAX)
    _term* term_x_max = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max  = composite_atom(ATOM, symbol_t, term_x_max);
    // 6 theta(_X,_Y)
    _formula* theta_x_y  = copy_formula(original_fml.get_formula());
    // 7 theta(_X,_Y)
    _formula* theta_x_y_2  = copy_formula(original_fml.get_formula());
    // 8 T(_X,_MAX)
    _term* term_x_max_2 = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max_2  = composite_atom(ATOM, symbol_t, term_x_max_2);
    
    _formula* l = generate_formula_4_left();
    _formula* rl = composite_bool(IMPL, t_x_max, theta_x_y);
    _formula* rr = composite_bool(IMPL, theta_x_y_2, t_x_max_2);
    _formula* r = composite_bool(CONJ, rl, rr);
    _formula* F = composite_bool(IMPL, l, r);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml; 
}
/**
 * 章衡量词消去公式四_1    (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y))) 
 *                              -> (t(_X,_MAX) -> theta(_X,_Y)) 
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_4_1(Formula original_fml) {
    // 5 T(_X,_MAX)
    _term* term_x_max = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max  = composite_atom(ATOM, symbol_t, term_x_max);
    // 6 theta(_X,_Y)
    _formula* theta_x_y  = copy_formula(original_fml.get_formula());
    
    _formula* left = generate_formula_4_left();
    _formula* right = composite_bool(IMPL, t_x_max, theta_x_y);
    
    _formula* F = composite_bool(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式四_2    (s(_X,_Y) & ((~s(_X,_Z) & succ(_Y,_Z)) | _max(Y))) 
 *                              -> (theta(_X,_Y) -> t(_X,_MAX)) 
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_4_2(Formula original_fml) {
    // 5 theta(_X,_Y)
    _formula* theta_x_y  = copy_formula(original_fml.get_formula());
    // 6 T(_X,_MAX)
    _term* term_x_max = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max  = composite_atom(ATOM, symbol_t, term_x_max);
    
    _formula* left = generate_formula_4_left();
    _formula* right = composite_bool(IMPL, theta_x_y, t_x_max);
    
    _formula* F = composite_bool(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式五    succ(_Y,_Z) -> 
 *              ( (t(_X,_Y) -> (theta(_X,_Z) | t(_X,_Z))) & 
 *                      ((theta(_X,_Z) | t(_X,_Z)) -> t(_X,_Y)) )
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_5(Formula original_fml) {
    //1 _succ(_Y,_Z)
    _term* term_y_z = combine_terms(terms_Y, terms_Z);
    _formula* succ_y_z  = composite_atom(ATOM, symbol_succ, term_y_z);

    //2 T(_X,_Y)
    _term* term_x_y = combine_terms(terms_X, terms_Y);
    _formula* t_x_y  = composite_atom(ATOM, symbol_t, term_x_y);

    //3 theta(_X,_Z)
    Formula tmp_formula_1 = Formula(original_fml);
    tmp_formula_1.replace_terms(terms_Y, terms_Z);
    _formula* theta_x_z = copy_formula(tmp_formula_1.get_formula());

    //4 T(_X,_Z)
    _term* term_x_z = combine_terms(terms_X, terms_Z);
    _formula* t_x_z = composite_atom(ATOM, symbol_t, term_x_z);
    
    //5 theta(_X,_Z)
    Formula tmp_formula_2 = Formula(original_fml);
    tmp_formula_2.replace_terms(terms_Y, terms_Z);
    _formula* theta_x_z_2 = copy_formula(tmp_formula_2.get_formula());
    
    //6 T(_X,_Z)
    _term* term_x_z_2 = combine_terms(terms_X, terms_Z);
    _formula* t_x_z_2 = composite_atom(ATOM, symbol_t, term_x_z_2);

    //7 T(_X,_Y)
    _term* term_x_y_2 = combine_terms(terms_X, terms_Y);
    _formula* t_x_y_2  = composite_atom(ATOM, symbol_t, term_x_y_2);

    //create structure
    _formula* rlr = composite_bool(DISJ, theta_x_z, t_x_z);
    _formula* rl = composite_bool(IMPL, t_x_y, rlr);
    _formula* rrl = composite_bool(DISJ, theta_x_z_2, t_x_z_2);
    _formula* rr = composite_bool(IMPL, rrl, t_x_y_2);
    _formula* ff = composite_bool(CONJ, rl, rr);
    _formula* F = composite_bool(IMPL, succ_y_z, ff);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式五 1    succ(_Y,_Z) -> (t(_X,_Y) -> (theta(_X,_Z) | t(_X,_Z)))
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_5_1(Formula original_fml) {
    //1 _succ(_Y,_Z)
    _term* term_y_z = combine_terms(terms_Y, terms_Z);
    _formula* succ_y_z  = composite_atom(ATOM, symbol_succ, term_y_z);

    //2 T(_X,_Y)
    _term* term_x_y = combine_terms(terms_X, terms_Y);
    _formula* t_x_y  = composite_atom(ATOM, symbol_t, term_x_y);

    //3 theta(_X,_Z)
    Formula tmp_formula_1 = Formula(original_fml);
    tmp_formula_1.replace_terms(terms_Y, terms_Z);
    _formula* theta_x_z = copy_formula(tmp_formula_1.get_formula());

    //4 T(_X,_Z)
    _term* term_x_z = combine_terms(terms_X, terms_Z);
    _formula* t_x_z = composite_atom(ATOM, symbol_t, term_x_z);

    //create structure
    _formula* left = succ_y_z;
    _formula* rr = composite_bool(DISJ, theta_x_z, t_x_z);
    _formula* right = composite_bool(IMPL, t_x_y, rr);
    _formula* F = composite_bool(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式五 2   succ(_Y,_Z) -> ((theta(_X,_Z) | t(_X,_Z)) -> t(_X,_Y)) 
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::create_formula_5_2(Formula original_fml) {
    //1 _succ(_Y,_Z)
    _term* term_y_z = combine_terms(terms_Y, terms_Z);
    _formula* succ_y_z  = composite_atom(ATOM, symbol_succ, term_y_z);

    //2 T(_X,_Y)
    _term* term_x_y = combine_terms(terms_X, terms_Y);
    _formula* t_x_y  = composite_atom(ATOM, symbol_t, term_x_y);

    //3 theta(_X,_Z)
    Formula tmp_formula_1 = Formula(original_fml);
    tmp_formula_1.replace_terms(terms_Y, terms_Z);
    _formula* theta_x_z = copy_formula(tmp_formula_1.get_formula());

    //4 T(_X,_Z)
    _term* term_x_z = combine_terms(terms_X, terms_Z);
    _formula* t_x_z = composite_atom(ATOM, symbol_t, term_x_z);

    //create structure
    _formula* left = succ_y_z;
    _formula* rl = composite_bool(DISJ, theta_x_z, t_x_z);
    _formula* right = composite_bool(IMPL, rl, t_x_y);
    _formula* F = composite_bool(IMPL, left, right);
    
    Formula fml = Formula(F, false);
    fml.formula_id = Formula::new_formula_id ++;
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}