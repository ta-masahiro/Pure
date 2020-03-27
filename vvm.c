/* 
#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
// #include <gmp.h>

#define malloc(size)        GC_malloc(size)
#define realloc(p, size)    GC_realloc(p, size)
#define calloc(n, m)        GC_malloc(n * m)
#define free                GC_free

#define TRUE    1
#define FALSE   0

#include "vector.c"
#include "hash.c"
*/
#include "vm.h"
enum CODE {STOP, LDC,   LD,   ADD,  CALL, RTN, SEL, JOIN, LDF, SET, LEQ,  LDG,  GSET, SUB, \
           DEC,  TCALL, TSEL, DROP, EQ,   INC, MUL, DIV,  VEC, LDV, VSET, HASH, LDH,  HSET, \
           VPUSH, VPOP};
 //        0     1      2     3     4     5    6    7     8    9    10    11    12    13   \
 //        14    15     16    17    18    19   20   21    22   23   24    25    26    27
 //        28    29
void * eval(Vector * S, Vector * E, Vector * C, Vector * R, Vector * EE, Hash * G) {
    char * key; 
    long inst, ff, i, j, n, p; 
    Vector * fn, * keys, * t_exp, * f_exp, * code, * args, * cl, * ref, * Es, * l; 
    void ** g, * v; 
    void * ( * func);
    Hash * h;  
     // mpz_t  mpr; 
    while (TRUE) {
        inst = (long)dequeue(C); 
         // printf("%d\n", inst); 
        switch(inst) {
            case STOP:
                return pop(S); 
            case LDC:
                push(S, dequeue(C)); 
                break;
            case LD:
                ref = (Vector * )dequeue(C); 
                i = (long)vector_ref(ref, 0); // printf("i = %d\n", i); 
                j = (long)vector_ref(ref, 1); // printf("j = %d\n", j); 
                Es = (Vector * )vector_ref(E, E ->_sp - i - 1); // printf("%d\n", E ->_sp); vector_print(Es);
                if ( j >= 0 ) {  
                    // push(S, (void * )vector_ref(Es, Es ->_sp - j - 1)); //  printf("%d\n", Es ->_sp);printf("data = %d\n", (int)vector_ref(Es, Es ->_sp -j -  1));  
                    push(S, (void * )vector_ref(Es, j)); // printf("data = %d\n", (int)vector_ref(Es, j));  
                    break;
                }
                Es->_cp =  - j - 1; push(S, (void * )Es);
                break; 
            case LDG:
                key = (char * )dequeue(C);
                if ((g = Hash_get(G, key)) == NULL) printf("Unknown Key: %s\n", key); 
                else push(S, (void * )( * g)); 
                break;
            case SET:
                v = vector_ref(S, S ->_sp - 1); 
                // v = pop(S); push(S, v); 
                ref = (Vector * )dequeue(C); 
                i = (long)vector_ref(ref, 0); 
                j = (long)vector_ref(ref, 1);
                Es = (Vector * )vector_ref(E, E ->_sp - i - 1); // printf("%d\n", E ->_sp); vector_print(Es);  
                vector_set(Es,Es ->_sp - j - 1, v); 
                break;  
            case GSET:
                // v = pop(S); push(S, v) ;
                v = vector_ref(S, S ->_sp - 1); 
                key = (char * )dequeue(C);
                Hash_put(G, key, v);  
                break;
            case ADD:
                push(S, (void * )((long)pop(S) + (long)pop(S)));
                break;
            case SUB:
                push(S, (void * )( - (long)pop(S) + (long)pop(S)));
                break;
            case DEC:
                 // push(S, (void * )((long)(pop(S)) -1)) ;
                S->_table[S ->_sp - 1] = (void * )((long)(S ->_table[S -> _sp - 1]) - 1); 
                 // S->_table[S ->_sp - 1]  -= 1; 
                break;
            case INC:  
                push(S, (void * )((long)(pop(S)) + 1)) ;
                break;
            case MUL:
                push(S, (void * )((long)pop(S) * (long)pop(S)));
                break;
            case DIV:
                p = (long)pop(S); 
                push(S, (void * )((long)pop(S) / p));
                break;
            /* 
            case LADD:
                mpz_add(mpr, (mpz_t * )pop(S), (mpz_t * )pop(S)); 
                push(S, (void * )mpr);
                break;
            */  
            case EQ:
                push(S, (void * )(long)((long)pop(S) == (long)pop(S)));
                break;  
            case LEQ:
                push(S, (void * )(long)((long)pop(S) >= (long)pop(S))); 
                break;  
            case CALL:
                n = (long)dequeue(C); 
                fn = (Vector * )pop(S);
                if (strcmp((char * )vector_ref(fn, 0), "CL") != 0 ) printf("not CL\n");
                l = vector_init(n);  
                 // for(i = 0; i<n; i ++ ) {
                 //     push(l, pop(S)); 
                 // }
                memcpy(l ->_table, (S ->_table) +(S ->_sp - n) , n * (sizeof(void * )) ); 
                l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);  
                push(R, (void * )C); 
                push(EE, (void * )E); 
                E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l); 
                 //  E = (Vector * )vector_ref(fn, 2); push(E,l); 
                C = vector_copy1((Vector * )vector_ref(fn, 1)); 
                break; 
            case TCALL:
                n = (long)dequeue(C); 
                fn = (Vector * )pop(S);
                if (strcmp((char * )vector_ref(fn, 0), "CL") != 0 ) printf("not CL\n");
                l = vector_init(n);  
                 // for(i = 0; i<n; i ++ ) {
                 //     push(l, pop(S)); 
                 // }
                memcpy(l ->_table, (S ->_table) +(S ->_sp - n) , n * (sizeof(void * )) ); 
                l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);  
                 // push(R, (void * )C); 
                 // push(EE, (void * )E); 
                E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l); 
                 // E = (Vector * )vector_ref(fn, 2); push(E,l); 
                C = vector_copy1((Vector * )vector_ref(fn, 1)); 
                break;
           /*   case PCALL:
                n = (long)dequeue(C); 
                fnc = pop(S);
                l = vector_init(n);  
                 // for(i = 0; i<n; i ++ ) {
                 //     push(l, pop(S)); 
                 // }
                memcpy(l ->_table, (S ->_table) +(S ->_sp - n) , n * (sizeof(void * )) ); 
                l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);  
                ( * func)()
                break;
            */   
            case RTN:
                E = (Vector * )pop(EE);
                C = (Vector * )pop(R); 
                break; 
            case SEL:
                p = (long)pop(S); 
                t_exp = (Vector * )dequeue(C); 
                f_exp = (Vector * )dequeue(C); 
                push(R, (void * )C); 
                 // if (p) C = vector_copy1(t_exp); 
                 // else   C = vector_copy1(f_exp); 
                if (p) {C = t_exp;C ->_cp = 0; } 
                else   {C = f_exp;C ->_cp = 0; } 
                break; 
            case TSEL:
                p = (long)pop(S); 
                t_exp = (Vector * )dequeue(C); 
                f_exp = (Vector * )dequeue(C); 
                 // push(R, (void * )C); 
                if (p) C = vector_copy1(t_exp); 
                else   C = vector_copy1(f_exp); 
                 // if (p) {C = t_exp;C ->_cp = 0; } 
                 // else   {C = f_exp;C ->_cp = 0; } 
                break; 
            case JOIN:
                C = (Vector * )pop(R); 
                break; 
            case LDF:
                code = (Vector * )dequeue(C); 
                 // Es = (Vector * )dequeue(C);  
                cl = vector_init(4); 
                 // push(cl, (void * )"CL"); push(cl, (void * )code); push(cl, (void * )Es); 
                push(cl, (void * )"CL"); push(cl, (void * )code); push(cl, (void * )E); 
                push(S, (void * )cl);
                break;
            case VEC:
                n = (long)dequeue(C); 
                l = vector_init(n);  
                 // for(i = 0; i<n; i ++ ) {
                 //     push(l, pop(S)); 
                 // }
                memcpy(l ->_table, (S ->_table) +(S ->_sp - n), n * (sizeof(void * )) ); 
                l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);
                push(S, (void * )l);  
                break;  
            case LDV:
                n = (long)pop(S); 
                push(S, (void * )vector_ref((Vector * )pop(S), n));
                break;
            case VSET: // うまく動いていない！！
                v = (void * )pop(S); 
                n = (long)pop(S);
                l = (Vector * )pop(S);  
                vector_set(l, n, v); 
                push(S, v);
                break;
            case HASH:
                n = (long)pop(S); 
                h = Hash_init(n); 
                push(S, (void * )h);  
                break;
            case LDH:
                key = (char * )pop(S); 
                h = (Hash * )pop(S);
                if ((g = Hash_get(h, key)) == NULL) printf("Unknown Key: %s\n", key);  
                else push(S, (void * )( * g)); 
                break; 
            case HSET:
                v = pop(S); 
                key = (char * )pop(S); 
                h = (Hash * )pop(S); 
                Hash_put(h, key, v); 
                push(S, v); 
                break;
            case VPUSH:
                v = pop(S); 
                l = (Vector * )pop(S); 
                push(l, v); 
                push(S, v); 
                break; 
            case VPOP:
                push(S, pop((Vector * )pop(S))); 
                break;  
            case DROP:
                pop(S); 
                break;
            default:
                printf("Unknown Command: %ld", inst);  
        }
    } 
}

Vector * vector_make(void * L[], int N) {
    int i ;
    Vector * V = vector_init(N);  
    for(i = 0;  i<N; i ++ ) {
        push(V, (void * )L[i]); 
    }; 
    return V; 
}
/*  
int main(int argc,char*argv[]) { 
     // Vector * S  = vector_init(50); 
    Vector * S  = vector_init(5); 
     // Vector * E  = vector_init(16); 
    Vector * E  = vector_init(5); 
     // Vector * C  = vector_init(64);
    Vector * C  = vector_init(5); 
     // Vector * R  = vector_init(50); 
    Vector * R  = vector_init(5); 
     // Vector * EE = vector_init(32);
    Vector * EE = vector_init(5);
    Hash * G = Hash_init(2); 


     int x=atoi(argv[1]);int y=atoi(argv[2]);int z=atoi(argv[3]);
     // int x=3;int y=1;int z=0;
    void * codex[] = {0, 0}; void * codey[] = {0, 1}; void * codez[] = {0, 2}; 
    Vector * X = vector_make(codex, 2);  
    Vector * Y = vector_make(codey, 2);  
    Vector * Z = vector_make(codez, 2);  
    void * code6[] = {LD, Z, JOIN}; // printf("OK\n"); 
     //   vector_print(vector_make(code6, 3));  
     
    void * code7[] = {(void * )LD, X, LDC,  - 1, ADD, LD, Y, LD, Z, LDG, "t", CALL, 3, 
                        LD, Y, LDC,  - 1, ADD, LD, Z, LD, X, LDG, "t", CALL, 3, 
                        LD, Z, LDC,  - 1, ADD, LD, X, LD, Y, LDG, "t", CALL, 3, 
                        LDG, "t", CALL, 3, JOIN};
     // printf("code7\n"); vector_print(vector_make(code7, 44));  
    void * code8[] = {LD, X, LD, Y, LEQ, SEL, (void * )vector_make(code6, 3), (void * )vector_make(code7, 44), RTN}; 
     // void * code9[] = {"x", "y", "z"}; 
    // printf("code8\n"); vector_print(vector_make(code8, 9));  
     // void * code10[] = {LDF, (void * )vector_make(code8, 9),(void * )vector_make(code9, 3), SET, "t", STOP };
    void * code10[] = {LDF, (void * )vector_make(code8, 9),E, GSET, "t", STOP };
    C = vector_make(code10, 6);  
    //printf("C\n"); 
    //vector_print(vector_make(code10, 6));
    // push(E, (void * )e);   
    printf("%ld\n", (long)eval(S, E, C, R, EE, G));

    printf("function call starts\n"); 
    void * code11[] = {LDC, x, LDC, y, LDC, z, LDG, "t", CALL, 3, STOP}; 
    C = vector_make(code11, 11); 
    printf("%ld\n", (long)eval(S, E, C, R, EE, G));

}
 */ 
