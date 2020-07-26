#include <gc.h>
#include <gmp.h>
#define malloc(size) GC_degug_malloc(size)

typedef struct Object {
    long _obj_type; 
    union {
        long _int_val; 
        double _real_val; 
        mpz_t *  _longint_val; 
        mpq_t  * _longrat_val;
    } _data; 
} Object;

Object * new_longint(char * s) {
    mpz_t val; 
    mpz_init_set_str(val, s, 10); 
    Object * p = (object * )malloc(sizeof(Object)); 
    P->_obj_type = LONG_INT; 
    P->_data._longint_val = &val; 
}

Object  * add_longint(mpz_t * a, mpz_t * b):
