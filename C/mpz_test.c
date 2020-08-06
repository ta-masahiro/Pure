#include <gc.h>
#include <gmp.h>
#define malloc(size) GC_degug_malloc(size)

typedef struct Object {
    long _obj_type; 
    union {
        long     _int_val; 
        double   _real_val; 
        mpz_ptr  _longint_val; 
        mpq_ptr  _longrat_val;
    } _data; 
} Object;

Object * new_longint(char * s) {
    MP_INT val; 
    mpz_init_set_str(&val, s, 10); 
    Object * p = (Object * )malloc(sizeof(Object)); 
    p->_obj_type = LONG_INT; 
    P->_data._longint_val = &val; 
}

Object  * add_longint(mpz_t * a, mpz_t * b):
