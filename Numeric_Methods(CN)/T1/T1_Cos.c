#include <stdio.h>  //printf
#include <time.h>   //clock
#include <stdlib.h> //system

// Pi
const long double pi = 3.14159265358979;

// Objetivo: calcula o fatorial de um número n
long double fatorial(long double n)
{
    long double i = 1;
    // Multiplica o numero pelos seus antecessores
    while (n > 1)
    {
        i *= n--;
    }
    return i;
}

// Objetivo: calcula a potencia de um número x elevado a n
long double potencia(long double x, int n)
{
    long double i = 1;
    // Multiplica o número por ele mesmo n vezes
    while (n-- > 0)
    {
        i *= x;
    }
    return i;
}

// Objetivo: calcula o cosseno de um dado ângulo em radianos
long double cosseno(long double rad, int nTermos)
{
    long double res = 0, i = 0, h = 1;
    // Caso o ângulo seja muito grande, reduz ele para um ângulo menor equivalente
    // aumentando a precisão e reduzindo overflows
    while (rad > 10 * pi)
    {
        rad -= 2 * pi;
    }

    // Calcula a série do consseno com n termos
    while (nTermos-- > 0)
    {
        res += h * potencia(rad, i) / fatorial(i);
        i += 2;
        h *= -1;
    }
    return res;
}

int main()
{
    long double x, cos, t;
    int n, a, b;
    clock_t temp;
    do
    {
        system("clear");
        printf("##### Cosseno #####\nInsira a razão que multilicada por Pi representa o angulo em radianos ou 0/0 para encerrar o programa: ");
        scanf("%d/%d", &a, &b);
        if (b != 0)
        {
            x = a * pi / b;
            printf("Insira o numero de termos utilizado pela função cosseno: ");
            scanf("%d", &n);
            // Temp recebe o instante atual
            temp = clock();
            cos = cosseno(x, n);
            // Temp recebe a diferença entre o istante anterior e posterior a função cosseno
            temp = clock() - temp;
            // Calcula-se o tempo em segundos
            t = ((long double)temp) / CLOCKS_PER_SEC;

            printf("Cosseno de %Lf eh: %Lf.\n\nO algoritmo levou %Lf segundos\n", x, cos, t);
            getchar();
            printf("Pressione enter para continuar...\n");
            getchar();
        }
    } while (b != 0);
    return 0;
}