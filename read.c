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
    S ->_fp = f;
    if (p == NULL) return NULL;  
    return S; 
}

char * new_symbol(char * str, int size) {
    char * s = (char * )malloc(size * sizeof(char) + 1); 
    strcpy(s, str); // printf("%s\n", s);
     // if (strcmp(s, "LDC") == 0) printf("!LDC!\n");  
    return s;  
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
Vector *  get_code(Stream * s) {
     // printf("in get_code \n"); 
    char c, cc; 
    Vector * code = vector_init(10); 
    char str_buff[255]; int str_buff_p = 0; 

    while (1) {
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

enum CODE {STOP, LDC, LD, ADD, CALL, RTN, SEL, JOIN, LDF, SET, LEQ, LDG, GSET, SUB, \
           DEC, TCALL, TSEL, POP, EQ, INC, MUL, DIV, VEC, LDV, VSET, HASH, LDH, HSET, \
           VPUSH, VPOP};
 //        0     1    2   3    4     5    6    7     8    9    10   11   12    13   \
 //        14   15     16    17   18  19   20   21   22   23   24    25    26   27

Vector * chg_byte_code(Vector * code, Hash * G) {
    void * c, * operand, ** v; 
    Vector * t = vector_init(3);
 
    while (1) {
         // vector_print(code);
        if ((c = dequeue(code)) == NULL) break;  
        if (strcmp((char * )c, "STOP") == 0) {
            push(t, (void *) STOP);
        } else if (strcmp((char * )c, "LDC") == 0) {
            push(t, (void * )LDC);  // printf("%d\n", (int)((void * )1));  
            push(t, dequeue(code)); 
        } else if (strcmp((char * )c, "LD") == 0) {
            push(t, (void * )LD); 
            push(t, dequeue(code)); 
             // push(t, dequeue(code)); 
        } else if (strcmp((char * )c, "ADD") == 0) {
            push(t, (void * )ADD); 
        } else if (strcmp((char * )c, "CALL") == 0) {
            push(t, (void * )CALL); 
            push(t, dequeue(code)); 
        } else if (strcmp((char * )c, "TCALL") == 0) {
            push(t, (void * )TCALL); 
            push(t, dequeue(code)); 
        } else if (strcmp((char * )c, "RTN") == 0) {
            push(t, (void * )RTN); 
        } else if (strcmp((char * )c, "SEL") == 0) {
            push(t, (void * )SEL); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
        } else if (strcmp((char * )c, "TSEL") == 0) {
            push(t, (void * )TSEL); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
        } else if (strcmp((char * )c, "JOIN") == 0) {
            push(t, (void * )JOIN); 
        } else if (strcmp((char * )c, "LDF") == 0) {
            push(t, (void * )LDF); 
            push(t, chg_byte_code ((Vector * ) dequeue(code), G)); 
             // push(t, chg_byte_code ((Vector * ) dequeue(code))); 
        } else if (strcmp((char * )c, "SET") == 0) {
            push(t, (void * )SET); 
            push(t, dequeue(code)); 
        } else if (strcmp((char * )c, "LEQ") == 0) {
            push(t, (void * )LEQ); 
        } else if (strcmp((char * )c, "EQ") == 0) {
            push(t, (void * )EQ); 
        } else if (strcmp((char * )c, "LDG") == 0) {
            operand = dequeue(code); 
            v = Hash_get(G, (char * )operand); 
             // if (v == NULL) {
                 push(t, (void * )LDG); 
                 push(t, operand);
             // } else {
             //     push(t, (void *) LDC); 
             //     push(t, (void * )( * v)); 
             // } 
        } else if (strcmp((char * )c, "GSET") == 0) {
            push(t, (void * )GSET); 
            push(t, dequeue(code)); 
        } else if (strcmp((char * )c, "SUB") == 0) {
            push(t, (void * )SUB); 
        } else if (strcmp((char * )c, "DEC") == 0) {
            push(t, (void * )DEC); 
        } else if (strcmp((char * )c, "INC") == 0) {
            push(t, (void * )INC); 
        } else if (strcmp((char * )c, "POP") == 0) {
            push(t, (void * )POP); 
        } else if (strcmp((char * )c, "MUL") == 0) {
            push(t, (void * )MUL); 
        } else if (strcmp((char * )c, "DIV") == 0) {
            push(t, (void * )DIV); 
        } else if (strcmp((char * )c, "VEC") == 0) {
            push(t, (void * )VEC); 
            push(t, dequeue(code)); 
        } else if (strcmp((char * )c, "LDV") == 0) {
            push(t, (void * )LDV); 
        } else if (strcmp((char * )c, "VSET") == 0) {
            push(t, (void * )VSET); 
        } else if (strcmp((char * )c, "HASH") == 0) {
            push(t, (void * )HASH); 
        } else if (strcmp((char * )c, "LDH") == 0) {
            push(t, (void * )LDH); 
        } else if (strcmp((char * )c, "HSET") == 0) {
            push(t, (void * )HSET); 
        } else if (strcmp((char * )c, "VPUSH") == 0) {
            push(t, (void * )VPUSH); 
        } else if (strcmp((char * )c, "VPOP") == 0) {
            push(t, (void * )VPOP); 
        } else {
            printf("Unknkown Command %s\n", (char * )c); 
            break; 
        }
    } 
    return t; 
}
 
void disassy(Vector * code, int indent) {
    int c, i; 
    Vector * v; 
    char * s; 

    while (TRUE) {
         // vector_print(code);
        if (is_queu_empty(code)) break;
        c = (int)dequeue(code); 
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
                printf("LD\t[%ld\t%ld]\n", (long)vector_ref(v, 0), (long)vector_ref(v, 1));  
                break; 
            case ADD:
                printf("ADD\n");
                break;  
            case CALL:
                printf("CALL\t%ld\n", (long)dequeue(code));
                break;  
            case TCALL:
                printf("TCALL\t%ld\n", (long)dequeue(code));
                break;  
            case RTN:
                printf("RTN\n");
                break;  
            case SEL:
                printf("SEL\t\n"); 
                disassy((Vector * ) dequeue(code), indent + 1 ); 
                disassy((Vector * ) dequeue(code), indent + 1 ); 
                break; 
            case TSEL:
                printf("TSEL\t\n"); 
                disassy((Vector * ) dequeue(code), indent + 1 ); 
                disassy((Vector * ) dequeue(code), indent + 1 ); 
                break; 
            case JOIN:
                printf("JOIN\n");
                break;  
            case LDF:
                printf("LDF\t[\n"); 
                disassy((Vector *)dequeue(code), indent + 1); 
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
                s = (char * )dequeue(code);
                printf("LDG\t%s\n", s);
                break;   
            case GSET:
                s = (char * )dequeue(code);
                printf("GSET\t%s\n", s);
                break;  
            case SUB:
                printf("SUB\n");
                break; 
            case DEC:
                printf("DEC\n");
                break;  
            case INC:
                printf("INC\n");
                break;  
            case POP:
                printf("POP\n");
                break;  
            case MUL:
                printf("MUL\n");
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
            default:
                printf("Unknkown Command %s\n", (char * )c); 
                break; 
        }
    }

    code ->_cp = 0;  
}
 
int main(int argc, char * argv[]) {
    char c; 
    Vector  * code,  * t; 
    Vector * S = vector_init(5); 
    Vector * E = vector_init(5); 
    Vector * C, * CC ; 
    Vector * R = vector_init(5); 
    Vector * EE = vector_init(5); 
    Hash * G = Hash_init(128); 
    long v; 
    FILE * fp; 
    Stream  * s; 

    if (argc <= 1 ) s = new_stream(stdin);
    else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {printf("file %s doesn't exist\n", argv[1]); return  - 1; }
        s = new_stream(fp); 
    }
    while (1) {
         while (1) {
            if ((c = get_char(s)) == '[') { 
                code = get_code(s);
                vector_print(code);
                C = chg_byte_code(code, G); 
                vector_print(C);
                disassy(C, 0);
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
