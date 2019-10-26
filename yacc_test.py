 # -*- coding: utf-8 -* 
import ply.yacc as yacc 
# Lex のサンプルを参照する 
from lex_test import tokens
import math
import cmath
import operator 
from fractions import Fraction

def List( * x):return list(x)
def List_set(L, i, v):
    L[i] = v
    return v
def Dict(keys, vals):
    return dict(zip(map(lambda x: tuple(x) if isinstance(x, list) else x, keys), vals))
def Dict_set(D, k, v):
    #print(D, k, v)
    if isinstance(k, list):k = tuple(k)
    D[k] = v
    return v
def Dict_ref(D, k):
        return D[tuple(k)] if isinstance(k, list) else D[k]
def isin(D, k):return k in D
def Dict_isin(D, k):
        if isinstance(k, list):k = tuple(k)
        return k in D
def search(ls, v):
    for i in range(len(ls)):
        if isinstance(ls[i], list):
            j = search(ls[i], v)
            if not (j is None): return [i] + j
        elif ls[i] == v:return [i]
    return None

def tail(e):
    #print(e)
    if len(e)>=3 and e[ - 3] == 'CALL':
        e[ - 3] = 'TCALL'
        return
    elif len(e)>=4 and e[ - 4] == 'SEL':
        e[ - 4] = 'TSEL'
        e[ - 2][ - 1] = 'RTN'
        e[ - 3][ - 1] = 'RTN'
        if e[ - 1] == 'RTN':e[ - 1] = 'STOP'
        tail(e[ - 2])
        tail(e[ - 3])
        return
    return

def load(file_name):
    f = open(file_name)
    s = f.read()
    print(s)
    result = parser.parse(s)
    #c_time = time.time()
    v = eval([], [G], result + ['STOP'], 0, [], [])
    #e_time = time.time()
    if type(v) == list and v != [] and  v[0] == 'CL':print('Usser Function')
    else:print(v)
    #print('c_time  : ', int((1000000 * (c_time - s_time))) / 1000000)
    #print('e_time  : ', int((1000000 * (e_time - c_time))) / 1000000)
    G['_'] = v

def Push(L, v):
    L.append(v)
    return v
G = {'_':None, 'push':Push, 'isin':isin, 'dict_isin':Dict_isin, 'load':load, 'range':range, 'list':list, 'Fraction':Fraction, 'print':print, 'len':len, 'list':List, 'list_set':List_set, 'dict':Dict, 'dict_set':Dict_set, 'dict_ref':Dict_ref}
#G.update(vars(operator))
#G.update(vars(math))
#G.update(vars(cmath))
#G = {'print':print}
#
# 式 expression = 項 | 2項演算式 | 代入式 | f式 | lambda式 |
#
def p_expression(p):
    '''
    expression  : term
                | expression_2op
                | expression_set
                | expression_if
                | expression_lambda
                | expression_decl
                | expression_function_decl 
    '''
    p[0] = p[1]
# 2項演算式
def p_expression_2op(p):
    '''
    expression_2op  : expression PLUS expression
                    | expression MINUS expression
    '''
    #print(p)
    p[0] = p[1] + p[3]
    if      p[2] == '+': p[0] += ['ADD']
    elif    p[2] == '-': p[0] += ['SUB']
# 代入式
def p_expression_set(p):
    '''
    expression_set  : ID LET expression
    '''
    p[0] = p[3] + ['SET', p[1]]
# if式
def p_expression_if(p):
    '''
    expression_if   : IF expression COL expression COL expression
    '''
    p[0] = p[2] + ['SEL', p[4] + ['JOIN'], p[6] + ['JOIN']]
# lambda式
def p_expression_lambda(p):
    '''
    expression_lambda   : LAMBDA LPAREN args DOTS RPAREN expression 
                        | LAMBDA LPAREN args RPAREN expression
                        | LAMBDA LPAREN RPAREN expression
    '''
    #def tail(e):
    #    #print(e)
    #    if len(e)>=3 and e[ - 3] == 'CALL':
    #        e[ - 3] = 'TCALL'
    #        return
    #    elif len(e)>=4 and e[ - 4] == 'SEL':
    #        e[ - 4] = 'TSEL'
    #        e[ - 2][ - 1] = 'RTN'
    #        e[ - 3][ - 1] = 'RTN'
    #        if e[ - 1] == 'RTN':e[ - 1] = 'STOP'
    #        tail(e[ - 2])
    #        tail(e[ - 3])
    #        return
    #    return

    if      len(p) == 7:
        e = p[6] + ['RTN']
        tail(e)
        p[0] = ['LDF'] +[e]+[p[3] + ['..']]
    elif    len(p) == 6: 
        e = p[5] + ['RTN']
        tail(e)
        p[0] = ['LDF'] +[e]+[p[3]]
    elif    len(p) == 5:
        e = p[4] + ['RTN']
        tail(e)
        p[0] = ['LDF'] +[e]+[[]]

# lambda式の部品
def p_args(p):
    '''
    args    : ID
            | args CAMMA ID
    '''
    if      len(p) == 2: p[0] = [p[1]]
    elif    len(p) == 4: p[0] = p[1] + [p[3]]
def p_expression_func_decl(p):
    '''
    expression_function_decl : DEF ID LPAREN args RPAREN LET expression
    '''
    p[0] = ['LDF'] +[p[7] + ['RTN']]+[p[4]] + ['SET', p[2]]

def p_decl_exp(p):
    '''
    expression_decl : VAR ID
    '''
    p[0] = ['DCL', p[2]]
# 複式 mult_expression = 式 | {式; 式; ...}
def p_mult_expression(p):
    '''
    mult_expression : left_mult_exp RBRAC
    '''
    p[0] = p[1]
# 複式を使う部品 left_mult_exp = {式; 式; ...
def p_lieft_mult_exp(p):
    '''
    left_mult_exp   : left_mult_exp SEMICOL expression
                    | LBRAC expression
                    | LBRAC
    '''
    if      len(p) == 4: p[0] = p[1] + ['POP'] + p[3]
    elif    len(p) == 3: p[0] = p[2]  
    else:    p[0] = ['LDC', None]
#
# 項 = 要素 | 項 2項演算子 要素 | 単項演算子 要素 
#
def p_term(p):
    '''
    term    : factor
            | term_2op
            | term_1op
    '''
    p[0] = p[1] 
# 2項演算子
def p_term_2op(p):
    '''
    term_2op    : term TIMES    factor
                | term DIVIDE   factor
                | term POW      factor
                | term EQUAL    factor
                | term NEQ      factor
                | term GEQ      factor
                | term LEQ      factor
                | term GT       factor
                | term LT       factor
                | term IS       factor
    '''
    p[0] = p[1] + p[3]
    if      p[2] == '*' : p[0] += ['MUL']
    elif    p[2] == '/' : p[0] += ['DIV']
    elif    p[2] == '**': p[0] += ['POW']   
    elif    p[2] == '==': p[0] += ['EQ' ]  
    elif    p[2] == '!=': p[0] += ['NEQ']
    elif    p[2] == '>=': p[0] += ['GEQ']   
    elif    p[2] == '<=': p[0] += ['LEQ'] 
    elif    p[2] == '>' : p[0] += ['GT']
    elif    p[2] == '<' : p[0] += ['LT']
    elif    p[2] == 'is': p[0] += ['IS'] 
# 単項演算子
def p_term_1op(p):
    '''
    term_1op    : MINUS factor
                | NOT factor
    '''
    if      p[1] == '-': p[0] = p[2] + ['MINUS']
    elif    p[1] == '!': p[0] = p[2] + ['NOT']
#
# 要素 factor = 数値 | 変数 |ベクター | ベクター参照 | 関数呼出 | 括弧式
#
def p_factor(p):
    '''
    factor  : bool
            | number
            | var
            | vector
            | string
            | vect_ref
            | f_call
            | br_expr
            | mult_expression
            | function_apply
            | function_set
            | function_if
            | function_lambda
            | call_cc
    '''
    p[0] = p[1]
# 真偽値
def p_factor_bool(p):
    '''
    bool    : TRUE
            | FALSE
    '''
    if p[1] == 'True'   : p[0] = ['LDC', True]
    else                : p[0] = ['LDC', False]
# 数
def p_factor_num(p):
    '''
    number  : INT 
            | FLOAT
            | E_FLOAT
            | FLOAT2
            | FRACT
    '''
    p[0] = ['LDC', p[1]]
# 変数
def p_factor_var(p):
    '''
    var : ID
    '''
    p[0] = ['LD', p[1]]
def p_factor_str(p):
    '''
    string : STR
    '''
    p[0] = ['LDC', p[1]]
# ベクター
def p_vector(p):
    '''
    vector  : lvector RBRAK
            | LBRAK RBRAK
    '''
    if p[1] == '[':
        p[0] = ['LDC', []]
    else:
        args = len(p[1])
        C = []
        for c in p[1]:
            C = C + c
        p[0] = C + ['VEC', args]
def p_lvector(p):
    '''
    lvector : lvector CAMMA expression
            | LBRAK expression
    '''
    if      len(p) == 4: p[0] = p[1] + [p[3]]
    elif    len(p) == 3: p[0] = [p[2]]
# ベクター参照
def p_factor_ref(p):
    '''
    vect_ref : factor LBRAK factor RBRAK
    '''
    p[0] = p[1] + p[3] + ['REF']
# 括弧式
def p_factor_br_expr(p):
    '''
    br_expr : LPAREN expression RPAREN
    '''
    p[0] = p[2] 
# 関数呼出
def p_factor_f_call(p):
    '''
    f_call  : factor LPAREN params RPAREN
            | factor LPAREN RPAREN
    '''
    if len(p) == 5: 
        args = len(p[3])
        C = []
        for c in p[3]:
            C = C + c
        p[0] = C + p[1] + ['CALL', args]
    elif len(p) == 4:
        p[0] = p[1] + ['CALL', 0]
#    
def p_params(p):
    '''
    params  : params CAMMA expression
            | expression
    '''
    if      len(p) == 4: p[0] = p[1] +[p[3]]
    elif    len(p) == 2: p[0] = [p[1]]
def p_factor_set(p):
    '''
    function_set    : SET LPAREN ID CAMMA expression RPAREN
    '''
    p[0] = p[5] + ['SET', p[3]]
def p_factor_apply(p):
    '''
    function_apply  : APPLY LPAREN params RPAREN
    '''
    args = len(p[3])
    C = []
    for c in p[3]:
        C = C + c
    p[0] = C + ['APL', args]
def p_factor_if(p):
    '''
    function_if : IF LPAREN expression CAMMA expression CAMMA expression RPAREN
    '''
    p[0] = p[3] + ['SEL', p[5] + ['JOIN'], p[7] + ['JOIN']]

def p_factor_lambda(p):
    '''
    function_lambda : LAMBDA LPAREN LPAREN args DOTS RPAREN  CAMMA expression RPAREN
                    | LAMBDA LPAREN LPAREN args RPAREN CAMMA expression RPAREN
                    | LAMBDA LPAREN LPAREN RPAREN CAMMA expression RPAREN
    '''
    if      len(p) == 10: p[0] = ['LDF'] +[ p[8] + ['RTN'] ]+[p[4] + ['..']]
    elif    len(p) ==  9: p[0] = ['LDF'] +[ p[7] + ['RTN'] ]+[p[4]]
    elif    len(p) ==  8: p[0] = ['LDF'] +[ p[6] + ['RTN'] ]+[[]]
def p_factor_callcc(p):
    '''
    call_cc : CALLCC LPAREN expression RPAREN
    '''
    p[0] = ['LDICT', '__CODE__'] + p[3] + ['CALL', 1]
#
# 構文エラー 
#
def p_error(p):     
    #print( "Syntax error in input")
    raise SyntaxError("SyntaxError: " + str(p))
# 構文解析器の構築 
parser = yacc.yacc() 
from eval_test import eval
import time
if __name__ == '__main__':
    load("lib.py")
    while True:
        try:
            s = input('calc > ')
        except EOFError:
            break
        if not s:
            continue
        s_time = time.time()
        try:
            result = parser.parse(s)
            #print(result)
        except SyntaxError as e:
            print(e)
            continue
        #
        # call_ccの処理
        #
        q = result + ['STOP']
        qq = q
        t = search(q, '__CODE__')
        while not (t is None):
            #print(t)
            for i in range(len(t) - 1):
                q = q[t[i]]
            #print(q[t[ - 1]:])
            q[t[ - 1]] = q[t[ - 1] + 6:]
            t = search(qq, '__CODE__')
        print(qq)
        c_time = time.time()
        #print( result)
        #try:
        #v = eval([], [G], result + ['STOP'], 0, [], [])
        v = eval([], [G], qq, 0, [], [])
        e_time = time.time()
        if type(v) == list and v != [] and  v[0] == 'CL':print('Usser Function')
        else:print(v)
        print('c_time  : ', int((1000000 * (c_time - s_time))) / 1000000)
        print('e_time  : ', int((1000000 * (e_time - c_time))) / 1000000)
        G['_'] = v
        #except (KeyError, IndexError) as e:
        #    print(e)
        #    continue
        #except :
        #    print("Some Error Occured!")
        #    continue
