// Nome: João Vitor Antunes da Silva         RGM: 48935
#include <stdlib.h> 
#include <stdio.h>
#include "gfx.h"

// Constantes para regular o desenho da árvore binária e da janela
#define g_NODE_WIDTH 30 //Largura do nó
#define g_NODE_HEIGHT 30 //Altura do nó
#define g_SCREEN_WIDTH 1200 //Largura da janela
#define g_SCREEN_HEIGHT 700 //Altura da janela
#define g_X_TREE_ORIGIN 600 //Coordenada x a partir de onde a árvore será desenhada
#define g_Y_TREE_ORIGIN 30 //Coordenada y a partir de onde a árvore será desenhada
#define g_NODES_DISTANCE 75 //Distancia horizontal entre cada nó

// Estrutura para oprações com a árvore binária de busca
typedef struct BinaryTree
{
    int key;
    struct BinaryTree *left;
    struct BinaryTree *right;
} BinaryTree;

// Desaloca todos os nós de uma árvore binária de busca
void deleteBinaryTree(BinaryTree **T)
{
    if (T)
    {
        if (*T)
        {
            if ((*T)->left)
            {
                deleteBinaryTree(&(*T)->left);
            }
            if ((*T)->right)
            {
                deleteBinaryTree(&(*T)->right);
            }
            free((*T));
            *T = NULL;
        }
    }
}

// Retorna um ponteiro para o nó com valor buscado ou NULL caso ele não exista na árvore binária de busca e salva um flag
// 0: Arvore vazia
// 1: Chave presente na arvore
// 2: Chave menor que o minimo
// 3: Chave maior que o maximo  
BinaryTree *searchkeyBinaryTree(BinaryTree *root, int key, int *flag)
{
    if (!root)
    {
        //Arvore vazia
        *flag = 0;
    }
    while (root)
    {
        if (key == root->key)
        {
            //Encontrou a chave
            *flag = 1;
            return root;
        }
        else if (key < root->key)
        {
            if (!root->left)
            {
                //Chave menor que nó atual, mas sem filhos a esquerda
                *flag = 2;
                return root;
            }
            root = root->left;
        }
        else
        {
            if (!root->right)
            {
                //Chave maior que nó atual, mas sem filhos a direita
                *flag = 3;
                return root;
            }
            root = root->right;
        }
    }
    return NULL;
}

//Aloca um nó
BinaryTree *newNodeBinaryTree(int key)
{
    BinaryTree *aux = malloc(sizeof(BinaryTree));
    aux->key = key;
    aux->left = aux->right = NULL;
    return aux;
}

// Insere um valor numa árvore binária de busca e retorna se a operação foi realizada com sucesso
int insertKeyBinaryTree(BinaryTree **root, int key)
{
    int flag;
    BinaryTree *new;
    if (root)
    {
        //Faz uma busca  pela chave, inserindo de acordo com a flag
        new = searchkeyBinaryTree(*root, key, &flag);
        //Insere apenas se a chave não foi encontrada
        if (flag != 1)
        {
            //Se a arvore for vazia insere na raiz
            if (flag == 0)
            {
                *root = newNodeBinaryTree(key);
            }
            else if (flag == 2)
            {
                //Insere a esquerda do ultimo valor maior que a chave a ser inserida
                new->left = newNodeBinaryTree(key);
            }
            else
            {
                //Insere a direita do ultimo valor menor que a chave a ser inserida
                new->right = newNodeBinaryTree(key);
            }
            //Retorna positivamente que a chave foi inserida
            return 1;
        }
    }
    //Retorna zero se não foi possivel inserir a chave
    return 0;
}

// Remove um nó qualquer da arvore e retorna o nó que substituiu ele
BinaryTree *removeNodeBinaryTree(BinaryTree *node)
{
    BinaryTree *aux, **sucessorPointer;
    //Se houver filho a esquerda ou direita, apenas substitui o nó removido por um deles
    if (!node->left)
    {
        aux = node->right;
        free(node);
        return aux;
    }
    if (!node->right)
    {
        aux = node->left;
        free(node);
        return aux;
    }
    // Se não, encontra o sucessor do nó a ser removido
    sucessorPointer = &node->right;
    while ((*sucessorPointer)->left)
    {
        sucessorPointer = &(*sucessorPointer)->left;
    }
    // Faz as trocas necessárias com os ponteiros e desaloca o nó
    (*sucessorPointer)->left = node->left;
    aux = *sucessorPointer;
    *sucessorPointer = (*sucessorPointer)->right;
    aux->right = node->right;
    free(node);

    return aux;
}

// Remove um nó da árvore a partir de uma chave
// Retorna 1 num sucesso e 0 se houver algum erro
int removeKeyBinaryTree(BinaryTree **root, int key)
{
    if (root)
    {
        // Procura pelo nó com a chave desejada
        while (*root)
        {
            if (key < (*root)->key)
            {
                // Se for menor busca na sub arvore esquerda
                root = &(*root)->left;
            }
            else if (key > (*root)->key)
            {
                // Se for maior busca na sub arvore direita
                root = &(*root)->right;
            }
            else
            {
                // Se encontrar, remove o nó de forma adequada
                *root = removeNodeBinaryTree(*root);
                return 1;
            }
        }
    }
    return 0;
}

// Retorna um ponteiro para o menor nó na árvore binária de busca
BinaryTree *minBinaryTree(BinaryTree *T)
{
    if (T)
    {
        // Percorre a árvore até encontrar o nó mais à esquerda
        while (T->left)
        {
            T = T->left;
        }
    }
    return T;
}

// Retorna um ponteiro para o maior nó na árvore binária de busca
BinaryTree *maxBinaryTree(BinaryTree *T)
{
    if (T)
    {
        // Percorre a árvore até encontrar o nó mais à direita
        while (T->right)
        {
            T = T->right;
        }
    }
    return T;
}

// Retorna um ponteiro para o sucessor de um dado valor na árvore binária de busca
BinaryTree *sucessorNodeBinaryTree(BinaryTree *T, int key)
{
    BinaryTree *ancestralNode = NULL;
    // Se a árvore não for vazia efetua uma busca pelo valor que se deseja descobrir o sucessor
    // Enquanto mantém um ponteiro para o último nó em que foi efetuado uma "virada para esquerda"
    // Que será o ancestral mais baixo que possui um nó filho à esquerda que é ancestral do nó buscado
    while (T)
    {
        if (T->key < key)
        {
            T = T->right;
        }
        else if (T->key > key)
        {
            ancestralNode = T;
            T = T->left;
        }
        else if (T->right && T->key == key)
        {
            // No caso do nó com o valor buscado tiver filho à direita, o sucessor
            // será o menor valor na sub-árvore direita dele
            return minBinaryTree(T->right);
        }
        else if (T->key == key)
        {
            // Caso não exista a sub-árvore direita, o sucessor será o nó ancestral que foi mantido durante a busca
            return ancestralNode;
        }
    }
    return NULL;
}

// Retorna um ponteiro para o antecessor de um dado valor na árvore binária de busca
BinaryTree *predecessorNodeBinaryTree(BinaryTree *T, int key)
{
    BinaryTree *ancestralNode = NULL;
    // Se a árvore não for vazia efetua uma busca pelo valor que se deseja descobrir o antecessor
    // Enquanto mantém um ponteiro para o último nó em que foi efetuado uma "virada para direita"
    // Que será o ancestral mais baixo que possui um nó filho à direita que é ancestral do nó buscado
    while (T)
    {
        if (T->key < key)
        {
            ancestralNode = T;
            T = T->right;
        }
        else if (T->key > key)
        {
            T = T->left;
        }
        else if (T->left && T->key == key)
        {
            // No caso do nó com o valor buscado tiver filho à direita, o antecessor
            // será o maior valor na sub-árvore esquerda dele
            return maxBinaryTree(T->left);
        }
        else if (T->key == key)
        {
            // Caso não exista a sub-árvore esquerda, o antecessor será o nó ancestral que foi mantido durante a busca
            return ancestralNode;
        }
    }
    return NULL;
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

// Estrutura para operações em Arquivo
struct s_arq_no
{
    int32_t chave : 30;
    uint32_t esq : 1;
    uint32_t dir : 1;
};

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

// Salva um nó da árvore em arquivo binário e salva os seus filhos recursivamente em pré-ordem
void saveNodesInFile(BinaryTree *T, FILE *dest)
{
    struct s_arq_no temp;
    temp.chave = T->key;
    temp.esq = T->left != NULL;
    temp.dir = T->right != NULL;
    fwrite(&temp, sizeof(struct s_arq_no), 1, dest);
    if (T->left)
    {
        saveNodesInFile(T->left, dest);
    }
    if (T->right)
    {
        saveNodesInFile(T->right, dest);
    }
}

// Prepara par salvar uma árvore em uma arquivo binário com nome "fileName"
void saveBinaryTreeInFile(BinaryTree *T, const char *fileName)
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
            //Apenas coloca um caracter zero se a árvore for vazia
            fputc(0, treeFIle);
        }
        fclose(treeFIle);
    }
}

// Aloca um nó a partir de uma árvore salva em arquivo e aloca os seus filhos recursivamente
void createNodesFromFile(BinaryTree **T, FILE *dest)
{
    struct s_arq_no temp;
    if (fread(&temp, sizeof(struct s_arq_no), 1, dest))
    {
        (*T) = newNodeBinaryTree(temp.chave);
        if (temp.esq)
        {
            createNodesFromFile(&(*T)->left, dest);
        }
        if (temp.dir)
        {
            createNodesFromFile(&(*T)->right, dest);
        }
    }
}

// Prepara a leitura de um arquivo com uma árvore e salva ele em "T"
void readBinaryTreeFromFile(BinaryTree **T, const char *fileName)
{
    FILE *treeFile = openFile(fileName, "rb");
    if (treeFile)
    {
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

// Desenha um nó
void drawNode(int x, int y, int key)
{
    char v[10];
    int largura, altura;
    gfx_set_color(255, 100, 255);
    gfx_filled_rectangle(x + 1, y + 1, x + g_NODE_WIDTH - 1, y + g_NODE_HEIGHT - 1);
    gfx_set_color(255, 255, 255);
    gfx_rectangle(x, y, x + g_NODE_WIDTH, y + g_NODE_HEIGHT);
    // Guarda o valor na string v
    sprintf(v, "%d", key);
    gfx_get_text_size(v, &largura, &altura);
    gfx_text(x + g_NODE_WIDTH / 2 - largura / 2, y + g_NODE_HEIGHT / 2 - altura / 2, v);
}

// Desenha uma árvore com centro em "x","y" e com distancia "dist" entre si 
void drawTree(BinaryTree *T, int x, int y, int dist)
{
    if (T)
    {
        dist /= 2;
        // Desenha recursivamente os filhos da esquerda e da direita
        if (T->left)
        {
            gfx_line(x, y + g_NODE_HEIGHT / 2, x - dist, y + g_NODES_DISTANCE + g_NODE_HEIGHT / 2);
            drawTree(T->left, x - dist, y + g_NODES_DISTANCE, dist);
        }
        if (T->right)
        {
            gfx_line(x, y + g_NODE_HEIGHT / 2, x + dist, y + g_NODES_DISTANCE + g_NODE_HEIGHT / 2);
            drawTree(T->right, x + dist, y + g_NODES_DISTANCE, dist);
        }
        drawNode(x - g_NODE_WIDTH / 2, y, T->key);
    }
}

int main()
{
    BinaryTree *root = NULL, *aux;
    char opcAcao, fileName[20];
    int inputkey, S;
    // Inicia a janela
    gfx_init(g_SCREEN_WIDTH, g_SCREEN_HEIGHT, "Arvore de Busca");
    while (1)
    {
        system("clear");
        printf("### Arvore Binaria de Busca ###\n[0] - Encerrar programa\
        \n[1] - Buscar Maior e Menor Chave;\
        \n[2] - Buscar Chave;\
        \n[3] - Buscar Antecessor e Sucessor;\
        \n[4] - Inserir Chave;\
        \n[5] - Remover Chave;\
        \n[6] - Ler Arquivo com Arvore;\
        \n[7] - Salvar Arvore em Arquivo;\
        \nDigite uma opção para escolher qual operação deve ser feita: ");
        opcAcao = readChar();

        switch (opcAcao)
        {
        case '0':
            gfx_quit();
            deleteBinaryTree(&root);
            return 0;
            break;
        case '1':
            if ((aux = maxBinaryTree(root)))
            {
                printf("\nA maior chave na árvore é: %d", aux->key);
            }
            if ((aux = minBinaryTree(root)))
            {
                printf("\nA menor chave na árvore é: %d\n", aux->key);
            }
            else
            {
                printf("\nA árvore não possui maior e nem menor chave pois está vazia!\n");
            }
            break;
        case '2':
            printf("\nInforme o valor da chave a ser buscada: ");
            scanf("%d", &inputkey);
            getchar();
            searchkeyBinaryTree(root, inputkey, &S);
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
        case '3':
            printf("\nInforme o valor da chave que deve ser encontrado o antecessor e sucessor: ");
            scanf("%d", &inputkey);
            getchar();
            if ((aux = predecessorNodeBinaryTree(root, inputkey)))
            {
                printf("O antecessor da chave é: %d\n", aux->key);
            }
            else
            {
                printf("A chave não possui antecessor!\n");
            }
            if ((aux = sucessorNodeBinaryTree(root, inputkey)))
            {
                printf("O sucessor da chave é: %d\n", aux->key);
            }
            else
            {
                printf("A chave não possui sucessor!\n");
            }
            break;
        case '4':
            printf("\nInforme o valor da chave que deve ser inserida: ");
            scanf("%d", &inputkey);
            getchar();
            if (insertKeyBinaryTree(&root, inputkey))
            {
                printf("A chave %d foi inserida com sucesso!\n", inputkey);
            }
            else
            {
                printf("Nao é possivel inserir valores repetidos na arvore de busca!\n");
            }
            break;
        case '5':
            printf("\nInforme o valor da chave que deve ser removida: ");
            scanf("%d", &inputkey);
            getchar();
            if (removeKeyBinaryTree(&root, inputkey))
            {
                printf("A chave %d foi removida com sucesso!\n", inputkey);
            }
            else
            {
                printf("A chave não estava presente na árvore!\n");
            }
            break;
        case '6':
            printf("\nInforme o nome do arquivo em que a árvore está salva: ");
            scanf("%s", fileName);
            getchar();
            deleteBinaryTree(&root);
            readBinaryTreeFromFile(&root, fileName);
            break;
        case '7':
            printf("\nInforme o nome do arquivo em que a árvore será salva: ");
            scanf("%s", fileName);
            getchar();
            saveBinaryTreeInFile(root, fileName);
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