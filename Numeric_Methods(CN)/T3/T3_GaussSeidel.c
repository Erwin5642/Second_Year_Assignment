#include <stdio.h>
#include <time.h>

typedef struct
{
    double matrizExtendida[10][11];
    unsigned short nVariaveis;
    unsigned short nConstantes;
    double vetorSolucao[10];
} SistemaLinear;

double absValue(double x)
{
    return x >= 0 ? x : -x;
}

void copiaVetor(double vetorDest[10], double vetorSrc[10], unsigned short n)
{
    while (n--)
    {
        vetorDest[n] = vetorSrc[n];
    }
}

double maxAbsVetor(double vetor[10], unsigned short n)
{
    double absMaior = absValue(vetor[0]), absVetor;
    while (n-- >= 1)
    {
        absVetor = absValue(vetor[n]);
        absMaior = (absMaior < absVetor) ? absVetor : absMaior;
    }
    return absMaior;
}

double calculaDKR(double solucaoAtual[10], double dk, unsigned short n)
{
    return dk / maxAbsVetor(solucaoAtual, n);
}

double calculaDK(double solucaoAtual[10], double solucaoAnterior[10], unsigned short n)
{
    double solucaoAux[10];
    unsigned short m = n;
    while (m--)
    {
        solucaoAux[m] = solucaoAtual[m] - solucaoAnterior[m];
    }
    return maxAbsVetor(solucaoAux, n);
}

void trocaLinha(double linha1[10], double linha2[10], unsigned short n)
{
    double linhaAux;
    while (n--)
    {
        linhaAux = linha1[n];
        linha1[n] = linha2[n];
        linha2[n] = linhaAux;
    }
}

int podeConvergir(double linha[10], unsigned short n)
{
    unsigned short i, j;
    double somaLinha;
    for (i = 0; i < n; i++)
    {
        somaLinha = 0;
        for (j = 0; j < n; j++)
        {
            if (j != i)
            {
                somaLinha += absValue(linha[j]);
            }
        }
        if (somaLinha <= absValue(linha[i]))
        {
            return i;
        }
    }
    return -1;
}

int convergeSistema(SistemaLinear *Axb)
{
    int i, n = Axb->nVariaveis, flag;
    for (i = 0; i < n; i++)
    {
        if ((flag = podeConvergir(Axb->matrizExtendida[i], n)) != -1)
        {
            if (flag != i)
            {
                trocaLinha(Axb->matrizExtendida[i], Axb->matrizExtendida[flag], n + 1);
                i = -1;
            }
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

int gaussSeidel(SistemaLinear *Axb, double erro)
{
    unsigned short i, j, n = Axb->nVariaveis, m = Axb->nConstantes, it = 1;
    double soma, erroAtual, solucaoAnterior[10];
    if (!convergeSistema(Axb))
    {
        return 0;
    }
    printf("Solução X(0): ");
    for(i = 0; i < n; i++){
        printf("%lf ", Axb->vetorSolucao[i]);
    }
    printf("\n");
    do
    {
        for (i = 0; i < n; i++)
        {
            soma = 0;
            copiaVetor(solucaoAnterior, Axb->vetorSolucao, m);
            for (j = 0; j < m; j++)
            {
                if (j != i)
                {
                    soma += Axb->matrizExtendida[i][j] * Axb->vetorSolucao[j];
                }
            }
            Axb->vetorSolucao[i] = (Axb->matrizExtendida[i][j] - soma) / Axb->matrizExtendida[i][i];
        }

        printf("Solução X(%hu): ", it++);
        for(i = 0; i < n; i++){
            printf("%lf ", Axb->vetorSolucao[i]);
        }
        printf("\n");
        erroAtual = calculaDKR(Axb->vetorSolucao, calculaDK(Axb->vetorSolucao, solucaoAnterior, m), m);
    } while (erro < erroAtual);
    return 1;
}

int main()
{
    SistemaLinear Axb;
    clock_t t;
    double tempo, erro;
    unsigned short i, j;
    do
    {
        printf("Informe o numero de variaveis e de constantes:\n");
        scanf("%hu %hu", &Axb.nVariaveis, &Axb.nConstantes);
        if (Axb.nConstantes && Axb.nVariaveis)
        {
            printf("Informe a matriz extendida do sistema:\n");
            for (i = 0; i < Axb.nConstantes; i++)
            {
                for (j = 0; j < Axb.nVariaveis + 1; j++)
                {
                    scanf("%lf", &Axb.matrizExtendida[i][j]);
                }
            }
            printf("Informe a solução inicial:\n");
            for (i = 0; i < Axb.nConstantes; i++)
            {
                scanf("%lf", &Axb.vetorSolucao[i]);
            }
            printf("Informe o erro:\n");
            scanf("%lf", &erro);
            t = clock();
            if (gaussSeidel(&Axb, erro))
            {

                t = clock() - t;
                tempo = ((double)t) / CLOCKS_PER_SEC;

                printf("O resultado encontrado foi: ");
                for (i = 0; i < Axb.nVariaveis; i++)
                {
                    printf("%lf ", Axb.vetorSolucao[i]);
                }
                printf("\n\nO algoritmo levou %lf segundos\n----------------------\n\n", tempo);
            }
            else
            {
                printf("O sistema não pode convergir!\n\n");
            }
        }
    } while (Axb.nConstantes && Axb.nVariaveis);

    return 0;
}
