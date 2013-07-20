#ifndef _CABALAR_H_
#define _CABALAR_H_

#include "Formula.h"
#include "structs.h"
#include "Formulas.h"
#include "Vocabulary.h"
#include "utility.h"

class Formula;
class Formulas;

class Cabalar{
private:
    _formula* Cabalar_DLT(_formula* originFml);
    _formula* Cabalar_DLF(_formula* originFml);
    _formula* Cabalar_CLT(_formula* originFml);
    _formula* Cabalar_CLF(_formula* originFml);

    _formula* Cabalar_ILT(_formula* originFml);
    _formula* Cabalar_ILF(_formula* originFml);
    _formula* Cabalar_IRT(_formula* originFml);
    _formula* Cabalar_IRF(_formula* originFml);

    _formula* Cabalar_N1(_formula* originFml);
    _formula* Cabalar_N2(_formula* originFml);
    _formula* Cabalar_N3(_formula* originFml);
    _formula* Cabalar_N4(_formula* originFml);
    _formula* Cabalar_N5(_formula* originFml);
    _formula* Cabalar_N6(_formula* originFml);

    Formulas Cabalar_L1(_formula* originFml);
    Formulas Cabalar_L2(_formula* originFml);
    Formulas Cabalar_L3(_formula* originFml);
    Formulas Cabalar_L4(_formula* originFml);
    Formulas Cabalar_L5(_formula* originFml);

    Formulas Cabalar_R1(_formula* originFml);
    Formulas Cabalar_R2(_formula* originFml);
    Formulas Cabalar_R3(_formula* originFml);
    Formulas Cabalar_R4(_formula* originFml);
    Formulas Cabalar_R5(_formula* originFml);

    Cabalar();  
     //tools
    int Commutative_Trans(_formula* root, _formula* curr,
                           FORMULA_TYPE commutativeSymbol);
    _formula* convert_negative_normal_form(_formula* fml);//Negative Normal Form
    Formulas convert_negative_normal_forms(Formulas fmls);
    Formulas Cabalar_Trans(_formula* fml);
        
public:
    Formulas convert_Cabalar(Formulas fmls);

    static Cabalar& instance();
        
};

#endif
