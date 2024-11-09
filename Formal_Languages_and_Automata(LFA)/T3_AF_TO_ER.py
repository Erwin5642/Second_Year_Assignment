class RE :
    
   def run(self,Sigma,w) :
       return self.NFA(Sigma).run(w)

class Empty(RE) :
    def __repr__(self) :
        return "Empty()"

    def NFA(self,Sigma) :
        return NFA.empty(Sigma)
       
class Epsilon(RE) :
    def __repr__(self) :
        return "Epsilon()" 
    
    def NFA(self,Sigma) :
        return NFA.epsilon(Sigma)
    
class Symbol(RE) :
    def __init__(self,sym) :
        self.sym = sym

    def NFA(self,Sigma) :
        return NFA.symbol(Sigma,self.sym)
        
    def __repr__(self) :
        return f"Symbol({self.sym})"
      
class Plus(RE) :
    def __init__(self,left,right) :
        self.left = left
        self.right = right        
        
    def __repr__(self) :
        return f"Plus({self.left},{self.right})"
    
    def NFA(self,Sigma) :
        return NFA.plus(self.left.NFA(Sigma),self.right.NFA(Sigma))  

class Append(RE) :
    def __init__(self,left,right) :
        self.left = left
        self.right = right        
        
    def __repr__(self) :
        return f"Append({self.left},{self.right})"
    
    def NFA(self,Sigma) :
        return NFA.append(self.left.NFA(Sigma),self.right.NFA(Sigma))  
    
class Star(RE) :
    def __init__(self,re) :
        self.re = re
        
    def __repr__(self) :
        return f"Star{self.re})"
    
    def NFA(self,Sigma) :
        return NFA.star(self.re.NFA(Sigma))    


# (b+epsilon)(ab)*(a+epsilon)
e1 = Append(Plus(Symbol("b"),Epsilon()),
       Append(Star(Append(Symbol("a"),Symbol("b"))),
              Plus(Symbol("a"),Epsilon())))
Sigma = {"a","b","c"}

def test(e,w) :
   print(f"e={e},w={w} => {e.run(Sigma,w)}")
# unit tests
    
test(Empty(),"")
test(Epsilon(),"")
test(Epsilon(),"a")
test(Symbol("a"),"")
test(Symbol("a"),"a")
test(Plus(Epsilon(),Symbol("a")),"a")  
test(Plus(Epsilon(),Symbol("a")),"")  

test(e1,"")
test(e1,"a")
test(e1,"ab")
test(e1,"abb")
test(e1,"aba")
test(e1,"babab")
test(e1,"babaab")

Sigma={"0","1"}
# (0+11+10(00+1)*01)*
e2 = Star(Plus(Symbol("0"),
               Plus(Append(Symbol("1"),Symbol("1")),
                    Append(Append(Symbol("1"),Symbol("0")),
                           Append(Star(Plus(Append(Symbol("0"),Symbol("0")),
                                            Symbol("1"))),
                                  Append(Symbol("0"),Symbol("1")))))))
test(e2,"11")
test(e2,"100")
test(e2,"101")
test(e2,"110")
test(e2,"1")
test(e2,"100001")