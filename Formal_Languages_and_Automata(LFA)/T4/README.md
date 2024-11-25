# Instruções de Uso para o Programa de Simulação de Máquina de Turing

O programa simula uma Máquina de Turing, permitindo criar, salvar, carregar e simular uma máquina de Turing. 
Além do código, também foi incluído um exemplo de máquina que reconhece a linguagem dos palíndromos formados pelo alfabto a,b,c. Essa máquina está salva no arquivo isPal.json.

## Requisitos

- Python 3.x
- Bibliotecas padrão de Python: `time` e `json`

## Como rodar
Insira no terminal o comando:
py T4_TuringMachine.py

## Funcionalidades

1. **Simulação da Máquina de Turing**
   - Permite simular uma máquina de Turing com uma string de entrada fornecida pelo usuário.
   
2. **Criação de uma nova Máquina de Turing**
   - O usuário pode definir todos os elementos da 7-tupla que compõe uma máquina de Turing (estados, alfabeto, alfabeto de fita, função de transição, estado de aceitação e  estado de rejeição).

3. **Mostrar Maquina de Turing**
   - Mostra as informações sobre a Máquina de Turing carregada no programa.

4. **Salvar a Máquina de Turing**
   - Salve as definições de uma máquina de Turing para um arquivo JSON. A formatação segue o padrão do arquivo isPal.json enviado em anexo.

5. **Carregar a Máquina de Turing**
   - Carregue uma máquina salva a partir de um arquivo JSON. A formatação segue o padrão do arquivo isPal.json enviado em anexo.

6. **Configuração da Simulação**
   - O usuário pode configurar como as transições da máquina são exibidas (linha a linha ou em uma linha única) e ajustar o tempo de espera entre as transições.

## Exemplo de Uso
Carregue o arquivo isPal.json no programa (opção 5) e mostre os dados da Máquina carregada (opção 3), em seguida, realize uma simulação (opção 1).

## Observações
- **Estado de Rejeição**: Se nenhum estado de rejeição for informado é criado por padrão um estado de rejeição concatenando "n" com o estado de aceitação.
- **Transição Vazia**: Se uma transição vazia for encontrada ou inserida, a máquina irá para o estado de rejeição automaticamente.