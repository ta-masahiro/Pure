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
//#define initN   4096     /* vectorのイニシャルサイズ  */

typedef struct Vector {
    int     _size;
    /* Item *  _table; */
    void ** _table;
    int     _cp;
    int     _sp;
} Vector;

typedef void*(*Funcpointer)(Vector*);

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
    Symbol  *key;
    void    *val;
} Data;

typedef struct Hash {
    Data *hashTable;
    unsigned long initval, size, entries;        // 初期値、サイズ、登録数
} Hash;

Hash * Hash_init(unsigned long size);
void Hash_resize(Hash * h, unsigned long newSize);     // 内部関数
void Hash_free(Hash *pHash);
unsigned long hash(unsigned char *key, unsigned long keysize, unsigned long initval);                 // 内部関数SH
unsigned long Hash_put(Hash * h, Symbol *key, void *val);
void  ** Hash_get(Hash * h, Symbol *key);
void  * eval(Vector * S, Vector * E, Vector * C, Vector * R, Vector * EE, Hash * G); 
void print_hashTable(Hash* h); 
Symbol*new_symbol(unsigned char*s, unsigned long i);
mpz_ptr new_long();
mpz_ptr new_long_valued(long val);
mpz_ptr new_long_str(char* s);
