#include "vm.h"
enum OBJ_TYPE {INT_TYPE,FLT_TYPE,LONG_TYPE,FRAC_TYPE,UFN_TYPE,PFN_TYPE,STR_TYPE,VEC_TYPE,FILE_TYPE};

typedef struct Object {
    enum OBJ_TYPE _type; 
    union {
        long     _int_val; 
        double   _flt_val; 
        mpz_ptr  _longint_val; 
        mpq_ptr  _longrat_val;
        Vector  *_user_func;
        void  *(*_pr_func)(Vector*);
        char    *_str;
        Vector  *_vec;
        FILE    *_file;
    } _data; 
} Object;

mpz_ptr new_long(char * s) {
    mpz_ptr val=(mpz_ptr)malloc(sizeof(MP_INT)); 
    mpz_init_set_str(val, s, 10); 
    return val;
}

char * new_str(char *s,int size) {
    char *p=(char *)malloc(sizeof(size)+1);
    strcpy(p, s);
    return p;
}
Object* new_object(enum OBJ_TYPE type, void* init_val) {
    Object*p=(Object*)malloc(sizeof(Object));
    p->_type=type;
    switch (type) {
        case INT_TYPE:
            p->_data._int_val=(long)init_val;
            break;
        case FLT_TYPE:
            p->_data._flt_val=*(double*)init_val;//!!!!注意!!!!問題なけど無理くりだな!!!
            break;
        case LONG_TYPE:
            p->_data._int_val=(mpz_ptr)init_val;
            break;
        case FRAC_TYPE:
            p->_data._int_val=(mpq_ptr)init_val;
            break;
        case UFN_TYPE:
            p->_data._int_val=(Vector*)init_val;
            break;
        case PFN_TYPE:
            p->_data._int_val=(Funcpointer)init_val;
            break;
        case STR_TYPE:
            p->_data._int_val=(char*)init_val;
            break;
        case VEC_TYPE:
            p->_data._int_val=(Vector*)init_val;
            break;
        case FILE_TYPE:
            p->_data._int_val=(FILE*)init_val;
            break;
    }
}

void add_object(Object *t, Object *s1, Object *s2) {

    
}
