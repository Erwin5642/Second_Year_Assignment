// Nome: João Vitor Antunes da Silva         RGM: 48935
#include <stdlib.h>
#include <stdio.h>
#include "gfx.h"

// Parâmetros para o desenho da árvore
#define g_NODE_WIDTH 30
#define g_NODE_HEIGHT 30
#define g_SCREEN_WIDTH 1200
#define g_SCREEN_HEIGHT 800
#define g_X_TREE_ORIGIN 600
#define g_Y_TREE_ORIGIN 30
#define g_NODES_DISTANCE 75

// Estrutura com nó para árvore AVL
typedef struct s_no
{
    int32_t chave : 28;
    int32_t bal : 2;
    int32_t reservado : 2; /* sem uso */
    struct s_no *esq;
    struct s_no *dir;
} AVLNodeTree;

// Estrutura com nó para árvore AVL em arquivo
typedef struct s_arq_no
{
    int32_t chave : 28;
    int32_t bal : 2;
    uint32_t esq : 1;
    uint32_t dir : 1;
} AVLNodeFile;

// Desaloca todos os nós de uma árvore AVL T
void deleteAVLTree(AVLNodeTree **T)
{
    if (T)
    {
        if (*T)
        {
            if ((*T)->esq)
            {
                deleteAVLTree(&(*T)->esq);
            }
            if ((*T)->dir)
            {
                deleteAVLTree(&(*T)->dir);
            }
            free(*T);
            *T = NULL;
        }
    }
}

// Aplica uma rotação esquerda em um nó AVL T
void leftRotation(AVLNodeTree **T)
{
    AVLNodeTree *aux = *T;
    *T = (*T)->dir;
    aux->dir = (*T)->esq;
    (*T)->esq = aux;
    // Considera temporariamente que ambos os nós ptu e pt ficaram com balanço 0
    aux->bal = 0;
    (*T)->bal = 0;
}

// Aplica uma rotação esquerda em um nó AVL T
void rightRotation(AVLNodeTree **T)
{
    AVLNodeTree *aux = *T;
    *T = (*T)->esq;
    aux->esq = (*T)->dir;
    (*T)->dir = aux;
    // Considera temporariamente que ambos os nós ptu e pt ficaram com balanço 0
    aux->bal = 0;
    (*T)->bal = 0;
}

// Aplica uma rotação dupla direita em um nó AVL T
void leftRightRotation(AVLNodeTree **T)
{
    int ptvb = (*T)->esq->dir->bal;
    leftRotation(&(*T)->esq);
    rightRotation(T);
    if (ptvb == -1)
    {
        (*T)->dir->bal = 1;
    }
    if (ptvb == 1)
    {
        (*T)->esq->bal = -1;
    }
}

// Aplica uma rotação dupla esquerda em um nó AVL T
void rightLeftRotation(AVLNodeTree **T)
{
    int ptvb = (*T)->dir->esq->bal;
    rightRotation(&(*T)->dir);
    leftRotation(T);
    if (ptvb == 1)
    {
        (*T)->esq->bal = -1;
    }
    if (ptvb == -1)
    {
        (*T)->dir->bal = 1;
    }
}

// Usado na inserção para recalcular o balanço de um nó de forma adequada
//Rebalanceia a subárvore direita
// Retorna 1 se o algoritmo deve seguir recalculando o balanço dos nós ancestrais
// Retorna 0 se o rebalanço deve parar
int rebalanceRight(AVLNodeTree **T)
{
    int flag = 1;
    switch ((*T)->bal)
    {
    case -1:
        (*T)->bal = flag = 0;
        break;
    case 0:
        (*T)->bal = 1;
        break;
    case 1:
        if ((*T)->dir->bal == 1)
        {
            leftRotation(T);
        }
        else
        {
            rightLeftRotation(T);
        }
        flag = 0;
        break;
    }
    return flag;
}

// Usado na inserção para recalcular o balanço de um nó de forma adequada
//Rebalanceia a subárvore esquerda
// Retorna 1 se o algoritmo deve seguir recalculando o balanço dos nós ancestrais
// Retorna 0 se o rebalanço deve parar
int rebalanceLeft(AVLNodeTree **T)
{
    int flag = 1;

    switch ((*T)->bal)
    {
    case 1:
        (*T)->bal = flag = 0;
        break;
    case 0:
        (*T)->bal = -1;
        break;
    case -1:
        if ((*T)->esq->bal == -1)
        {
            rightRotation(T);
        }
        else
        {
            leftRightRotation(T);
        }
        flag = 0;
        break;
    }

    return flag;
}

// Usado na remoção para recalcular o balanço de um nó de forma adequada
// Após voltar de um nó filho à direita
// Retorna 1 se o algoritmo deve seguir recalculando o balanço dos nós ancestrais
// Retorna 0 se o rebalanço deve parar
int rebalanceFromRight(AVLNodeTree **T)
{
    int flag = 1, temp;
    switch ((*T)->bal)
    {
    case 1:
        (*T)->bal = 0;
        break;
    case 0:
        (*T)->bal = -1;
        flag = 0;
        break;
    case -1:
        if ((temp = (*T)->esq->bal) != 1)
        {
            rightRotation(T);
            if (temp == 0)
            {
                (*T)->dir->bal = -1;
                (*T)->bal = 1;
                flag = 0;
            }
        }
        else
        {
            leftRightRotation(T);
        }
        break;
    }

    return flag;
}

// Usado na remoção para recalcular o balanço de um nó de forma adequada
// Após voltar de um nó filho à esquerda
// Retorna 1 se o algoritmo deve seguir recalculando o balanço dos nós ancestrais
// Retorna 0 se o rebalanço deve parar
int rebalanceFromLeft(AVLNodeTree **T)
{
    int flag = 1, temp;

    switch ((*T)->bal)
    {
    case -1:
        (*T)->bal = 0;
        break;
    case 0:
        (*T)->bal = 1;
        flag = 0;
        break;
    case 1:
        if ((temp = (*T)->dir->bal) != -1)
        {
            leftRotation(T);
            if (temp == 0)
            {
                (*T)->esq->bal = 1;
                (*T)->bal = -1;
                flag = 0;
            }
        }
        else
        {
            rightLeftRotation(T);
        }
        break;
    }
    return flag;
}

// Retorna um ponteiro para o menor nó na árvore AVL
AVLNodeTree *minAVLTree(AVLNodeTree *T)
{
    if (T)
    {
        // Percorre a árvore até encontrar o nó mais à esquerda
        while (T->esq)
        {
            T = T->esq;
        }
    }
    return T;
}

// Le apenas o primeiro caracter válido do buffer de teclado e descarta o resto
char readChar()
{
    char c;
    // Enquanto o caracter inserido for inválido, continue lendo do teclado
    while ((c = getchar()) < '0')
        ;
    // Após ler, descarta qualquer outro caracter no buffer de teclado até o enter
    while (getchar() != '\n')
        ;
    // Retorna o caracter lido
    return c;
}

// Retorna um ponteiro para um nove nó para AVL recém alocado
AVLNodeTree *newNodeAVLTree(int key)
{
    AVLNodeTree *new = malloc(sizeof(AVLNodeTree));
    new->chave = key;
    new->esq = new->dir = NULL;
    new->bal = 0;
    return new;
}

// Insere um nó com a chave desejada na árvore AVL
// Retorna 1 se a chave foi inserida com sucesso, ou 0 se a chave já existia na AVL
int insertKeyAVLTree(AVLNodeTree **T, int key)
{
    static int flag;
    flag = 0;
    int success;
    if (!(*T))
    {
        flag = 1;
        (*T) = newNodeAVLTree(key);
        return 1;
    }
    if ((*T)->chave == key)
    {
        return 0;
    }
    if ((*T)->chave < key)
    {
        success = insertKeyAVLTree(&(*T)->dir, key);
        if (flag)
        {
            flag = rebalanceRight(T);
        }
    }
    else if ((*T)->chave > key)
    {
        success = insertKeyAVLTree(&(*T)->esq, key);
        if (flag)
        {
            flag = rebalanceLeft(T);
        }
    }
    return success;
}

// Remove um nó da árvore a partir de uma chave
// Retorna 1 se a chave foi removida com sucesso, ou 0 caso ela não estivesse na árvore
int removeKeyAVLTree(AVLNodeTree **T, int key)
{
    static int flag;
    AVLNodeTree *aux;
    int success;
    flag = 0;
    if (*T)
    {
        // Procura pelo nó com a chave desejada
        if (key == (*T)->chave)
        {
            flag = 1;
            if (!(*T)->esq)
            {
                aux = (*T)->dir;
                free(*T);
                *T = aux;
            }
            else if (!(*T)->dir)
            {
                aux = (*T)->esq;
                free(*T);
                *T = aux;
            }
            else
            {
                aux = minAVLTree((*T)->dir);
                (*T)->chave = aux->chave;
                success = removeKeyAVLTree(&(*T)->dir, aux->chave);
                if (flag)
                {
                    flag = rebalanceFromRight(T);
                }
            }
            return 1;
        }
        if (key < (*T)->chave)
        {
            // Se for menor busca na sub arvore esquerda
            success = removeKeyAVLTree(&(*T)->esq, key);
            if (flag)
            {
                flag = rebalanceFromLeft(T);
            }
        }
        else if (key > (*T)->chave)
        {
            // Se for maior busca na sub arvore direita
            success = removeKeyAVLTree(&(*T)->dir, key);
            if (flag)
            {
                flag = rebalanceFromRight(T);
            }
        }
        return success;
    }
    return 0;
}

// Retorna um ponteiro para o nó com valor buscado ou NULL caso ele não exista na árvore AVL e salva um flag
// 0: Arvore vazia
// 1: Chave presente na arvore
// 2: Chave menor que o minimo
// 3: Chave maior que o maximo
AVLNodeTree *searchKeyAVLTree(AVLNodeTree *root, int key, int *flag)
{
    if (!root)
    {
        // Arvore vazia
        *flag = 0;
    }
    while (root)
    {
        if (key == root->chave)
        {
            // Encontrou a chave
            *flag = 1;
            return root;
        }
        else if (key < root->chave)
        {
            if (!root->esq)
            {
                // Chave menor que nó atual, mas sem filhos a esquerda
                *flag = 2;
                return root;
            }
            root = root->esq;
        }
        else
        {
            if (!root->dir)
            {
                // Chave maior que nó atual, mas sem filhos a direita
                *flag = 3;
                return root;
            }
            root = root->dir;
        }
    }
    return NULL;
}

// Desenha um nó na tela mostrando sua chave e balanço
void drawNode(int x, int y, int key, int bal)
{
    char v[10];
    int largura, altura;
    gfx_set_color(0, 0, 0);
    gfx_filled_rectangle(x + 1, y + 1, x + g_NODE_WIDTH - 1, y + g_NODE_HEIGHT - 1);
    gfx_set_color(255, 255, 255);
    gfx_rectangle(x, y, x + g_NODE_WIDTH, y + g_NODE_HEIGHT);
    // Guarda o valor na string v
    sprintf(v, "%d", key);
    gfx_get_text_size(v, &largura, &altura);
    gfx_text(x + g_NODE_WIDTH / 2 - largura / 2, y + g_NODE_HEIGHT / 2 - altura / 2, v);

    sprintf(v, "%d", bal);
    gfx_get_text_size(v, &largura, &altura);
    gfx_text(x + g_NODE_WIDTH / 2 - largura / 2, y - altura, v);
}

// Desenha uma árvore AVL recursivamente
void drawTree(AVLNodeTree *T, int x, int y, int dist)
{
    if (T)
    {
        dist /= 2;
        if (T->esq)
        {
            gfx_line(x, y + g_NODE_HEIGHT / 2, x - dist, y + g_NODES_DISTANCE + g_NODE_HEIGHT / 2);
            drawTree(T->esq, x - dist, y + g_NODES_DISTANCE, dist);
        }
        if (T->dir)
        {
            gfx_line(x, y + g_NODE_HEIGHT / 2, x + dist, y + g_NODES_DISTANCE + g_NODE_HEIGHT / 2);
            drawTree(T->dir, x + dist, y + g_NODES_DISTANCE, dist);
        }
        drawNode(x - g_NODE_WIDTH / 2, y, T->chave, T->bal);
    }
}

// Operações em Arquivo
// Abre um arquivo
FILE *openFile(const char *name, const char *mode)
{
    FILE *file;
    if ((file = fopen(name, mode)) == NULL)
    {
        printf("Não foi possível abrir o arquivo %s\n", name);
    }
    return file;
}

// Salva os nós de uma árvore AVL em um arquivo dest
void saveNodesInFile(AVLNodeTree *T, FILE *dest)
{
    AVLNodeFile temp;
    temp.chave = T->chave;
    temp.bal = T->bal;
    temp.esq = T->esq != NULL;
    temp.dir = T->dir != NULL;
    fwrite(&temp, sizeof(AVLNodeFile), 1, dest);
    if (T->esq)
    {
        saveNodesInFile(T->esq, dest);
    }
    if (T->dir)
    {
        saveNodesInFile(T->dir, dest);
    }
}

// Prepara e salva os nós de uma árvore AVL em um arquivo ou escreve 0 se for vazia
void saveAVLTreeInFile(AVLNodeTree *T, const char *fileName)
{
    FILE *treeFIle = openFile(fileName, "wb");
    if (treeFIle)
    {
        if (T)
        {
            saveNodesInFile(T, treeFIle);
        }
        else
        {
            fputc(0, treeFIle);
        }
        fclose(treeFIle);
    }
}

// Aloca um nó a partir de uma árvore salva em arquivo e aloca os seus filhos recursivamente
void createNodesFromFile(AVLNodeTree **T, FILE *dest)
{
    AVLNodeFile temp;
    if (fread(&temp, sizeof(AVLNodeFile), 1, dest))
    {
        (*T) = newNodeAVLTree(temp.chave);
        (*T)->bal = temp.bal;
        if (temp.esq)
        {
            createNodesFromFile(&(*T)->esq, dest);
        }
        if (temp.dir)
        {
            createNodesFromFile(&(*T)->dir, dest);
        }
    }
}

// Prepara a leitura de um arquivo com uma árvore e salva ele em "T"
void readAVLTreeFromFile(AVLNodeTree **T, const char *fileName)
{
    FILE *treeFile = openFile(fileName, "rb");
    if (treeFile)
    {
        deleteAVLTree(T);
        if (feof(treeFile))
        {
            (*T) = NULL;
        }
        else
        {
            createNodesFromFile(T, treeFile);
        }
        fclose(treeFile);
    }
}

int main()
{
    AVLNodeTree *root = NULL;
    char opcAcao, fileName[20];
    int inputkey, S;
    gfx_init(g_SCREEN_WIDTH, g_SCREEN_HEIGHT, "Arvore de Busca");

    while (1)
    {
        system("clear");
        printf("###\t\t\tArvore AVL\t\t\t###\n[0] - Encerrar programa\
        \n[1] - Buscar Chave;\
        \n[2] - Inserir Chave;\
        \n[3] - Remover Chave;\
        \n[4] - Ler Arquivo com Arvore;\
        \n[5] - Salvar Arvore em Arquivo;\
        \nDigite uma opção para escolher qual operação deve ser feita: ");
        opcAcao = readChar();

        switch (opcAcao)
        {
        case '0':
            gfx_quit();
            deleteAVLTree(&root);
            return 0;
            break;
        case '1':
            printf("\nInforme o valor da chave a ser buscada: ");
            scanf("%d", &inputkey);
            getchar();
            searchKeyAVLTree(root, inputkey, &S);
            switch (S)
            {
            case 0:
                printf("Arvore vazia!\n");
                break;
            case 1:
                printf("A chave foi encontrada na arvore!\n");
                break;
            case 2:
            case 3:
                printf("O valor nao esta presente na arvore!\n");
                break;
            default:
                break;
            }
            break;
        case '2':
            printf("\nInforme o valor da chave que deve ser inserida: ");
            scanf("%d", &inputkey);
            getchar();
            if (insertKeyAVLTree(&root, inputkey))
            {
                printf("A chave %d foi inserida com sucesso!\n", inputkey);
            }
            else
            {
                printf("Nao é possivel inserir valores repetidos na arvore AVL!\n");
            }
            break;
        case '3':
            printf("\nInforme o valor da chave que deve ser removida: ");
            scanf("%d", &inputkey);
            getchar();
            if (removeKeyAVLTree(&root, inputkey))
            {
                printf("A chave %d foi removida com sucesso!\n", inputkey);
            }
            else
            {
                printf("A chave não estava presente na árvore!\n");
            }
            break;
        case '4':
            printf("\nInforme o nome do arquivo em que a árvore está salva: ");
            scanf("%s", fileName);
            getchar();
            readAVLTreeFromFile(&root, fileName);
            break;
        case '5':
            printf("\nInforme o nome do arquivo em que a árvore será salva: ");
            scanf("%s", fileName);
            getchar();
            saveAVLTreeInFile(root, fileName);
            printf("Arquivo salvo com sucesso!\n");
            break;
        default:
            printf("\nOpção inválida! Tente novamente...\n");
            break;
        }
        printf("Pressione Enter para continuar\n");
        gfx_clear();
        drawTree(root, g_X_TREE_ORIGIN, g_Y_TREE_ORIGIN, g_SCREEN_WIDTH / 2);
        gfx_paint();
        getchar();
    }
    return 0;
}