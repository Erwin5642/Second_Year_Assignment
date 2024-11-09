#include <stdio.h>
#include <time.h>

double mod(double v)
{
    return v >= 0 ? v : -v;
}

// Objetivo: multiplica uma linha de uma matriz por um escalar
void escalarLinha(double escalar, double linha[10], int n)
{
    while (n--)
    {
        linha[n] *= escalar;
    }
}

// Objetivo: soma duas linhas e salva o resultado na linhaDest
void somaLinha(double linhaDest[10], double linhaSrc[10], int n)
{
    while (n--)
    {
        linhaDest[n] += linhaSrc[n];
    }
}

// Objetivo: troca os valores entre duas linhas
void trocaLinha(double linha1[10], double linha2[10], int n)
{
    double linhaAux;
    while (n--)
    {
        linhaAux = linha1[n];
        linha1[n] = linha2[n];
        linha2[n] = linhaAux;
    }
}

// Objetivo: mostra os valores em uma matriz de forma formatada
void mostrarMatriz(double matriz[10][10], unsigned short n)
{
    unsigned short i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%.1lf ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Objetivo: calcula o determinante de uma matriz
double determinante(double matriz[10][10], unsigned short n)
{
    unsigned short i, j = 0, p = 1, indicePivo;
    double det = 1, cm, pivo;
    printf("Passo 0:\n");
    mostrarMatriz(matriz, n);
    for (j = 0; j < n; j++)
    {
        // Procura o maior numero em módulo
        pivo = matriz[j][j];
        indicePivo = j;
        for (i = j + 1; i < n; i++)
        {
            if (mod(matriz[i][j]) > mod(pivo))
            {
                indicePivo = i;
            }
        }
        if (matriz[indicePivo][j])
        {
            //  Se o pivo não estiver na primeira linha, troca as linhas e atualiza a direção do determinante
            if (indicePivo != j)
            {
                trocaLinha(matriz[indicePivo], matriz[j], n);
                det *= -1;
                printf("Passo %u:\n", p);
                mostrarMatriz(matriz, n);
                p++;
            }

            for (i = j + 1; i < n; i++)
            {
                if (matriz[i][j])
                {
                    printf("Passo %u:\n", p);
                    p++;

                    cm = matriz[i][j] / matriz[j][j];
                    escalarLinha(-cm, matriz[j] + j, n - j);
                    somaLinha(matriz[i] + j, matriz[j] + j, n - j);
                    // Volta a linha com os valores anteriores
                    escalarLinha(-1 / cm, matriz[j] + j, n - j);
                    // Mostra o resultado do passo p
                    mostrarMatriz(matriz, n);
                }
            }
        }
    }
    // Calcula o determinante baseado na diagonal matriz triangular resultante
    for (i = 0; i < n; i++)
    {
        det *= matriz[i][i];
    }
    return det;
}

int main()
{
    double matriz[10][10], tempo, det;
    unsigned i, j, n;
    clock_t t;
    do
    { // Le o tamanho da matriz quadrada
        // Le os valores da matriz
        printf("Insira a ordem da matriz quadrada:\n");
        scanf("%u", &n);
        if (n)
        {

            printf("Insira os valores da matriz:\n");
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    scanf("%lf", &matriz[i][j]);
                }
            }
            t = clock();
            det = determinante(matriz, n);
            // Calcula o tempo levado pela função
            t = clock() - t;
            tempo = ((double)t) / CLOCKS_PER_SEC;
            // Exibe os resultados do calculo do determinante da matriz
            printf("O resultado encontrado foi %lf\n\n", det);
            printf("O algoritmo levou %lf segundos\n----------------------\n\n", tempo);
        }
    } while (n != 0);

    return 0;
}