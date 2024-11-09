#include "../gfx.h"
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep  */

//Define a constante raiz de 2
#define sqrt2 1.41421

//Objetivo: desenhar circulos recursivamente
//Parametros: coordenadas x e y do centro do círculo, tamanho r do raio, e nivel de recursividade rec
//Retorno: nenhum
void desenharCirc(double  x, double y, double r, int rec){
	//Chama a função de novo enquanto houver recursão planejada
	if(rec > 0){
		rec--; 
		//Calcula a distância entre o centro do circulo atual e do centro dos 
		//próximos circulos recursivamente definidos
		double d = sqrt2 * r/4;
		desenharCirc(x + d, y + d, r/2, rec);
		desenharCirc(x + d, y - d, r/2, rec);
		desenharCirc(x - d, y - d, r/2, rec);
		desenharCirc(x - d, y + d, r/2, rec);
	}
	//Após fazer a chamada do próximo círculo recursivo, ou não haverem mais recursões para serem feitas,
	//desenha um circulo com centro em x e y e raio r
	gfx_set_color(255, 255, 255);
	gfx_ellipse(x, y, r, r);
}

//Objetivos: desenhar quadrados recursivamente
//Parametros: coordenadas x e y do centro do quadrado, menor distância entre o centro e qualque lado 
//do quadrado (ratio), recursividade requisitada rec
//Retorno: nenhum
void desenhaQuad(double x, double y, double ratio, double rec){
	//Enquanto houverem recursões para serem feitas, chama a função novamente
	if(rec > 0){
		rec--;
		desenhaQuad(x, y-3*ratio/2, ratio/2, rec);
		desenhaQuad(x, y+3*ratio/2, ratio/2, rec);
		desenhaQuad(x+3*ratio/2, y, ratio/2, rec);
		desenhaQuad(x-3*ratio/2, y, ratio/2, rec);
	}
	//Após fazer a chamada do próximo quadrado recursivo, ou não haverem mais recursões para serem feitas,
	//desenha um quadrao com centro em x e y e diagonal de tamanho 2 * sqrt2 * ratio
	gfx_set_color(255, 255, 255);
	gfx_rectangle(x-ratio, y-ratio, x + ratio, y + ratio);
	//Sobrepoe os quadrados desenhados em cima de outros
	gfx_set_color(0, 0, 0);
	gfx_filled_rectangle(x-ratio+1, y-ratio+1, x+ratio-1, y+ratio-1);
}

int main()
{
	int escolha, recursividade;
	printf("Informe o fractal que deverá ser desenhado!\n1 - Circulo:\n2- Quadrado:\n");
	scanf("%d", &escolha);

	//Caso a escolha seja diferente de 1 ou 2, trata como se o quadrado tivesse sido escolhido
	if(escolha == 1){
		printf("Fractal circulo escolhido!\n");
	}
	else{
		printf("Fractal quadrado escolhido!\n");
	}
	printf("Informe agora o nivel de recursividade desejado:\n");
	scanf("%d", &recursividade);
	//Se a recursividade for menor ou igual a zero, não haverá chamada recursiva
	//e se for maior que 10 trata como se fosse 10
	recursividade > 10 ? recursividade = 10: 1;
	//Inicia uma janela
	gfx_init(600, 600, "Fractais");
	//Chama a função requisitada pelo usuário de acordo
	escolha == 1 ? desenharCirc(300, 300, 280, recursividade) : desenhaQuad(300, 300, 100, recursividade);
	//Desenha na tela os objetos criados pelas funções
	gfx_paint();
	//Pausa o programa por 5 segundos
	sleep(5);
	//Encera a janela após 5 segundos
	gfx_quit();

	return 0;
}
