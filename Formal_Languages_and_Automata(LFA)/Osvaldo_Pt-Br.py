# Conversão de épsilon-NFA para DFA e visualização usando Graphviz
import copy
class NFA:
	def __init__(self, no_state, states, no_alphabet, alphabets, start,
				no_final, finals, no_transition, transitions):
		self.no_state = no_state
		self.states = states
		self.no_alphabet = no_alphabet
		self.alphabets = alphabets
		
		# Adicionando alfabeto épsilon na lista
		# e incrementando a contagem do alfabeto
		self.alphabets.append('e')
		self.no_alphabet += 1
		self.start = start
		self.no_final = no_final
		self.finals = finals
		self.no_transition = no_transition
		self.transitions = transitions
		
		# Dicionários para obter índice de estados ou alfabetos
		self.states_dict = dict()
		for i in range(self.no_state):
			self.states_dict[self.states[i]] = i
		self.alphabets_dict = dict()
		for i in range(self.no_alphabet):
			self.alphabets_dict[self.alphabets[i]] = i
			
		# tabela de transição tem o formato
		# [Par Estado + Alfabeto] -> [Conjunto de Estados Para]
		self.transition_table = dict()
		for i in range(self.no_state):
			for j in range(self.no_alphabet):
				self.transition_table[str(i)+str(j)] = []
		for i in range(self.no_transition):
			self.transition_table[str(self.states_dict[self.transitions[i][0]])
								+ str(self.alphabets_dict[
									self.transitions[i][1]])].append(
										self.states_dict[self.transitions[i][2]])

	# Método para obter entrada do usuário
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

	# Método para representar quíntuplo
	def __repr__(self):
		return "Q : " + str(self.states)+"\nΣ : "
		+ str(self.alphabets)+"\nq0 : "
		+ str(self.start)+"\nF : "+str(self.finals) + \
			"\nδ : \n" + str(self.transition_table)

	def getEpsilonClosure(self, state):
	
		# Método para obter o fechamento Epsilon de um estado de NFA
		# Faça um dicionário para rastrear se o estado já foi visitado antes
		# E um array que atuará como uma pilha para obter o próximo estado a ser visitado
		closure = dict()
		closure[self.states_dict[state]] = 0
		closure_stack = [self.states_dict[state]]

		# Enquanto a pilha não estiver vazia, o loop será executado
		while (len(closure_stack) > 0):
		
			# Obtendo o topo da pilha que será usado
			cur = closure_stack.pop(0)
			
			# Para a transição épsilon desse estado,
			# se não estiver presente na matriz de fechamento, adiciona ao dict e empurra para empilhar
			for x in self.transition_table[
					str(cur)+str(self.alphabets_dict['e'])]:
				if x not in closure.keys():
					closure[x] = 0
					closure_stack.append(x)
			closure[cur] = 1
		return closure.keys()

	def getStateName(self, state_list):
	
		# Obtenha o nome do conjunto de estados para exibir no diagrama final do DFA
		name = ''
		for x in state_list:
			name += self.states[x]
		return name

	def isFinalDFA(self, state_list):
	
		# Método para verificar se o conjunto de estado é o estado final no DFA
		# verificando se algum do conjunto é um estado final no NFA
		for x in state_list:
			for y in self.finals:
				if (x == self.states_dict[y]):
					return True
		return False

# nfa = NFA.fromUser() 
# Para obter a entrada do usuário
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
                indices = [ii for ii, v in enumerate(self.transition_funct[i]) if v == j]  # obter índices de estados
                if len(indices) != 0:
                    dict_states[i][j] = '+'.join([str(self.alphabets[v]) for v in indices])
        self.ds = dict_states
        self.transition_dict = copy.deepcopy(dict_states)

    def convertToGTG(self):
        gtg = DFA(self.states, self.alphabets, self.init_state, self.final_states, self.transition_funct)
        
        if len(gtg.final_states) > 1:
            gtg.states.append('FinalGTG')
            gtg.ds['FinalGTG'] = {}
            for s in gtg.states:
                gtg.ds['FinalGTG'][s] = '-'
                gtg.ds[s]['FinalGTG'] = '-'
            for f in gtg.final_states:
                gtg.ds[f]['FinalGTG'] = 'e'
            gtg.final_states.clear()
            gtg.final_states.append('FinalGTG')
        
        if "['-']" in gtg.states:
            gtg.states.remove("['-']")
            for k in gtg.states:
                gtg.ds[k].pop("['-']")
            gtg.ds.pop("['-']")

        
        while len(gtg.states) > 2:
            i = gtg.states[1]
            if i != gtg.init_state and i not in gtg.final_states:
                for j in gtg.states:
                    if j != i:
                        for k in gtg.states:
                            if j != i and k != i:
                                gtg.ds[j][k] = gtg.getExpressionForElimination(i, j, k)
                        gtg.ds[j].pop(i)
                gtg.states.remove(i)
                gtg.ds.pop(i)
                    
        return gtg

    def getExpressionForElimination(self, i, j, k):
        wji = self.ds[j][i]
        wik = self.ds[i][k]
        wii = self.ds[i][i]
        wjk = self.ds[j][k]
        re = wjk
        if wji != '-' and wik != '-':
            if wii == '-':
                re = wji + wik
            else:
                re = wji + '(' + wii + ')*' + wik 
            if wjk != '-':
                re = re + '+' + wjk
        return re
    
    def deriveFinalExpression(self):
        c = self.init_state
        f = self.final_states[0]
        cc = self.ds[c][c]
        cf = self.ds[c][f]
        fc = self.ds[f][c]        
        ff = self.ds[f][f]                
        re = ''
        if cc != '-':
            re = '(' + cc + ')*'
        re = re + '(' + cf + ')'
        if ff != '-' or fc != '-':
            re = re + '('
            if ff != '-':
                re = re + '(' + ff + ')'
            if fc != '-':
                if ff != '-':
                    re = re + '+'
                re = re + '(' + fc + ')'
                if cc != '-':
                    re = re + '(' + cc + ')*'
                re = re + '(' + cf + ')' 
            re = re + ')*'
        return re
    

def main():


	# print(repr(nfa)) # Para imprimir o quíntuplo no console
	nfa = NFA.fromUser()
	# Encontrar o fechamento épsilon de antemão para não recalcular todas as vezes
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
	# Verifique se o estado inicial é o estado final no DFA
	if (nfa.isFinalDFA(dfa_stack[0])):
		dfa_finals.append(str(dfa_stack[0]))

	# O primeiro estado do DFA será o fechamento épsilon do estado inicial do NFA
	# Esta lista funcionará como uma pilha para manter até quando avaliar os estados
	# Lista para armazenar os estados do DFA

	# O loop será executado até que esta pilha não esteja vazia
	while (len(dfa_stack) > 0):
		# Chegando ao topo da pilha para avaliação atual
		cur_state = dfa_stack.pop(0)
		transition = ""

		# Percorrendo todos os alfabetos para avaliar transições no DFA
		for al in range((nfa.no_alphabet) - 1):
			# Defina para ver se o fechamento épsilon do conjunto está vazio ou não
			from_closure = set()
			for x in cur_state:
				# Executando a atualização da União e adicionando todos os novos estados no conjunto
				from_closure.update(
				set(nfa.transition_table[str(x)+str(al)]))

				# Verifica se o fechamento épsilon do novo conjunto não está vazio
			if (len(from_closure) > 0):
				# Definindo para o estado definido no DFA
				to_state = set()
				for x in list(from_closure):
					to_state.update(set(epsilon_closure[nfa.states[x]]))

				# Verifica se o estado já existe no DFA e se não, adicione-o
				if list(to_state) not in dfa_states:
					dfa_stack.append(list(to_state))
					dfa_states.append(list(to_state))

					# Verifica se este conjunto contém o estado final do NFA
					# para saber se este conjunto será o estado final no DFA
					if (nfa.isFinalDFA(list(to_state))):
						dfa_finals.append(str(list(to_state)))


				# Adicionando borda entre os estados
				transition = transition + str(list(to_state)) + "#"		
			else:

				# Verifica se algum estado morto estava presente antes disso
				# se não, então crie um novo estado morto ϕ
				
				# Adicionando -1 à lista para marcar que o estado morto está presente
				flag = True

				# Adicionando transição para estado morto
				transition = transition + "-#"
		transition = transition[:-1]
		dfa_transition_matrix.append(list(transition.split('#')))


	if flag == True:
		# Para um novo estado morto, adiciona todas as transições a si mesmo,
		# para que a máquina não possa sair do estado morto
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

if __name__ == '__main__':
    main()