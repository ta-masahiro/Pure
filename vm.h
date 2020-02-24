#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>
// #include <gmp.h>

#define malloc(size) GC_malloc(size)
#define realloc(p, size) GC_realloc(p, size)
#define calloc(n,m) GC_malloc(n*m)
#define free GC_free
 
#define TRUE    1
#define FALSE   0
#define initN   4096     /* vectorのイニシャルサイズ  */

typedef struct Vector {
    int     _size;
    /* Item *  _table; */
    void ** _table;
    int     _cp;
    int     _sp;
} Vector;

Vector *  vector_init( int maxN);
//int is_stac_empty(Vector * s);
//int is_queu_empty(Vector * s);
//extern void push(Vector * s, void * data);
#define enqueue push
//extern void * pop(Vector * s) ;
//extern void * dequeue(Vector * s);
//extern void ** vector_ref(Vector * v, int index) ;
//extern void vector_set(Vector * v, int index, void ** val);
Vector * vector_copy1(Vector * v);
Vector * vector_copy0(Vector * v) ;
void vector_resize(Vector * s);
void vector_print(Vector * s) ;


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

typedef struct Data {
    char *key;
    void *val;
} Data;

typedef struct Hash {
    Data *hashTable;
    // int   type;
    int   size, entries;        // サイズ、登録数
} Hash;

Hash * Hash_init(int size);
void Hash_resize(Hash * h, int newSize);     // 内部関数
void Hash_free(Hash *pHash);
int  hash(char *key, int size);                 // 内部関数SH
int  Hash_put(Hash * h, char *key, void *val);
void  ** Hash_get(Hash * h, char *key);
void  * eval(Vector * S, Vector * E, Vector * C, Vector * R, Vector * EE, Hash * G); 
 // #include "vector.c"
 // #include "hash.c"
