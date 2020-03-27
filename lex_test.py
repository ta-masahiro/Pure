 # -*- coding: utf-8 -*
import ply.lex as lex
from fractions import Fraction
# トークンリスト 常に必須
reserved = {'set':'SET', 'if':'IF', 'lambda':'LAMBDA','def':'DEF', 'True':'TRUE', 
        'False':'FALSE', 'is':'IS', 'apply':'APPLY', 'call_cc':'CALLCC', 'var':'VAR', 'while':'WHILE', 'macro':'MACRO'}
tokens = ['COMMENT', 'STR', 'INT', 'FLOAT','FRACT', 'PLUS','MINUS','TIMES','POW', 'DIVIDE',
        'LPAREN','RPAREN','LBRAC', 'RBRAC','LBRAK', 'RBRAK',  'CAMMA','COL','SEMICOL','DOTS', 'LET',
        #'ADD_LET', 'SUB_LET', 'MUL_LET', 'DIV_LET', 'REF_LET', 'FN_LET', 
        'EQUAL','NEQ', 'GEQ', 'LEQ', 'GT', 'LT', 'NOT', 'ID', 'PERC'] + list(reserved.values())
 # 正規表現による簡単なトークンのルール
t_PLUS   = r'\+'
t_MINUS  = r'-'
t_TIMES  = r'\*'
t_POW    = r'\*\*'
t_DIVIDE = r'/'
t_PERC   = r'%'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_LBRAC  = r'{'
t_RBRAC  = r'}'
t_LBRAK  = r'\['
t_RBRAK  = r'\]'
t_CAMMA  = r','
t_COL    = r':'
t_SEMICOL= r';'
t_DOTS   = r'\.\.'
t_LET    = r'='
#t_ADD_LET = r'\+='
#t_SUB_LET = r'-='
#t_MUL_LET = r'\*='
#t_DIV_LET = r'/='
#t_REF_LET = r'\]='
#t_FN_LET  = r'\)='
t_EQUAL  = r'=='
t_NEQ    = r'!='
t_GEQ    = r'>='
t_LEQ    = r'<='
t_GT     = r'>'
t_LT     = r'<'
t_NOT    = r'!'
t_IF     = r'if'
t_DEF    = r'def'
t_SET    = r'set'
t_LAMBDA = r'lambda'
t_TRUE   = r'True'
t_FALSE  = r'False'
t_IS     = r'is'
t_APPLY  = r'apply'
t_CALLCC = r'call_cc'
t_VAR    = r'var'
t_WHILE  = r'while'
t_MACRO  = r'macro'
# 正規表現とアクションコード
#def t_E_FLOAT(t):
#    r'\d+[eE][+-]?\d+'
#    t.value = float(t.value)
#    return t
#def t_FLOAT(t):
#     r'\d+\.\d*([eE][+-]?\d+)? '
#     t.value = float(t.value)
#     return t
#def t_FLOAT2(t):
#     r'\d*\.\d+([eE][+-]?\d+)? '
#     t.value = float(t.value)
#     return t
def t_FLOAT(t):
     r'\d+[eE][+-]?\d+|(\d*\.\d+|\d+\.\d*)([eE][+-]?\d+)?'
     t.value = float(t.value)
     return t
def t_FRACT(t):
     r'\d+/ \d+'
     t.value =Fraction(t.value)
     return t
def t_INT(t):
     r'\d+'
     t.value = int(t.value)
     return t
def t_ID(t):
    r'[a-zA-Z_]\w*'
    t.type = reserved.get(t.value, 'ID')
    return t
def t_STR(t):
    #r'"(?:[\\].|[^\\"])*"'
    r'"((\\"|[^"]) * )"'
    t.value = (t.value)[1: - 1].encode().decode('unicode_escape')
    return t
def t_COMMENT(t):
    #r'/*""/"*([^*/]|[^*]"/"|"*"[^/])*"*"*"*/'
    r'\#. * '
    pass
# 行番号をたどれるように 
def t_newline(t):
     r'\n+'
     t.lexer.lineno += len(t.value)
# スペースおよびタブは無視 
t_ignore = ' \t'
# エラーハンドリングルール 
def t_error(t):
     #print( u" 不正な文字 '%s'" % t.value[0])
     raise SyntaxError("Illegal charactor" + t.value[0])
# lexer を構築 
lexer = lex.lex() 

if __name__ == '__main__':
     # ここからテスト
     data = '''
       3 + 4 * 10.123
       + -20 *2 - 9s00T_frR +_67(x, y)
        - lambda(a, b, c) + if(z, y, x)
     '''
     lexer.input(data)
     while True:
         tok = lexer.token()
         if not tok:
             # これ以上トークンはない
             break
         print( tok)
