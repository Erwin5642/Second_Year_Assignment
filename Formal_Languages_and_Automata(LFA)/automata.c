/*
    ###Trabalho de LFA: automato finito deterministico###
    23/05/2024
    João Vitor Antunes da Silva
    48935
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void clear()
{
#ifdef __unix__
    system("clear");
#else
    system("cls");
#endif
}

// Constantes para teste
const char TRAVOU = '-';
const short NOT_ELEM = -1;

// Constantes de limite para os vetores
const unsigned MAX_INPUT = 30;
const unsigned MAX_SET = 10;

// Estrutura para armazenar strings e manter o tamanho delas
typedef struct
{
    // Campo S aponta para uma variavel vetor char
    char *S;
    unsigned tamS;
} string;

// Associa uma estrutura string dest com um vetor char src e seu tamanho limite
void relacionaString(string *dest, char *src, unsigned tam)
{
    dest->S = src;
    dest->tamS = tam;
}

// Recebe do usuário uma sequencia de caracteres e armazena ela em str
// Retorna o numero de caracteres armazenados
unsigned readString(string str)
{
    scanf("%s", str.S);
    // Exclui do buffer de teclado qualquer caracter restante
    while (getchar() != '\n')
        ;
    // Retorna o tamanho da string
    return strlen(str.S);
}

// Armazena em uma string dest todos os caracteres de src, exceto pelo char token e espaços
// Retorna o tamanho da string dest
unsigned split(string src, string dest, char token)
{
    unsigned i, j;
    // Armazena um caractere de src em dest enquanto houver espaço em dest ou a src não ter terminado
    for (i = 0, j = 0; (j < dest.tamS) && (src.S[i]) && (src.S[i] != '\n'); i++)
    {
        // Não inclui um caracter de src em dest, toda vez que ele for um espaço ou igual ao token
        if ((src.S[i] != token) && (src.S[i] != ' '))
        {
            // Atribui o caracter em dest e avança
            dest.S[j] = src.S[i];
            j++;
        }
    }
    // Retorna a quantidade de caracteres armazenados em dest
    return j;
}

// Retorna o indice em que um char elem aparace na string set
int indexElem(string set, char elem)
{
    unsigned i;
    // Enquanto não ultrapassar o tamahno da string set, procura pelo elem
    for (i = 0; i < set.tamS; i++)
    {
        // Caso encontre retorne o indice onde o encontrou
        if (set.S[i] == elem)
        {
            return i;
        }
    }
    // Se não encontra retorne a flag que indica que elem não está em set
    return NOT_ELEM;
}

// Simula a função programa do automato
/*Dado um estado q e um caracter c, determina qual o proximo estado
a partir da string com os estados, do alfabeto, e da tabela D com todas as possiveis transições*/
// Retorna o próximo estado
char transita(char q, char c, string Q, string A, char D[MAX_SET][MAX_SET])
{
    int i, j;
    // Se o estado passado para a função ou o alfabeto não estiverem no conjunto dos estados
    // ou dos alfabetos, então a transição não existe e o automato trava
    if (((i = indexElem(Q, q)) != NOT_ELEM) && ((j = indexElem(A, c)) != NOT_ELEM))
    {
        // Se existe, utiliza o indice de onde o estado e o simbolo do alfabeto esta para
        // acessar o novo estado na tabela
        return D[i][j];
    }
    // Retorna a flag que indica que o automato travou, caso os parametros foram invalidos ou
    // caso uma transição não levava a nenhum estado
    return TRAVOU;
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

// Indica se um subconjunto de caracteres está presente em um outro conjunto
// Retorna verdadeiro(1) ou falso(0) de acordo
int verificaSubconjunto(string subset, string set)
{
    unsigned i;
    // Enquanto houverem caracteres para serem verificados no subconjunto
    for (i = 0; i < subset.tamS; i++)
    {
        // Se qualquer elemento do subconjunto não for um elemento do conjunto, retorna falso
        if (indexElem(set, subset.S[i]) == NOT_ELEM)
        {
            return 0;
        }
    }
    // Caso nunca encontre um elemento fora, retorna verdadeiro
    return 1;
}

// Programa principal
int main()
{
    // Espaços para armazenar os dados do automato
    char w[MAX_INPUT], a[MAX_SET], q[MAX_SET], estadoInicial, f[MAX_SET], funcaoPrograma[MAX_SET][MAX_SET], proxq;
    // Estruturas para facilitar o trabalho com strings
    string input, alfabeto, estados, estadosFinais, temp;
    // Flags para navegar pelo programa
    int novoAutomato, novaPalavra, finaisCorreto, inicialCorreto;
    // Variaveis contadoras
    unsigned i, j, k;

    do
    {
        // Limpa o terminal e cria um novo automato
        clear();
        printf("Criando novo automato\n");
        // Associa cada uma das estruturas de string com seu respectivo vetor de char
        relacionaString(&input, w, MAX_INPUT);
        relacionaString(&alfabeto, a, MAX_SET);
        relacionaString(&estados, q, MAX_SET);
        relacionaString(&estadosFinais, f, MAX_SET);

        // Recebe os estados do automato e adequa eles para uso no programa
        printf("Entre com o conjunto de estados, separados por virgulas:\n");
        readString(input);
        estados.tamS = split(input, estados, ',');

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
