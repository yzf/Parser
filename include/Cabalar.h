#ifndef _CABALAR_H_
#define _CABALAR_H_

#include "Formula.h"

class Cabalar{
private:
	Formula Cabalar_DLT(Formula originFml);
	Formula Cabalar_DLF(Formula originFml);
	Formula Cabalar_CLT(Formula originFml);
	Formula Cabalar_CLF(Formula originFml);

	Formula Cabalar_ILT(Formula originFml);
	Formula Cabalar_ILF(Formula originFml);
	Formula Cabalar_IRT(Formula originFml);
	Formula Cabalar_IRF(Formula originFml);

	Formula Cabalar_N1(Formula originFml);
	Formula Cabalar_N2(Formula originFml);
	Formula Cabalar_N3(Formula originFml);
	Formula Cabalar_N4(Formula originFml);
	Formula Cabalar_N5(Formula originFml);
	Formula Cabalar_N6(Formula originFml);

	Formulas Cabalar_L1(Formula originFml);
	Formulas Cabalar_L2(Formula originFml);
	Formulas Cabalar_L3(Formula originFml);
	Formulas Cabalar_L4(Formula originFml);
	Formulas Cabalar_L5(Formula originFml);

	Formulas Cabalar_R1(Formula originFml);
	Formulas Cabalar_R2(Formula originFml);
	Formulas Cabalar_R3(Formula originFml);
	Formulas Cabalar_R4(Formula originFml);
	Formulas Cabalar_R5(Formula originFml);


	 //tools
	int Commutative_Trans(Formula root, Formula curr,
					FORMULA_TYPE commutativeSymbol);

	Formula convert_negative_normal_form(Formula fml);//Negative Normal Form
	Formulas convert_negative_normal_forms(Formulas fmls);

	Formulas Cabalar_Trans(Formula fml);
	static Formulas convert_Cabalar(Formulas fmls);

};

#endif