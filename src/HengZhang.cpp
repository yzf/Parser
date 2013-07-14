#include "HengZhang.h"
#include "utility.h"
#include "S2DLP.h"

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
    _formula* fml = originalFml.getFormula();
    while(fml->formula_type == UNIV)
    {
        terms_X.push_back(fml->variable_id);
        fml_temp = fml;
        fml = fml->subformula_l;
    }
    while(fml->formula_type == EXIS)
    {
        terms_Y.push_back(fml->variable_id);
        terms_MIN.push_back(symbol_MIN);
        terms_MAX.push_back(symbol_MAX);

        sprintf(str_buf,"NV_%d",i++);
        terms_Z.push_back(addSymbol(str_buf, VARIABLE, 0));

        fml_temp = fml;
        fml = fml->subformula_l;
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
    fmls.push_formula(createFormula_4(originalFml));
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
    fml.output(stdout);
    fprintf(stdout, "\n");
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
    _formula* LR = copy_formula(tmp.getFormula());


    //4
    _term* RT = combine_terms(terms_X, terms_Y);
    _formula* R   = composite_atom(ATOM, symbol_s, RT);

    //create structure
    _formula* LL  = composite_bool(CONJ,LLL,LLR);
    _formula* L   = composite_bool(DISJ,LL,LR);
    _formula* F   = composite_bool(IMPL,L,R);
    
    Formula fml = Formula(F, false);
    fml.output(stdout);
    fprintf(stdout, "\n");
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
    _formula* R = copy_formula(tmp.getFormula());

    //create structure
    _formula* F   = composite_bool(DISJ,L,R);
    
    Formula fml = Formula(F, false);
    fml.output(stdout);
    fprintf(stdout, "\n");
    return fml;
}
/**
 * 章衡量词消去公式四    -(_succ(_Y,_Z)) v -(S(_X,_Z)) -> ( t(_T,_MAX)->theta(_X,_Y)
 * @param originalFml 一阶语句
 * @return 
 */
Formula HengZhang::createFormula_4(Formula originalFml) {
    //create atom formulas
    _term* LLLLT = combine_terms(terms_Y, terms_Z);
    _formula* LLLL  = composite_atom(ATOM, symbol_succ, LLLLT);

    //2
    _term* LLRLT = combine_terms(terms_X,terms_Z);
    _formula* LLRL  = composite_atom(ATOM, symbol_s, LLRLT);

    //3
    _term* LRT = combine_terms(terms_X, terms_Y);
    _formula* LR  = composite_atom(ATOM, symbol_s, LRT);

    //4
    _term* RLLT = combine_terms(terms_X, terms_MAX);
    _formula* RLL  = composite_atom(ATOM, symbol_t, RLLT);

    //5
    _formula* RLR = copy_formula(originalFml.getFormula());

    //6
    _formula* RRL = copy_formula(originalFml.getFormula());

    //7
    _term* RRRT = combine_terms(terms_X, terms_MAX);
    _formula* RRR  = composite_atom(ATOM, symbol_t, RRRT);

    //create structure
    _formula* LLL = composite_bool(NEGA,LLLL,NULL);
    _formula* LLR = composite_bool(NEGA,LLRL,NULL);
    _formula* LL  = composite_bool(DISJ,LLL,LLR);
    _formula* L   = composite_bool(CONJ,LL,LR);
    _formula* RL  = composite_bool(IMPL,RLL,RLR);
    _formula* RR  = composite_bool(IMPL,RRL,RRR);
    _formula* R   = composite_bool(CONJ,RL,RR);
    _formula* F   = composite_bool(IMPL,L,R);
    
    Formula fml = Formula(F, false);
    fml.output(stdout);
    fprintf(stdout, "\n");
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
    _term* LT = combine_terms(terms_Y, terms_Z);
    _formula* SUCC  = composite_atom(ATOM, symbol_succ, LT);

    //2 T(_X,_Y)
    _term* RLLLT = combine_terms(terms_X, terms_Y);
    _formula* TXY  = composite_atom(ATOM, symbol_t, RLLLT);

    //3 theta(_X,_Z)
    Formula tmp = Formula(originalFml);
    tmp.replace_terms(terms_Y, terms_Z);
    _formula* THETA= copy_formula(tmp.getFormula());

    //4 T(_X,_Z)
    _term* RLRRT = combine_terms(terms_X, terms_Z);
    _formula* TXZ  = composite_atom(ATOM, symbol_t, RLRRT);

    //create structure
    _formula* RLR = composite_bool(DISJ, THETA, TXZ);
    _formula* RL = composite_bool(IMPL, TXY, RLR);
    _formula* RRL = composite_bool(DISJ, THETA, TXZ);
    _formula* RR = composite_bool(IMPL, RRL, TXY);
    _formula* FF = composite_bool(CONJ, RL, RR);
    _formula* F = composite_bool(IMPL, SUCC, FF);
    
    Formula fml = Formula(F, true);
    fml.output(stdout);
    fprintf(stdout, "\n");
    return fml;
}