#include "vm.h"
typedef void*(*Funcpointer)(Vector*);

enum CODE {STOP, LDC,   LD,   ADD,  CALL, RTN, SEL, JOIN, LDF, SET, LEQ,  LDG,  GSET, SUB,  \
           DEC,  TCALL, TSEL, DROP, EQ,   INC, MUL, DIV,  VEC, LDV, VSET, HASH, LDH,  HSET, \
           VPUSH,VPOP,  LADD, LSUB, LMUL, ITOL,LPR, PCALL,LDM};
 //        0     1      2     3     4     5    6    7     8    9    10    11    12    13 
 //        14    15     16    17    18    19   20   21    22   23   24    25    26    27
 //        28    29     30    31    32    33   34   35    36
int op_size[] = \
          {0,    1,     1,    0,    1,    0,   2,   0,    1,   1,   0,    1,    1,    0,    \
           0,    1,     2,    0,    0,    0,   0,   0,    1,   0,   0,    0,    0,    0,    \
           0,    0,     0,    0,    0,    0,   0,   1,    0} ;

Vector *tosqs(Vector*code, const void** table) {
    enum CODE op;
    Vector *C = vector_copy0(code);
    while (code->_cp < code->_sp) {
        op=(enum CODE)dequeue(code); //printf("%d ",op);
        vector_set(C, (C->_cp)++, (void*)table[op]); //printf("%ld\n",(long)table[op]);
        if (op == LDF) {
            vector_set(C,(C->_cp)++,(void*)tosqs(dequeue(code),table));
        } else if (op == SEL || op == TSEL) {
            vector_set(C,(C->_cp)++,(void*)tosqs(dequeue(code),table));
            vector_set(C,(C->_cp)++,(void*)tosqs(dequeue(code),table));
        } else {
            (code->_cp)+=op_size[op]; 
            (C->_cp)+=op_size[op];
        }
    }
    C->_cp=0;
    return C;
}


void * eval(Vector * S, Vector * E, Vector * Code, Vector * R, Vector * EE, Hash * G) {
    char * key; 
    long inst, ff, i, j, n, p; 
    Vector * fn, * keys, * t_exp, * f_exp, * code, * args, * cl, * ref, * Es, * l; 
    void ** g, * v;
    Funcpointer func; 
    Hash * h;  
    mpz_ptr x, y, z;
    enum CODE op;
    Vector *C = vector_copy0(Code);

    static const void * table[] = {
        &&_STOP, &&_LDC, &&_LD, &&_ADD, &&_CALL, &&_RTN, &&_SEL, &&_JOIN, &&_LDF, &&_SET, &&_LEQ, &&_LDG, &&_GSET, &&_SUB, \
        &&_DEC,  &&_TCALL, &&_TSEL, &&_DROP, &&_EQ, &&_INC, &&_MUL, &&_DIV, &&_VEC, &&_LDV, &&_VSET, &&_HASH, &&_LDH, &&_HSET, \
        &&_VPUSH, &&_VPOP, &&_LADD, &&_LSUB, &&_LMUL, &&_ITOL,&&_LPR, &&_PCALL, &&_LDM};

    C = tosqs(Code,table);//vector_print(C);

    _STARt:
        C->_cp = 0;
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];
    _STOP:
        return pop(S); 
    _LDC:
        push(S, dequeue(C)); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];
    _LD:
        ref = (Vector * )dequeue(C); 
        i = (long)vector_ref(ref, 0); // printf("i = %d\n", i); 
        j = (long)vector_ref(ref, 1); // printf("j = %d\n", j); 
        Es = (Vector * )vector_ref(E, E ->_sp - i - 1); // printf("%d\n", E ->_sp); vector_print(Es);
        if ( j >= 0 ) {  
            push(S, (void * )vector_ref(Es, j)); // printf("data = %d\n", (int)vector_ref(Es, j));  
            goto * dequeue(C);
            //goto  * table[(int)dequeue(C)];                 
        }
        Es->_cp =  - j - 1; push(S, (void * )Es);
        goto * dequeue(C);
        //goto  * table[(int)dequeue(C)];                 
    _LDG:
        key = (char * )dequeue(C);
        if ((g = Hash_get(G, key)) == NULL) printf("Unknown Key: %s\n", key); 
        else push(S, (void * )( * g)); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LDM:
        i = * (long * )pop(S); push(S, (void * )i);  
        goto * dequeue(C);
        //goto  * table[(int)dequeue(C)];                 
    _SET:
        v = vector_ref(S, S ->_sp - 1); 
        ref = (Vector * )dequeue(C); 
        i = (long)vector_ref(ref, 0); 
        j = (long)vector_ref(ref, 1);
        Es = (Vector * )vector_ref(E, E ->_sp - i - 1); // printf("%d\n", E ->_sp); vector_print(Es);  
        vector_set(Es,Es ->_sp - j - 1, v); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _GSET:
        v = vector_ref(S, S ->_sp - 1); 
        key = (char * )dequeue(C);
        Hash_put(G, key, v);  
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _ADD:
        push(S, (void * )((long)pop(S) + (long)pop(S)));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _ITOL: 
        z = (mpz_ptr)malloc(sizeof(MP_INT)); 
        mpz_init_set_si(z, (long)pop(S)); 
        push(S, (void * )z); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LADD:
        y = (mpz_ptr)(S->_table[--(S->_sp)]); x = (mpz_ptr)(S->_table[(S->_sp)-1]); 
        mpz_add(x,x,y);
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _SUB:
        push(S, (void * )( - (long)pop(S) + (long)pop(S)));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LSUB:
        y = (mpz_ptr)(S->_table[--(S->_sp)]); x = (mpz_ptr)(S->_table[(S->_sp)-1]); 
        mpz_sub(x,x,y);
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _DEC:
        S->_table[S ->_sp - 1] = (void * )((long)(S ->_table[S -> _sp - 1]) - 1); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _INC:  
        push(S, (void * )((long)(pop(S)) + 1)) ;
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _MUL:
        push(S, (void * )((long)pop(S) * (long)pop(S)));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LMUL:
        y = (mpz_ptr)(S->_table[--(S->_sp)]); x = (mpz_ptr)(S->_table[(S->_sp)-1]); 
        mpz_mul(x, x, y);
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _DIV:
        p = (long)pop(S); 
        push(S, (void * )((long)pop(S) / p));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _EQ:
        push(S, (void * )(long)((long)pop(S) == (long)pop(S)));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LEQ:
        push(S, (void * )(long)((long)pop(S) >= (long)pop(S))); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _CALL:
        n = (long)dequeue(C); 
        fn = (Vector * )pop(S);
        if (strcmp((char * )vector_ref(fn, 0), "CL") != 0 ) printf("not CL\n");
        l = vector_init(n);  
        memcpy(l ->_table, (S ->_table) +(S ->_sp - n) , n * (sizeof(void * )) ); 
        l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);  
        push(R, (void * )C); 
        push(EE, (void * )E); 
        E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l); 
         //  E = (Vector * )vector_ref(fn, 2); push(E,l); 
        C = vector_copy1((Vector * )vector_ref(fn, 1)); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _TCALL:
        n = (long)dequeue(C); 
        fn = (Vector * )pop(S);
        if (strcmp((char * )vector_ref(fn, 0), "CL") != 0 ) printf("not CL\n");
        l = vector_init(n);  
        memcpy(l ->_table, (S ->_table) +(S ->_sp - n) , n * (sizeof(void * )) ); 
        l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);  
        E = vector_copy0((Vector * )vector_ref(fn, 2)); push(E,l); 
        C = vector_copy1((Vector * )vector_ref(fn, 1)); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _PCALL:
        n = (long)dequeue(C); 
        func = (Funcpointer)pop(S);
        l = vector_init(n);  
        memcpy(l->_table, (S->_table) + (S->_sp - n) , n * (sizeof(void * ))); 
        l->_sp = n; S->_sp = S->_sp - n;  // vector_print(l);  
        push(S, func(l));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _RTN:
        E = (Vector * )pop(EE);
        C = (Vector * )pop(R); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _SEL:
        p = (long)pop(S); 
        t_exp = (Vector * )dequeue(C); 
        f_exp = (Vector * )dequeue(C); 
        push(R, (void * )C); 
        if (p) {C = t_exp;C ->_cp = 0; } 
        else   {C = f_exp;C ->_cp = 0; } 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _TSEL:
        p = (long)pop(S); 
        t_exp = (Vector * )dequeue(C); 
        f_exp = (Vector * )dequeue(C); 
        if (p) C = vector_copy1(t_exp); 
        else   C = vector_copy1(f_exp); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _JOIN:
        C = (Vector * )pop(R); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LDF:
        code = (Vector * )dequeue(C); 
        cl = vector_init(4); 
        push(cl, (void * )"CL"); push(cl, (void * )code); push(cl, (void * )E); 
        push(S, (void * )cl);
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _VEC:
        n = (long)dequeue(C); 
        l = vector_init(n);  
        memcpy(l ->_table, (S ->_table) +(S ->_sp - n), n * (sizeof(void * )) ); 
        l ->_sp = n; S ->_sp = S ->_sp - n;  // vector_print(l);
        push(S, (void * )l);  
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LDV:
        n = (long)pop(S); 
        push(S, (void * )vector_ref((Vector * )pop(S), n));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _VSET: // うまく動いていない！！
        v = (void * )pop(S); 
        n = (long)pop(S);
        l = (Vector * )pop(S);  
        vector_set(l, n, v); 
        push(S, v);
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _HASH:
        n = (long)pop(S); 
        h = Hash_init(n); 
        push(S, (void * )h);  
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LDH:
        key = (char * )pop(S); 
        h = (Hash * )pop(S);
        if ((g = Hash_get(h, key)) == NULL) printf("Unknown Key: %s\n", key);  
        else push(S, (void * )( * g)); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _HSET:
        v = pop(S); 
        key = (char * )pop(S); 
        h = (Hash * )pop(S); 
        Hash_put(h, key, v); 
        push(S, v); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _VPUSH:
        v = pop(S); 
        l = (Vector * )pop(S); 
        push(l, v); 
        push(S, v); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _VPOP:
        push(S, pop((Vector * )pop(S))); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _DROP:
        pop(S); 
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
    _LPR:
        gmp_printf("%Zd\n", (mpz_ptr)(S->_table[(S->_sp) -1]));
        goto * dequeue(C);
        //goto * table[(int)dequeue(C)];                 
}

Vector * vector_make(void * L[], int N) {
    int i ;
    Vector * V = vector_init(N);  
    for(i = 0;  i<N; i ++ ) {
        push(V, (void * )L[i]); 
    }; 
    return V; 
}