 # -*- coding: utf-8 -* 
import copy
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
def Dict(keys = None, vals = None):
    if keys is None:return {}
    elif isinstance(keys, list) and isinstance(vals, list):
        return dict(zip(map(lambda x: tuple(x) if isinstance(x, list) else x, keys), vals))
    else:return {keys:vals}
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
def search_r(ls, v, p, exc = None):         #リストlsにvが含まれる場所をすべてpに入れて返す
                                            # ただしexcの次にサブリストがあればそれは省く
    FLG = False
    for i in range(len(ls)):
        if FLG:
            FLG = False
            continue
        if (ls[i] == exc) and (i < len(ls) - 1) and isinstance(ls[i + 1], list): #　追加  
            FLG = True
            #print("yahoo:", i)
            continue
        if isinstance(ls[i], list):
            search_r(ls[i], v, p)
        elif ls[i] == v:
            p.append((ls, i))   #vが含まれるリストと位置をタプルにして返す
    pass
def search_nr(ls, vs, exc = None):           #リストlsに リストvsが含まれる場所をすべて返す
    p = []
    if not isinstance(vs, list):
        search_r(ls, vs, p, exc)
        return p
    n = len(vs)
    newp = []
    search_r(ls, vs[0], p, exc)
    if p == []:return []
    for q in p:
        if vs == q[0][q[1]:q[1] + n]:newp.append(q)
    return newp
def replace(ls, vs, ts, exc = None):        #リストlsの全てのリストvsをtsで置き換える
    p = search_nr(ls, vs, exc)
    #print(p)
    n1 = len(vs)
    if p == []:return
    for q in reversed(p):       #入れ替えると該当位置が変わってしまうので後ろから実行
        q[0][q[1]:q[1] + n1] = ts
def tail(e):
    #print(e)
    if len(e)>=3 and e[ - 3] == 'CALL':     # 末尾が CALL n RET である場合は 
        e[ - 3] = 'TCALL'                   # 末尾呼び出し
        return
    elif len(e)>=4 and e[ - 4] == 'SEL':    # 末尾が SEL CODE1 CODE2 RETである場合は末尾呼び出し
        e[ - 4] = 'TSEL'                    # SEL をTSELに変えて
        e[ - 2][ - 1] = 'RTN'               # CODE内のJOINはRETに変える
        e[ - 3][ - 1] = 'RTN'               # 最後のRETは不要；とりあえずSTOPを挿入
        if e[ - 1] == 'RTN':e[ - 1] = 'STOP'
        tail(e[ - 2])                       # CODE1について末尾処理を
        tail(e[ - 3])                       # CODE2も同じ
        return
    return
def macrofunction_expand(c, p):
            body = copy.deepcopy(c[1])                      # bodyはマクロ本体
            #print(body)
            prms = c[2]                                     # prmsはパラメータリスト
            #print(body, prms)
            i = 0
            for pp in prms:
                #print(pp)
                cc = search_nr(body, 'WHILE')               # whileの位置を覚えておく
                replace(p[i], ['WHILE'], ['__WHILE__'])     # 実引数のWHILEはリネームしておく
                                                            # 実質やりたいことはこの行のみ
                replace(body, ['LD', pp], p[i], 'LDF')      # 仮引数を実引数で置き換える
                                                            # 以後は置き換えでずれたWHILEの戻り先修正
                                                            #
                dd = search_nr(body, 'WHILE')               # 引数入れ替え後のWHILE位置を算出
                if cc == []:
                    if dd != []:error() 
                elif len(cc) != len(dd)  :error()
                else:
                    j = 0
                    for ccc in cc:
                        ddd = dd[j]
                        cn, dn = ccc[1], ddd[1]
                        if cn != dn:
                            #ddd[0][dn + 1] =ddd[0][dn + 1] + (dn - cn) - 1
                            for k in range(dn, 0,  - 1):    # 直前のPOPの次が戻り先
                                if ddd[0][k] == 'POP':
                                    break
                            ddd[0][dn + 1] = dn - k - 1
                        j += 1
                #
                replace(body, ['__WHILE__'], ['WHILE'])     #リネームしたWHILEを戻す
                #
                i += 1
            #
            #p[0] = body
            #print(p[0])
            #v = eval([], [G], c[1] + ['STOP'], 0, [], [])
            #p[0] = ['LDC', v]
            return body
def load(file_name):
    f = open(file_name)
    s = f.read()
    print(s)
    result = parser.parse(s)
    #c_time = time.time()
    v = eval([], [G], result + ['STOP'], 0, [], [])
    #e_time = time.time()
    if   type(v) == list and v != [] and v[0] == 'CL':print('Usser Function')
    elif type(v) == list and v != [] and v[0] == 'MACRO_CL':print ('User Macro')
    elif type(v) == list and v != [] and v[0] == 'CONT':print ('User Continueation')
    else:print(v)
    #print('c_time  : ', int((1000000 * (c_time - s_time))) / 1000000)
    #print('e_time  : ', int((1000000 * (e_time - c_time))) / 1000000)
    G['_'] = v

def Push(L, v):
    L.append(v)
    return v

G = {'_':None, 'push':Push, 'isin':isin, 'dict_isin':Dict_isin, 'load':load, 'range':range,
        'list':list, 'Fraction':Fraction, 'print':print, 'len':len, 'list':List, 
        'list_set':List_set, 'dict':Dict, 'dict_set':Dict_set, 'dict_ref':Dict_ref,
        '_time':True, '_code':True}
G.update(vars(operator))
G.update(vars(math))
#G.update(vars(cmath))
#G = {'print':print}
#
#
def expr(p):
    '''
    expr        : sngl_expr
                | LBRAC expr_list RBRAC
    '''
    if len(p) == 2: p[0] = p[1]
    else: p[0] = p[2]       # 複式をLDFで置き換えるなら書き直すこと

def expr_list(p):
    '''
    expr_list   : sngl_expr
                | expr_list SEMICOL sngle_expr
    '''
    if len(p) == 2: p[0] = p[1]
    else: p[0] = p[1] + ['POP'] + p[3]

def sngl_expr(p):
    '''
    sngl_expr   : if_expr
                | while_expr
                | break_expr
                | contnue_expr
                | set_expr
    '''
    p[0] = p[1]

def if_expr(p):
    '''
    if_expr     : IF expr COL expr COL expr
    '''
    p[0] = p[2] + ['SEL', p[4] + ['JOIN'], p[6] + ['JOIN']]
    # 定数の展開
    if len(p[2]) == 2 and p[2][0] == 'LDC':
        if p[2][1]: p[0] = p[4]
        else: p[0] = p[6]

def while_expr(p):
    '''
    while_expr  : WHILE expr COL expr
    '''
    p[0] = p[2] + ['WHILE', len(p[2]), p[4]]

def break_expr(p):
    '''
    break_expr: BREAK
    '''
    p[0] = ['_BREAK_'] # if文の中ならRを一つPOPして

def continue_expr(p):
    '''
    continue_expr   : CONTINUE
    '''
    p[0] = ['_CONTINUE_'] # if文の中ならRを一つPOPしてJOINするような命令

NON_TYPE, INC_TYPE, DEC_TYPE, CALL_TYPE, APLY_TYPE, REF_TYPE = 0, 1, 2, 3, 4

def set_expr(p):
    '''
    set_expr    : eq_expr
                | pfix_expr LET expr
                | pfix_expr ADDLET expr
                | pfix_expr SUBLET expr
                | pfix_expr MULLET expr
                | pfix_expr DIVLET expr
    '''
    if len(p) == 2:
        p[0] = p[1]
        return
    # ここのpfix_exprは右辺式なのでそれ用にコンパイルする
    if p[1][0] != '_PF_': # 後置式ではないのでp[1]を通常のコンパイル
        if p[2] == ' = ':
            p[0] = p[3] + ['SET', p[1]]
        elif p[2] == '+=':
            p[0] = p[1] + p[3] + ['ADD', 'SET'] + p[1]] 
        elif p[2] == '-=':
            p[0] = p[1] + p[3] + ['SUB', 'SET'] + p[1]
        elif p[2]  == '*='
            p[0] = p[1] + p[3] + ['MUL', 'SET'] + p[1]
        elif p[2] == ' /= ':
            p[0] = p[1] + p[3] + ['DIV', 'SET'] + p[1]
        return
    pexp_type = p[1][1]
    if pexp_type == NON_TYPE:
        if p[2] == '=': p[0] = p[3] + ['SET', p[1][1]]
        elif p[2] == '+=':p[0] = p[3] 
    elif pexp_type == VECT_REF:
        v = p[2][1]
        r = p[2][2]
    elif pexp_type == FUNC_CALL:

    elif pexp_type  == INC_TYPE:

        elif pexp_type == DEC_TYPE:

def eq_expression(p):   
    '''
    eq_expr     : rel_expr
                | eq_expr IS rel_expr
                | eq_expr EQUAL rel_expr
                | eq_expr NEQ rel_expr
    '''
    if len(p) == 2:
        p[0] = p[1]
        return
    p[0] = p[1] + p[3]
    if   p[2] == 'is': p[0] += ['IS']
    elif p[2] == '==': p[0] += ['EQ']
    elif p[2] == '!=': p[0] += ['NEQ']
    #　定数畳み込み
    if len(p[1]) == 2 and len(p[3]) == 2 and p[1][0] ==  'LDC' and p[3][0] ==  'LDC':
        v = eval([], [G], p[0] + ['STOP'],0,  [], [])
        p[0] = ['LDC', v]

def rel_expression(p):
    '''
    rel_expr    : add_expr
                | rel_expr GT add_expr
                | rel_expr GEQ add_e]pr
                | rel_expr LT add_expr
                | rel_expr LEQ add_expr
    '''
    if len(p) == 2:
        p[0] = p[1]
        return
    p[0] = p[1] + p[3]
    if   p[2] == '>': p[0] += ['GT']
    elif p[2] == '>=': p[0] += ['GEQ']
    elif p[2] == '<': p[0] += ['LT']
    elif p[2] == '<=': p[0] += ['LEQ']
    #　定数畳み込み
    if len(p[1]) == 2 and len(p[3]) == 2 and p[1][0] ==  'LDC' and p[3][0] ==  'LDC':
        v = eval([], [G], p[0] + ['STOP'],0,  [], [])
        p[0] = ['LDC', v]

def add_expr(p):
    '''
    add_expr    : mul_expr
                | add_expr ADD mul_expr
                | add_expr SUB mul_expr
    '''
    if len(p) == 2:
        p[0] = p[1]
        return
    p[0] = p[1] + p[3]
    if   p[2] == '+': p[0] += ['ADD']
    elif p[2] == '-': p[0] += ['SUB']
    #　定数畳み込み
    if len(p[1]) == 2 and len(p[3]) == 2 and p[1][0] ==  'LDC' and p[3][0] ==  'LDC':
        v = eval([], [G], p[0] + ['STOP'],0,  [], [])
        p[0] = ['LDC', v]

def mul_expr(p):
    '''
    mul_exp     : uni_exper
                | mul_expr MUL uni_expr
                | mul_expr DIV uni_expr
                | mul_expr MOD uni_expr
                | mul_expr POW uni_expr
                | mul_expr IDIV uni_expr
    '''
    if len(p) == 2:
        p[0] = p[1]
        return
    p[0] = p[1] + p[3]
    if   p[2] == '*': p[0] += ['MUL']
    elif p[2] == '/': p[0] += ['DIV']
    elif p[2] == '%': p[0] += ['MOD']
    elif p[2] == '**': p[0] += ['POW']
    elif p[2] == '//': p[0] += ['IDIV']
    #　定数畳み込み
    if len(p[1]) == 2 and len(p[3]) == 2 and p[1][0] ==  'LDC' and p[3][0] ==  'LDC':
        v = eval([], [G], p[0] + ['STOP'],0,  [], [])
        p[0] = ['LDC', v]

NON_TYPE, INC_TYPE, DEC_TYPE, CALL_TYPE, APLY_TYPE, REF_TYPE = 0, 1, 2, 3, 4

def compile_pfix_expr(p):
    # pri_fix_expression p をコンパイルする
    if p[0] != '_PF_': return p  # pri_fix_exprでなければそのまま返す

    t = p[1]
    if t == NON_TYPE:
        r = p[2]
    elif t == INC_TYPE:
        r = p[2] + ['INC']
    elif t == DEC_TYPE:
        r = p[2] + ['DEC']
    elif t == CALL_TYPE:    # 関数呼び出し
        # 関数マクロの場合
        # マクロでない場合
        args = len(p[2])
        C = p[3]
        for c in p[2]:
            C = C + c
        r = C + ['CALL', args] 
    elif t == APLY_TYPE:    # 関数呼び出し(APPLY)
        # 関数マクロの場合
        # マクロでない場合
        args = len(p[2])
        C = p[3]
        for c in p[2]:
            C = C + c
        r = C + ['APL', args]
    elif t == REF_TYPE:     # vector参照
        r = p[2] + p[3] + ['REF']
    return r

def uni_expr(p):
    '''
    uni_expr    : pfix_expr
                | MINUS uni_expr
                | NOT uni_expr
                | CALLCC LPAREN expr RPAREN
    '''
    if len(p) == 2: # 単純式または後置式
        p[0] = compile_pfix_expr(p[1])
        return
    if len(p) == 5: # call_cc
        p[0] = ['LDICT', '__CODE__'] + p[3] + ['CALL', 1]
        return
    # 単項演算子 '-'または'!'の場合
    p[0] = p[2]
    if p[1] == '-':p[0] += ['MINUS']
    elif p[1] == '!':p[0] += ['NOT']
    else
    # 定数の畳み込み
    if len(p[2]) == 2 and p[2][0] ==  'LDC' :
        v = eval([], [G], p[0] + ['STOP'],0,  [], [])
        p[0] = ['LDC', v]

NON_TYPE, INC_TYPE, DEC_TYPE, CALL_TYPE, APLY_TYPE = 0, 1, 2, 3, 4
def pfix_expr(p):
    '''
    pfix_expr   : prm_expr
                | pfix_expr LBRAK expr RBRAK
                | pfix_expr LPAREN args DOTS RPAREN
                | pfix_expr LPAREN args RPAREN
                | pfix_expr LPAREN RPAREN
                | pfix_expr INC
                | pfix_expr DEC
    '''
    if len(p) == 2:
        p[0] =  p[1]         # 後置表記なし
        return
    # 長さが2でないならp[1]がpfix_exprなのでまずそれをコンパイル
    p[1] = compile_pfix_expr(p[1])
    #　右辺式のコードを生成しp[0]に入れる
    if   p[2] == '++': p[0] = ['_PF_', INC_TYPE, p[1]]  # ++ の後置表記
    elif p[2] == '--': p[0] = ['_PF_', DEC_TYPE, p[1]]  # -- の後置表記
    elif p[2] == '(' :   # 関数呼び出し
        if len(p) == 4:  # 引数なしの関数呼び出し表記
            p[0] = ['_PF_', CALL_TYPE,[], p[1] ]
        elif len(p) == 5:
            p[0] = ['_PF_', CALL_TYPE, p[3], p[1]]
        elif len(p) == 6:
            p[0] = ['_PF_', APLY_TYPE, p[3], p[1]]
    elif p[2] == '[': p[0] = ['_PF_', REF_TYPE, p[3], p[1]]   # Vector参照表記

def prm_exp(p):     # primitive expression
    '''
    prm_expr    : atm
                | LPAREN expr RPAREN
                | var
                | vector
                | closure
                | macro_expr
    '''
    if len(p) == 2: p[0] = p[1]
    else:
        p[0] = p[2]

def atm(p):         # basic factor
    '''
    atm         : INT | FLOAT | E_FLOAT |FLOAT2 | FACT
                | TRUE | FALSE
                | STR
    '''
    p[0] = ['LDC', p[1]]

def var(p):
    '''
    var         : ID
    '''
    p[0] = ['LD', p[1]]
    # マクロ処理を追加すること

def vector(p):
    '''
    vector      : LBRAK args RBRAK
                | LBRAK RBRAK
    '''
    if len(p) == 2: p[0] = ['LDC', []]  # NULL vector
    else:
        n = len(p[1])
        C = []
        for c in p[1]:
            C = C + c
        p[0] = C + ['VEC', n]
# closure (LAMBDA式)
def closure(p):
    '''
    closure     : LAMBDA LPAREN id_list DOTS RPAREN expr
                | LAMBDA LPAREN id_list RPAREN expr
                | LAMBDA LPAREN RPAREN expr
    '''
    if len(p) == 5:
        p[0] = ['LDF', tail(p[4] + ['RTN']), []]
    if len(p) == 6:
        p[0] = ['LDF', tail(p[5] + ['RTN']), p[3]]
    if len(p) == 7:
        p[0] = ['LDF', tail(p[6] + ['RTN']), p[3], '..']
    
# 関数マクロ
def macro_expr(p):
    '''
    macro_expr  : MACRO LPAREN args DOTS RPAREN expression 
                | MACRO LPAREN args RPAREN expression
                | MACRO LPAREN RPAREN expression
                | MACRO expression
    '''
    if len(p) == 7:p[0] = ['LDM_CL', p[6], p[3], ['..']]
    elif len(p) == 6:p[0] = ['LDM_CL', p[5], p[3]]
    elif len(p) == 5:p[0] = ['LDM_CL', p[4], []]
    elif len(p) == 3:p[0] = ['LDM', p[2]]

def args(p):
    '''
    args        : expr
                | args CAMMA expr
    '''
    if   len(p) == 2: p[0] = [p[1]]
    elif len(p) == 4: p[0] = p[1] + [p[3]]

def id_list(p):
    '''
    id_list     : ID
                | id_list CAMMA ID
    '''
    if   len(p) == 2: p[0] = [p[1]]
    elif len(p) == 4: p[0] = p[1] + [p[3]]

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
            s = input('pure> ')
        except EOFError:
            break
        if not s:
            continue
        s_time = time.time()
        try:
            result = parser.parse(s)
            #print(result)
        except (SyntaxError, ZeroDivisionError) as e:
            print(e)
            continue
        #
        # call_ccの処理
        #
        q = result + ['STOP']
        qq = q
        cc = search_nr(qq, '__CODE__')
        if cc != []: 
            for (ccc, j) in cc:
                ccc[j] = ccc[j + 6:]
        # __DCL__を元に戻す
        replace(qq, ['__DCL__'], ['DCL'])
        if G['_code']:print(qq)
        c_time = time.time()
        #print( result)
        try:
            #v = eval([], [G], result + ['STOP'], 0, [], [])
            v = eval([], [G], qq, 0, [], [])
            e_time = time.time()
            if type(v) == list and v != [] and v[0] == 'CL':print('Usser Function')
            elif type(v) == list and v != [] and v[0] == 'MACRO_CL':print ('User Macro')
            elif type(v) == list and v != [] and v[0] == 'CONT':print ('User Continueation')
            else:print(v)
            if G['_time']:print('c_time  : ', int((1000000 * (c_time - s_time))) / 1000000, '\te_time  : ', int((1000000 * (e_time - c_time))) / 1000000)
            G['_'] = v
        except (KeyError, IndexError, SyntaxError, TypeError, ZeroDivisionError) as e:
            print(e)
            continue
        #except :
        #    print("Some Error Occured!")
        #   continue
