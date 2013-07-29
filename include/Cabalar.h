/* 
 * File:   Cabalar.h
 * Author: yzf
 *
 * Created on July 29, 2013, 3:15 PM
 */

#ifndef CABALAR_H
#define	CABALAR_H

#include "Formula.h"
#include "Formulas.h"

class Cabalar {
private:
    Cabalar();
    ~Cabalar();
    Cabalar(const Cabalar& _rhs) {};
    Cabalar& operator = (const Cabalar& _rhs) {return *this;};
public:
    static Cabalar& instance();
};


#endif	/* CABALAR_H */

