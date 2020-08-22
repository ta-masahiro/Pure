#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include <gmp.h>

#define malloc(size) GC_malloc(size)
#define realloc(p, size) GC_realloc(p, size)
#define calloc(n,m) GC_malloc(n*m)
#define free GC_free
 
#define TRUE    1
#define FALSE   0
enum OBJ_TYPE {INT_TYPE,LONG_TYPE,FRAC_TYPE,FLT_TYPE, UFN_TYPE,PFN_TYPE,SYM_TYPE,VEC_TYPE,DICT_TYPE, FILE_TYPE};
typedef struct Object {
    enum OBJ_TYPE _type; 
     /* union {
        long     _int_num; 
        mpz_ptr  _lon_num; 
        mpq_ptr  _fract_num;
        double   _flt_num; 
        Vector  *_user_func;
        void  *(*_pr_func)(Vector*);
        Symbol  *_sym;
        Vector  *_vec;
        Hash    *_dict; 
        FILE    *_file;
    } _data;   */
    void * _data;   
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
    p->_data = init_val; 
}
 /*     switch (type) {
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
} */ 

void add_object(Object *t, Object *s1, Object *s2) {

    
}
//#define initN   4096     /* vectorのイニシャルサイズ  */

typedef struct Vector {
    int     _size;
    /* Item *  _table; */
    void ** _table;
    int     _cp;
    int     _sp;
} Vector;

Vector *  vector_init( int maxN);
#define enqueue push
Vector * vector_copy1(Vector * v);
Vector * vector_copy0(Vector * v) ;
void vector_resize(Vector * s);
void vector_print(Vector * s) ;

#ifndef INLINE
#define INLINE
inline int is_stac_empty(Vector * s) {
    return s ->_sp == 0; 
}

inline int is_queu_empty(Vector * s) {
    return s ->_sp == s ->_cp; 
}
inline void push(Vector * s, void * data) {
    if (s ->_sp >= s ->_size ) vector_resize(s);  
    s ->_table[(s ->_sp) ++ ] = data;
}

inline void * pop(Vector * s) {
    return s ->_table[  -- (s ->_sp)]; 
}

inline void * dequeue(Vector * s) {
    if (s ->_cp > s ->_sp ) {
        // printf("queue underflow\n");
        return NULL;  }
    return s ->_table[ (s ->_cp) ++  ]; 
}
 
inline void ** vector_ref(Vector * v, int index) {
    return v ->_table[index]; 
}

inline void vector_set(Vector * v, int index, void ** val) {
    v ->_table[index] = val; 
}
#endif

typedef struct Symbol {
    unsigned long _size;
    unsigned char *_table;
} Symbol;

typedef struct Data {
    Symbol *key;
    void   *val;
} Data;

typedef struct Hash {
    Data *hashTable;
    unsigned long initval, size, entries;        // 初期値、サイズ、登録数
} Hash;

Hash * Hash_init(unsigned long size);
void Hash_resize(Hash * h, unsigned long newSize);     // 内部関数
void Hash_free(Hash *pHash);
unsigned long  hash(unsigned char *key, unsigned long keysize, unsigned long initval);                 // 内部関数SH
unsigned long  Hash_put(Hash * h, Symbol *key, void *val);
void  ** Hash_get(Hash * h, Symbol *key);
void  * eval(Vector * S, Vector * E, Vector * C, Vector * R, Vector * EE, Hash * G);
void print_hashTable(Hash* h);
Symbol*new_symbol(unsigned char*s, unsigned long i);
