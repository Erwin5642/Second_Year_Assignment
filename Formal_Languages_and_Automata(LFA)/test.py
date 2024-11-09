import copy

class NFA:
    def __init__(self, no_state, states, no_alphabet, alphabets, start,
                 no_final, finals, no_transition, transitions):
        self.no_state = no_state
        self.states = states
        self.no_alphabet = no_alphabet
        self.alphabets = alphabets
         
        # Adding epsilon alphabet to the list
        # and incrementing the alphabet count
        self.alphabets.append('e')
        self.no_alphabet += 1
        self.start = start
        self.no_final = no_final
        self.finals = finals
        self.no_transition = no_transition
        self.transitions = transitions
 
        # Dictionaries to get index of states or alphabets
        self.states_dict = dict()
        for i in range(self.no_state):
            self.states_dict[self.states[i]] = i
        self.alphabets_dict = dict()
        for i in range(self.no_alphabet):
            self.alphabets_dict[self.alphabets[i]] = i
             
        # transition table is of the form
        # [From State + Alphabet pair] -> [Set of To States]
        self.transition_table = dict()
        for i in range(self.no_state):
            for j in range(self.no_alphabet):
                self.transition_table[str(i)+str(j)] = []
        for i in range(self.no_transition):
            self.transition_table[str(self.states_dict[self.transitions[i][0]])
                                  + str(self.alphabets_dict[
                                      self.transitions[i][1]])].append(
                                          self.states_dict[self.transitions[i][2]])
 
    # Method to get input from User
    @classmethod
    def fromUser(cls):
        states = list(input("States : ").split())
        alphabets = list(input("Alphabets : ").split())
        start = input("Start State : ")
        finals = list(input("Final States : ").split())
        transitions = list()
        no_transition = int(input("Insira o numero de transições: "))
        print("Enter Transitions (from alphabet to) (e for epsilon): ")
        no_state = len(states)
        no_alphabet = len(alphabets)
        no_final = len(finals)
        for i in range(no_transition):
            transitions.append(input("-> ").split())
        return cls(no_state, states, no_alphabet, alphabets, start,
                   no_final, finals, no_transition, transitions)
 
    # Method to represent quintuple
    def __repr__(self):
        return "Q : " + str(self.states)+"\nΣ : "
        + str(self.alphabets)+"\nq0 : "
        + str(self.start)+"\nF : "+str(self.finals) + \
            "\nδ : \n" + str(self.transition_table)
 
    def getEpsilonClosure(self, state):
       
        # Method to get Epsilon Closure of a state of NFA
        # Make a dictionary to track if the state has been visited before
        # And a array that will act as a stack to get the state to visit next
        closure = dict()
        closure[self.states_dict[state]] = 0
        closure_stack = [self.states_dict[state]]
 
        # While stack is not empty the loop will run
        while (len(closure_stack) > 0):
           
            # Get the top of stack that will be evaluated now
            cur = closure_stack.pop(0)
             
            # For the epsilon transition of that state,
            # if not present in closure array then add to dict and push to stack
            for x in self.transition_table[
                    str(cur)+str(self.alphabets_dict['e'])]:
                if x not in closure.keys():
                    closure[x] = 0
                    closure_stack.append(x)
            closure[cur] = 1
        return closure.keys()
 
    def getStateName(self, state_list):
       
        # Get name from set of states to display in the final DFA diagram
        name = ''
        for x in state_list:
            name += self.states[x]
        return name
 
    def isFinalDFA(self, state_list):
       
        # Method to check if the set of state is final state in DFA
        # by checking if any of the set is a final state in NFA
        for x in state_list:
            for y in self.finals:
                if (x == self.states_dict[y]):
                    return True
        return False
 
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
        dict_states = {r: {c: 'e' for c in self.states} for r in self.states}
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

    def toregex(self):
        intermediate_states = self.get_intermediate_states()
        dict_states = self.ds

        for inter in intermediate_states:
            predecessors = self.get_predecessors(inter)
            successors = self.get_successors(inter)
            # print('inter : ', inter)
            # print('predecessor : ', predecessors)
            # print('successor : ', successors)

            # dd = {r: {c: 'e' for c in self.states if c != inter} for r in self.states if r != inter}
            ##dd = {r: {c: 'e' for c in temp_states if c != inter} for r in temp_states if r != inter}
            for i in predecessors:
                for j in successors:
                    inter_loop = self.get_if_loop(inter)
                    # print('i : ', i, ' j : ', j)
                    dict_states[i][j] = '+'.join(('(' + dict_states[i][j] + ')', ''.join(('(' + dict_states[i][
                        inter] + ')', '(' + inter_loop + ')' + '*', '(' + dict_states[inter][j] + ')'))))

            dict_states = {r: {c: v for c, v in val.items() if c != inter} for r, val in dict_states.items() if
                           r != inter}  # remove inter node
            self.ds = copy.deepcopy(dict_states)

        init_loop = dict_states[self.init_state][self.init_state]
        init_to_final = dict_states[self.init_state][self.final_states[0]] + '(' + dict_states[self.final_states[0]][
            self.final_states[0]] + ')' + '*'
        final_to_init = dict_states[self.final_states[0]][self.init_state]
        re = '(' + '(' + init_loop + ')' + '+' + '(' + init_to_final + ')' + '(' + final_to_init + ')' + ')' + '*' + '(' + init_to_final + ')'
        #re = '(' + re + ')*'
        #print('Regular Expression : ', re)
        return re

def main():
    states = input('Enter the states in your DFA : ')
    states = states.split()
    alphabets = input('Enter the alphabets : ')
    alphabets = alphabets.split()
    init_state = input('Enter initial state : ')
    final_states = input('Enter the final states : ')
    final_states = final_states.split()
    print('Define the transition function : ')
    transition_matrix = [list(input().split()) for _ in range(len(states))]
    transition_funct = dict(zip(states, transition_matrix))
    # print('transition funct : ', transition_funct)
    r = ''
    for f in final_states:
        #dfa = DFA(states, alphabets, init_state, final_states, transition_funct)
        #regex = dfa.toregex()

        dfa = DFA(states, alphabets, init_state, [f], transition_funct)
        r+= '+' + dfa.toregex()
    #dfa.draw_graph(regex, 'second')
    print(r[1:])
    # print(dfa.transition_dict)
    # print(dfa.ds)

    print(states)
    print(alphabets)
    print(init_state)
    print(final_states)
    print(transition_matrix)
    print(transition_funct)

if __name__ == '__main__':
    main()