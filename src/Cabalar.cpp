#include "Cabalar.h"

Cabalar::Cabalar() {
    
}

Cabalar::~Cabalar() {
    
}

Cabalar& Cabalar::instance() {
    static Cabalar theInstance;
    return theInstance;
}
