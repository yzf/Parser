/* 
 * File:   PNFUtils.h
 * Author: yzf
 *
 * Created on August 5, 2013, 7:06 PM
 */

#ifndef PNFUTILS_H
#define	PNFUTILS_H

#include "structs.h"

class PNFUtils {
private:
    static bool isVariableInFormula(_formula* _fml, int _nVariableId);
    static void extractQuantifier_NEGA(_formula* _fml);
    static void extractQuantifier_IMPL_CONJ_DISJ(_formula* _fml);
public:
    static void convertToPrenex(_formula* _fml);
};

#endif	/* PNFUTILS_H */

