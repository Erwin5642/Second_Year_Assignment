#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef int int32_t;

//Função auxiliar para reaizar trocas entre duas variáveis
void swap(int32_t *a, int32_t *b)
{
    int32_t aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

//Ordena um vetor usando bolha com implementação simples
void bubbleSortOriginal(int32_t *vector, int n)
{
    int i, j;
    //Percorre o vetor comparando cada elemento com todos os subsquentes á sua frente até encontrar um que seja menor
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            if (vector[j] > vector[j + 1])
            {
                swap(&vector[j], &vector[j+1]);
            }
        }
    }
}

//Ordena um vetor usando bolha com implementação melhorada
void bubbleSortImproved(int32_t *vector, int n)
{
    int i, last = n;
    char flag = 1;
    //Enquanto tiver havido trocas continua tentando ordenar o vetor
    while (flag)
    {
        flag = 0;
        //Ordena do ínicio até o índice do último elemento ordenado
        for (i = 0; i < last - 1; i++)
        {
            //Se for preciso a troca, faz e registra na flag
            if (vector[i] > vector[i + 1])
            {
                swap(&vector[i], &vector[i + 1]);
                flag = 1;
            }
        }
        //Salva a última iteração na variável last
        last = i;
    }
}
//Ordena um vetor usando inserção
void insertionSort(int32_t *vector, int n)
{
    int i, j;
    int32_t value;
    //Para todo elemento do vetor
    for(i = 1; i < n; i++){
        value = vector[i];
        j = i - 1;
        //Avança uma casa para frente valores menores que o de comparação até encontrar um menor
        while(j >= 0 && value < vector[j]){
            vector[j + 1] = vector[j];
            j--;
        }
        //Coloca o valor usado na comparação na nova posição dele
        vector[j + 1] = value;
    }    

}
//Intercala um vetor com ele mesmo de forma ordenada crescente
void intercalar(int32_t *vector, int n1, int n2)
{
    int i, j, k;
    //Aloca um vetor auxiliar temporário
    int32_t *temp = malloc((n1 + n2) * sizeof(int32_t));
    i = j = k = 0;
    //Enquanto houverem elementos na primeira e segunda parte do vetor
    while (i < n1 && j < n2)
    {
        //Se a primeira for menor que a segunda, registra ela no vetor temporário
        if (vector[i] < vector[j + n1])
        {
            temp[k] = vector[i];
            i++;
        }
        else
        {
            //Se a segunda for menor ou igual que a primeira, registra ela no vetor temporário
            temp[k] = vector[j + n1];
            j++;
        }
        //Avança o índice do vetor temporário
        k++;
    }
    //Salva o resto do vetores que podem ter sobrado
    while(i < n1){
        temp[k] = vector[i];
        i++;
        k++;
    }
    while(j < n2){
        temp[k] = vector[j + n1];
        j++;
        k++;
    }
    //Sobrescreve o vetor original com o vetor temporário
    for (i = 0; i < n1 + n2; i++)
    {
        vector[i] = temp[i];
    }
    free(temp);
}

//Ordena um vetor usando merge sort
void mergeSort(int32_t *vector, int n)
{
    //Se o vetor não for vazio
    if (n > 1)
    {
        //Ordena recursivamente a metade esquerda e direita do vetor recursivamente
        mergeSort(vector, (n + 1) / 2);
        mergeSort(vector + (n + 1) / 2, n / 2);
        //Intercala a primeira metade com a segunda metade
        intercalar(vector, (n + 1) / 2, n / 2);
    }
}

//Ordena um vetor usando quick sort com pivô último elemento
void quickSortEnd(int32_t *vector, int n)
{
    int pivot, i, j;
    int32_t pivotValue;
    if(n <= 2){
        if(n == 2){
            if(vector[0] > vector[1]){
                swap(&vector[0], &vector[1]);
            }
        }
        return;
    }           
    //Usa o último elemento como pivô
    pivot = n - 1;
    pivotValue = vector[pivot];
    i = 0;
    j = n - 2;
    while(j >= i){
        //Incrementa a partir do inicio se existem elementos maiores que o pivô e para se existir ou segue até o fim
        while((vector[i] < pivotValue) && (i <= n - 1)){
            i++;
        }
        //Decrementa a partir do fim se existem elementos menores que o pivô e para se existir ou segue até o começo
        while((vector[j] > pivotValue) && (j >= 0)){
            j--;
        }
        //Se os indices se encontraram no meio, troca ambos e para
        if(j >= i){
            swap(&vector[i], &vector[j]);
            i++;
            j--;
        }
    }
    swap(&vector[i], &vector[n - 1]);
    quickSortEnd(vector, i);
    quickSortEnd(vector + i, n - i);
}

//Ordena um vetor usando quick sort com pivô aleatório
void quickSortRand(int32_t *vector, int n)
{
    int pivot, i, j;
    int32_t pivotValue;
    //Caso base: vetor com tamanho 2, simplesmente deixa ou coloca o menor elemento na primeira posição
    if(n <= 2){
        if(n == 2){
            if(vector[0] > vector[1]){
                swap(&vector[0], &vector[1]);
            }
        }
        return;
    }           
    //Determina um indice aleatório para ser o pivô
    pivot = rand() % n;
    pivotValue = vector[pivot];
    swap(&vector[pivot], &vector[n - 1]);
    i = 0;
    j = n - 2;
    while(j >= i){
        //Incrementa a partir do inicio se existem elementos maiores que o pivô e para se existir ou segue até o fim
        while((vector[i] < pivotValue) && (i <= n - 1)){
            i++;
        }
        //Decrementa a partir do fim se existem elementos menores que o pivô e para se existir ou segue até o começo
        while((vector[j] > pivotValue) && (j >= 0)){
            j--;
        }
        //Se os indices se encontraram no meio, troca ambos e para
        if(j >= i){
            swap(&vector[i], &vector[j]);
            i++;
            j--;
        }
    }
    swap(&vector[i], &vector[n - 1]);
    //Repete o algoritmo a esquera e a direita do pivo
    quickSortRand(vector, i);
    quickSortRand(vector + i, n - i);
}

//Ordena um vetor usando quick sort com mediana de três
void quickSortMedian(int32_t *vector, int n)
{
    
    int i, j;
    int32_t pivotValue;
    //Caso base: vetor com tamanho 2, simplesmente deixa ou coloca o menor elemento na primeira posição
    if(n <= 2){
        if(n == 2){
            if(vector[0] > vector[1]){
                swap(&vector[0], &vector[1]);
            }
        }
        return;
    }
    //Determina qual elemento é a mediana entre o inicio, meio e fim e troca com o último elemento
    if(vector[n/2] > vector[0]){
        if(vector[0] > vector[n - 1]){
            swap(&vector[0], &vector[n-1]);
        }
        else{
            swap(&vector[n/2], &vector[n-1]);
        }
    }
    else{
        if(vector[0] > vector[n - 1]){
            swap(&vector[n/2], &vector[n-1]);
        }
        else{
            swap(&vector[0], &vector[n-1]);
        }
    }
    //Valor do pivo recebe o último elemento
    pivotValue = vector[n - 1];
    i = 0;
    j = n - 2;
    while(j >= i){
        //Incrementa a partir do inicio se existem elementos maiores que o pivô e para se existir ou segue até o fim
        while((vector[i] < pivotValue) && (i <= n - 1)){
            i++;
        }
        //Decrementa a partir do fim se existem elementos menores que o pivô e para se existir ou segue até o começo
        while((vector[j] > pivotValue) && (j >= 0)){
            j--;
        }
        //Se os indices se encontraram no meio, troca ambos e para
        if(j >= i){
            swap(&vector[i], &vector[j]);
            i++;
            j--;
        }
    }
    //Volta o pivô para o último elemento menor que ele
    swap(&vector[i], &vector[n - 1]);
    //Repete o algoritmo a esquera e a direita do pivo
    quickSortMedian(vector, i);
    quickSortMedian(vector + i, n - i);
}

//Faz a descida de um elemento seguindo a estrutura de um heap
void descend(int32_t *vector, int i, int n){
    int j = 2 * i + 1;
    if(j < n){
        if(j < n - 1){
            if(vector[j + 1] > vector[j]){
                j++;
            }
        }
        if(vector[i] < vector[j]){
            swap(&vector[i], &vector[j]);
            descend(vector, j, n);
        }
    }
}

//Organiza um vetor num heap
void arrange(int32_t *vector, int n){
    int i;
    for(i = (n - 1)/2; i >= 0; i--){
        descend(vector, i, n);
    }
}

//Ordena um vetor utilizando o heap sort
void heapSort(int32_t *vector, int n){
    arrange(vector, n);

    while (n - 1 > 0)
    {
        swap(&vector[0], &vector[n - 1]);
        n--;
        descend(vector, 0, n);
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

//Aloca um vetor e armazena o conteúdo de um arquivo binário nele e retorna um ponteiro para os dados 
//e o tamanho do conjunto
int32_t *readVectorFromFile(const char *fileName, int *size)
{
    FILE *file = openFile(fileName, "rb");
    int bytes;
    int32_t *vector;
    if(file){
        //Calcula o tamanho do arquivo a partir da quantidade de bytes do ínicio ao fim
        fseek(file, 0, SEEK_END);
        bytes = ftell(file);
        fseek(file, 0, SEEK_SET);
        //Aloca um vetor com o número de bytes no vetor
        vector = malloc(bytes);
        //Salva todos os bytes no vetor
        fread(vector, bytes, 1, file);
        fclose(file);
        //Divide o número de bytes por 4 (32 bits) para obter o tamanho do conjunto
        *size = bytes/4;
        return vector;
    }
    *size = 0;
    return NULL;
}

//Salva o conteúdo de um vetor em um arquivo binário
int saveVectorInFile(int32_t *vector, int n, const char *fileName)
{
    FILE *file = openFile(fileName, "wb");
    if (file)
    {
        fwrite(vector, sizeof(int32_t), n, file);
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int opc, size;
    char inputFileName[70], outputFileName[70];
    int32_t *vetor;
    clock_t t;
    //FILE *saida;
    srand(time(NULL));
    if(argc != 4){
        printf("Numero de argumentos invalido!\n");
        return 0;
    }
    opc = atoi(argv[1]);
    if(opc > 8 || opc < 1){
        printf("Opção de ordenação invalida!\n");
        return 0;
    }
    strcpy(inputFileName, argv[2]);
    strcpy(outputFileName, argv[3]);
    if(!(vetor = readVectorFromFile(inputFileName, &size))){
        printf("Houve um erro com o arquivo de entrada!\n");
        return 0;
    }
    t = clock();
    switch (opc)
    {
    case 1:
        bubbleSortOriginal(vetor, size);
        break;
    case 2:
        bubbleSortImproved(vetor, size);
        break;
    case 3:
        insertionSort(vetor, size);
        break;
    case 4:
        mergeSort(vetor, size);
        break;
    case 5:
        quickSortEnd(vetor, size);
        break;
    case 6:
        quickSortRand(vetor, size);
        break;
    case 7:
        quickSortMedian(vetor, size);
        break;
    case 8:
        heapSort(vetor, size);   
        break;
    }
    //calcula a variação de clocks antes e depois
    t = clock() - t;
    printf("O algoritmo levou %lf segundos\n", ((double)t) / CLOCKS_PER_SEC);
    //Salva o vetor ordenado no arquivo
    saveVectorInFile(vetor, size, outputFileName);
    free(vetor);
    return 0;
}