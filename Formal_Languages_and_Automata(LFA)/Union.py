def inj(b,s) :
    "labels all elements of a set"
    return {(b,x) for x in s}

def dunion(s1,s2) :
    "construct the disjoint union of two sets"
    return inj(False,s1) | inj(True,s2)  

def inj_delta(delta,b) :
    return { ((b,qa[0]),qa[1]):inj(b,delta[qa]) 
                for qa in delta.keys()}
    
def dunion_delta(delta1,delta2) :
        "lifts delta to the disjoint union"
        # return inj_delta(delta1,False) | inj_delta(delta2,True)
        d = inj_delta(delta1,False)
        d.update(inj_delta(delta2,True))
        return d