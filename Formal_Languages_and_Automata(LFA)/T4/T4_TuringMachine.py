from time import sleep
import json

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
        tape.insert(0, '<')
        tape.append('_')
        tape.append('_')
        currentState = self.initialState
        headPosition = 1
        print(f"Estado atual: \033[36m{currentState}\033[0m Fita:" , end="")
        print(f"\033[34m{tape[:1]}\033[0m", end="")
        print(f"\033[32m{tape[1:2]}\033[0m", end="")
        print(f"\033[34m{tape[2:]}\033[0m", end="")
        sleep(0.3)

        # A máquina funciona até que um estado de aceitação ou rejeição seja encontrado
        while True:
            if currentState == self.acceptState:
                return "Aceito"
            if currentState == self.rejectState:
                return "Rejeitado"

            # Le o simbolo na cabeça de entrada
            headSymbol = tape[headPosition]

            # Se não existe nenhuma transição dada o símbolo da fita e o estado atual
            # Assumimos que a string é rejeitada
            transition = (currentState, headSymbol)
            if transition not in self.deltaFunction:
                return "Rejeitado"
            newState, writeSymbol, direction = self.deltaFunction[transition]
            # Escrevemos o símbolo na fita de acordo com a transição
            if writeSymbol is not None:
                tape[headPosition] = writeSymbol
            print(f"\rEstado atual: \033[36m{currentState}\033[0m Fita:", end="")
            print(f"\033[34m{tape[:headPosition]}\033[0m", end="")
            print(f"\033[32m{tape[headPosition:headPosition + 1]}\033[0m", end="")
            print(f"\033[34m{tape[headPosition + 1:]}\033[0m", end="")
            sleep(0.3)

            # Movemos a cabeça para a direção indicada
            if direction == 'E':
                headPosition -= 1
            elif direction == 'D':
                headPosition += 1
            currentState = newState

            # Se as operações excederam a nossa fita finita, estendemos ela
            # para que ela se virtualmente infinita
            if headPosition < 0:
                print("Erro: Limite esquerdo da fita excedido!")
                return "Rejeitado"
            if headPosition + 1 >= len(tape):
                tape.append('_')
            print(f"\rEstado atual: \033[36m{currentState}\033[0m Fita:", end="")
            print(f"\033[34m{tape[:headPosition]}\033[0m", end="")
            print(f"\033[32m{tape[headPosition:headPosition+1]}\033[0m", end="")
            print(f"\033[34m{tape[headPosition+1:]}\033[0m", end="")
            sleep(0.3)


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
                        deltaFunction[(state, symbol)] = (rejectState, symbol, 'D')
                        break

                    newState, writeSymbol, direction = transitionInput.split(',')
                    erro = ""
                    # Valida cada parte do input
                    if newState == "" or newState not in states:
                        erro = "Estado inválido!"
                    elif writeSymbol == "" or writeSymbol not in tapeAlphabet:
                        erro = "Simbolo de fita inválido!"
                    elif direction == "" or direction not in ['E', 'D']:
                        if direction == 'e':
                            direction = 'E'
                        elif direction == 'd':
                            direction = 'D'
                        else:
                            erro = "Direção deve ser 'E' ou 'D'"

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
            rejectState = "n" + acceptState
            states.append(rejectState)
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
        print("Simbolos branco '_' e delimitador da fita a esquerda '<' inseridos automaticamente.")
        tapeAlphabet = input("Insira o restante dos símbolos de fita (ex: 0,1,2,x,y): ").split(',')
        tapeAlphabet = [symbol.strip() for symbol in tapeAlphabet]
        if tapeAlphabet == [""]:
            print("Nenhum símbolo de fita foi informado!")
        else:
            if '_' not in tapeAlphabet:
                tapeAlphabet.append('_')
            if '<' not in tapeAlphabet:
                tapeAlphabet.append('<')
            break

    # Função de transição
    deltaFunction = readTransitions(states, acceptState, rejectState, tapeAlphabet)

    # Junta tudo para criar a máquina de Turing
    tm = TuringMachine(states, initialState, acceptState, rejectState, alphabet, tapeAlphabet, deltaFunction)

    return tm

def saveTuringMachine(tm, filename):
    data = {
        "states": tm.states,
        "initialState": tm.initialState,
        "acceptState": tm.acceptState,
        "rejectState": tm.rejectState,
        "alphabet": tm.alphabet,
        "tapeAlphabet": tm.tapeAlphabet,
        "deltaFunction": {str(k): v for k, v in tm.deltaFunction.items()},
    }
    if not filename.endswith(".json"):
        filename += ".json"
    with open(filename, "w") as file:
        json.dump(data, file, indent=4)
    print(f"Máquina de Turing salva em '{filename}'.")

def loadTuringMachine(filename):
    if not filename.endswith(".json"):
        filename += ".json"
    with open(filename, "r") as file:
        data = json.load(file)
    deltaFunction = {eval(k): tuple(v) for k, v in data["deltaFunction"].items()}
    return TuringMachine(
        data["states"],
        data["initialState"],
        data["acceptState"],
        data["rejectState"],
        data["alphabet"],
        data["tapeAlphabet"],
        deltaFunction,
    )

def displayMenu():
    print("\nMenu:")
    print("1. Simule")
    print("2. Construa uma nova Máquina de Turing")
    print("3. Salvar Máquina de Turing")
    print("4. Carregar Máquina de Turing")
    print("5. Encerrar programa")

def main():
    tm = None

    while True:
        displayMenu()
        choice = input("Escolha uma opção: ").strip()

        if choice == '1':
            if tm is None:
                print("Nenhuma Máquina foi criada ou carregada ainda! Insira uma no programa antes de simular.")
            else:
                inputString = input("Insira uma string de entrada: ").strip()
                result = tm.run(inputString)
                print(f"\nResultado: {result}")
        elif choice == '2':
            print("Criando Maquina de Turing")
            tm = createTuringMachine()
        elif choice == '3':
            filename = input("Insira o nome do arquivo para salvar (ex: Tm_Exemplo): ").strip()
            saveTuringMachine(tm, filename)
        elif choice == '4':
            filename = input("Insira o nome do arquivo para carregar (ex: Tm_Exemplo): ").strip()
            try:
                tm = loadTuringMachine(filename)
                print(f"Máquina de Turing carregada de '{filename}'.")
            except FileNotFoundError:
                print(f"Erro: Arquivo '{filename}' não encontrado.")
        elif choice == '5':
            print("Encerrando programa...")
            break
        else:
            print("Escolha inválida. Tente novamente.")

if __name__ == "__main__":
    main()
