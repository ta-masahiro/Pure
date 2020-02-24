/* 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define initN   4096       //   vectorのイニシャルサイズ  

typedef struct Vector {
    int     _size; 
    void ** _table;  
    int     _cp;
    int     _sp;  
} Vector; 
*/
#include "vm.h"
Vector *  vector_init( int maxN) {
    Vector * s = (Vector * )malloc(sizeof(Vector)); 
    s ->_size = maxN; 
    s ->_table = (void ** )malloc(maxN * sizeof(void * ));
    s ->_cp = 0;
    s ->_sp = 0;  
    return s; 
}
/*
int is_stac_empty(Vector * s) {
    return s ->_sp == 0; 
}

int is_queu_empty(Vector * s) {
    return s ->_sp == s ->_cp; 
}

void vector_resize(); 

inline void push(Vector * s, void * data) {
    if (s ->_sp >= s ->_size ) vector_resize(s);  
    s ->_table[(s ->_sp) ++ ] = data;
}

#define enqueue push

inline void * pop(Vector * s) {
    return s ->_table[  -- (s ->_sp)]; 
}

 // #define dequeue(s) s->_table[(s->_cp)** ]
 
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
*/ 
Vector * vector_copy1(Vector * v) {
    Vector * s = (Vector * )malloc(sizeof(Vector)); 
    s ->_size = v ->_size; 
    s ->_table = v ->_table;
    s ->_cp = v ->_cp;
    s ->_sp = v ->_sp;  
    return s; 
}
 
Vector * vector_copy0(Vector * v) {
    Vector * r = vector_init(v ->_size );
    memcpy(r ->_table, v ->_table, (v->_size)*sizeof(void*));
    r->_sp = v->_sp;r ->_cp = v ->_cp; 
    return r;
}
/* 
Vector * vector_copy00(Vector * v) {
    Vector * r = vector_init((v ->_sp) - (v ->_cp) + 1);
    memcpy(r ->_table, (v ->_table) + (v ->_cp) * sizeof(void * ), (r->_size)*sizeof(void*));
    r->_sp = r ->_size; r ->_cp = 0; 
    return r;
}

void vector_copy2(Vector * S, int i, int j, Vector *D, int k);
    memcpy(S + i*sizeof(void*),D + k*(sizeof(void *)),(i-i+1)*sizeof(void*));

Vector * vector_sub(Vector v, int i, int j);
    return vector_init(j-i+1);
*/
void vector_resize(Vector * s) {
    int oldN = s ->_size; 
    int maxN = 3 * oldN / 2 + 1;     /* 1.5倍に拡大  */   
    void ** table =(void ** )realloc(s ->_table, maxN * sizeof(void * )) ;  
    s ->_table = table;  
    s ->_size = maxN;
    printf("vector resize:%d to %d\n", oldN, maxN); // vector_print(s);    
}
void vector_print(Vector * s) {
    int i;
    printf("[ ");  
    for(i = s->_cp; i< s ->_sp; i ++ ) {
        printf("%ld ", (long)(s ->_table[i])); 
    }
    printf("]\n"); 
}
 /* 
Item new_int(long l) {
    Item t;  
    t._int_data = l; 
    return t; 
}
 */ 

 /* 以下は適当なテスト   
int main(int argc, char * argv[]) {
    long i; 
    Vector * v = vector_init(2); 
    for (i = 1000; i<1020; i ++ ) {
        push(v, (void * )i); 
    }
    vector_print(v); 
   for (i = 1; i<= 5; i ++ ){
        printf("pop:%ld\n", (long )pop(v)); 
   } 
    vector_print(v); 
    printf("Queue Test\n");  
    for (i = 0; i<10; i ++ ) {
        push(v, (void * )i); 
    }
    vector_print(v); 

    for(i = 0; i<12; i ++ ){
        printf("dequeue:%ld\n", (long)dequeue(v)); 
    }
    vector_print(v);
    printf("Index Test\n"); 
    for(i = 0; i<(v ->_sp); i ++ ) {
        printf("v[%ld] = %ld\n", i, (long)vector_ref(v, i)); 
    }
    for(i = 0; i<(v ->_sp); i ++ ) {
        vector_set(v, i, (void * )(i * i)); 
    }
    vector_print(v); 
}
*/
