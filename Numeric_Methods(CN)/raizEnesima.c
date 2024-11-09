#include <stdio.h>
#include <time.h>

double mod(double x)
{
    return x >= 0.0 ? x : -x;
}

double pot(double x, double n)
{
    double s = 1;
    while (n--)
    {
        s *= x;
    }
    return s;
}

double metodoNewtonRaphson(double p, double m)
{
    double erro = 1.0, xk_1, xk;
    xk_1 = 1.0;
    int i = 1;
    printf("Iteração 0: %.16lf\n", xk_1);
    while (erro != 0.0)
    {
        xk = xk_1;
        xk_1 = (1.0 / m) * ((m - 1.0) * xk_1 + (p / (pot(xk_1, m - 1.0))));
        erro = mod(xk_1 - xk);
        if (erro)
        {
            printf("Iteração %d: %.16lf\n", i++, xk_1);
        }
    }
    return xk_1;
}

int main()
{
    int opc;
    unsigned m;
    double p, tempo;
    clock_t t;
    do
    {
        printf("Insira o radicando e o indice da raiz para calcular a raiz enesima:\n[Radicando]: ");
        scanf("%lf", &p);
        printf("[Indice]: ");
        scanf("%u", &m);
        t = clock();
        metodoNewtonRaphson(p, m);
        t = clock() - t;
        tempo = ((double)t) / CLOCKS_PER_SEC;
        printf("O algoritmo levou %lf segundos\n----------------------\n", tempo);
        printf("\nDeseja realizar outro calculo?\n[0] - Nao\n[1] - Sim\n-:");
        scanf("%d", &opc);
    } while (opc);

    return 0;
}