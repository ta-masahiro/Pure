import copy
def eval(S, E, C, cp, R, EE):
    #print(S, E,C , cp, R, EE)
    while True:
        inst = C[cp]
        #print('S:', S)
        #print('C:', C[cp:])i
        #print(E[0])
        #if not ('z' in E[0]):
        #    print("!!!!!!!!!!!", C[cp:])
        if inst == 'STOP':
            #print(S, R)
            #return S[ - 1]
            return S.pop()
        cp += 1
        if inst == 'LDC':
            S.append(C[cp])
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
            S[ - 2] /= S[ - 1]
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
        elif inst == 'TCALL': 
            n = C[cp]
            cp += 1
            fn = S.pop()
            if n == 0:l = []
            else:
                l = copy.deepcopy(S[ - n:])
                del(S[ - n:])
            if type(fn) == list and fn[0] == 'CL':
                k = fn[2]
                #print(k, l)
                if k != [] and k[ - 1] == '..':
                    ln =  - len(l) + len(k) - 2
                    c = l[ln:]
                    if ln !=  - 1:del(l[ln + 1:])
                    l[ - 1] = c
                    #print(l)
                #if len(k) != len(l):print("#########TCALL#####################", k, l, C[cp - 4:cp]) 
                e = dict(zip(k, l))
                #print(e)
                #R.append([C, cp])
                #EE.append(E)
                E = [e] + fn[3]
                C = fn[1] 
                cp = 0
            elif n == 0:S.append(fn())
            elif n == 1:S.append(fn(l[0]))
            else: S.append(fn( * l))
        elif inst == 'CALL': 
            n = C[cp]
            cp += 1
            fn = S.pop()
            if n == 0:l = []
            else:
                l = copy.deepcopy(S[ - n:])
                del(S[ - n:])
            if type(fn) == list and fn[0] == 'CL':
                k = fn[2]
                #print(k, l)
                if k != [] and k[ - 1] == '..':
                    ln =  - len(l) + len(k) - 2
                    c = l[ln:]
                    if ln !=  - 1:del(l[ln + 1:])
                    l[ - 1] = c
                    #print(l)
                #if len(k)!=len(l):print("##############CALL################", k, l, C[cp - 4:cp]) 
                e = dict(zip(k, l))
                #print(e)
                R.append([C, cp])
                EE.append(E)
                E = [e] + fn[3]
                C = fn[1] 
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
            elif n == 0:S.append(fn())
            elif n == 1:S.append(fn(l[0]))
            else: S.append(fn( * l))
        elif inst == 'RTN':
            E = EE.pop()
            C, cp = R.pop()
        elif inst == 'SET':
            v = S[ - 1]
            k = C[cp]
            cp += 1
            ff = False
            #print(E)
            for e in E:
                if k in e:
                    e[k] = v
                    ff = True
                    break
            if not ff:(E[0])[k] = v
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
        elif inst == 'JOIN':
            C, cp =R.pop() 
        elif inst == 'LDF':
            k = C[cp + 1]
            #print(k)
            S.append(['CL', C[cp], C[cp + 1], E])
            cp += 2
        elif inst == 'POP':
            del(S[ -1])

        else:
            raise KeyError('Unknown Code:' + inst)


