#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char *str;
    unsigned n;
}String;

typedef struct{
    String label;
    unsigned short eFinal:1;
    unsigned short eInicial:1;
}State;

typedef struct{
    State from;
    char caracter;
    State to;
}Transtion;

typedef struct{
    Transtion d;
    Transtion *nextTransition;
    Transtion *nextState;
}TransitionTable;

typedef struct{
    char *alfabeto;
    State *estados;    
    State estadoInicial;
    TransitionTable *delta;
    State *estadosFinais;
}Automato;


void clear()
{
#ifdef __unix__
    system("clear");
#else
    system("cls");
#endif
}

// Recebe do usuário uma sequencia de caracteres e armazena ela em str
// Retorna o numero de caracteres armazenados
unsigned readString(String str)
{
    char buffer[30];
    scanf("%s", buffer);
    if(str.str != NULL){
        free(str.str);
    }
    str.str = malloc(str.n = (strlen(buffer) +  1) * sizeof(char));
    // Exclui do buffer de teclado qualquer caracter restante
    while (getchar() != '\n')
        ;
    // Retorna o tamanho da string
    return str.n;
}

// Armazena em uma string dest todos os caracteres de src, exceto pelo char token e espaços
// Retorna o tamanho da string dest
unsigned splitString(String src, String dest, char token)
{
    unsigned i, j;
    // Armazena um caractere de src em dest enquanto houver espaço em dest ou a src não ter terminado
    for (i = 0, j = 0; (j < dest.n) && (src.str[i]) && (src.str[i] != '\n'); i++)
    {
        // Não inclui um caracter de src em dest, toda vez que ele for um espaço ou igual ao token
        if ((src.str[i] != token) && (src.str[i] != ' '))
        {
            // Atribui o caracter em dest e avança
            dest.str[j] = src.str[i];
            j++;
        }
    }
    // Retorna a quantidade de caracteres armazenados em dest
    return j;
}

unsigned splitString(String src, String dest, char token)
{
    unsigned i, j;
    // Armazena um caractere de src em dest enquanto houver espaço em dest ou a src não ter terminado
    for (i = 0, j = 0; (j < dest.n) && (src.str[i]) && (src.str[i] != '\n'); i++)
    {
        // Não inclui um caracter de src em dest, toda vez que ele for um espaço ou igual ao token
        if ((src.str[i] != token) && (src.str[i] != ' '))
        {
            // Atribui o caracter em dest e avança
            dest.str[j] = src.str[i];
            j++;
        }
    }
    // Retorna a quantidade de caracteres armazenados em dest
    return j;
}

// Recebe do usuario um caracter e retorna ele
char readChar()
{
    char c;
    // Enquanto o caracter inserido for inválido, continue lendo do teclado
    while ((c = getchar()) < '!')
        ;
    // Após ler, descarta qualquer outro caracter no buffer de teclado
    while (getchar() != '\n')
        ;
    // Retorna o caracter lido
    return c;
}


int main(){
    Automato AFD, AFND;
    String buffer;
    do
    {
        // Limpa o terminal e cria um novo automato
        clear();
        printf("Criando novo automato\n");

        // Recebe os estados do automato e adequa eles para uso no programa
        printf("Entre com o conjunto de estados, separados por virgulas:\n");
        readString(buffer);
        
        // Recebe o alfabeto do automato e adequa ele para uso no programa
        printf("Entre com os simbolos do alfabeto, separados por virgulas:\n");
        readString(input);
        alfabeto.tamS = split(input, alfabeto, ',');

        // Relaciona temp com o estado inicial para usar a função verificaSubconjunto posteriorement
        relacionaString(&temp, &estadoInicial, 1);
        do
        {
            // Recebe o estado inicial
            printf("Entre com o estado inicial:\n");
            estadoInicial = readChar();

            // Caso o estado inicial não seja um dos anterioremente informados, relata o erro e
            // pede ao usuário um novo estado
            inicialCorreto = verificaSubconjunto(temp, estados);
            if (!inicialCorreto)
            {
                printf("[ERRO] O estado informados é invalido, por favor tente novamente\n");
            }
            // Repete o pedido até que o estado inicial esteja correto
        } while (!inicialCorreto);

        do
        {
            // Recebe os estados finais do automato e adequa eles para uso no programa
            printf("Entre com os estados finais, separados por virgula:\n");
            readString(input);
            estadosFinais.tamS = split(input, estadosFinais, ',');

            // Caso algum dos estados finais não esteja entre os estados anterioremente informados
            // pede ao usuário que informe novamente os estados finais
            finaisCorreto = verificaSubconjunto(estadosFinais, estados);
            if (!finaisCorreto)
            {
                printf("[ERRO] Pelo menos um dos estados informados é invalido, por favor tente novamente\n");
            }
            // Repete o pedido até que os estados finais estejam corretos
        } while (!finaisCorreto);

        // Leitura da tabela que representará a função programa do automato
        printf("Entre com os dados da função de programa:\n");
        for (i = 0; i < estados.tamS; i++)
        {
            for (j = 0; j < alfabeto.tamS; j++)
            {
                // Para cada possibilidade de combinação entre estado e simbolo, pede uma transição
                printf("delta(%c, %c) = ", estados.S[i], alfabeto.S[j]);
                funcaoPrograma[i][j] = readChar();
                relacionaString(&temp, &funcaoPrograma[i][j], 1);
                // Caso a transição informada implique em um estado que não existe no automato
                // Considera ela como um travamento
                if (!verificaSubconjunto(temp, estados))
                {
                    funcaoPrograma[i][j] = '-';
                }
            }
        }

        // Relaciona temp com o próximo estado da transição para usar a função verificaSubconjunto posteriorement
        relacionaString(&temp, &proxq, 1);
        do
        {
            // Recebe a palavra do usuário e salva o tamanho dela
            printf("Entre com a palavra a ser verificada:\n");
            input.tamS = readString(input);
            // Imprime a sequencia de estados para esta palavra
            printf("Sequencia de estados:\n");
            proxq = estadoInicial;
            printf("%c", estadoInicial);
            // Enquanto houver caracteres na palavra ou houver um travamento
            for (k = 0; (k < input.tamS) && (input.S[k]); k++)
            {
                // Proxq recebe o próximo estado do automato
                proxq = transita(proxq, input.S[k], estados, alfabeto, funcaoPrograma);
                // Se o próximo estado não existir resulta em um travamento e para o loop
                if (proxq == TRAVOU)
                {
                    printf(" -> %c\nO automato travou!\n", TRAVOU);
                    k = input.tamS;
                }
                else
                {
                    // Se não, exibe o proximo estado e avança para a próxima iteração
                    printf(" -> %c", proxq);
                }
            }
            // Verifica se o estado resultante das transições está entre os estados finais
            if (verificaSubconjunto(temp, estadosFinais))
            {
                // Se sim a palavra é aceita
                printf("\nPalavra foi aceita\n");
            }
            else
            {
                // Se não, ou se o automato travou, a palavra é rejeitada
                printf("\nPalavra nao foi aceita\n");
            }

            // Segue testando com outras palavras caso o usuário queira
            printf("\nDeseja insesir uma nova palavra?\n[0] - Nao\n[1] - Sim\nDigite uma opção: ");
            scanf("%d", &novaPalavra);
        } while (novaPalavra);

        // Se o usuário não quiser realizar mais testes com um automato, pode criar um novo ou encerrar o programa
        printf("\nDeseja começar um novo automato ou encerrar o programa?\n[0] - Encerrar programa\n[1] - Novo automato\nDigite uma opção: ");
        scanf("%d", &novoAutomato);
        // Segue o programa até que o usuário solicite seu encerramento
    } while (novoAutomato);

    return 0;
}