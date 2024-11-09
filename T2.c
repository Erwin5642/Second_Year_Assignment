//Trabalho 2 de AED-II
//Nome: João Vitor Antunes da Silva         RGM:48935

#include <stdlib.h>
#include <stdio.h>
#include "gfx.h"

//Constantes para regular a visualização das listas
#define g_NODE_LENGTH 50
#define g_NODE_HEIGHT 50
#define g_SCREEN_LENGTH 800
#define g_SCREEN_HEIGHT 600
#define g_X_LIST_ORIGIN 30
#define g_Y_LIST_ORIGIN 30
#define g_NODES_DISTANCE 25
#define g_SECTION_SIZE 110

// Declaração do tipo Lista Ordenada Simplesmente Encadeada e Lista Circular Ordenada Simplesmente Encadeada
typedef struct LinkedList
{
    int value;
    struct LinkedList *next;
} LinkedList;

// ### Operações na Lista Encadeada Simples ###
// Inicializa a lista como vazia
void createLinkedList(LinkedList **L)
{
    *L = NULL;
}

// Desaloca os nós da Lista Encadeada Simples
void deleteLinkedList(LinkedList **L)
{
    LinkedList *aux;
    // Garante que o ponteiro apontava para algo
    if (L != NULL)
    {
        // Enquanto não chegar no fim da lista, salva o próximo nó à *L em aux, desaloca *L
        // e reconecta *L com o restante da lista em aux 
        while (*L != NULL)
        {
            aux = (*L)->next;
            free(*L);
            *L = aux;
        }
    }
}

// Insere um valor numa Lista Encadeada Simples de forma Ordenada
void insertValueOrderedLinkedList(LinkedList **L, int newValue)
{
    LinkedList *newNode;
    // Garante que o ponteiro apontava para algo
    if (L != NULL)
    {
        //Enquanto não estiver no fim da lista e o novo valor for maior que os valores de cada nó
        //avança para o próximo ponteiro na lista
        while ((*L != NULL) && ((*L)->value < newValue))
        {
            L = &((*L)->next);
        }
        //Ao chegar no fim da lista, ou encontrar um número maior que o novo valor, insere um novo nó
        //após o nó apontado pelo ponteiro *L 
        newNode = malloc(sizeof(LinkedList));
        newNode->value = newValue;
        newNode->next = *L;
        *L = newNode;
    }
}

/* Remove a primeira ocorrência de um valor na lista
    Retornando 0 se o valor não estava na lista e 1 caso contrário*/
int removeValueOrderedLinkedList(LinkedList **L, int rValue)
{
    LinkedList *aux;
    //Garante que o ponteiro apontava para algo
    if (L != NULL)
    {
        //Enquanto não chegar ao fim da lista ou houverem valores maiores que o valor a ser removido
        while ((*L != NULL) && ((*L)->value <= rValue))
        {
            //Se o valor no nó apontado por *L for igual ao valor a ser removido, desaloca ele
            if ((*L)->value == rValue)
            {
                aux = (*L)->next;
                free(*L);
                *L = aux;
                return 1;
            }
            //Se não, avança para o próximo ponteiro na lista
            L = &(*L)->next;
        }
    }
    //Retorna zero se o elemento não estava na lista
    return 0;
}

/*Busca um valor numa lista encadeada, retornando o endereço do nó com o valor desejado
ou NULL caso o valor não esteja na lista*/
LinkedList *searchValueOrderedLinkedList(LinkedList *L, int sValue)
{
    //Enquanto não chegar ao fim da lista ou houverem valores maiores que o valor a ser removido
    while ((L != NULL) && (L->value <= sValue))
    {
        //Se o valor no nó apontado por L for igual ao valor procurado, retorna o endereço desse nó
        if (L->value == sValue)
        {
            return L;
        }
        //Se não, avança o ponteiro L para o próximo nó na lista
        L = L->next;
    }
    //Se o valor não estiver na lista, retorna NULL
    return NULL;
}

//Mesma estrutrura para lista simplesmente encadeada e lista circular
//Exceto que o ponteiro para lista circular aponta para o último elemento
//Essa escolha foi feita pra garantir inserção no inicio e fim da lista com complexidade O(1)

// ### Lista Circular Ordenada Simplesmente Encadeada ###

//Mesma função que cria lista simples também cria lista circular

//Desaloca todos os nós da Lista Circular Simplesmente Encadeada
void deleteCircularLinkedList(LinkedList **CL)
{
    LinkedList *aux;
    //Se a lista for vazia, não faz nada
    if ((CL != NULL) && (*CL != NULL))
    {
        //Desmembra o último elemento da lista do primeiro elemento e atualiza o pointeiro para o 
        //primeiro elemento, de forma que o ultimo nó aponte para NULL,
        //resultando em uma lista não circular
        aux = (*CL)->next;
        (*CL)->next = NULL;
        *CL = aux;
        //Desaloca os nós da lista circular no mesmo processo que da lista não circular
        while (*CL != NULL)
        {
            aux = (*CL)->next;
            free(*CL);
            *CL = aux;
        }
    }
}

// Insere um valor numa Lista Circular Simplesmente Encadeada de forma Ordenada
void insertValueOrderedCircularLinkedList(LinkedList **CL, int newValue)
{
    LinkedList *newNode;
    //Garante que o ponteiro apontava para algo
    if (CL != NULL)
    {
        //Cria um novo nó
        newNode = malloc(sizeof(LinkedList));
        newNode->value = newValue;

        //Se a lista não estava vazia
        if (*CL != NULL)
        {
            //Se o novo valor for menor que o último e maior que o primeiro, insere ele na sua correespondente
            //posição no meio da lista
            if (((*CL)->value > newValue) && ((*CL)->next->value < newValue))
            {      
                //Pula o último e primeiro nó da lista, avançando para ponteiro no primeiro nó da lista
                CL = &((*CL)->next->next);
                //Procura pelo primeiro nó com o valor menor que o novo valor
                while ((*CL)->value < newValue)
                {
                    CL = &((*CL)->next);
                }
                //Insere o novo nó no ponteiro que aponta para o primeiro nó menor que o novo valor
                newNode->next = (*CL);
                *CL = newNode;
            }
            else
            {
                //Se não, insere no fim/inicio da lista
                newNode->next = (*CL)->next;
                (*CL)->next = newNode;
                //Se o valor for maior ou igual que o maior elemento da lista, atualiza o ponteiro da lista 
                //para o novo valor maior
                if ((*CL)->value <= newValue)
                {
                    *CL = newNode;
                }
            }
        }
        else
        {
            //Se não, insere o novo nó no inicio e faz ele apontar para si mesmo
            newNode->next = newNode;
            *CL = newNode;
        }
    }
}

/*Remove uma ocorrência de um valor na lista circular
Retornando 0 caso o elemento não estava na lista ou 1 caso contrário*/
int removeValueOrderedCircularLinkedList(LinkedList **CL, int rValue)
{
    LinkedList *end, *aux;
    //Se a houver lista e ela não for vazia
    if ((CL != NULL) && (*CL != NULL))
    {
        // Se a lista possuir apenas um nó
        if ((*CL)->next == *CL)
        {
            // E esse nó possuir o elemento a ser removido
            if ((*CL)->value == rValue)
            {
                //Desaloca o nó e torna a lista vazia
                free(*CL);
                *CL = NULL;
                return 1;
            }
            //Se não for o elemento procurado somente retorna 0
            return 0;
        }
        //Se o valor a ser removido for o último elemento
        if ((*CL)->value == rValue)
        {
            // Avança com a ponteiro "raiz" da lista até encontrar o nó que será o novo fim
            end = *CL;
            while ((*CL)->next != end)
            {
                *CL = (*CL)->next;
            }
            //Remove o último elemento e faz reconecta a lista
            (*CL)->next = end->next;
            free(end);
            return 1;
        }
        //Se o valor a ser removido estiver entre o maior e o menor valor da lista
        if (((*CL)->value > rValue) && ((*CL)->next->value <= rValue))
        {
            //Pula o ponteiro para o último nó
            CL = &(*CL)->next;
            //Enquanto for possível que o valor esteja na lista ou até que ele seja encontrado
            //avança para o ponteiro no próximo nó
            while ((*CL)->value < rValue)
            {
                CL = &(*CL)->next;
            }
            //Se o valor a ser removido foi encontrado, desaloca ele e reconecta a lista
            if ((*CL)->value == rValue)
            {
                aux = (*CL)->next;
                free(*CL);
                (*CL) = aux;
                return 1;
            }
        }
    }
    //Se o valor não estava na lista retorna 0
    return 0;
}

/*Busca um valor numa lista circular, retornando o endereço do nó com o valor desejado
ou NULL caso o valor não esteja na lista*/
LinkedList *searchValueCircularLinkedList(LinkedList *CL, int sValue)
{
    LinkedList *end = CL;
    //Se a lista não for vazia
    if (CL != NULL)
    {
        //Se o valor estar no intervalo entre o maior e o menor valor da lista
        if ((CL->value >= sValue) && (CL->next->value <= sValue))
        {
            //Verifica se o valor no nó é o valor buscado ate que ele seja encontrado ou chegue o fim da lista
            do
            {
                if (CL->value == sValue)
                {
                    return CL;
                }
                CL = CL->next;
            } while (CL != end);
        }
    }
    //Se o valor não estava na lista retorna NULL
    return NULL;
}

// ### Lista duplamente encadeada
typedef struct DoublyLinkedList
{
    int value;
    struct DoublyLinkedList *prev;
    struct DoublyLinkedList *next;
} DoublyLinkedList;

//### Operações com lista duplamente encadeada com nó cabeça

//Ocupa um nó cabeça como uma lista vazia
void createDoublyLinkedList(DoublyLinkedList *DL)
{
    //Se foi passado um nó cabeça
    if (DL != NULL)
    {
        DL->next = NULL;
        DL->prev = NULL;
    }
}

//Desaloca os nós da lista apontada pelo nós cabeça
void deleteDoublyLinkedList(DoublyLinkedList *DL)
{
    DoublyLinkedList *aux;
    //Garante que o ponteiro aponta para algo
    if (DL != NULL)
    {
        //Enquanto não chegar ao fim da lista, desaloca cada nó e reconecta o próximo com a lista
        while (DL->next != NULL)
        {
            aux = DL->next->next;
            free(DL->next);
            DL->next = aux;
        }
    }
}

// Insere um valor numa Lista Circular Simplesmente Encadeada de forma Ordenada
void insertValueDoublyLinkedList(DoublyLinkedList *DL, int newValue)
{
    DoublyLinkedList *newNode;
    //Garantindo que o ponteiro aponte para algo
    if (DL != NULL)
    {
        //Insere um novo nó após o nó cabeça
        newNode = malloc(sizeof(DoublyLinkedList));
        newNode->value = newValue;
        newNode->prev = DL;
        newNode->next = DL->next;
        //Se havia um nó após o nó cabeça, faz com que ele aponte para o novo nó
        if (DL->next != NULL)
        {
            DL->next->prev = newNode;
        }
        DL->next = newNode;
    }
}

/*Remove uma ocorrência de um valor na lista duplamente encadeada
Retornando 0 caso o elemento não estava na lista ou 1 caso contrário*/
int removeValueDoublyLinkedList(DoublyLinkedList *DL, int rValue)
{
    DoublyLinkedList *aux;
    //Garantindo que o ponteiro aponta para algo
    if (DL != NULL)
    {
        //Enquanto não chegar no fim da lista
        while (DL->next != NULL)
        {
            // Se encontrar o valor a ser removido
            if (DL->next->value == rValue)
            {
                //Remove o nó 
                aux = DL->next->next;
                free(DL->next);
                //Se houver um outro nó após o que foi removido, reconecta ele com a lista
                if (aux != NULL)
                {
                    aux->prev = DL;
                }
                DL->next = aux;
                return 1;
            }
            //Se não, avança para o pŕoximo nó
            DL = DL->next;
        }
    }
    //Se o elemento não estava na lista retorna 0
    return 0;
}

/*Busca um valor numa lista duplamente encadeada, retornando o endereço do nó com o valor desejado
ou NULL caso o valor não esteja na lista*/
DoublyLinkedList *searchValueDoublyLinkedList(DoublyLinkedList DL, int sValue)
{
    //Enquanto não chegar ao fim da lista
    while (DL.next != NULL)
    {
        //Se encontrar o valor procurado retorna ele
        if (DL.next->value == sValue)
        {
            return DL.next;
        }
        //Se não, avança para o próximo nó
        DL = *(DL.next);
    }
    //Se o valor não estava na lista, retorna NULL
    return NULL;
}

// ### Nó para fila ###
typedef struct QNode
{
    int value;
    struct QNode *next;
} QNode;

// ### Fila ###
typedef struct Queue
{
    struct QNode *front, *rear;
} Queue;

// ### Operações com Fila

//Cria uma fila vazia
void createQueue(Queue *Q)
{
    //Garante que o ponteiro apontava para algo
    if (Q != NULL)
    {
        //Aponta a fila pra uma lista vazia
        Q->front = NULL;
        Q->rear = NULL;
    }
}

//Desaloca uma lista apontada pela fila
void deleteQueue(Queue *Q)
{
    QNode *aux;
    //Garante que o ponteiro apontava para algo
    if (Q != NULL)
    {
        //Enquanto a frente não chegar no fim da lista
        while (Q->front != NULL)
        {
            //Desaloca os nós da lista e avança
            aux = Q->front->next;
            free(Q->front);
            Q->front = aux;
        }
        Q->rear = NULL;
    }
}

//Enfileira um novo valor na fila inserindo um novo nó após o seu último
void enQueue(Queue *Q, int newValue)
{
    QNode *newNode;
    //Garante que o ponteiro apontava para algo
    if (Q != NULL)
    {
        //Cria um novo nó
        newNode = malloc(sizeof(QNode));
        newNode->value = newValue;
        newNode->next = NULL;

        //Se a lista não estiver vazia
        if (Q->rear != NULL)
        {
            //Insere o novo nó após o ultimo nó e atualiza o ponteiro para o útimo nó
            Q->rear->next = newNode;
            Q->rear = newNode;
        }
        else
        {
            //Se não, insere o novo nó e faz com que o inicio e o fim apontem para o mesmo nó
            Q->rear = newNode;
            Q->front = newNode;
        }
    }
}

/*Desenfileira o primeiro valor da fila removendo o seu primeiro nó
Retorna 0 se a fila estava vazia e 1 caso o primeiro elemento foi removido*/
int deQueue(Queue *Q)
{
    QNode *aux;
    //Garante que o ponteiro apontava para algo
    if (Q != NULL)
    {
        //Se a fila não estava vazia
        if (Q->front != NULL)
        {
            //Salva o segundo nó e remove o primeiro
            aux = Q->front->next;
            free(Q->front);
            Q->front = aux;
            //Se a fila se tornar vazia, faz o ponteiro para o final também apontar para vazio
            if (Q->front == NULL)
            {
                Q->rear = NULL;
            }
            //Retorna 
            return 1;
        }
    }
    return 0;
}

// ### Nó para Pilha ###
typedef struct SNode
{
    int valor;
    struct SNode *prox;
} SNode;

// ### Pilha ###
typedef struct Stack
{
    SNode *topo;
} Stack;

// ### Operações com Pilha ###

//Cria uma pilha vazia
void createStack(Stack *S)
{
    //Garante que o ponteiro apontava para algo
    if (S != NULL)
    {
        //Faz o topo apontar para nenhum nó
        S->topo = NULL;
    }
}

//Desaloca os nós da pilha
void deleteStack(Stack *S)
{
    SNode *aux;
    if (S != NULL)
    {
        //Enquanto a lista não for vazia remove os nós no topo em loop
        while (S->topo != NULL)
        {
            aux = S->topo->prox;
            free(S->topo);
            S->topo = aux;
        }
    }
}

//Insere um valor no topo da lista
void push(Stack *S, int valor)
{
    SNode *aux;
    if (S != NULL)
    {
        aux = malloc(sizeof(SNode));
        aux->valor = valor;
        aux->prox = S->topo;
        S->topo = aux;
    }
}

/*Remove o valor no topo da pilha
Retorna 0 se a pilha estava vazia e 1 caso contrário*/
int pop(Stack *S)
{
    SNode *aux;
    if (S != NULL)
    {
        //Se a lista não for vazia, remove o topo e retorna 1
        if (S->topo != NULL)
        {
            aux = S->topo->prox;
            free(S->topo);
            S->topo = aux;
            return 1;
        }
    }
    //Se a lista for vazia, somente retorna 0
    return 0;
}

//Le apenas o primeiro caracter válido do buffer de teclado e descarta o resto
char readChar()
{
    char c;
    // Enquanto o caracter inserido for inválido, continue lendo do teclado
    while ((c = getchar()) < '0')
        ;
    // Após ler, descarta qualquer outro caracter no buffer de teclado
    while (getchar() != '\n')
        ;
    // Retorna o caracter lido
    return c;
}

//Desenha um nó qualquer
void drawNode(int x, int y, int value)
{
    char v[10];
    int largura, altura;
    gfx_rectangle(x, y, x + g_NODE_LENGTH, y + g_NODE_HEIGHT);
    //Guarda o valor na string v
    sprintf(v, "%d", value);
    gfx_get_text_size(v, &largura, &altura);
    gfx_text(x + g_NODE_LENGTH/2 - largura/2, y + g_NODE_HEIGHT/2 - altura/2, v);
}

//Desenha uma seta que aponta para direita
void drawArrow(int x, int y)
{
    gfx_line(x, y, x + 20, y);
    gfx_line(x + 10, y + 10, x + 20, y);
    gfx_line(x + 10, y - 10, x + 20, y);
}

//Desenha uma seta que aponta para um nó anterior
void drawToBackArrow(int x1, int y1, int x2, int y2)
{
    gfx_line(x1, y1, x1 + 20, y1);
    gfx_line(x1 + 20, y1, x1 + 20, y1 + g_NODE_HEIGHT/2 + 20);
    gfx_line(x1 + 20, y1 + g_NODE_HEIGHT/2 + 20, x2, y2 + 20);
    gfx_line(x2, y2, x2, y2 + 20);
    gfx_line(x2 - 10, y2 + 10, x2, y2);
    gfx_line(x2 + 10, y2 + 10, x2, y2);
}
//Desenha uma seta abaixo de um nó apontando para ele
void drawUnderArrow(int x, int y){
    gfx_line(x, y, x, y + 10);
    gfx_line(x, y, x + 10, y + 5);
    gfx_line(x, y, x - 10, y + 5);
}

//Desenha uma seta que aponta para direita e outra que aponta para esquerda
void drawTwoArrow(int x, int y)
{
    gfx_line(x + 0, y - 10, x + 20, y - 10);
    gfx_line(x + 10, y, x + 20, y - 10);
    gfx_line(x + 10, y - 20, x + 20, y - 10);

    gfx_line(x - g_NODE_LENGTH, y + 10, x - g_NODE_LENGTH - 20, y + 10);
    gfx_line(x - g_NODE_LENGTH - 10, y, x - g_NODE_LENGTH - 20, y + 10);
    gfx_line(x - g_NODE_LENGTH - 10, y + 20, x - g_NODE_LENGTH - 20, y + 10);
}

//Desenha a letra lambda
void drawLambda(int x, int y){
    gfx_line(x + 12, y + 15, x + g_NODE_LENGTH - 12, y + g_NODE_HEIGHT - 15);
    gfx_line(x + 12, y + g_NODE_HEIGHT - 15, x + g_NODE_LENGTH/2, y + g_NODE_HEIGHT/2);
}

//Desenha um lista simplesmente encadeada
void drawLinkedList(LinkedList *L)
{
    //Começa a lista na origem das listas
    int x = g_X_LIST_ORIGIN, y = g_Y_LIST_ORIGIN;
    //Limpa a seção da tela pertencente a lista simplesmente encadeada
    gfx_set_color(0, 0, 0);
    gfx_filled_rectangle(0, y - 20, g_SCREEN_LENGTH, y + g_SECTION_SIZE - 20);
    gfx_set_color(255, 255, 255);
    gfx_text(g_SCREEN_LENGTH/2 - 270/2, y - 20, "Lista Ordenada Simplesmente Encadeada");
    //Desenha a seta que representa o ponteiro da lista
    drawArrow(x - g_NODES_DISTANCE, y + g_NODE_HEIGHT/2);
    //Enquanto não chegar no fim da lista desenha cada nó
    while (L != NULL)
    {
        drawNode(x, y, L->value);
        drawArrow(x + g_NODE_LENGTH, y + (g_NODE_HEIGHT) / 2);
        L = L->next;
        x += g_NODE_LENGTH + g_NODES_DISTANCE;
    }
    drawLambda(x, y);
    gfx_paint();
}

//Desenha uma lista circular simplesmente encadeada
void drawCircularList(LinkedList *CL)
{
    //Começa a lista na segunda seção a partir da origem
    int x = g_X_LIST_ORIGIN, y = g_Y_LIST_ORIGIN + g_SECTION_SIZE;
    LinkedList *begin;
    //Apaga a seção da tela dedicada a lista circular
    gfx_set_color(0, 0, 0);
    gfx_filled_rectangle(0, y - 20, g_SCREEN_LENGTH, y + g_SECTION_SIZE - 20);
    gfx_set_color(255, 255, 255);    
    //Titulo da estrutura
    gfx_text(g_SCREEN_LENGTH/2 - 323/2, y - 20, "Lista Ordenada Circular Simplesmente Encadeada");

    //Desenha a seta que representa o ponteiro para lista
    drawArrow(x - g_NODES_DISTANCE, y + g_NODE_HEIGHT/2);
    if (CL != NULL)
    {
        begin = CL->next;
        do
        {
            //Se a lista não for vazia, desenha cada próximo nó até ele ser o menor valor
            CL = CL->next;
            drawNode(x, y, CL->value);
            //Se estiver desenhando o último nó, desenha a seta que aponta para o primeiro nó
            if(CL->next == begin){
                drawToBackArrow(x + g_NODE_LENGTH, y + g_NODE_HEIGHT/2, g_X_LIST_ORIGIN + g_NODE_LENGTH/2, y + g_NODE_HEIGHT);
            }
            else{
                drawArrow(x + g_NODE_LENGTH, y + (g_NODE_HEIGHT) / 2);
            }
            x += g_NODE_LENGTH + g_NODES_DISTANCE;
        } while(CL->next != begin);
    }
    else{ 
        //Se a lista for vazia desenha apenas o lambda
        drawLambda(x, y);
    }
    gfx_paint();
}

//Desenha uma lista duplamente encadeada com nó cabeça
void drawDoublyLinkedList(DoublyLinkedList head)
{
    //Começa a lista na terceira seção a partir da origem
    int x = g_X_LIST_ORIGIN, y = g_Y_LIST_ORIGIN + 2*g_SECTION_SIZE;    
    // Apaga a seção dedicada a lista duplamente encadeada
    gfx_set_color(0, 0, 0);
    gfx_filled_rectangle(0, y - 20, g_SCREEN_LENGTH, y + g_SECTION_SIZE - 20);
    gfx_set_color(255, 255, 255);    
    //Titulo da estrutura
    gfx_text(g_SCREEN_LENGTH/2 - 293/2, y - 20, "Lista Duplamente Encadeada com No Cabeca");

    //Desenha o nó cabeça
    gfx_rectangle(x, y, x + g_NODE_LENGTH, y + g_NODE_HEIGHT);
    drawTwoArrow(x + g_NODE_LENGTH, y + g_NODE_HEIGHT / 2);
    x += g_NODES_DISTANCE + g_NODE_LENGTH;
    //Desenha cada um dos nós até chegar ao último
    while (head.next != NULL)
    {
        drawNode(x, y, head.next->value);
        drawTwoArrow(x + g_NODE_LENGTH, y + g_NODE_HEIGHT / 2);
        head = *(head.next);
        x += g_NODES_DISTANCE + g_NODE_LENGTH;
    }
    drawLambda(x, y - 10);
    gfx_paint();
}

//Desenha uma fila
void drawQueue(Queue Q)
{
    //Começa a fila na quarta seção a partir da origem
    int x = g_X_LIST_ORIGIN, y = g_Y_LIST_ORIGIN + 3*g_SECTION_SIZE;
    QNode *aux;
        //Limpa a seção da fila
    gfx_set_color(0, 0, 0);
    gfx_filled_rectangle(0, y - 20, g_SCREEN_LENGTH, y + g_SECTION_SIZE - 20);
    gfx_set_color(255, 255, 255);
    //Titulo da estrutura
    gfx_text(g_SCREEN_LENGTH/2 - 22/2, y - 20, "Fila");

    //Desenha os ponteiros para frente a para atras
    if(Q.front == Q.rear){
        gfx_text(x + g_NODE_LENGTH/2 - 77/2, y + g_NODE_HEIGHT + 20, "Frente Tras");
        drawUnderArrow(x + g_NODE_LENGTH/2 + 10, y + g_NODE_HEIGHT);
        drawUnderArrow(x + g_NODE_LENGTH/2 - 10, y + g_NODE_HEIGHT);
    }
    else{
        //Se não desenha a seta frente separada
        gfx_text(x + g_NODE_LENGTH/2 - 42/2, y + g_NODE_HEIGHT + 20, "Frente");
        drawUnderArrow(x + g_NODE_LENGTH/2, y + g_NODE_HEIGHT);
    }
    //Desenha cada um dos nós da fila até chegar no seu último
    aux = Q.front;
    while (aux != NULL)
    {
        drawNode(x, y, aux->value);
        drawArrow(x + g_NODE_LENGTH, y + (g_NODE_HEIGHT) / 2);
        aux = aux->next;
        x += g_NODE_LENGTH + g_NODES_DISTANCE;
    }
    if(Q.rear != Q.front){
        //E a seta atras separada também
        gfx_text(x - g_NODES_DISTANCE - g_NODE_LENGTH/2 - 31/2, y + g_NODE_HEIGHT + 20, "Tras");
        drawUnderArrow(x - g_NODES_DISTANCE - g_NODE_LENGTH/2, y + g_NODE_HEIGHT);
    }
    drawLambda(x, y);
    gfx_paint();
}

//Desenha uma pilha
void drawStack(Stack S)
{
    //Começa a pilha na quinta seção da tela a partir da origem 
    int x = g_X_LIST_ORIGIN, y = g_Y_LIST_ORIGIN + 4 * g_SECTION_SIZE;

    //Limpa a seção da pilha
    gfx_set_color(0, 0, 0);
    gfx_filled_rectangle(0, y - 20, g_SCREEN_LENGTH, y + g_SECTION_SIZE - 20);
    gfx_set_color(255, 255, 255);    
    //Titulo da estrutura
    gfx_text(g_SCREEN_LENGTH/2 - 31/2, y - 20, "Pilha");

    //Desenha o ponteiro topo
    gfx_text(x + g_NODE_LENGTH/2 - 34/2, y + g_NODE_HEIGHT + 20, "Topo");
    drawUnderArrow(x + g_NODE_LENGTH/2, y + g_NODE_HEIGHT);
    //Desenha todos os nós até chegar no fim da pilha
    while (S.topo != NULL)
    {
        drawNode(x, y, S.topo->valor);
        drawArrow(x + g_NODE_LENGTH, y + (g_NODE_HEIGHT) / 2);
        S.topo = S.topo->prox;
        x += g_NODE_LENGTH + g_NODES_DISTANCE;
    }
    drawLambda(x, y);
    gfx_paint();
}

//Programa principal
int main()
{
    //Variaveis do Menu
    char opcEstrutura, opcAcao;
    int inputValue;
    //Estruturas do programa
    LinkedList *ListaOrdenada;
    LinkedList *ListaCircular;
    DoublyLinkedList Cabeca;
    Queue Fila;
    Stack Pilha;
    //Criação das estruturas
    createLinkedList(&ListaOrdenada);
    createLinkedList(&ListaCircular);
    createDoublyLinkedList(&Cabeca);
    createQueue(&Fila);
    createStack(&Pilha);
    //Inicio da gfx
    gfx_init(g_SCREEN_LENGTH, g_SCREEN_HEIGHT, "Alocação Encadeada");
    
    //Inicio do menu
    opcEstrutura = 1;
    while (opcEstrutura != '0')
    {
        printf("### Menu Principal ###\n0 - Encerrar programa\
        \n1 - Lista Ordenada Simplesmente Encadeada;\
        \n2 - Lista Ordenada Circular Simplesmente Encadeada;\
        \n3 - Lista Duplamente Encadeada com Nó Cabeça;\
        \n4 - Fila;\
        \n5 - Pilha.\
        \nDigite uma opção para escolher uma estrutura: ");
        opcEstrutura = readChar();
        system("clear");
        opcAcao = 1;
        switch (opcEstrutura)
        {
        case '0':
            break;
        case '1':
            while (opcAcao != '0')
            {
                drawLinkedList(ListaOrdenada);
                printf("### Lista Ordenada Simplesmente Encadeada ###\n");
                printf("Lista de operações:\
                \n0 - Voltar\
                \n1 - Busca\
                \n2 - Inserção\
                \n3 - Remoção\
                \nDigite uma opção: ");
                opcAcao = readChar();
                system("clear");
                switch (opcAcao)
                {
                case '0':
                    break;
                case '1':
                    printf("### Busca ###\nDigite o valor que deverá ser buscado: ");
                    scanf("%d", &inputValue);
                    if (searchValueOrderedLinkedList(ListaOrdenada, inputValue) != NULL)
                    {
                        printf("O valor está presente na lista\n");
                    }
                    else
                    {
                        printf("O valor não está presente na lista\n");
                    }
                    break;
                case '2':
                    printf("### Inserção ###\nDigite o valor que deverá ser inserido na lista: ");
                    scanf("%d", &inputValue);
                    insertValueOrderedLinkedList(&ListaOrdenada, inputValue);
                    printf("Inserido o valor %d na lista\n", inputValue);
                    break;
                case '3':
                    printf("### Remoção ###\nDigite o valor que deverá ser removido da lista: ");
                    scanf("%d", &inputValue);
                    if (removeValueOrderedLinkedList(&ListaOrdenada, inputValue))
                    {
                        printf("Removido da lista a primeira ocorrência do valor\n");
                    }
                    else
                    {
                        printf("O valor não está presente na lista\n");
                    }
                    break;
                default:
                    printf("Opção inválida! Tente novamente\n");
                    break;
                }
                printf("\n");
            }
            break;
        case '2':
            while (opcAcao != '0')
            {
                drawCircularList(ListaCircular);
                printf("### Lista Ordenada Circular Simplesmente Encadeada ###\n");
                printf("Lista de operações:\
                \n0 - Voltar\
                \n1 - Busca\
                \n2 - Inserção\
                \n3 - Remoção\
                \nDigite uma opção: ");
                opcAcao = readChar();
                system("clear");
                switch (opcAcao)
                {
                case '0':
                    break;
                case '1':
                    printf("### Busca ###\nDigite o valor que deverá ser buscado: ");
                    scanf("%d", &inputValue);
                    if (searchValueCircularLinkedList(ListaCircular, inputValue) != NULL)
                    {
                        printf("O valor está presente na lista\n");
                    }
                    else
                    {
                        printf("O valor não está presente na lista\n");
                    }
                    break;
                case '2':
                    printf("### Inserção ###\nDigite o valor que deverá ser inserido na lista: ");
                    scanf("%d", &inputValue);
                    insertValueOrderedCircularLinkedList(&ListaCircular, inputValue);
                    printf("Inserido o valor %d na lista\n", inputValue);
                    break;
                case '3':
                    printf("### Remoção ###\nDigite o valor que deverá ser removido da lista: ");
                    scanf("%d", &inputValue);
                    if (removeValueOrderedCircularLinkedList(&ListaCircular, inputValue))
                    {
                        printf("Removido da lista a primeira ocorrência do valor\n");
                    }
                    else
                    {
                        printf("O valor não está presente na lista\n");
                    }
                    break;
                default:
                    printf("Opção inválida! Tente novamente\n");
                    break;
                }
                printf("\n");
            }
            break;
        case '3':
            while (opcAcao != '0')
            {
                drawDoublyLinkedList(Cabeca);
                printf("### Lista Duplamente Encadeada com Nó Cabeça ###\n");
                printf("Lista de operações:\
                \n0 - Voltar\
                \n1 - Busca\
                \n2 - Inserção\
                \n3 - Remoção\
                \nDigite uma opção: ");
                opcAcao = readChar();
                system("clear");
                switch (opcAcao)
                {
                case '0':
                    break;
                case '1':
                    printf("### Busca ###\nDigite o valor que deverá ser buscado: ");
                    scanf("%d", &inputValue);
                    if (searchValueDoublyLinkedList(Cabeca, inputValue) != NULL)
                    {
                        printf("O valor está presente na lista\n");
                    }
                    else
                    {
                        printf("O valor não está presente na lista\n");
                    }
                    break;
                case '2':
                    printf("### Inserção ###\nDigite o valor que deverá ser inserido na lista: ");
                    scanf("%d", &inputValue);
                    insertValueDoublyLinkedList(&Cabeca, inputValue);
                    printf("Inserido o valor %d na lista\n", inputValue);
                    break;
                case '3':
                    printf("### Remoção ###\nDigite o valor que deverá ser removido da lista: ");
                    scanf("%d", &inputValue);
                    if (removeValueDoublyLinkedList(&Cabeca, inputValue))
                    {
                        printf("Removido da lista a primeira ocorrência do valor\n");
                    }
                    else
                    {
                        printf("O valor não estava presente na lista\n");
                    }
                    break;
                default:
                    printf("Opção inválida! Tente novamente\n");
                    break;
                }
                printf("\n");
                }
            break;
        case '4':
            while (opcAcao != '0')
            {
                drawQueue(Fila);
                printf("### Fila ###\n");
                printf("Lista de operações:\
                \n0 - Voltar\
                \n1 - Inserção\
                \n2 - Remoção\
                \nDigite uma opção: ");
                opcAcao = readChar();
                system("clear");
                switch (opcAcao)
                {
                case '0':
                    break;
                case '1':
                    printf("### Inserção ###\nDigite o valor que deverá ser inserido no fim fila: ");
                    scanf("%d", &inputValue);
                    enQueue(&Fila, inputValue);
                    printf("Inserido o valor %d na fila\n", inputValue);
                    break;
                case '2':
                    printf("### Remoção ###\n");
                    if (deQueue(&Fila))
                    {
                        printf("Removido o valor na frente da fila\n");
                    }
                    else
                    {
                        printf("A lista já está vazia\n");
                    }
                    break;
                default:
                    printf("Opção inválida! Tente novamente\n");
                    break;
                }
                printf("\n");
            }
            break;
        case '5':
            while (opcAcao != '0')
            {
                drawStack(Pilha);
                printf("### Pilha ###\n");
                printf("Lista de operações:\
                \n0 - Voltar\
                \n1 - Inserção\
                \n2 - Remoção\
                \nDigite uma opção: ");
                opcAcao = readChar();
                system("clear");                
                switch (opcAcao)
                {
                case '0':
                    break;
                case '1':
                    printf("### Inserção ###\nDigite o valor que deverá ser inserido no topo da pilha: ");
                    scanf("%d", &inputValue);
                    push(&Pilha, inputValue);
                    printf("Inserido o valor %d na lista\n", inputValue);
                    break;
                case '2':
                    printf("### Remoção ###\n");
                    if (pop(&Pilha))
                    {
                        printf("Removido o valor no topo da pilha\n");
                    }
                    else
                    {
                        printf("A pilha já está vazia\n");
                    }
                    break;
                default:
                    printf("Opção inválida! Tente novamente\n");
                    break;
                }
                printf("\n");
            }
            break;
        default:
            printf("Opção inválida! Tente novamente\n");
            break;
        }
    }
    //Fecha a janela
    gfx_quit();
    //Deleta por completo cada uma das estruturas 
    deleteLinkedList(&ListaOrdenada);
    deleteDoublyLinkedList(&Cabeca);
    deleteQueue(&Fila);
    deleteStack(&Pilha);
    deleteCircularLinkedList(&ListaCircular);
    return 0;
}