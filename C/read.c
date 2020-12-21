#include "vm.h"
#include <ctype.h>
typedef struct Stream{
    int     _pos; 
    FILE *  _fp; 
    char *  _buff;
} Stream; 

#define MAXBUFF 1024

Stream  * new_stream(FILE  * f) {
    char * p; 
    Stream * S = (Stream * )malloc(sizeof(Stream) );
    S ->_pos = 0;
    S ->_buff = (char * )malloc(MAXBUFF * sizeof(char));  
    p = fgets(S ->_buff, MAXBUFF, f); 
    S -> _fp = f;
    if (p == NULL) return NULL;  
    return S; 
}

Symbol * new_symbol(unsigned char * str, unsigned long size) {
    char * s = (char * )malloc(size * sizeof(char) + 1); 
    // strcpy(s, str); // printf("%s\n", s);
    memcpy(s,str,size);
    // if (strcmp(s, "LDC") == 0) printf("!LDC!\n");  
    Symbol * sym = (Symbol * )malloc(sizeof(Symbol));
    sym -> _size = size; 
    sym -> _table = s;  
    return sym;  
}
char get_char(Stream * S) {
    return S ->_buff[(S ->_pos) ++ ]; 
    }

void unget_char(Stream * S) {
    (S ->_pos) -- ; 
}

char * re_load(Stream * S) {
    char * p; 
    S ->_pos = 0; 
    p = fgets(S ->_buff, MAXBUFF, S ->_fp);
     // if (p == NULL) printf("faile EOF\n");
    return p;   
}
Vector *  get_code(Stream * s);

int strcmp_ignorecase(const char *s1, const char *s2) {
    int i = 0;
    /* 文字が等しい間繰り返す */
    while (toupper((unsigned char)s1[i]) == toupper((unsigned char)s2[i])) 
        if (s1[i ++ ] == '\0') return 0;
    return toupper((unsigned char)s1[i]) - toupper((unsigned char)s2[i]);
}
#define strcmp strcmp_ignorecase

mpz_ptr new_long() {
    mpz_ptr long_int_p = (mpz_ptr)malloc(sizeof(MP_INT)); 
    mpz_init(long_int_p);
    return long_int_p;
}

mpz_ptr new_long_valued(long val) {
    mpz_ptr long_int_p = (mpz_ptr)malloc(sizeof(MP_INT)); 
    mpz_init_set_si(long_int_p,val);
    return long_int_p;
}
mpz_ptr new_long_str(char* s) {
    mpz_ptr long_int_p = (mpz_ptr)malloc(sizeof(MP_INT)); 
    mpz_init_set_str(long_int_p,s,10);
    return long_int_p;
}
 /* 
void * read(Vector * code, char * file_name) {
    FILE * fp;
    Stream * s;
    char c; 
    Vector  * v; 
    if (file_name == NULL) fp = stdin; 
    else fp = fopen(file_name, "r"); 
     
    s = new_stream(fp); 

    if ((c = get_char(s)) == '[') { 
        v = get_code(s);
        code ->_size = v ->_size; 
        code ->_table = v ->_table; 
        code ->_cp = v ->_cp; 
        code ->_sp = v ->_sp;   
        vector_print(v);
        vector_print(code);  
        return v;
    }  
    printf("error!\n");
    return NULL;   
}
 */
 /*
#define MAX_STRBUFF 255
int is_int(Stream s, long ret) {
    char c; 
    char str_buff[MAX_STRBUFF]; int str_buff_p = 0;

    while isblank(c = get_char(s)); 
    
    if (isdigit(c)) { 
        str_buff_p = 1; 
        str_buff[0] = c; 
        while (isdigit(c = get_char(s))) {
            sr_buff[str_buff_p ++ ] = c;
            if (str_buff_p > MAXBUFF) {
                printf("string to long"\n); 
                return FALSE; 
            } 
        }
        unget_char(s); 
        str_buff[str_buff_p] = '\0';
        ret = atol(str_buff);
        return TRUE; 
    } else {
        unget_char(s); 
        return FALAE; 
    }
}

int is_decimal(Stream s, double fret) {
    char c; 
    char str_buff[MAX_STRBUFF]; int str_buff_p = 0;
 
    if ((c = get_char(s)) != '.') {
        ungetchar(s); 
        return FALSE; 
    }
    str_buff_[0] = '.'; 
    str_buff_p = 1
    if (isdigit(c)) { 
        str_buff_p = 2; 
        str_buff[1] = c; 
        while (isdigit(c = get_char(s))) {
            sr_buff[str_buff_p ++ ] = c;
            if (str_buff_p > MAXBUFF) {
                printf("string to long"\n); 
                return FALSE; 
            } 
        }
        unget_char(s); 
        str_buff[str_buff_p] = '\0';
        ret = atof(str_buff);
        return TRUE; 
    } else {
        unget_char(s);unget_char(s);  
        return FALAE; 
    }
}

long is_exponent(Stream s, long ret) {
    char c; 
    int m_flg = 1;
    long ret;  

    c = getchar(s); 
    if (c == 'e') || (c == 'E') {
        if (is_int(s, ret)) {
            return ret; 
        } else {
            c = get_char(s); 
            if (c == '-') m_flg =  - 1; 
            if (c == '+') {
                if (is_int(s, ret))
                    return ret; 
                else {
                    ungetchar(s); 
                    return FALSE;
                }
            } else if (is_int(s, ret)) {
                return m_flg * ret; 
            } else {
                ungetchar(s); 
                return FALSE; 
            }  
    } else {
        ungetchar(s); 
        return FALSE; 
    }
:}

int is_float(Stream s, double ret) {
    long iret1, iret2, expret; 
    if is_int(s, iret1) {
        if is_exponent(s, expret) {
                return iret1 * (10^expret); 
        } else if ((c = getchar(s)) == '.') {
            if (is_int(s, iret2) {
                    if (is_expornent(s, expret)) {
                        iret1 * iret2 }}}}
}
 */

Vector *  get_code(Stream * s) {
     // printf("in get_code \n"); 
    char c, cc; 
    Vector * code = vector_init(64); 
    char str_buff[255]; int str_buff_p = 0; 

    while (TRUE) {
        c = get_char(s); // printf("%c\n", c);  
        if (isblank(c)) continue; 
        if (c == ']')
            break;
        else if (c == '[') {
            push(code, get_code(s)); 
        } else if (isalpha(c) || c == '_') {
            str_buff_p = 1;
            str_buff[0] = c;  
            while (isalnum(c = get_char(s))) {
                str_buff[str_buff_p ++ ] = c; 
            }
            unget_char(s); 
            str_buff[str_buff_p] = '\0'; 
            push(code, (void * )new_symbol(str_buff, str_buff_p + 1));
        } else if ((c == '-') && (isdigit(cc = get_char(s)))) {
            str_buff_p = 1; 
            str_buff[0] = cc; 
            while (isdigit(cc = get_char(s))) {
                str_buff[str_buff_p ++ ] = cc; 
            }
            unget_char(s); 
            str_buff[str_buff_p] = '\0';
            push(code, (void * )( - atol(str_buff)));  
        } else if (isdigit(c)) {
            str_buff_p = 1; 
            str_buff[0] = c; 
            while (isdigit(c = get_char(s))) {
                str_buff[str_buff_p ++ ] = c; 
            }
            unget_char(s); 
            str_buff[str_buff_p] = '\0';
            push(code, (void * )atol(str_buff));  
        } else if (c == '\n' || c == ';' ) {
            if (re_load(s) == NULL) return NULL;
        } else  
            printf("erro!:%c",c ); 
   
    }
     // printf("end of code\n");
     // vector_print(code);  
    return code; 
}

enum CODE {STOP,  LDC,  LD,  ADD, CALL,RTN, SEL, JOIN, LDF, SET, LEQ,  LDG, GSET,SUB,   \
           DEC,   TCALL,TSEL,DROP,EQ,  INC, MUL, DIV,  VEC, LDV, VSET, HASH,LDH, HSET,  \
           VPUSH, VPOP, LADD,LSUB,LMUL,ITOL,LPR, PCALL,LDM, DUP, SWAP, ROT, _2ROT,CALLS,\
           TCALLS,RTNS, LDP, LDL ,FADD,FSUB,FMUL,FDIV, FPR, ITOF,LCPY };
 //        0      1     2    3    4    5    6    7     8    9    10    11   12   13   
 //        14     15    16   17   18   19   20   21    22   23   24    25   26   27
 //        28     29    30   31   32   33   34   35    36   37   38    39   40   41
 //        42     43    44   45   46   47   48   49    50   60

char code_name[][6] = \
    {"STOP",  "LDC",  "LD",  "ADD", "CALL","RTN", "SEL", "JOIN", "LDF","SET", "LEQ", "LDG", "GSET", "SUB",   \
     "DEC",   "TCALL","TSEL","DPOP","EQ",  "INC", "MUL", "DIV",  "VEC","LDV", "VSET","HASH","LDH",  "HSET",  \
     "VPUSH", "VPOP", "LADD","LSUB","LMUL","ITOL","LPR", "PCALL","LDM","DUP", "SWAP","ROT", "_2ROT","CALLS", \
     "TCALLS","RTNS", "LDP", "LDL", "FADD","FSUB","FMUL","FDIV", "FPR","ITOF","LCPY" };

int code_pr_size[] = {0, 1, 1, 0, 1, 0, 2, 0, 1, 1, 0, 1, 1, 0, \
                      0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, \
                      0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, \
                      1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0} ;

void code_optimize(Vector * code, Hash *G){
    char * key;
    Symbol * sym; 
    void ** v;
    void * c;
    while ((c = dequeue(code)) != NULL) {
        //printf("%ld\t%s\n",(long)c,code_name[(long)c]);
        if ((long)c==LDG) {
            sym = (Symbol *)dequeue(code);
            if ((v=Hash_get(G, sym))==NULL || (long)code->_table[code->_cp] != CALL || (long)code->_table[code->_cp] != TCALL)  continue;
            code->_table[code->_cp - 1] = (void *)( *v);
            code->_table[code->_cp - 2] =(void *)LDC;
        } else if ((long)c==SEL || (long)c==TSEL) {
            code_optimize((Vector*)dequeue(code),G);
            code_optimize((Vector*)dequeue(code),G);
        } else if ((long)c==LDF) {
            code_optimize((Vector*)dequeue(code),G);
        } else (code->_cp) += code_pr_size[(long)c];
    }
    code->_cp = 0;
}


Vector * chg_byte_code(Vector * code, Hash * G) {
    void * c, * operand, ** v; 
    Vector * t = vector_init(64);
    Symbol * sym; 
 
    while (TRUE) {
         // vector_print(code);
        if ((sym = (Symbol * )dequeue(code)) == NULL) break;
        c = sym -> _table;   
        if (strcmp(c, "STOP") == 0) {
            push(t, (void *) STOP);
        } else if (strcmp(c, "LDC") == 0) {
            push(t, (void * )LDC);  // printf("%d\n", (int)((void * )1));  
            push(t, dequeue(code)); 
        } else if (strcmp(c, "LD") == 0) {
            push(t, (void * )LD); 
            push(t, dequeue(code)); 
             // push(t, dequeue(code)); 
        } else if (strcmp(c, "LDM") == 0) {
            push(t, (void * )LDM); 
        } else if (strcmp(c, "ADD") == 0) {
            push(t, (void * )ADD); 
        } else if (strcmp(c, "LADD") == 0) {
            push(t, (void * )LADD); 
        } else if (strcmp(c, "CALL") == 0) {
            push(t, (void * )CALL); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "TCALL") == 0) {
            push(t, (void * )TCALL); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "PCALL") == 0) {
            push(t, (void * )PCALL); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "RTN") == 0) {
            push(t, (void * )RTN); 
        } else if (strcmp(c, "SEL") == 0) {
            push(t, (void * )SEL); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
        } else if (strcmp(c, "TSEL") == 0) {
            push(t, (void * )TSEL); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
        } else if (strcmp(c, "JOIN") == 0) {
            push(t, (void * )JOIN); 
        } else if (strcmp(c, "LDF") == 0) {
            push(t, (void * )LDF); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
             // push(t, chg_byte_code ((Vector * ) dequeue(code))); 
        } else if (strcmp(c, "SET") == 0) {
            push(t, (void * )SET); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "LEQ") == 0) {
            push(t, (void * )LEQ); 
        } else if (strcmp(c, "EQ") == 0) {
            push(t, (void * )EQ); 
        } else if (strcmp(c, "LDG") == 0) {
            operand = dequeue(code); 
             // v = Hash_get(G, (char * )operand); 
             // if (v == NULL) {
                 push(t, (void * )LDG); 
                 push(t, operand);
             // } else {
             //     push(t, (void *) LDC); 
             //     push(t, (void * )( * v)); 
             // } 
        } else if (strcmp(c, "GSET") == 0) {
            push(t, (void * )GSET); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "SUB") == 0) {
            push(t, (void * )SUB); 
        } else if (strcmp(c, "LSUB") == 0) {
            push(t, (void * )LSUB); 
        } else if (strcmp(c, "DEC") == 0) {
            push(t, (void * )DEC); 
        } else if (strcmp(c, "INC") == 0) {
            push(t, (void * )INC); 
        } else if (strcmp(c, "DROP") == 0) {
            push(t, (void * )DROP); 
        } else if (strcmp(c, "MUL") == 0) {
            push(t, (void * )MUL); 
        } else if (strcmp(c, "LMUL") == 0) {
            push(t, (void * )LMUL); 
        } else if (strcmp(c, "DIV") == 0) {
            push(t, (void * )DIV); 
        } else if (strcmp(c, "VEC") == 0) {
            push(t, (void * )VEC); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "LDV") == 0) {
            push(t, (void * )LDV); 
        } else if (strcmp(c, "VSET") == 0) {
            push(t, (void * )VSET); 
        } else if (strcmp(c, "HASH") == 0) {
            push(t, (void * )HASH); 
        } else if (strcmp(c, "LDH") == 0) {
            push(t, (void * )LDH); 
        } else if (strcmp(c, "HSET") == 0) {
            push(t, (void * )HSET); 
        } else if (strcmp(c, "VPUSH") == 0) {
            push(t, (void * )VPUSH); 
        } else if (strcmp(c, "VPOP") == 0) {
            push(t, (void * )VPOP); 
        } else if (strcmp(c, "ITOL") == 0) {
            push(t, (void * )ITOL); 
        } else if (strcmp(c, "LPR") == 0) {
            push(t, (void * )LPR); 
        } else if (strcmp(c, "DUP") == 0) {
            push(t, (void * )DUP); 
        } else if (strcmp(c, "SWAP") == 0) {
            push(t, (void * )SWAP); 
        } else if (strcmp(c, "ROT") == 0) {
            push(t, (void * )ROT); 
        } else if (strcmp(c, "_2ROT") == 0) {
            push(t, (void * )_2ROT); 
        } else if (strcmp(c, "CALLS") == 0) {
            push(t, (void * )CALLS); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "TCALLS") == 0) {
            push(t, (void * )TCALLS); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "RTNS") == 0) {
            push(t, (void * )RTNS); 
        } else if (strcmp(c, "LDP") == 0) {
            push(t, (void * )LDP); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
        } else if (strcmp(c, "LDL") == 0) {
            push(t, (void * )LDL); 
            push(t, dequeue(code)); 
        } else if (strcmp(c, "FADD") == 0) {
            push(t, (void * )FADD); 
        } else if (strcmp(c, "FSUB") == 0) {
            push(t, (void * )FSUB); 
        } else if (strcmp(c, "FMUL") == 0) {
            push(t, (void * )FMUL); 
        } else if (strcmp(c, "FDIV") == 0) {
            push(t, (void * )FDIV); 
        } else if (strcmp(c, "FPR") == 0) {
            push(t, (void * )FPR); 
        } else if (strcmp(c, "ITOF") ==0) {
            push(t, (void * )ITOF);
        } else if (strcmp(c, "LCPY") ==0) {
            push(t, (void * )LCPY);
        } else {
            printf("Unknkown Command %s\n", (char * )c); 
            break; 
        }
    } 
    return t; 
}
 
void disassy(Vector * code, int indent) {
    int i;
    long c; 
    Vector * v; 
    char * s; 
    for(i= 0; i< indent; i ++ ) printf("\t"); 
    printf("[\n");indent++; 
    while (TRUE) {
         // vector_print(code);
        if (is_queu_empty(code)) break;
        c = (long)dequeue(code); 
        for(i= 0; i< indent; i ++ ) printf("\t"); 
        switch(c) {  
            case STOP:
                printf("STOP\n");
                break;  
            case LDC :
                printf("LDC\t%ld\n", (long)dequeue(code));
                break;   
            case LD :
                v = (Vector * )dequeue(code); 
                printf("LD\t[%ld %ld]\n", (long)vector_ref(v, 0), (long)vector_ref(v, 1));  
                break; 
            case LDM:
                printf("LDM\n");
                break;
            case ADD:
                printf("ADD\n");
                break;
            case LADD:
                printf("LADD\n");
                break;  
            case CALL:
                printf("CALL\t%ld\n", (long)dequeue(code));
                break;  
            case TCALL:
                printf("TCALL\t%ld\n", (long)dequeue(code));
                break;  
            case PCALL:
                printf("PCALL\t%ld\n", (long)dequeue(code));
                break;  
            case RTN:
                printf("RTN\n");
                break;  
            case SEL:
                printf("SEL\n"); 
                disassy((Vector * ) dequeue(code), indent ); 
                disassy((Vector * ) dequeue(code), indent ); 
                break; 
            case TSEL:
                printf("TSEL\n"); 
                disassy((Vector * ) dequeue(code), indent ); 
                disassy((Vector * ) dequeue(code), indent ); 
                break; 
            case JOIN:
                printf("JOIN\n");
                break;  
            case LDF:
                printf("LDF\n"); 
                disassy((Vector *)dequeue(code), indent ); 
                break; 
            case SET:
                v = (Vector * )dequeue(code); 
                printf("SET\t[%ld %ld]\n", (long)vector_ref(v, 0), (long)vector_ref(v, 1));
                break;  
            case LEQ:
                printf("LEQ\n");
                break;  
            case EQ:
                printf("EQ\n");
                break;  
            case LDG:
                s = ((Symbol * )dequeue(code)) -> _table;
                printf("LDG\t%s\n", s);
                break;   
            case GSET:
                s = ((Symbol * )dequeue(code)) -> _table;
                printf("GSET\t%s\n", s);
                break;  
            case SUB:
                printf("SUB\n");
                break;
            case LSUB:
                printf("LSUB\n");
                break; 
            case DEC:
                printf("DEC\n");
                break;  
            case INC:
                printf("INC\n");
                break;  
            case DROP:
                printf("DROP\n");
                break;  
            case MUL:
                printf("MUL\n");
                break;
            case LMUL:
                printf("LMUL\n");
                break;  
            case DIV:
                printf("DIV\n");
                break;  
            case VEC:
                printf("VEC\t%ld\n",(long)dequeue(code));
                break;  
            case LDV:
                printf("LDV\n");
                break;  
            case VSET:
                printf("VSET\n");
                break;  
            case HASH:
                printf("HASH\n");
                break;  
            case LDH:
                printf("LDH\n");
                break;  
            case HSET:
                printf("HSET\n");
                break;  
            case VPUSH:
                printf("VPUSH\n");
                break;  
            case VPOP:
                printf("VPOP\n");
                break; 
            case ITOL:
                printf("ITOL\n");
                break; 
            case LPR:
                printf("LPR\n");
                break ;
            case DUP:
                printf("DUP\n");
                break ;
            case SWAP:
                printf("SWAP\n");
                break ;
            case ROT:
                printf("ROT\n");
                break ;
            case _2ROT:
                printf("_2ROT\n");
                break ;
            case CALLS:
                printf("CALLS\t%ld\n", (long)dequeue(code));
                break ;
            case TCALLS:
                printf("TCALLS\t%ld\n", (long)dequeue(code));
                break ;
            case RTNS:
                printf("RTNS\n");
                break ;
            case LDP:
                printf("LDP\n"); 
                disassy((Vector *)dequeue(code), indent ); 
                break; 
            case LDL:
                printf("LDL\t%ld\n", (long)dequeue(code));
                break ;
            case FADD:
                printf("FADD\n");
                break ;
            case FSUB:
                printf("FSUB\n");
                break ;
            case FMUL:
                printf("FMUL\n");
                break ;
            case FDIV:
                printf("FDIV\n");
                break ;
            case FPR:
                printf("FPR\n");
                break ;
            case ITOF:
                printf("ITOF\n");
                break;
            case LCPY:
                printf("LCPY\n");
                break;
            default:
                printf("Unknkown Command %s\n", (char * )c); 
                break; 
        }
    }
    indent--;
    for(i= 0; i< indent; i ++ ) printf("\t"); 
    printf("]\n"); 

    code ->_cp = 0;  
}

//仮置きprimitive関数
//Vector*を引数にとりvoid*を返すこと
void *sum(Vector*v) {
    int i;
    long n=(long)(v->_sp);
    long s=0;
    for (i=0;i<n;i++) s+=(long)v->_table[i];
    return (void*)s;
}
void*list(Vector*v) {
    return (void*)vector_copy0(v);
}
void*iprint(Vector*v) {
    for(long i=0;i<(long)(v->_sp);i++) printf("%ld ",(long)(v->_table[i]));
    printf("\n");
    return (void*)v;
}
void*vprint(Vector*v) {
    Vector*vv=(Vector*)(v->_table[0]);
    printf("[ "); 
    for(long i=0;i<(long)(vv->_sp);i++) printf("%ld ",(long)(vv->_table[i]));
    printf("]\n");
    return (void*)v;
}
void * lprint(Vector * v) {
    for(long i=0;i<(long)(v->_sp);i++) gmp_printf("%Zd ",(mpz_ptr)(v->_table[i]));
    printf("\n"); 
    }
 /* void * hash_delete(Vector * v){
    Hash * h = (Hash * )v[0]; 
    Symbol * s = (Symbol * )v[1]; 
    Hash_delete(h, s); 
    return NULL; 
} */ 
void * _realloc(void * ptr, size_t old_size, size_t new_size) {
    return GC_realloc(ptr, new_size); 
}

int main(int argc, char * argv[]) {
    char c; 
    long v; 
    FILE * fp; 
    Stream  * s;

    // mp_set_memory_functions((void * )GC_malloc, (void * )GC_realloc,(void * )GC_free);
    mp_set_memory_functions((void *)GC_malloc, (void * )_realloc, (void * ) GC_free);

    Vector  * code,  * t; 
    Vector * S = vector_init(500000); 
    Vector * E = vector_init(5); 
    Vector * C, * CC ; 
    Vector * R = vector_init(500); 
    Vector * EE = vector_init(50); 
    Hash * G = Hash_init(128); // must be 2^n 

    //primitive関数のセット
    Hash_put(G, new_symbol("sum", 4), (void*)sum);
    Hash_put(G, new_symbol("list", 5), (void*)list);
    Hash_put(G, new_symbol("iprint", 7), (void*)iprint);
    Hash_put(G, new_symbol("vprint", 7), (void*)vprint);
    Hash_put(G, new_symbol("lprint", 7), (void*)lprint);
     // Hash_put(G, "hash_delete", (void * )hash_delete); 
    print_hashTable(G); 
    if (argc <= 1 ) s = new_stream(stdin);
    else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {printf("file %s doesn't exist\n", argv[1]); return  - 1; }
        s = new_stream(fp); 
    }
    while (TRUE) {
         while (TRUE) {
            if ((c = get_char(s)) == '[') { 
                code = get_code(s);
                vector_print(code);
                C = chg_byte_code(code, G); 
                vector_print(C);
                //disassy(C, 0);
                //code_optimize(C,G);
                disassy(C,0);
                v = (long)eval(S, E, C, R, EE, G);
                printf("%ld\n", v);    
                break;   
            } else if (isblank(c)) continue;  
            else if (c == '\n') { 
                if (re_load(s) == NULL) return 0;
                break;
            } else {printf("error!\n");break; }
        }
    }
}
