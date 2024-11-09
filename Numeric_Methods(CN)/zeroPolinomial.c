#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    unsigned grau;
    double *coeficientes;
} polinomio;

typedef struct {
    int a;
    int b;
} intervalo;


double mod(double x)
{
    return x >= 0.0 ? x : -x;
}

double pot(double x, unsigned n)
{
    double s = 1;
    while (n--)
    {
        s *= x;
    }
    return s;
}

double f(polinomio P, double x)
{
    unsigned i;
    double s = 0;
    for (i = 0; i <= P.grau; i++)
    {
        s += P.coeficientes[i] * pot(x, i);
    }
    return s;
}

intervalo *encontraRaizes(polinomio P, intervalo inicial, unsigned* numeroRaizes) {
    int i, j = 0, flag;
    intervalo *intervalos = (intervalo*)malloc(sizeof(intervalo) * P.grau);
    for(i = inicial.a; i < inicial.b; i++) {
        if(f(P, i) * f(P, i + 1) < 0) {
            flag = 1;
            intervalos[j].a = i;
            intervalos[j].b = i + 1;
            j++;
        }
    }
    *numeroRaizes = j;
    if(flag) {
        return intervalos;
    }
    free(intervalos);
    return NULL;
}

int main()
{
    unsigned i, opc, nRaizes;
    intervalo *intervalos, inicial;
    polinomio P;
    clock_t t;
    do
    {
        printf("Insira o grau do polinômio:\n[grau]: ");
        scanf("%u", &P.grau);
        P.coeficientes = (double*)malloc(sizeof(double) * (P.grau + 1));
        printf("Insira cada um dos coeficientes do polinomio:\n");
        for (i = 0; i <= P.grau; i++)
        {
            printf("[C%u]: ", i);
            scanf("%lf", &P.coeficientes[i]);
        }
        printf("Insira o intervalo inicial:\n[a]: ");
        scanf("%d", &inicial.a);
        printf("[b]: ");
        scanf("%d", &inicial.b);
        t = clock();
        intervalos = encontraRaizes(P, inicial, &nRaizes);
        t = clock() - t;
        if (intervalos) {
            printf("%d raizes encontradas:\n", nRaizes);
            for(i = 0; i < nRaizes; i++) {
                printf("[%d,%d] => f(%d) = %lf      f(%d) = %lf\n", intervalos[i].a, intervalos[i].b,intervalos[i].a,f(P, intervalos[i].a),intervalos[i].b,f(P, intervalos[i].b));
            }
            free(intervalos);
        }
        else
        {
            printf("Nenhuma raiz no intervalo\n");
        }
        free(coeficientes);
        printf("\nDeseja realizar outro calculo?\n[0] - Nao\n[1] - Sim\n-:");
        scanf("%u", &opc);
    }
    while (opc);

    return 0;
}