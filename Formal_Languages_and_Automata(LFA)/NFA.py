from Union import *

class NFA : 
    
    def __init__(self,Q,Sigma,delta,S,F) :
        self.Q = Q # set of states
        self.Sigma = Sigma # set of symbols
        self.delta = delta # transition relation
        self.S = S # set of initial states
        self.F = F # set of final states 
    
    def __repr__(self) :
        return f"NFA({self.Q},\n\t{self.Sigma},\n\t{self.delta},\n\t{self.S},\n\t{self.F})"

    def do_delta(self,q,x) :
        try :
            return self.delta[(q,x)]
        except KeyError :
            return set({})
        
    def run (self,w) :
        P = self.S
        while w!="" :
            Pnew = set ({})
            for q in P :
                Pnew = Pnew | self.do_delta(q,w[0])
            w = w[1:]
            P = Pnew
        return (P & self.F) != set({})
    
    def add_epsilon(self,q1,q2) :
            "adds an epsilon transition"
            for a in self.Sigma : 
                for q in self.do_delta(q2,a) :
                    self.delta[(q1,a)] = self.do_delta(q1,a) |  {q}
            if  q2 in self.F :
                self.F = self.F | {q1}

# factories for regular expressions

    def empty(Sigma) :
        return NFA(set(),Sigma,dict(),set(),set())
    
    def epsilon(Sigma) :
        return NFA({0},Sigma,dict(),{0},{0})
    
    def symbol(Sigma,sym) :
        return NFA({0,1},Sigma,{(0,sym):{1}},{0},{1})

    def plus(a1,a2) :
        Sigma = a1.Sigma | a2.Sigma
        Q = dunion (a1.Q, a2.Q)     
        delta = dunion_delta(a1.delta,a2.delta)
        S = dunion(a1.S,a2.S)      
        F = dunion(a1.F,a2.F)
        return NFA(Q,Sigma,delta,S,F)

    def append(a1,a2) :
        Sigma = a1.Sigma | a2.Sigma
        Q = dunion (a1.Q, a2.Q)     
        delta = dunion_delta(a1.delta,a2.delta)
        S = inj(False,a1.S)
        F = inj(True,a2.F)
        a = NFA(Q,Sigma,delta,S,F)
        for q1 in a1.F :
            for q2 in a2.S :
                a.add_epsilon((False,q1),(True,q2))
        return a   
     
    def star(a1) :
        a = NFA.plus(a1,NFA.epsilon(a1.Sigma))
        for q1 in a1.F :
            for q2 in a1.S :
                a.add_epsilon((False,q1),(False,q2))
        return a    
 
    
N0 = NFA({0,1,2},{"0","1"},
         {(0,"0"):{0},
          (0,"1"):{0,1},
          (1,"0"):{2},
          (1,"1"):{2}},
          {0},
          {2})
    
N1 = NFA({0,1},{"0","1"},
         {(0,"0"):{0},
          (0,"1"):{0,1}},
          {0},
          {1})
      
"""
N0.Q | N1.Q
dunion(N0.Q,N1.Q) 
dunion_delta(N0.delta,N1.delta)
inj(False,N0.Q)
"""   