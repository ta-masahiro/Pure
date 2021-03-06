import copy
from fractions import Fraction
def eval(S, E, C, cp, R, EE):
    #print(S, E,C , cp, R, EE)
    while True:
        inst = C[cp]
        #print('S:', S)
        #print('C:', C[cp:])i
        #print(E[0])
        #if not ('z' in E[0]):
        #print(inst)
        if inst == 'STOP':
            #print(S, R)
            #return S[ - 1]
            #return S.pop()
            return S
        cp += 1
        if inst == 'LDC':
            #
            if C[cp] == []:S.append([])
            else:S.append(C[cp])
            cp += 1
        elif inst == 'LD':
            v = C[cp]
            cp += 1
            ff = False
            for e in E:
                try:
                    S.append(e[v])
                    #print(v, e[v])
                except:continue
                ff = True
                break
            if not ff:
                #print(C[cp:])
                raise KeyError("UnknownKey:" + v)
        elif inst == 'INC':
            S[ - 1] += 1
        elif inst == 'DEC':
            S[ - 1]  -= 1
        elif inst == 'MINUS':
            S[ - 1] =  - S[ - 1]
        elif inst == 'NOT':
            S[ - 1] = not S[ - 1]
        elif inst == 'ADD':
            S[ - 2] += S[ - 1]
            del(S[ - 1])
        elif inst == 'SUB':
            S[ - 2] -= S[ - 1]
            del(S[ - 1])
        elif inst == 'MUL':
            S[ - 2]  *= S[ - 1]
            del(S[ - 1])
        elif inst == 'DIV':
            if isinstance(S[ - 2], int) and isinstance(S[ - 1], int):S[ - 2] = Fraction(S[ - 2], S[ - 1])
            else:S[ - 2] /= S[ - 1]
            del(S[ - 1])
        elif inst == 'POW':
            S[ - 2] = S[ - 2] ** S[ - 1]
            del(S[ - 1])
        elif inst == 'EQ':
            S[ - 2] = (S[ - 2] == S[ - 1])
            del(S[ - 1])
        elif inst == 'NEQ':
            S[ - 2] = (S[ - 2] != S[ - 1])
            del(S[ - 1])
        elif inst == 'GEQ':
            S[ - 2] = S[ - 2] >= S[ - 1]
            del(S[ - 1])
        elif inst == 'LEQ':
            S[ - 2] = S[ - 2] <= S[ - 1]
            del(S[ - 1])
        elif inst == 'GT':
            S[ - 2] = S[ - 2] > S[ - 1]
            del(S[ - 1])
        elif inst == 'LT':
            S[ - 2] = S[ - 2] < S[ - 1]
            del(S[ - 1])
        elif inst == 'IS':
            S[ - 2] = S[ - 2] is S[ - 1]
            del(S[ - 1])
        elif inst == 'VEC':
            n = C[cp]
            cp += 1
            v = S[ - n:]
            del(S[ - n:])
            S.append(v)
        elif inst == 'REF':
            ref = S.pop()
            t = S.pop()
            S.append(t[ref])
        elif inst == 'SLS':
            sl_s = S.pop()
            sl_e = S.pop()
            t = S.pop()
            S.append(t[sl_s:sl_e])
        elif inst == 'TCALL': 
            n = C[cp]
            cp += 1
            fn = S.pop()
            l = []
            if n != 0:
                #l = copy.deepcopy(S[ - n:]) #deep copyはやりすぎ！要素1個1個のcopyが望ましい
                l = (S[ - n:]) 
                del(S[ - n:])
                #for i in range(n):
                #    l = [S.pop()] + l
            if type(fn) == list and fn[0] == 'CL':
                k = fn[2]
                #print(k, l)
                if k != [] and k[ - 1] == '..':
                    ln =  - len(l) + len(k) - 2
                    c = l[ln:]
                    if ln !=  - 1:del(l[ln + 1:])
                    l[ - 1] = c
                    #print(l)
                e = dict(zip(k, l))
                #print(e)
                #R.append([C, cp])
                #EE.append(E)
                E = [e] + fn[3]
                C = fn[1] 
                cp = 0
            elif type(fn) == list and fn[0] == 'CONT':
                S, E, C, R, EE = fn[1] + l, fn[2][:], fn[3], fn[4][:], fn[5][:]
                cp = 0
            elif n == 0:S.append(fn())
            elif n == 1:S.append(fn(l[0]))
            else: S.append(fn( * l))
        elif inst == 'CALL': 
            n = C[cp]
            cp += 1
            fn = S.pop()
            l = []
            if n != 0:
                #l = copy.deepcopy(S[ - n:])
                l = S[ - n:]
                del(S[ - n:])
                #for i in range(n):
                #    l = [S.pop()] + l
            if type(fn) == list and fn[0] == 'CL':
                k = fn[2]
                #print(k, l)
                if k != [] and k[ - 1] == '..':
                    ln =  - len(l) + len(k) - 2
                    c = l[ln:]
                    if ln !=  - 1:del(l[ln + 1:])
                    l[ - 1] = c
                    #print(l)
                e = dict(zip(k, l))
                #print(e)
                R.append([C, cp])
                EE.append(E)
                E = [e] + fn[3]
                C = fn[1] 
                cp = 0
            elif type(fn) == list and fn[0] == 'CONT':
                S, E, C, R, EE = fn[1] + l, fn[2][:], fn[3], fn[4][:], fn[5][:]
                #print(fn[2])
                cp = 0
            elif n == 0:S.append(fn())
            elif n == 1:S.append(fn(l[0]))
            else: S.append(fn( * l))
        elif inst == 'TAPL': 
            n = C[cp]
            cp += 1
            fn, l = S[ - n ], S[ - n + 1 : - 1] + S[ - 1]
            del(S[ - n:])
            #print(fn, l)
            if type(fn) == list and fn[0] == 'CL':
                k = fn[2]
                #print(k, l)
                if k != [] and k[ - 1] == '..':
                    ln =  - len(l) + len(k) - 2
                    c = l[ln:]
                    if ln !=  - 1:del(l[ln + 1:])
                    l[ - 1] = c
                #    #print(l)
                e = dict(zip(k, l ))
                #print(e)
                #R.append([C, cp])
                #EE.append(E)
                E = [e] + fn[3]
                C = fn[1] 
                cp = 0
            elif type(fn) == list and fn[0] == 'CONT':
                S, E, C, R, EE = fn[1] + l, fn[2][:], fn[3], fn[4][:], fn[5][:]
                cp = 0
            elif n == 0:S.append(fn())
            elif n == 1:S.append(fn(l[0]))
            else: S.append(fn( * l))
        elif inst == 'APL': 
            n = C[cp]
            cp += 1
            fn, l = S[ - n ], S[ - n + 1 : - 1] + S[ - 1]
            del(S[ - n:])
            #print(fn, l)
            if type(fn) == list and fn[0] == 'CL':
                k = fn[2]
                #print(k, l)
                if k != [] and k[ - 1] == '..':
                    ln =  - len(l) + len(k) - 2
                    c = l[ln:]
                    if ln !=  - 1:del(l[ln + 1:])
                    l[ - 1] = c
                #    #print(l)
                e = dict(zip(k, l ))
                #print(e)
                R.append([C, cp])
                EE.append(E)
                E = [e] + fn[3]
                C = fn[1] 
                cp = 0
            elif type(fn) == list and fn[0] == 'CONT':
                S, E, C, R, EE = fn[1] + l, fn[2][:], fn[3], fn[4][:], fn[5][:]
                cp = 0
            elif n == 0:S.append(fn())
            elif n == 1:S.append(fn(l[0]))
            else: S.append(fn( * l))
        elif inst == 'RTN':
            E = EE.pop()
            C, cp = R.pop()
        elif inst == 'SET': # ... value key SET ...  -> key = value に変更 
            v = S[ - 1]
            #k = S.pop()
            k = C[cp]
            #v = S[ - 1]
            #print( "val = ", v)
            #print("key = ", k)
            cp += 1
            ff = False
            #print(E)
            for e in E:
                if k in e:
                    e[k] = v
                    ff = True
                    break
            if not ff:(E[0])[k] = v
            #raise KeyError('Unknown Key: ',k)
        elif inst == 'VSET':
            ind = S.pop()
            v = S[ - 1]
            k = C[cp]
            cp += 1
            ff = False
            #print(E)
            for e in E:
                if k in e:
                    e[k][ind] = v
                    ff = True
                    break
            if not ff:(E[0])[k][ind] = v
        elif inst == 'DCL':
            k = C[cp]
            cp += 1
            E[0][k] = None
            S.append(None)
        elif inst == 'TSEL':
            p = S.pop()
            t_exp = C[cp]
            f_exp = C[cp + 1]
            cp += 2
            #R.append([C, cp])
            cp = 0
            if p: C = t_exp
            else: C = f_exp
        elif inst == 'SEL':
            p = S.pop()
            t_exp = C[cp]
            f_exp = C[cp + 1]
            cp += 2
            R.append([C, cp])
            cp = 0
            if p: C = t_exp
            else: C = f_exp
        elif inst == 'WHILE':
            p = S.pop()
            back = C[cp]
            loop_exp = C[cp + 1]
            if p:
                R.append([C, cp - back - 1])
                #print(C[cp - back - 1])
                C = loop_exp
                cp = 0
            else:
                cp = cp + 2 
                S.append(None)
                #print(C[cp])
        elif inst == 'JOIN':
            C, cp =R.pop() 
        elif inst == 'LDF':
            k = C[cp + 1]
            #print(k)
            S.append(['CL', C[cp], C[cp + 1], E])
            cp += 2
        elif inst == 'LDICT':
            S.append(['CONT', S[:], E[:], C[cp], R[:], EE[:]])
            cp += 1
        elif inst == 'LDM':                                     #############################
            S.append(['MACRO', C[cp]])
            cp += 1
        elif inst == 'LDM_CL':                                  #############################
            S.append(['MACRO_CL', C[cp], C[cp + 1], E])
            cp += 2
        elif inst == 'DICT':
            n = C[cp]//2
            cp += 1
            d={}
            for i in range(n):
                k=S.pop()
                if isinstance(k,list):k=tuple(k)
                v=S.pop()
                d[k] = v
            S.append(d)
        elif inst == 'POP':
            del(S[ -1])
        #elif inst == '__CODE__':
        #    S.append('Continuation!')
        else:
            print(inst)
            raise KeyError('Unknown Code:' + inst)


