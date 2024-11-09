// Trabalho T6: Compactação e Decompactação de Arquivos
// Nome: João Vitor Antunes da Silva 
// RGM: 48935

// OBS: A implementação da descompactação ficou incompleta. A árvore de Huffman equivalente ao arquivo compactado
// é decodificada, entretanto o algoritmo para por aí.

#include <stdio.h> // printf, fread e fwrite
#include <stdlib.h> // malloc e free
#include <stdint.h> // uint_8
#include <string.h> // strncat e strncpy

// Tipo que define 1 byte
typedef uint8_t Byte;

// Cabeçalho do arquivo codificado
typedef struct
{
    unsigned long fileInit;
    unsigned uselessBits : 3;
} Header;

// Lista encadeada de bytes
typedef struct ListNode
{
    struct ListNode *next;
    Byte character;
} List;

// Árovre de Huffman
typedef struct HuffmanNode
{
    struct HuffmanNode *children[2];
    unsigned frequency;
    Byte character;
} HuffmanTrie;

// Nó de uma tabela de dispersão com frequências e código equivalente ao byte
typedef struct
{
    unsigned frequency;
    unsigned long long code;
    short unsigned bitsActuallyUsed;
} TableNode;

// Tabela de dispersão que mapeia um byte ao seu código e frequênca no arquivo
typedef struct
{
    TableNode *nodes;
    List *characters;
    int n, m;
} Table;

// Lista de prioridade com nós de uma árvore de Huffman
typedef struct
{
    HuffmanTrie **array;
    int n, m;
} Heap;

//Cria um nó com um novo byte para uma lista encadeada
List *newListNode(Byte character)
{
    List *aux = malloc(sizeof(List));
    aux->next = NULL;
    aux->character = character;
    return aux;
}

// Insere um nó nma lista encadeada
void insertList(List **L, Byte newChar)
{
    List *aux;
    aux = *L;
    *L = newListNode(newChar);
    (*L)->next = aux;
}

// Desaloca os nós da Lista Encadeada Simples
List *deleteList(List *L)
{
    List *aux;
    // Enquanto não chegar no fim da lista, salva o próximo nó à *L em aux, desaloca *L
    // e reconecta *L com o restante da lista em aux
    while (L != NULL)
    {
        aux = (L)->next;
        free(L);
        L = aux;
    }
    return NULL;
}

// Cria uma tabela de dispersão com as frequências de todos os bytes de um dado texto
Table createFrequencyTable(Byte *text, int size)
{
    Table newTable;
    int i;
    newTable.nodes = calloc(256, sizeof(TableNode));
    newTable.m = 256;
    newTable.n = 0;
    newTable.characters = NULL;
    for (i = 0; i < size; i++)
    {
        if (newTable.nodes[(unsigned)text[i]].frequency == 0)
        {
            newTable.n++;
            insertList(&newTable.characters, text[i]);
        }
        newTable.nodes[(unsigned)text[i]].frequency++;
    }
    return newTable;
}

// Deleta uma tabela de dispersão
void deleteTable(Table *table)
{
    free(table->nodes);
    table->nodes = NULL;
    table->characters = deleteList(table->characters);
    table->m = 0;
    table->n = 0;
}

//Troca o endereço de dois nós de uma árvore de Huffman
void swapNode(HuffmanTrie **a, HuffmanTrie **b)
{
    HuffmanTrie *aux;
    aux = *b;
    *b = *a;
    *a = aux;
}

// Aloca um novo nó para uma árvore de Huffman
HuffmanTrie *newTrieNode(Byte newChar, unsigned newFreq)
{
    HuffmanTrie *aux = malloc(sizeof(HuffmanTrie));
    aux->character = newChar;
    aux->frequency = newFreq;
    aux->children[0] = aux->children[1] = NULL;
    return aux;
}

//Realiza uma descida do elemento de índice i num heap minimo
void descendHeap(Heap heap, int i)
{
    int j = 2 * i + 1, n = heap.n;
    if (j < n)
    {
        if (j < n - 1)
        {
            if (heap.array[j + 1]->frequency < heap.array[j]->frequency)
            {
                j++;
            }
        }
        if (heap.array[i]->frequency > heap.array[j]->frequency)
        {
            swapNode(&heap.array[i], &heap.array[j]);
            descendHeap(heap, j);
        }
    }
}

// Realiza uma subida do elemento no índice i de um heap minimo
void ascendHeap(Heap heap, int index)
{
    int j = (index - 1) / 2;
    if (j >= 0 && index)
    {
        if (heap.array[j]->frequency > heap.array[index]->frequency)
        {
            swapNode(&(heap.array[j]), &(heap.array[index]));
            ascendHeap(heap, j);
        }
    }
}

// Insere um novo nó de árvore de Huffman em um heap minimo
void insertHeap(Heap *heap, HuffmanTrie *newNode)
{
    if (heap->n < heap->m)
    {
        heap->array[heap->n] = newNode;
        ascendHeap(*heap, heap->n);
        heap->n++;
    }
}

// Remove um nó da Árvore de Huffman de um heap mínimo, mas sem desalocar ele da memória
HuffmanTrie *removeHeap(Heap *heap)
{
    HuffmanTrie *aux;
    if (heap->n)
    {
        aux = heap->array[0];
        heap->array[0] = heap->array[heap->n - 1];
        heap->n--;
        descendHeap(*heap, 0);
        return aux;
    }
    return NULL;
}

// Cria um heap mínimo a partir de uma tabela de dispersão com as frequências de cada byte
Heap createMinHeap(Table table)
{
    Heap newHeap;
    List *auxList = table.characters;
    newHeap.array = malloc(table.n * sizeof(HuffmanTrie *));
    newHeap.m = table.n;
    newHeap.n = 0;
    while (auxList)
    {
        insertHeap(&newHeap, newTrieNode(auxList->character, table.nodes[auxList->character].frequency));
        auxList = auxList->next;
    }
    return newHeap;
}

// Deleta um heap minimo
void deleteHeap(Heap *ptheap)
{
    free(ptheap->array);
    ptheap->array = NULL;
    ptheap->n = 0;
    ptheap->m = 0;
}

// Funde dois nós de uma árvore de Huffman
HuffmanTrie *mergeTries(HuffmanTrie *t1, HuffmanTrie *t2)
{
    HuffmanTrie *aux = newTrieNode('\0', t1->frequency + t2->frequency);
    aux->children[0] = t1;
    aux->children[1] = t2;
    return aux;
}

// Cria uma árvore de Huffman a partir dos nós armazenados em um heap minimo
HuffmanTrie *createHuffmanTrie(Heap *heap)
{
    HuffmanTrie *aux1, *aux2;
    while (heap->n > 1)
    {
        aux1 = removeHeap(heap);
        aux2 = removeHeap(heap);
        insertHeap(heap, mergeTries(aux2, aux1));
    }
    return heap->array[0];
}

// Desaloca uma árvore de Huffman
HuffmanTrie *deleteHuffmanTrie(HuffmanTrie *trie)
{
    if (trie)
    {
        if (trie->children[0])
        {
            deleteHuffmanTrie(trie->children[0]);
            deleteHuffmanTrie(trie->children[1]);
        }
        free(trie);
    }
    return NULL;
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

// Aloca um vetor e armazena o conteúdo de um arquivo binário nele e retorna um ponteiro para os dados
// e o tamanho do conjunto
Byte *readTextFromFile(const char *fileName, int *size)
{
    FILE *file = openFile(fileName, "rb");
    int bytes;
    Byte *t;
    if (file)
    {
        // Calcula o tamanho do arquivo a partir da quantidade de bytes do ínicio ao fim
        fseek(file, 0, SEEK_END);
        bytes = ftell(file);
        fseek(file, 0, SEEK_SET);
        // Aloca um vetor com o número de bytes no vetor
        t = malloc(bytes);
        // Salva todos os bytes no vetor
        fread(t, bytes, 1, file);
        fclose(file);
        *size = bytes;
        return t;
    }
    *size = 0;
    return NULL;
}

// Atribui os respectivos códigos a cada um dos bytes registrados na tabela de dispersão
void encodeBytes(HuffmanTrie *codesTrie, Table bytesTable, unsigned long long codesAux, unsigned bitsUsed)
{
    if (codesTrie)
    {
        if (codesTrie->children[0])
        {
            codesAux = codesAux << 1;
            bitsUsed++;
            encodeBytes(codesTrie->children[0], bytesTable, codesAux, bitsUsed);
            codesAux++;
            encodeBytes(codesTrie->children[1], bytesTable, codesAux, bitsUsed);
        }
        else
        {
            bytesTable.nodes[codesTrie->character].bitsActuallyUsed = bitsUsed;
            bytesTable.nodes[codesTrie->character].code = codesAux;
        }
    }
}

// Codifica em pré ordem em um arquivo uma dada árvore de Huffman
// Onde:
// 0 significa que o nó é interno
// 1 significa que o nó é uma folha e é seguido de um byte que representa o caracter na folha
unsigned long encodeHuffmanTrie(FILE *filePointer, HuffmanTrie *codesTrie, Byte *aux, Byte *used)
{
    Byte temp;
    static unsigned long size = 0;
    if (codesTrie->children[0])
    {
        if (*used == 8)
        {
            fwrite(aux, 1, 1, filePointer);
            size = size + 8;
            *aux = 0;
            *used = 0;
        }
        (*used)++;
        encodeHuffmanTrie(filePointer, codesTrie->children[0], aux, used);
        encodeHuffmanTrie(filePointer, codesTrie->children[1], aux, used);
    }
    else
    {
        *aux = (*aux) | (1 << (7 - *used));
        (*used)++;
        if (*used == 8)
        {
            fwrite(aux, 1, 1, filePointer);
            fwrite(&codesTrie->character, 1, 1, filePointer);
            size = size + 16;
            *aux = 0;
            *used = 0;
        }
        else
        {
            temp = (codesTrie->character) << (8 - *used);
            *aux = *aux | ((codesTrie->character) >> *used);
            fwrite(aux, 1, 1, filePointer);
            *aux = temp;
            size = size + 8;
        }
    }
    return size;
}

// Codifica um texto usando uma árvore de Huffman e escreve a compactação em um arquivo 
void encodeFile(char *fileName, HuffmanTrie *codesTrie, Table codesTable, Byte *text, int textSize)
{
    FILE *file = openFile(fileName, "w");
    Byte used = 0, aux = 0, codeShifted = 0, codeExtra = 0;
    Header h;
    TableNode auxNode;
    int i;
    h.fileInit = 0;
    h.uselessBits = 0;
    if (file)
    {
        fwrite(&h, sizeof(Header), 1, file);
        h.fileInit = encodeHuffmanTrie(file, codesTrie, &aux, &used);
        if (aux != 0)
        {
            h.fileInit += used;
        }
        for (i = 0; i < textSize; i++)
        {
            auxNode = codesTable.nodes[text[i]];
            if (used == 8)
            {
                fwrite(&aux, 1, 1, file);
                aux = 0;
                used = 0;
            }
            if (8 - (auxNode.bitsActuallyUsed + used) < 0)
            {
                codeShifted = auxNode.code >> ((auxNode.bitsActuallyUsed + used) - 8);
                aux = aux | codeShifted;
                fwrite(&aux, 1, 1, file);
                used = (auxNode.bitsActuallyUsed + used) - 8;
                codeExtra = auxNode.code << (8 - used);
                aux = codeExtra;
            }
            else
            {
                codeShifted = auxNode.code << (8 - (auxNode.bitsActuallyUsed + used));
                aux = aux | codeShifted;
                used += auxNode.bitsActuallyUsed;
                if (used == 8)
                {
                    fwrite(&aux, 1, 1, file);
                    aux = 0;
                    used = 0;
                }
            }
        }

        if (used != 0)
        {
            fwrite(&aux, 1, 1, file);
        }
        h.uselessBits = (8 - used) % 8;
        fseek(file, 0, SEEK_SET);
        fwrite(&h, sizeof(Header), 1, file);
        fclose(file);
    }
}

// Compacta os dados de um arquivo
void compressFile(char *filename)
{
    int size;
    Byte *text = readTextFromFile(filename, &size);
    Table bytesTable;
    Heap priorityListNodes;
    HuffmanTrie *codesTrie;
    char filenameTemp[80];
    if (text)
    {
        strncpy(filenameTemp, filename, strlen(filename));
        strncat(filenameTemp, ".cmp", 5);

        bytesTable = createFrequencyTable(text, size);
        priorityListNodes = createMinHeap(bytesTable);
        codesTrie = createHuffmanTrie(&priorityListNodes);
        encodeBytes(codesTrie, bytesTable, 0, 0);
        encodeFile(filenameTemp, codesTrie, bytesTable, text, size);
        deleteTable(&bytesTable);
        deleteHeap(&priorityListNodes);
        codesTrie = deleteHuffmanTrie(codesTrie);
        free(text);
    }
}

// Retira bit a bit os dados de um byte que é dado em uma stream 
// fazendo alterações necessárias nas informações sobre esse byte e essa stream
int processCode(Byte *code, unsigned *used, int *index, unsigned long *total)
{
    int r = *code >> 7;
    *code = *code << 1;
    (*used)--;
    (*total)++;
    if (!(*used))
    {
        *used = 8;
        (*index)++;
    }
    return r;
}

// Decodifica uma árovre de Huffman a partir de uma stream de byts em pré ordem 
HuffmanTrie *byteToHuffmanTrie(Byte *codeStream, unsigned *used, int *index, unsigned long *total, unsigned long max)
{
    int op;
    HuffmanTrie *aux = NULL;
    Byte newChar, newCharAux;
    if (codeStream)
    {
        if (used)
        {
            if (*total < max)
            {
                op = processCode(&codeStream[*index], used, index, total);
                if (!op)
                {
                    aux = newTrieNode('\0', 0);
                    aux->children[0] = byteToHuffmanTrie(codeStream, used, index, total, max);
                    aux->children[1] = byteToHuffmanTrie(codeStream, used, index, total, max);
                    return aux;
                }
                else
                {
                    if (*used == 8)
                    {
                        *total = *total + 8;
                        *used = 8;
                        newChar = codeStream[*index];
                        (*index)++;
                    }
                    else
                    {
                        newCharAux = codeStream[*index + 1] >> (*used);
                        newChar = codeStream[*index] | newCharAux;
                        (*index)++;
                        codeStream[*index] = codeStream[*index] << (8 - *used);
                        *total = *total + 8;
                    }
                    return newTrieNode(newChar, 0);
                }
            }
        }
    }
    return aux;
}

//Decodifica a árvore de Huffman escrita em um arquivo
void decompressFile(char *filename)
{
    FILE *file = openFile(filename, "r");
    HuffmanTrie *codesTrie;
    Header size;
    int indexAux = 0;
    unsigned long totalAux = 0, i;
    unsigned usedAux = 8;
    Byte *streamTemp;
    char filenameTemp[80];
    if (file)
    {
        strncpy(filenameTemp, filename, strlen(filename));
        strncat(filenameTemp, ".dcmp", 6);

        fread(&size, sizeof(Header), 1, file);
        streamTemp = malloc((size.fileInit / 8) + 1);
        for (i = 0; i <= size.fileInit / 8; i++)
        {
            fread(&(streamTemp[i]), 1, 1, file);
        }

        codesTrie = byteToHuffmanTrie(streamTemp, &usedAux, &indexAux, &totalAux, size.fileInit);
        codesTrie = deleteHuffmanTrie(codesTrie);
        free(streamTemp);
    }
}

// Programa principal
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Numero de argumentos invalido!\n");
        return 0;
    }
    if ((argv[1][0] != 'd') && (argv[1][0] != 'c'))
    {
        printf("Operacao invalida!\n");
        return 0;
    }

    if (argv[1][0] == 'c')
    {
        compressFile(argv[2]);
        printf("Arquivo foi compactado!\n");
    }
    else
    {
        decompressFile(argv[2]);
        printf("Descompactação incompleta...\n");
    }

    return 0;
}