class TuringMachine:
    def __init__(self, states, initialState, acceptState, rejectState, alphabet, tapeAlphabet, deltaFunction):
        self.states = states
        self.initialState = initialState
        self.acceptState = acceptState
        self.rejectState = rejectState
        self.alphabet = alphabet
        self.tapeAlphabet = tapeAlphabet
        self.deltaFunction = deltaFunction

    def run(self, inputString):
        # Inicializa a fita com os símbolos do string de entrada
        tape = list(inputString)

        currentState = self.initialState
        headPosition = 0

        # A máquina funciona até que um estado de aceitação ou rejeição seja encontrado
        while True:
            if currentState == self.acceptState:
                print(f"Fita: {tape}")
                return "Accepted"
            if currentState == self.rejectState:
                print(f"Fita: {tape}")
                return "Rejected"

            # Se as operações excederam a nossa fita finita, estendemos ela
            # para que ela se virtualmente infinita
            if headPosition < 0:
                tape.insert(0, '_')
                headPosition = 0
            if headPosition >= len(tape):
                tape.append('_')

            # Le o simbolo na cabeça de entrada
            headSymbol = tape[headPosition]

            # Se não existe nenhuma transição dada o símbolo da fita e o estado atual
            # Assumimos que a string é rejeitada
            transition = (currentState, headSymbol)
            if transition not in self.deltaFunction:
                print(f"Fita: {tape}")
                return "Rejected"

            newState, writeSymbol, direction = self.deltaFunction[transition]
            # Escrevemos o símbolo na fita de acordo com a transição
            tape[headPosition] = writeSymbol
            # Movemos a cabeça para a direção indicada
            if direction == 'L':
                headPosition -= 1
            elif direction == 'R':
                headPosition += 1

            currentState = newState


def readTransitions(states, acceptState, rejectState, tapeAlphabet):
    deltaFunction = {}

    print("Defina a função de transição:")
    for state in states:
        if state != acceptState and state != rejectState:
            for symbol in tapeAlphabet:
                while True:
                    transitionInput = input(f"({state}, {symbol}) [formato: novoEstado,novoSimbolo,direção]: ")
                    if not transitionInput:
                        # Define a transição para o estado de rejeição caso nada seja informado
                        deltaFunction[(state, symbol)] = (None, None, None)
                        break

                    newState, writeSymbol, direction = transitionInput.split(',')
                    erro = ""
                    # Valida cada parte do input
                    if newState == "" or newState not in states:
                        erro = "Estado inválido!"
                    elif writeSymbol == "" or writeSymbol not in tapeAlphabet:
                        erro = "Simbolo de fita inválido!"
                    elif direction == "" or direction not in ['L', 'R']:
                        erro = "Direção deve ser 'L' ou 'R'"

                    # Se passar por todos os testes a transição é dita como válida
                    if erro == "":
                        deltaFunction[(state, symbol)] = (newState, writeSymbol, direction)
                        break
                    else:
                        print(f"Entrada inválida para transição ({state}, {symbol}): {erro}. Tente novamente.")

    return deltaFunction


def createTuringMachine():
    print("Entre com a definição da Máquina de Turing:")

    # Estados

    while True:
        states = input("Insira os estados(ex: a,b,c,d): ").split(',')
        states = [state.strip() for state in states]
        if states == [""]:
            print("Erro: Nenhum estados foi informado! Tente novamente.")
        else:
            break

    # Estado inicial
    while True:
            initialState = input("Insira o estado inicial: ").strip()
            erro = ""
            if initialState == "":
                erro = "Nenhum estado inicial foi informado!"
            elif initialState not in states:
                erro = "Estado inicial não corresponde a nenhum estado declarado anteriormente!"
            if erro == "":
                break
            else:
                print(f"Erro: {erro} Tente novamente.")


    # Estado de aceitação
    while True:
        acceptState = input("Insira o estado de aceitação: ").strip()
        erro = ""
        if acceptState == "":
            erro = "Nenhum estado de aceitação foi informado!"

        elif acceptState not in states:
            erro = "Estado de aceitação não corresponde a nenhum estado declarado anteriormente!"
        if erro == "":
            break
        else:
            print(f"Erro: {erro} Tente novamente.")

    # Estado de rejeição
    while True:
        rejectState = input("Insira estado de rejeição: ").strip()
        erro = ""
        if rejectState == "":
            erro = "Nenhum estado de rejeição foi informado!"
        elif rejectState not in states:
            erro = "Estado não corresponde a nenhum estado declarado anteriormente!"
        if erro == "":
            break
        else:
            print(f"Erro: {erro} Tente novamente.")

    # Alfabeto
    while True:
        alphabet = input("Insira o alfabeto (ex: 0,1,2): ").split(',')
        alphabet = [symbol.strip() for symbol in alphabet]
        if alphabet == [""]:
            print("Nenhum símbolo do alfabeto foi informado!")
        else:
            break

    # Símbolos de fita
    while True:
        tapeAlphabet = input("Insira os símbolos de fita (ex: 0,1,2,x,y,_): ").split(',')
        tapeAlphabet = [symbol.strip() for symbol in tapeAlphabet]
        if tapeAlphabet == [""]:
            print("Nenhum símbolo de fita foi informado!")
        else:
            break

    # Função de transição
    deltaFunction = readTransitions(states, acceptState, rejectState, tapeAlphabet)

    # Create Turing Machine object
    tm = TuringMachine(states, initialState, acceptState, rejectState, alphabet, tapeAlphabet, deltaFunction)

    return tm


def displayMenu():
    print("\nMenu:")
    print("1. Simule")
    print("2. Construa uma nova Maquina de Turing")
    print("3. Encerrar programa")


def main():
    tm = None

    while True:
        if tm is None:
            print("\nConstruindo uma Maquina de Turing.")
            tm = createTuringMachine()

        displayMenu()
        choice = input("Escolha uma opção: ").strip()

        if choice == '1':
            inputString = input("Insira uma string de entrada: ").strip()
            result = tm.run(inputString)
            print(f"Resultado: {result}")
        elif choice == '2':
            tm = createTuringMachine()  # Criando uma nova Máquina de Turing
        elif choice == '3':
            print("Encerrando programa...")
            break
        else:
            print("Escolha inválida. Tente novamente.")


if __name__ == "__main__":
    main()
