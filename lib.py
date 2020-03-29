{   # 任意個数引数の加算
    add = lambda(x ..) {
        n=len(x); 
        sum = lambda(s,i) 
            if i>=n: 
                s
            : 
                sum(s+x[i],i+1); 
        sum(0,0)
    }
    ;
    # mapref([[1, 2, 3], [4, 5, 6], [7, 8, 9]], 1) = [2, 5, 8]
    #def mapref(L,n) = {
    mapref = lambda(L, n) {
        loop=lambda(i,s) 
            if i<0 :
                s 
            :
                loop(i-1, [L[i][n]] + s);
        loop(len(L)-1,[])
    }
    ;
    map = lambda(f,L ..) 
        if len(L)==1: {
            loop=lambda(s,i) 
                if i<0:
                    s
                :
                    loop([f(L[0][i])]+s,i-1);
            loop([],len(L[0])-1)
        } : {
            loop=lambda(s,i) 
                if i<0 :
                    s :
                    loop([apply(f,mapref(L,i))]+s,i-1);
            loop([],len(L[0])-1)
        }
    ; 
    range = lambda(l ..) {
        s=0;
        e=0;
        stp=1;
        n=len(l);
        if n==1:
            e=l[0]
        :
            if n==2:{
                s=l[0];
                e=l[1]
            }:
                if n==3:{
                    s=l[0];
                    e=l[1];
                    stp=l[2]
                }:
                    [];

        if stp>0 :{
            loop=lambda(i,r) if i>=e:r:loop(i+stp,r + [i]); 
            loop(s,[])
        }:{
            loop = lambda(i, r) if i<=e:r:loop(i + stp, r + [i]); 
            loop(s, [])
        }
    }
    ; 
    # クロージャによる遅延関数の例
    tarai=lambda(x,y,z) 
        if x<=y: 
            y 
        :{
            zz=z();
            tarai(  tarai(x-1,y,lambda() zz),
                    tarai(y-1,zz,lambda() x),
                    lambda() tarai(zz-1,x,lambda() y)
                )
        }
    ; # 形式的に完全な遅延関数化
    tarai1 = lambda(x, y, z) {
            t = lambda(x, y, z)
                if x()<= y():
                    y()
                :
                    t(lambda() t(lambda() x() - 1, lambda() y(), lambda() z()), 
                      lambda() t(lambda() y() - 1, lambda() z(), lambda() x()), 
                      lambda() t(lambda() z() - 1, lambda() x(), lambda() y())); 
             t(lambda()x, lambda()y, lambda()z) 
    }
    ; 
    # たらい回し関数のサンプル
    count = 0;                          # 何回呼出したかのカウンタ
    t = lambda(x, y, z) { 
        count = count + 1; 
        if x <= y:
	    z
        :
            t(  t(x-1, y, z),
                t(y-1, z, x),
                t(z-1, x, y)
            )
    }
    ; 
    # メモ化関数
    memois = lambda(f) {
        # d = dict([1],[1]);
        d = {}; 
        lambda(args ..) 
            if dict_isin(d, args):
                dict_ref(d, args)
            :
                dict_set(d, args, f(args ..))
            
    }
    ; 
    # マクロ
    for  =  macro(exp1, exp2, exp3) {
                exp1; 
                while exp2: exp3
            }
    ; 
    and  =  macro(exp1, exp2) 
                if exp1: exp2: False
    ; 
    or  =   macro(exp1, exp2)
                if exp1: True: exp2
    ; 
    foreach  = macro(x, l, exp) 
                for(i = 0, i<len(l), {x = l[i]; exp; i = i + 1})
    ;
    filter  = lambda(f, l) {
                N=len(l);
                g   = lambda(i, s)
                    if i>= N:
                        s
                    : g(i+1, if f(l[i])
                                :push(s,l[i])
                                :s
                        );
                g(0,[])}
    ; 
    reduce   =  lambda(fn, a, ls) {
                N = len(ls); 
                g = lambda(i, a)
                    if i >= N:
                        a:
                        g(i + 1, fn(a, ls[i]))
                ;
                g(0, a)
    }
}
