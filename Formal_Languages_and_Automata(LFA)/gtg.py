
# nfa = NFA.fromUser() # To get input from user
class DFA:
    def __init__(self, states, alphabets, init_state, final_states, transition_funct):
        self.states = states
        self.alphabets = alphabets
        self.init_state = init_state
        self.final_states = final_states
        self.transition_funct = transition_funct
        self.regex = ''
        self.ds = {}
        self.transition_dict = {}
        self.set_transition_dict()

    def set_transition_dict(self):
        dict_states = {r: {c: '-' for c in self.states} for r in self.states}
        for i in self.states:
            for j in self.states:
                indices = [ii for ii, v in enumerate(self.transition_funct[i]) if v == j]  # get indices of states
                if len(indices) != 0:
                    dict_states[i][j] = '+'.join([str(self.alphabets[v]) for v in indices])
        self.ds = dict_states
        self.transition_dict = copy.deepcopy(dict_states)

    def get_intermediate_states(self):
        return [state for state in self.states if state not in ([self.init_state] + self.final_states)]

    def get_predecessors(self, state):
        return [key for key, value in self.ds.items() if state in value.keys() and value[state] != 'e' and key != state]

    def get_successors(self, state):
        return [key for key, value in self.ds[state].items() if value != 'e' and key != state]

    def get_if_loop(self, state):
        if self.ds[state][state] != 'e':
            return self.ds[state][state]
        else:
            return ''

    def convertToGTG(self):
        gtg = DFA(self.states, self.alphabets, self.init_state, self.final_states, self.transition_funct)
        
        for k in gtg.states:
            if k != gtg.init_state or k not in gtg.final_states:
                for p in gtg.states:
                    for q in gtg.states:
                        if p != k and q != k:
                            self.ds[p][q] = gtg.getExpressionForElimination(k, p, q)
                gtg.states.popitem(k)
        return gtg

    def getExpressionForElimination(self, k, p, q):
        pq = self.ds[p][q]
        pk = self.ds[p][k]
        kk = self.ds[k][k]
        kq = self.ds[k][q]
        return '(' + '(' + pq + ')' + '+' + '(' + pk + ')' + '(' + kk + ')' + '*' + '(' + kq + ')' + ')' 
    

    def deriveFinalExpression(self):
        i = self.init_state
        f = self.final_states[0]
        i_i = self.ds[i][i]
        i_f = self.ds[i][f]
        f_i = self.ds[f][f]        
        f_f = self.ds[f][i]                
        return '(' + '(' + i_i + ')' + '+' + '(' + i_f + ')' + '(' + f_f + ')' + '*' + '(' + i_i + ')' + '*' + '(' + i_f + ')' + '(' + f_f + ')' + '*' + ')' 
    

