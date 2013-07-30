/* 
 * File:   NNFUtils.h
 * Author: yzf
 *
 * Created on July 30, 2013, 12:12 PM
 */

#ifndef NNFUTILS_H
#define	NNFUTILS_H

#include "structs.h"

class NNFUtils {
private:
    static _formula* negativeNormalForm_1(_formula* _originalFml);
    static _formula* negativeNormalForm_2(_formula* _originalFml);
    static _formula* negativeNormalForm_3(_formula* _originalFml);
    static _formula* negativeNormalForm_4(_formula* _originalFml);
    static _formula* negativeNormalForm_5(_formula* _originalFml);
    static _formula* negativeNormalForm_6(_formula* _originalFml);
public:
    static _formula* convertToNegativeNormalForm(_formula* _originalFml);
};


#endif	/* NNFUTILS_H */

