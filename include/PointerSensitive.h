/* 
 * File:   PointerSensitive.h
 * Author: ray
 *
 * Created on July 25, 2013, 2:52 PM
 */

#ifndef POINTERSENSITIVE_H
#define	POINTERSENSITIVE_H

#include "Vocabulary.h"
#include "Formula.h"
#include "Cabalar.h"
#include "Formulas.h"
#include "structs.h"


class PointerSensitive{
private:
    _formula* PointerSensitive_6_1(_formula* fml);
    _formula* PointerSensitive_6_2(_formula* fml);
    _formula* PointerSensitive_7_1(_formula* fml);
    _formula* PointerSensitive_7_2(_formula* fml);
    _formula* PointerSensitive_8(_formula* fml);
    _formula* PointerSensitive_9(_formula* fml);
    _formula* PointerSensitive_10(_formula* fml);
    _formula* PointerSensitive_11(_formula* fml);
    
public:
    PointerSensitive();
    ~PointerSensitive(); 
    Formulas PointerSensitive_Convert(_formula* fml);
    
};


#endif	/* POINTERSENSITIVE_H */

