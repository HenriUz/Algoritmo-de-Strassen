/*
main.c

Descrição: este código tem o objetivo de implementar o algoritmo de strassen para multiplicação de matrizes. A intenção é simular um arquivo.ppm com codificação P3 para aplicação de filtros.

A representação da matriz será por meio de um vetor contínuo da estrutura píxel, para evitar cache miss, e consequentemente rodar mais rápido.
Para acessar um elemento precisamos fazer linha*(dimensão total da matriz) + coluna. A solução para sempre sabermos a dimensão total, foi utilizar uma estrutura e ficar passando como parâmetro.

Importante: na maioria dos loops, está sendo feito somente uma verificação, mesmo com muitas variáveis sendo declaradas. Isso acontece pelo fato do intervalo (i2 - i1 ou j2 - j1) sempre ser o mesmo, independente do quadrante.
*/


/* --- Includes. --- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Estruturas. --- */

typedef struct dimensao {
    int dO, dF, dR; //Dimensão das matrizes O, F e R.
}dimensao;

typedef struct intervalo {
    int i1, i2; //Linha inicial e linha final.
    int j1, j2; //Coluna inicial e coluna final.
}intervalo;

typedef struct pixel {
    int r, g, b; //Os três elementos de um píxel.
}pixel;

/* --- Funções. --- */

/*
Descrição: função que imprime uma matriz nxn.
Entrada: matriz, dimensão n.
Saída: nada.
*/
void imprimeMatriz(pixel *matriz, int n) {
    int elemento; //Elemento que estamos acessando, no caso vai ser linha * n + coluna.

    /* Percorrendo a matriz. */
    for(int i = 0; i < n; i++) { //Percorrendo as linhas.
        for(int j = 0; j < n; j++) { //Percorrendo as colunas.
            elemento = i * n + j;
            printf("%d %d %d ", matriz[elemento].r, matriz[elemento].g, matriz[elemento].b);
        }
        printf("\n");
    }
}

/*
Descrição: função responsável por multiplicar as matrizes O e F, e salvar o resultado na matriz R.
Entrada: matriz original, matriz filtro, matriz resultante, intervalo da matriz original, intervalo da matriz filtro, dimensão das matrizes.
Saída: nada.
*/
void multiplicarMatrizes(pixel *matrizO, pixel *matrizF, pixel *matrizR, intervalo inO, intervalo inF, dimensao dim) {
    int elementoR, elementoO, elementoF; //Elemento que estamos acessando das respectivas matrizes R, O, F.

    /* Percorrendo as matrizes e realizando a multiplicação. */
    for(int iO = inO.i1, iR = 0; iO < inO.i2; iO++, iR++) { //Percorrendo as linhas (iO - linha da matrizO, iR - linha da matrizR).
        for(int jF = inF.j1, jR = 0; jF < inF.j2; jF++, jR++) { //Percorrendo as colunas (jF - coluna da matrizF, jR - coluna da matrizR).
            elementoR = iR * dim.dR + jR;
            matrizR[elementoR].r = 0;
            matrizR[elementoR].g = 0;
            matrizR[elementoR].b = 0;
            for(int linha = inF.i1, coluna = inO.j1; linha < inF.i2; linha++, coluna++) { //Percorrendo os elementos das linhas da matrizF e colunas da matrizO.
                elementoO = iO * dim.dO + coluna;
                elementoF = linha * dim.dF + jF;
                matrizR[elementoR].r += matrizO[elementoO].r * matrizF[elementoF].r;
                matrizR[elementoR].g += matrizO[elementoO].g * matrizF[elementoF].g;
                matrizR[elementoR].b += matrizO[elementoO].b * matrizF[elementoF].b;
            }
        }
    }
}

/*
Descrição: função responsável por somar ou subtrair dois quadrantes da matrizO. O resultado é salvo na matrizR.
Entrada: matrizO (a matriz que contém os quadrantes), matrizR (a matriz onde será salvo a soma), intervalo do quadrante 1, intervalo do quadrante 2, operação que será realizada, dimensão das matrizes.
Saída: nada.
Observação: sempre que essa função é chamada, a matriz resultante é uma das matrizes criadas no Strassen, logo ela sempre será acessada por 0 até o intervalo do quadrante.
*/
void operacaoBasica(pixel *matrizO, pixel *matrizR, intervalo in1, intervalo in2, char operacao, dimensao dim) {
    int elementoR, elemento1, elemento2; //Elemento que estamos acessando das respectivas matrizes R, O quadrante 1, O quadrante 2.

    for(int i = 0, i1 = in1.i1, i2 = in2.i1; i1 < in1.i2; i++, i1++, i2++) { //Percorrendo as linhas.
        for(int j = 0, j1 = in1.j1, j2 = in2.j1; j1 < in1.j2; j++, j1++, j2++) { //Percorrendo as colunas.
            elementoR = i * dim.dR + j;
            elemento1 = i1 * dim.dO + j1;
            elemento2 = i2 * dim.dO + j2;
            if(operacao == '+') {
                matrizR[elementoR].r = matrizO[elemento1].r + matrizO[elemento2].r;
                matrizR[elementoR].g = matrizO[elemento1].g + matrizO[elemento2].g;
                matrizR[elementoR].b = matrizO[elemento1].b + matrizO[elemento2].b;
            }else {
                matrizR[elementoR].r = matrizO[elemento1].r - matrizO[elemento2].r;
                matrizR[elementoR].g = matrizO[elemento1].g - matrizO[elemento2].g;
                matrizR[elementoR].b = matrizO[elemento1].b - matrizO[elemento2].b;
            }
        }
    }
}

/*
Descrição: função responsável por multiplicar as matrizes O e F, usando o algoritmo de Strassen (reduz o problema em 7 subproblemas recursivamente). O resultado é salvo na matriz R
Entrada: matriz original, matriz filtro, matriz resultante, intervalo da matriz original, intervalo da matriz filtro, dimensão do quadrante atual, dimensão das matrizes.
Saída: nada.
*/
void strassen(pixel *matrizO, pixel *matrizF, pixel *matrizR, intervalo inO, intervalo inF, int n, dimensao dim) {
    intervalo inqA, inqB, inqC, inqD, inqE, inqF, inqG, inqH; //Intervalos dos quadrantes.
    intervalo in0new; //Intervalo das matrizes de operações básicas e de produtos (0 até newN).
    pixel *P1, *P2, *P3, *P4, *P5, *P6, *P7; //Os sete produtos.
    pixel *bas1, *bas2; //Matrizes resultantes das 10 operações básicas. A segunda só será usada nos casos de produtos de duas operações.
    dimensao newDim; //Dimensão para as novas matrizes.
    int elemento, elemento2; //Auxiliares para contas repetitivas.
    int newN = n/2; //Nova dimensão (newN x newN).

    if(n <= 64) {
        multiplicarMatrizes(matrizO, matrizF, matrizR, inO, inF, dim); //Multiplicando as matrizes com o algoritmo primitivo.
    }else {
        /* Setando o intervalo das novas matrizes. */
        in0new.i1 = in0new.j1 = 0;
        in0new.i2 = in0new.j2 = newN;

        /* Setando os intervalos dos quadrantes A, B, C, e D. */
        inqA.i1 = inqB.i1 = inO.i1;
        inqA.i2 = inqB.i2 = inqC.i1 = inqD.i1 = (inO.i1 + inO.i2)/2;
        inqC.i2 = inqD.i2 = inO.i2;

        inqA.j1 = inqC.j1 = inO.j1;
        inqA.j2 = inqC.j2 = inqB.j1 = inqD.j1 = (inO.j1 + inO.j2)/2;
        inqB.j2 = inqD.j2 = inO.j2;

        /* Setando os intervalos dos quadrantes E, F, G, e H. */
        inqE.i1 = inqF.i1 = inF.i1;
        inqE.i2 = inqF.i2 = inqG.i1 = inqH.i1 = (inF.i1 + inF.i2)/2;
        inqG.i2 = inqH.i2 = inF.i2;

        inqE.j1 = inqG.j1 = inF.j1;
        inqE.j2 = inqG.j2 = inqF.j1 = inqH.j1 = (inF.j1 + inF.j2)/2;
        inqF.j2 = inqH.j2 = inF.j2;

        /* Alocando espaço para os produtos e operações básicas. */
        {
            elemento = newN * newN;
            P1 = (pixel *)malloc(elemento * sizeof(pixel));
            P2 = (pixel *)malloc(elemento * sizeof(pixel));
            P3 = (pixel *)malloc(elemento * sizeof(pixel));
            P4 = (pixel *)malloc(elemento * sizeof(pixel));
            P5 = (pixel *)malloc(elemento * sizeof(pixel));
            P6 = (pixel *)malloc(elemento * sizeof(pixel));
            P7 = (pixel *)malloc(elemento * sizeof(pixel));
            bas1 = (pixel *)malloc(elemento * sizeof(pixel));
            bas2 = (pixel *)malloc(elemento * sizeof(pixel));
        } //Está em bloco para poder reduzir na IDE.

        /* Calculando os 7 produtos. */
        newDim.dR = newN; //A dimensão da matriz resultante sempre será a mesma (pois é matriz P, ou é matriz bas).

        newDim.dO = dim.dF;
        operacaoBasica(matrizF, bas1, inqF, inqH, '-', newDim); //F - H
        newDim.dO = dim.dO;
        newDim.dF = newN;
        strassen(matrizO, bas1, P1, inqA, in0new, newN, newDim); //P1 = A(F - H)

        operacaoBasica(matrizO, bas1, inqA, inqB, '+', newDim); //A + B
        newDim.dO = newN;
        newDim.dF = dim.dF;
        strassen(bas1, matrizF, P2, in0new, inqH, newN, newDim); //P2 = (A + B)H

        newDim.dO = dim.dO;
        operacaoBasica(matrizO, bas1, inqC, inqD, '+', newDim); //C + D
        newDim.dO = newN;
        newDim.dF = dim.dF;
        strassen(bas1, matrizF, P3, in0new, inqE, newN, newDim); //P3 = (C + D)E

        newDim.dO = dim.dF;
        operacaoBasica(matrizF, bas1, inqG, inqE, '-', newDim); //G - E
        newDim.dO = dim.dO;
        newDim.dF = newN;
        strassen(matrizO, bas1, P4, inqD, in0new, newN, newDim); //P4 = D(G - E)

        operacaoBasica(matrizO, bas1, inqA, inqD, '+', newDim); //A + D
        newDim.dO = dim.dF;
        operacaoBasica(matrizF, bas2, inqE, inqH, '+', newDim); //E + H
        newDim.dO = newN;
        strassen(bas1, bas2, P5, in0new, in0new, newN, newDim); //P5 = (A + D)(E + H)

        newDim.dO = dim.dO;
        operacaoBasica(matrizO, bas1, inqB, inqD, '-', newDim); //B - D
        newDim.dO = dim.dF;
        operacaoBasica(matrizF, bas2, inqG, inqH, '+', newDim); //G + H
        newDim.dO = newN;
        strassen(bas1, bas2, P6, in0new, in0new, newN, newDim); //P6 = (B - D)(G + H)

        newDim.dO = dim.dO;
        operacaoBasica(matrizO, bas1, inqA, inqC, '-', newDim); //A - C
        newDim.dO = dim.dF;
        operacaoBasica(matrizF, bas2, inqE, inqF, '+', newDim); //E + F
        newDim.dO = newN;
        newDim.dF = newN;
        strassen(bas1, bas2, P7, in0new, in0new, newN, newDim); //P7 = (A - C)(E + F)

        /* Calculando os quadrantes, e salvando na matriz final. */
        for(int i = in0new.i1; i < in0new.i2; i++) { //Percorrendo as linhas.
            for(int j = in0new.j1; j < in0new.j2; j++) { //Percorrendo as colunas.
                elemento = i * newN + j;

                /* 1º quadrante -> P5 + P4 - P2 + P6. */
                elemento2 = i * dim.dR + j;
                matrizR[elemento2].r = P5[elemento].r + P4[elemento].r - P2[elemento].r + P6[elemento].r;
                matrizR[elemento2].g = P5[elemento].g + P4[elemento].g - P2[elemento].g + P6[elemento].g;
                matrizR[elemento2].b = P5[elemento].b + P4[elemento].b - P2[elemento].b + P6[elemento].b;

                /* 2º quadrante -> P1 + P2. */
                elemento2 = i * dim.dR + j + newN;
                matrizR[elemento2].r = P1[elemento].r + P2[elemento].r;
                matrizR[elemento2].g = P1[elemento].g + P2[elemento].g;
                matrizR[elemento2].b = P1[elemento].b + P2[elemento].b;

                /* 3º quadrante -> P3 + P4. */
                elemento2 = (i + newN) * dim.dR + j;
                matrizR[elemento2].r = P3[elemento].r + P4[elemento].r;
                matrizR[elemento2].g = P3[elemento].g + P4[elemento].g;
                matrizR[elemento2].b = P3[elemento].b + P4[elemento].b;

                /* 4º quadrante -> P1 + P5 - P3 - P7. */
                elemento2 = (i + newN) * dim.dR + j + newN;
                matrizR[elemento2].r = P1[elemento].r + P5[elemento].r - P3[elemento].r - P7[elemento].r;
                matrizR[elemento2].g = P1[elemento].g + P5[elemento].g - P3[elemento].g - P7[elemento].g;
                matrizR[elemento2].b = P1[elemento].b + P5[elemento].b - P3[elemento].b - P7[elemento].b;
            }
        }

        /* Liberando memória. */
        {
            free(P1);
            free(P2);
            free(P3);
            free(P4);
            free(P5);
            free(P6);
            free(P7);
            free(bas1);
            free(bas2);
        } //Está em bloco para poder reduzir na IDE.
    }
}

int main(void) {
    FILE *imagem = fopen("C:\\Users\\Henrique Zucato\\Documents\\01-Universidade\\4-Periodo\\CTCO04-Projeto-E-Analise-De-Algoritmos\\Trabalho-01-Multiplicacao-Matrizes\\Codigos\\ppm\\10.in", "r");
    char codificacao[3]; //Codificação da imagem.
    int c; //Maior pixel.
    int elemento; //Auxiliar para contas repetitivas.
    pixel *matrizO, *matrizF, *matrizR;
    intervalo inImagem; //Intervalo da matriz da imagem.
    dimensao diImagem; //Dimensão da matriz da imagem.

    /* Verificando se foi possível abrir o arquivo. */
    if(!imagem) {
        printf("\nErro na abertura do arquivo.");
        return 1;
    }

    /* Lendo a codificação da imagem e verificando se é válida. */
    fscanf(imagem, " %2s", codificacao);
    if(strcmp(codificacao, "P3") != 0) {
        printf("\nCodificação é inválida - %s.", codificacao);
        return 1;
    }

    /* Lendo a dimensão e maior píxel. */
    inImagem.i1 = inImagem.j1 = 0;

    fscanf(imagem, " %d", &diImagem.dO);
    fscanf(imagem, " %d", &diImagem.dF);
    fscanf(imagem, " %d", &c);

    diImagem.dR = diImagem.dO;
    inImagem.i2 = inImagem.j2 = diImagem.dO;

    /* Alocando espaço para as matrizes. */
    elemento = inImagem.i2 * inImagem.i2;
    matrizO = (pixel *)malloc(elemento * sizeof(pixel));
    matrizF = (pixel *)malloc(elemento * sizeof(pixel));
    matrizR = (pixel *)malloc(elemento * sizeof(pixel));

    /* Lendo as matrizes. */
    for(int i = 0; i < inImagem.i2; i++) { //Percorrendo as linhas.
        for(int j = 0; j < inImagem.j2; j++) { //Percorrendo as colunas.
            elemento = i * inImagem.i2 + j;
            fscanf(imagem, " %d", &matrizO[elemento].r);
            fscanf(imagem, " %d", &matrizO[elemento].g);
            fscanf(imagem, " %d", &matrizO[elemento].b);
        }
    }
    for(int i = 0; i < inImagem.i2; i++) { //Percorrendo as linhas.
        for(int j = 0; j < inImagem.j2; j++) { //Percorrendo as colunas.
            elemento = i * inImagem.i2 + j;
            fscanf(imagem, " %d", &matrizF[elemento].r);
            fscanf(imagem, " %d", &matrizF[elemento].g);
            fscanf(imagem, " %d", &matrizF[elemento].b);
        }
    }

    /* Aplicando o filtro (multiplicando por meio do algoritmo de Strassen). */
    strassen(matrizO, matrizF, matrizR, inImagem, inImagem, inImagem.i2, diImagem);

    /* Imprimindo resultado. */
    printf("%s", codificacao);
    printf("\n%d %d", inImagem.i2, inImagem.i2);
    printf("\n%d\n", c);
    imprimeMatriz(matrizR, inImagem.i2);

    /* Liberando memória e fechando o arquivo. */
    free(matrizO);
    free(matrizF);
    free(matrizR);
    fclose(imagem);

    return 0;
}
