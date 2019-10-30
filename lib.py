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
    def mapref(L,n) = {
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
            loop([],len(L)-1)
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
                e=l[1]
            }:
                if n==3:
                    stp=l[3]
                :
                    [];

        loop=lambda(i,r) if i>=e:r:loop(i+stp,r + [i]); 
        loop(s,[])
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
        d = dict([1],[1]);
        lambda(args ..) 
            if dict_isin(d, args):
                dict_ref(d, args)
            :{
                v = apply(f, args);
                dict_set(d, args, v);
                v
            }
    }
}
