//#include "HengZhang.h"
//#include "utility.h"
//
///**
// * 
// * @param name
// * @param type
// * @param arity
// * @return 
// */
//int HengZhang::addSymbol(const char* name, SYMBOL_TYPE type, int arity) {
//    char name_buf[512];
//    int i = 0;
//
//    sprintf(name_buf,"%s",name);
//
//    while( vocabulary.query_symbol(name_buf,type) >= 0) // symbol exist
//    {
//        sprintf(name_buf,"%s_%d",name,i);
//    }
//
//    return vocabulary.add_symbol(name_buf,type,arity);
//}
///**
// * 把量词保存到对应的vector
// * @param originalFml 一阶语句
// * @return 
// */
//Formula HengZhang::recordQuantifier(Formula originalFml) {
//    char str_buf[512];
//    _formula* fml_temp;
//
//    terms_X.clear();
//    terms_Y.clear();
//    terms_Z.clear();
//    terms_MIN.clear();
//    terms_MAX.clear();
//    
//    int i = 0;
//    _formula* fml = originalFml.getFormula();
//    while(fml->formula_type == UNIV)
//    {
//        terms_X.push_back(fml->variable_id);
//        fml_temp = fml;
//        fml = fml->subformula_l;
//    }
//    while(fml->formula_type == EXIS)
//    {
//        terms_Y.push_back(fml->variable_id);
//        terms_MIN.push_back(symbol_MIN);
//        terms_MAX.push_back(symbol_MAX);
//
//        sprintf(str_buf,"NV_%d",i++);
//        terms_Z.push_back(addSymbol(str_buf, VARIABLE, 0));
//
//        fml_temp = fml;
//        fml = fml->subformula_l;
//    }
//
//    return fml;
//}
///*
//Formulas HengZhang::create(Formula originalFml) {
//    return NULL;
//}*/
///**
// * 章衡量词消去公式一
// * @param originalFml 一阶语句
// * @return 
// */
//Formula HengZhang::createFormula_1(Formula originalFml) {
//    return NULL;
//}
///**
// * 章衡量词消去公式二
// * @param originalFml 一阶语句
// * @return 
// */
//Formula HengZhang::createFormula_2(Formula originalFml) {
//    return NULL;
//}
///**
// * 章衡量词消去公式三
// * @param originalFml 一阶语句
// * @return 
// */
//Formula HengZhang::createFormula_3(Formula originalFml) {
//    return NULL;
//}
///**
// * 章衡量词消去公式四
// * @param originalFml 一阶语句
// * @return 
// */
//Formula HengZhang::createFormula_4(Formula originalFml) {
//    return NULL;
//}
///**
// * 章衡量词消去公式五
// * @param originalFml 一阶语句
// * @return 
// */
//Formula HengZhang::createFormula_5(Formula originalFml) {
//    return NULL;
//}