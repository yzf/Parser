#include "HengZhang.h"
#include "utility.h"
#include "S2DLP.h"

#define DEBUG

extern Vocabulary vocabulary;
/**
 * 
 * @param name
 * @param type
 * @param arity
 * @return 
 */
int HengZhang::addSymbol(const char* name, SYMBOL_TYPE type, int arity) {
    char name_buf[512];
    int i = 0;

    sprintf(name_buf,"%s",name);

    while( vocabulary.query_symbol(name_buf,type) >= 0) // symbol exist
    {
        sprintf(name_buf,"%s_%d",name,i);
    }

    return vocabulary.add_symbol(name_buf,type,arity);
}
/**
 * 把量词保存到对应的vector
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::recordQuantifier(Formula originalFml) {
    char str_buf[512];
    _formula* fml_temp;

    terms_X.clear();
    terms_Y.clear();
    terms_Z.clear();
    terms_MIN.clear();
    terms_MAX.clear();
    
    int i = 0;
    _formula* fml = originalFml.get_formula();
    while(fml->formula_type == UNIV | fml->formula_type == EXIS)
    {
        if (fml->formula_type == UNIV) {
            terms_X.push_back(fml->variable_id);
            fml_temp = fml;
            fml = fml->subformula_l;
        }
        else {
            terms_Y.push_back(fml->variable_id);
        terms_MIN.push_back(symbol_MIN);
        terms_MAX.push_back(symbol_MAX);

        sprintf(str_buf,"NV_%d",i++);
        terms_Z.push_back(addSymbol(str_buf, VARIABLE, 0));

        fml_temp = fml;
        fml = fml->subformula_l;
        }
    }

    return Formula(fml, true);
}

Formulas HengZhang::create(Formula fml) {
    symbol_MAX = addSymbol("MAX", VARIABLE, 0);
    symbol_MIN = addSymbol("MIN", VARIABLE, 0);
    
    Formula originalFml = recordQuantifier(fml);

    if(terms_Y.size() == 0)
    {
        Formulas fmls;
        fmls.push_formula(fml);
        return fmls;
    }


    symbol_s = addSymbol("s", PREDICATE, terms_X.size()+terms_Y.size());
    symbol_t = addSymbol("t", PREDICATE, terms_X.size()+terms_Y.size());
    symbol_succ = addSymbol("succ", PREDICATE, terms_Y.size()+terms_Z.size());
	
    Formulas fmls;
    fmls.push_formula(createFormula_1(originalFml));
    fmls.push_formula(createFormula_2(originalFml));
    fmls.push_formula(createFormula_3(originalFml));
    fmls.push_formula(createFormula_4_1(originalFml));
    fmls.push_formula(createFormula_4_2(originalFml));
    fmls.push_formula(createFormula_5(originalFml));

    return fmls;
}
/**
 * 章衡量词消去公式一    ~~s(_X,_MIN)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_1(Formula originalFml) {
    // Add S(_X, _MIN)
    _term* LLT   = combine_terms(terms_X, terms_MIN);
    _formula* LL = composite_atom(ATOM, symbol_s, LLT);

    // create structure
    _formula* L  = composite_bool(NEGA, LL, NULL);
    _formula* F  = composite_bool(NEGA, L,  NULL);
    
    Formula fml = Formula(F, false);
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式二    (_succ(_Y,_Z) ^ s(_X,_Z)) v theta__(_X,_Y) -> s(_X,_Y)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_2(Formula originalFml) {
    //create atom formulas
    _term* LLLT = combine_terms(terms_Y, terms_Z);
    _formula* LLL = composite_atom(ATOM, symbol_succ, LLLT);

    //2
    _term* LLRT = combine_terms(terms_X, terms_Z);
    _formula* LLR = composite_atom(ATOM, symbol_s, LLRT);

    //3
    Formula tmp = originalFml;
    tmp.double_negation(vocabulary.index_intension_predicate,
            vocabulary.num_intension_predicate);
    _formula* LR = copy_formula(tmp.get_formula());


    //4
    _term* RT = combine_terms(terms_X, terms_Y);
    _formula* R   = composite_atom(ATOM, symbol_s, RT);

    //create structure
    _formula* LL  = composite_bool(CONJ,LLL,LLR);
    _formula* L   = composite_bool(DISJ,LL,LR);
    _formula* F   = composite_bool(IMPL,L,R);
    
    Formula fml = Formula(F, false);
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式三    t(_X, _MIN) v theta(_X,_MIN)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_3(Formula originalFml) {
    //create left sub-formula t(_X,_MIN)
    _term* LT = combine_terms(terms_X, terms_MIN);
    _formula* L   = composite_atom(ATOM, symbol_t, LT );

    //create right sub-formula theta(_X,_MIN)
    Formula tmp = Formula(originalFml);
    tmp.replace_terms(terms_Y, terms_MIN);
    _formula* R = copy_formula(tmp.get_formula());

    //create structure
    _formula* F   = composite_bool(DISJ,L,R);
    
    Formula fml = Formula(F, false);
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式四_1    S(_X,_Y) ^ ~S(_X,_Z) ^ succ(_Y,_Z) 
 *                      -> ( (T(_X,_MAX) -> theta(_X,_Y)) 
 *                            ^ (theta(_X,_Y) -> T(_X,_MAX)) )
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_4_1(Formula originalFml) {
    //1 S(_X,_Y)
    _term* term_x_y = combine_terms(terms_X, terms_Y);
    _formula* s_x_y = composite_atom(ATOM, symbol_s, term_x_y);

    //2 ~S(_X,_Z)
    _term* term_x_z = combine_terms(terms_X,terms_Z);
    _formula* s_x_z  = composite_atom(ATOM, symbol_s, term_x_z);
    _formula* _s_x_z = composite_bool(NEGA, s_x_z, NULL);

    //3 succ(_Y,_Z)
    _term* term_y_z = combine_terms(terms_Y, terms_Z);
    _formula* succ_y_z = composite_atom(ATOM, symbol_succ, term_y_z);

    //4 T(_X,_MAX)
    _term* term_x_max = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max  = composite_atom(ATOM, symbol_t, term_x_max);

    //5 theta(_X,_Y)
    Formula tmp_formula_1 = Formula(originalFml);
    _formula* theta_x_y  = copy_formula(tmp_formula_1.get_formula());

    //6 theta(_X,_Y)
    Formula tmp_formula_2 = Formula(originalFml);
    _formula* theta_x_y_2  = copy_formula(tmp_formula_2.get_formula());
    
    //7 T(_X,_MAX)
    _term* term_x_max_2 = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max_2  = composite_atom(ATOM, symbol_t, term_x_max_2);

    //create structure
    // S(_X,_Y) ^ ~S(_X,_Z) ^ succ(_Y,_Z) 
    _formula* left = composite_bool(CONJ, s_x_y, _s_x_z);
    left = composite_bool(CONJ, left, succ_y_z);
    // (T(_X,_MAX) -> theta(_X,_Y)
    _formula* right_left = composite_bool(IMPL, t_x_max, theta_x_y);
    // (theta(_X,_Y) -> T(_X,_MAX)
    _formula* right_right = composite_bool(IMPL, theta_x_y_2, t_x_max_2);
    _formula* right = composite_bool(CONJ, right_left, right_right);
    
    _formula* F = composite_bool(IMPL, left, right);
    
    Formula fml = Formula(F, false);
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式四_2    ( T(_X,_MAX) -> theta(_X,_MAX) ) 
 *                        ^ ( theta(_X,_MAX) -> T(_X,_MAX) ) 
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_4_2(Formula originalFml) {
    //1 T(_X,_MAX)
    _term* term_x_max = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max  = composite_atom(ATOM, symbol_t, term_x_max);

    //2 theta(_X,_MAX)
    Formula tmp_formula_1 = Formula(originalFml);
    tmp_formula_1.replace_terms(terms_Y, terms_MAX);
    _formula* theta_x_max  = copy_formula(tmp_formula_1.get_formula());
    
    //3 theta(_X,_MAX)
    Formula tmp_formula_2 = Formula(originalFml);
    tmp_formula_2.replace_terms(terms_Y, terms_MAX);
    _formula* theta_x_max_2 = copy_formula(tmp_formula_2.get_formula());
    
    //4 T(_X,_MAX)
    _term* term_x_max_2 = combine_terms(terms_X, terms_MAX);
    _formula* t_x_max_2  = composite_atom(ATOM, symbol_t, term_x_max_2);
    
    _formula* left = composite_bool(IMPL, t_x_max, theta_x_max);
    _formula* right = composite_bool(IMPL, theta_x_max_2, t_x_max_2);
    _formula* F = composite_bool(CONJ, left, right);
    
    Formula fml = Formula(F, false);
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}
/**
 * 章衡量词消去公式五    _succ(_Y,_Z) -> 
 *      ( ( T(_X,_Y)-> (theta(_X,_Z) | T(_X,_Z)) ) ^ 
 *         ( (theta(_X,_Z) | T(_X,_Z)) -> T(_X,_Y) ) )
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_5(Formula originalFml) {
    //1 _succ(_Y,_Z)
    _term* tmp_term_1 = combine_terms(terms_Y, terms_Z);
    _formula* SUCC  = composite_atom(ATOM, symbol_succ, tmp_term_1);

    //2 T(_X,_Y)
    _term* tmp_term_2 = combine_terms(terms_X, terms_Y);
    _formula* TXY_1  = composite_atom(ATOM, symbol_t, tmp_term_2);

    //3 theta(_X,_Z)
    Formula tmp_formula_1 = Formula(originalFml);
    tmp_formula_1.replace_terms(terms_Y, terms_Z);
    _formula* THETA_1 = copy_formula(tmp_formula_1.get_formula());

    //4 T(_X,_Z)
    _term* tmp_term_3 = combine_terms(terms_X, terms_Z);
    _formula* TXZ_1 = composite_atom(ATOM, symbol_t, tmp_term_3);
    
    //5 theta(_X,_Z)
    Formula tmp_formula_2 = Formula(originalFml);
    tmp_formula_2.replace_terms(terms_Y, terms_Z);
    _formula* THETA_2= copy_formula(tmp_formula_2.get_formula());
    
    //6 T(_X,_Z)
    _term* tmp_term_4 = combine_terms(terms_X, terms_Z);
    _formula* TXZ_2 = composite_atom(ATOM, symbol_t, tmp_term_4);

    //7 T(_X,_Y)
    _term* tmp_term_5 = combine_terms(terms_X, terms_Y);
    _formula* TXY_2  = composite_atom(ATOM, symbol_t, tmp_term_5);

    //create structure
    _formula* RLR = composite_bool(DISJ, THETA_1, TXZ_1);
    _formula* RL = composite_bool(IMPL, TXY_1, RLR);
    _formula* RRL = composite_bool(DISJ, THETA_2, TXZ_2);
    _formula* RR = composite_bool(IMPL, RRL, TXY_2);
    _formula* FF = composite_bool(CONJ, RL, RR);
    _formula* F = composite_bool(IMPL, SUCC, FF);
    
    Formula fml = Formula(F, false);
#ifdef DEBUG
    fml.output(stdout);
    fprintf(stdout, "\n");
#endif
    
    return fml;
}