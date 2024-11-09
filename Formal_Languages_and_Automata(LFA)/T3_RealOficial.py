# Conversion of epsilon-NFA to DFA and visualization using Graphviz
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
		no_transition = int(input("Number of Transitions : "))
		transitions = list()
		print("Enter Transitions (from alphabet to) (e for epsilon): ")
		for i in range(no_transition):
			transitions.append(input("-> ").split())
		return cls(len(states), states, len(alphabets), alphabets, start,
				len(finals), finals, no_transition, transitions)

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
        dict_states = {r: {c: 'e' for c in self.states} for r in self.states}
        for i in self.states:
            for j in self.states:
                indices = [ii for ii, v in enumerate(self.transition_funct[i]) if v == j]  # get indices of states
                if len(indices) != 0:
                    dict_states[i][j] = '+'.join([str(self.alphabets[v]) for v in indices])
        self.ds = dict_states
        self.transition_dict = copy.deepcopy(dict_states)

    def convertToGTG(self):
        gtg = DFA(self.states, self.alphabets, self.init_state, self.final_states, self.transition_funct)
		
        for k in gtg.states:
            if k != gtg.init_state and k not in gtg.final_states:
                for p in gtg.states:
                    for q in gtg.states:
                        if p != k and q != k:gtg.ds[p][q] = gtg.getExpressionForElimination(k, p, q)
                gtg.states.remove(k)
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
        return '(' + '(' + i_i + ')' + '*' + '(' + i_f + ')' + '(' + f_f + ')' + '*' + '(' + f_i + ')' + ')' + '*' + '(' + i_i + ')' + '*' + '(' + i_f + ')' + '(' + f_f + ')' + '*' 
    

def main():


	# print(repr(nfa)) # To print the quintuple in console
	nfa = NFA.fromUser()
	# Finding epsilon closure beforehand so to not recalculate each time
	epsilon_closure = dict()
	for x in nfa.states:
		epsilon_closure[x] = list(nfa.getEpsilonClosure(x))

	dfa_stack = list()
	dfa_stack.append(epsilon_closure[nfa.start])
	dfa_states = list()
	dfa_finals = list()
	dfa_states.append(epsilon_closure[nfa.start])
	dfa_alphabets = nfa.alphabets
	dfa_alphabets.pop()
	dfa_start = str(epsilon_closure[nfa.start])
	dfa_transition_matrix = list()
	flag = False
	# Check if start state is the final state in DFA
	if (nfa.isFinalDFA(dfa_stack[0])):
		dfa_finals.append(str(dfa_stack[0]))

	# First state of DFA will be epsilon closure of start state of NFA
	# This list will act as stack to maintain till when to evaluate the states
	# List to store the states of DFA

	# Loop will run till this stack is not empty
	while (len(dfa_stack) > 0):
		# Getting top of the stack for current evaluation
		cur_state = dfa_stack.pop(0)
		transition = ""

		# Traversing through all the alphabets for evaluating transitions in DFA
		for al in range((nfa.no_alphabet) - 1):
			# Set to see if the epsilon closure of the set is empty or not
			from_closure = set()
			for x in cur_state:
				# Performing Union up date and adding all the new states in set
				from_closure.update(
				set(nfa.transition_table[str(x)+str(al)]))

				# Check if epsilon closure of the new set is not empty
			if (len(from_closure) > 0):
				# Set for the To state set in DFA
				to_state = set()
				for x in list(from_closure):
					to_state.update(set(epsilon_closure[nfa.states[x]]))

				# Check if the to state already exists in DFA and if not then add it
				if list(to_state) not in dfa_states:
					dfa_stack.append(list(to_state))
					dfa_states.append(list(to_state))

					# Check if this set contains final state of NFA
					# to get if this set will be final state in DFA
					if (nfa.isFinalDFA(list(to_state))):
						dfa_finals.append(str(list(to_state)))


				# Adding edge between from state and to state
				transition = transition + str(list(to_state)) + "#"		
			else:

				# Check if any dead state was present before this
				# if not then make a new dead state ϕ
				
				# Adding -1 to list to mark that dead state is present
				flag = True

				# Adding transition to dead state
				transition = transition + "-#"
		transition = transition[:-1]
		dfa_transition_matrix.append(list(transition.split('#')))


	if flag == True:
		# For new dead state, add all transitions to itself,
		# so that machine cannot leave the dead state
		transition = ""
		for alpha in range(nfa.no_alphabet - 1):
			transition = transition + "-#" 
		transition = transition[:-1]
		dfa_transition_matrix.append(list(transition.split('#')))
		dfa_states.append(list('-'))
	
	for i in range(len(dfa_states)):
		dfa_states[i] = str(dfa_states[i])
	dfa_transition_funct = dict(zip(dfa_states, dfa_transition_matrix))

	dfa = DFA(dfa_states, dfa_alphabets, dfa_start, dfa_finals, dfa_transition_funct)
	
	gtg = dfa.convertToGTG()
	print(gtg.deriveFinalExpression())

	# print(dfa.transition_dict)
    # print(dfa.ds)
	print(dfa_states)
	print(dfa_alphabets)
	print(dfa_start)
	print(dfa_finals)
	print(dfa_transition_funct)

if __name__ == '__main__':
    main()







