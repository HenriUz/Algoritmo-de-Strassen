/*
main.c

Descrição: este código tem o objetivo de implementar o algoritmo de strassen para multiplicação de matrizes. A intenção é simular um arquivo.ppm com codificação P3 para aplicação de filtros.

Visualização de uma matriz 3x3:
| [a, b, c] [d, e, f] [g, h, i] |
| [j, k, l] [m, n, o] [p, q, r] |
| [s, t, u] [v, w, x] [y, z, ç] |
Note que cada elemento da matriz é um vetor de 3 elementos, simulando um píxel RGB.
*/


/* --- Includes. --- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Estruturas. --- */

typedef struct intervalo {
    int i1, i2; //Linha inicial e linha final.
    int j1, j2; //Coluna inicial e coluna final.
}intervalo;

/* --- Funções. --- */

/*
Descrição: função que imprime uma matriz nxn.
Entrada: matriz, dimensão n.
Saída: nada.
*/
void imprimeMatriz(int ***matriz, int n) {
    printf("\nImprimindo matriz:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            for(int k = 0; k < 3; k++) {
                printf("%d ", matriz[i][j][k]);
            }
            printf("| ");
        }
        printf("\n");
    }
}

/*
Descrição: função responsável por multiplicar as matrizes O e F, e salvar o resultado na matriz R.
Entrada: matriz original, matriz filtro, matriz resultante, intervalo da matriz original, intervalo da matriz filtro.
Saída: nada.
*/
void multiplicarMatrizes(int ***matrizO, int ***matrizF, int ***matrizR, intervalo *inO, intervalo *inF) {
    int elemento; //Vai armazenar o valor de cada elemento da matriz resultante.

    /* Fazendo a multiplicação. */
    for(int iO = inO->i1, iR = 0; iO < inO->i2; iO++, iR++) { //Percorrendo as linhas (iO - linha da matrizO, iR - linha da matrizR). Somente uma verificação, pois a área das matrizes sempre será a mesma.
        for(int jF = inF->j1, jR = 0; jF < inF->j2; jF++, jR++) { //Percorrendo as colunas (jF - coluna da matrizF, jR - coluna da matrizR).
            for(int k = 0; k < 3; k++) { //Percorrendo o vetor dos píxels.
                elemento = 0;
                for(int linha = inF->i1, coluna = inO->j1; linha < inF->i2; linha++, coluna++) { //Percorrendo os elementos das linhas da matrizF e colunas da matrizO.
                    elemento += matrizO[iO][coluna][k] * matrizF[linha][jF][k];
                }
                matrizR[iR][jR][k] = elemento;
            }
        }
    }
}

/*
Descrição: função responsável por multiplicar as matrizes O e F, usando o algoritmo de Strassen (reduz o problema em 7 subproblemas recursivamente). O resultado é salvo na matriz R
Entrada: matriz original, matriz filtro, matriz resultante, intervalo da matriz original, intervalo da matriz filtro, dimensão das matrizes (nxn).
Saída: nada.
*/
void strassen(int ***matrizO, int ***matrizF, int ***matrizR, intervalo *inO, intervalo *inF, int n) {
    intervalo inAE, inBF, inCG, inDH; //Intervalos dos quadrantes A e E, B e F, C e G, D e H.
    intervalo in0new; //Intervalo das matrizes de operações básicas e de produtos (0 até newN).
    int ***P1, ***P2, ***P3, ***P4, ***P5, ***P6, ***P7; //Os sete produtos.
    int ***bas1, ***bas2, ***bas3, ***bas4, ***bas5, ***bas6, ***bas7, ***bas8, ***bas9, ***bas10; //Matrizes resultantes das 10 operações básicas.
    int newN = n/2; //Nova dimensão (newN x newN).

    if(n <= 2) {
        multiplicarMatrizes(matrizO, matrizF, matrizR, inO, inF); //Multiplicando as matrizes com o algoritmo primitivo.
    }else {
        /* Setando as linhas dos quadrantes. */
        inAE.i1 = inBF.i1 = in0new.i1 = 0;
        inAE.i2 = inBF.i2 = inCG.i1 = inDH.i1 = in0new.i2 = newN;
        inCG.i2 = inDH.i2 = n;

        /* Setando as colunas dos quadrantes. */
        inAE.j1 = inCG.j1 = in0new.j1 = 0;
        inAE.j2 = inCG.j2 = inBF.j1 = inDH.j1 = in0new.j2 = newN;
        inBF.j2 = inDH.j2 = n;

        /* Alocando espaço para os produtos e operações básicas. */
        {
            P1 = (int ***)malloc(newN * sizeof(int **));
            P2 = (int ***)malloc(newN * sizeof(int **));
            P3 = (int ***)malloc(newN * sizeof(int **));
            P4 = (int ***)malloc(newN * sizeof(int **));
            P5 = (int ***)malloc(newN * sizeof(int **));
            P6 = (int ***)malloc(newN * sizeof(int **));
            P7 = (int ***)malloc(newN * sizeof(int **));
            bas1 = (int ***)malloc(newN * sizeof(int **));
            bas2 = (int ***)malloc(newN * sizeof(int **));
            bas3 = (int ***)malloc(newN * sizeof(int **));
            bas4 = (int ***)malloc(newN * sizeof(int **));
            bas5 = (int ***)malloc(newN * sizeof(int **));
            bas6 = (int ***)malloc(newN * sizeof(int **));
            bas7 = (int ***)malloc(newN * sizeof(int **));
            bas8 = (int ***)malloc(newN * sizeof(int **));
            bas9 = (int ***)malloc(newN * sizeof(int **));
            bas10 = (int ***)malloc(newN * sizeof(int **));
        } //Está em bloco para poder reduzir na IDE.
        for(int i = 0; i < newN; i++) {
            P1[i] = (int **)malloc(newN * sizeof(int *));
            P2[i] = (int **)malloc(newN * sizeof(int *));
            P3[i] = (int **)malloc(newN * sizeof(int *));
            P4[i] = (int **)malloc(newN * sizeof(int *));
            P5[i] = (int **)malloc(newN * sizeof(int *));
            P6[i] = (int **)malloc(newN * sizeof(int *));
            P7[i] = (int **)malloc(newN * sizeof(int *));
            bas1[i] = (int **)malloc(newN * sizeof(int *));
            bas2[i] = (int **)malloc(newN * sizeof(int *));
            bas3[i] = (int **)malloc(newN * sizeof(int *));
            bas4[i] = (int **)malloc(newN * sizeof(int *));
            bas5[i] = (int **)malloc(newN * sizeof(int *));
            bas6[i] = (int **)malloc(newN * sizeof(int *));
            bas7[i] = (int **)malloc(newN * sizeof(int *));
            bas8[i] = (int **)malloc(newN * sizeof(int *));
            bas9[i] = (int **)malloc(newN * sizeof(int *));
            bas10[i] = (int **)malloc(newN * sizeof(int *));
            for(int j = 0; j < newN; j++) {
                P1[i][j] = (int *)malloc(3 * sizeof(int));
                P2[i][j] = (int *)malloc(3 * sizeof(int));
                P3[i][j] = (int *)malloc(3 * sizeof(int));
                P4[i][j] = (int *)malloc(3 * sizeof(int));
                P5[i][j] = (int *)malloc(3 * sizeof(int));
                P6[i][j] = (int *)malloc(3 * sizeof(int));
                P7[i][j] = (int *)malloc(3 * sizeof(int));
                bas1[i][j] = (int *)malloc(3 * sizeof(int));
                bas2[i][j] = (int *)malloc(3 * sizeof(int));
                bas3[i][j] = (int *)malloc(3 * sizeof(int));
                bas4[i][j] = (int *)malloc(3 * sizeof(int));
                bas5[i][j] = (int *)malloc(3 * sizeof(int));
                bas6[i][j] = (int *)malloc(3 * sizeof(int));
                bas7[i][j] = (int *)malloc(3 * sizeof(int));
                bas8[i][j] = (int *)malloc(3 * sizeof(int));
                bas9[i][j] = (int *)malloc(3 * sizeof(int));
                bas10[i][j] = (int *)malloc(3 * sizeof(int));
            }
        }

        /* Calculando as matrizes resultantes das operações básicas (+ ou -), para o cálculo dos produtos. */
        for(int linha1 = inAE.i1, linha2 = inCG.i1, i = 0; linha1 < inAE.i2; linha1++, linha2++, i++) { //Percorrendo as linhas dos quadrantes.
            for(int coluna1 = inAE.j1, coluna2 = inBF.j1, j = 0; coluna1 < inAE.j2; coluna1++, coluna2++, j++) { //Percorrendo as colunas dos quadrantes.
                for(int k = 0; k < 3; k++) { //Percorrendo os píxels.
                    bas1[i][j][k] = matrizF[linha1][coluna2][k] - matrizF[linha2][coluna2][k]; //F - H
                    bas2[i][j][k] = matrizO[linha1][coluna1][k] + matrizO[linha1][coluna2][k]; //A + B
                    bas3[i][j][k] = matrizO[linha2][coluna1][k] + matrizO[linha2][coluna2][k]; //C + D
                    bas4[i][j][k] = matrizF[linha2][coluna1][k] - matrizF[linha1][coluna1][k]; //G - E
                    bas5[i][j][k] = matrizO[linha1][coluna1][k] + matrizO[linha2][coluna2][k]; //A + D
                    bas6[i][j][k] = matrizF[linha1][coluna1][k] + matrizF[linha2][coluna2][k]; //E + H
                    bas7[i][j][k] = matrizO[linha1][coluna2][k] - matrizO[linha2][coluna2][k]; //B - D
                    bas8[i][j][k] = matrizF[linha2][coluna1][k] + matrizF[linha2][coluna2][k]; //G + H
                    bas9[i][j][k] = matrizO[linha1][coluna1][k] - matrizO[linha2][coluna1][k]; //A - C
                    bas10[i][j][k] = matrizF[linha1][coluna1][k] + matrizF[linha1][coluna2][k]; //E + F
                }
            }
        }

        /* Calculando os 7 produtos. */
        strassen(matrizO, bas1, P1, &inAE, &in0new, newN); //P1 = A(F - H)
        strassen(bas2, matrizF, P2, &in0new, &inDH, newN); //P2 = (A + B)H
        strassen(bas3, matrizF, P3, &in0new, &inAE, newN); //P3 = (C + D)E
        strassen(matrizO, bas4, P4, &inDH, &in0new, newN); //P4 = D(G - E)
        strassen(bas5, bas6, P5, &in0new, &in0new, newN); //P5 = (A + D)(E + H)
        strassen(bas7, bas8, P6, &in0new, &in0new, newN); //P6 = (B - D)(G + H)
        strassen(bas9, bas10, P7, &in0new, &in0new, newN); //P7 = (A - C)(E + F)

        /* Calculando os quadrantes, reutilizando as matrizes básicas 1, 2, 3, 4. */
        for(int i = in0new.i1; i < in0new.i2; i++) { //Percorrendo as linhas.
            for(int j = in0new.j1; j < in0new.j2; j++) { //Percorrendo as colunas.
                for(int k = 0; k < 3; k++) { //Percorrendo os píxels.
                    bas1[i][j][k] = P5[i][j][k] + P4[i][j][k] - P2[i][j][k] + P6[i][j][k];
                    bas2[i][j][k] = P1[i][j][k] + P2[i][j][k];
                    bas3[i][j][k] = P3[i][j][k] + P4[i][j][k];
                    bas4[i][j][k] = P1[i][j][k] + P5[i][j][k] - P3[i][j][k] - P7[i][j][k];
                }
            }
        }

        /* Salvando os resultados na matrizR. */
        for(int i = 0; i < n; i++) { //Percorrendo as linhas.
            for(int j = 0; j < n; j++) { //Percorrendo as colunas.
                for(int k = 0; k < 3; k++) { //Percorrendo os píxels.
                    if(i < newN && j < newN) {
                        //Primeiro quadrante.
                        matrizR[i][j][k] = bas1[i][j][k];
                    }else if(i < newN && j >= newN) {
                        //Segundo quadrante.
                        matrizR[i][j][k] = bas2[i][j-newN][k];
                    }else if(i >= newN && j < newN) {
                        //Terceiro quadrante.
                        matrizR[i][j][k] = bas3[i-newN][j][k];
                    }else {
                        //Quarto quadrante.
                        matrizR[i][j][k] = bas4[i-newN][j-newN][k];
                    }
                }
            }
        }

        /* Liberando memória. */
        for(int i = 0; i < newN; i++) {
            for(int j = 0; j < newN; j++) {
                free(P1[i][j]);
                free(P2[i][j]);
                free(P3[i][j]);
                free(P4[i][j]);
                free(P5[i][j]);
                free(P6[i][j]);
                free(P7[i][j]);
                free(bas1[i][j]);
                free(bas2[i][j]);
                free(bas3[i][j]);
                free(bas4[i][j]);
                free(bas5[i][j]);
                free(bas6[i][j]);
                free(bas7[i][j]);
                free(bas8[i][j]);
                free(bas9[i][j]);
                free(bas10[i][j]);
            }
            free(P1[i]);
            free(P2[i]);
            free(P3[i]);
            free(P4[i]);
            free(P5[i]);
            free(P6[i]);
            free(P7[i]);
            free(bas1[i]);
            free(bas2[i]);
            free(bas3[i]);
            free(bas4[i]);
            free(bas5[i]);
            free(bas6[i]);
            free(bas7[i]);
            free(bas8[i]);
            free(bas9[i]);
            free(bas10[i]);
        }
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
            free(bas3);
            free(bas4);
            free(bas5);
            free(bas6);
            free(bas7);
            free(bas8);
            free(bas9);
            free(bas10);
        } //Está em bloco para poder reduzir na IDE.
    }
}

int main(void) {
    FILE *imagem = fopen("C:\\Users\\Henrique Zucato\\Documents\\01-Universidade\\4-Periodo\\CTCO04-Projeto-E-Analise-De-Algoritmos\\Trabalho-01-Multiplicacao-Matrizes\\Codigos\\ppm\\Exemplo1.txt", "r");
    char codificacao[3]; //Codificação da imagem.
    int c; //Maior pixel.
    int ***matrizO, ***matrizF, ***matrizR; //Matriz original (imagem), matriz filtro, matriz resultado (imagem após o filtro).
    intervalo inImagem; //Intervalo da matriz da imagem.

    /* Verificando se a imagem abriu. */
    if(!imagem) {
        printf("\nErro na abertura do arquivo.");
        return 1;
    }

    /* Lendo a codificação da imagem e verificando se é válida. */
    fscanf(imagem, " %2s", codificacao);
    if(strcmp(codificacao, "P3") != 0) {
        printf("\nCodificação é inválida.");
        return 1;
    }

    /* Lendo a dimensão e maior píxel. */
    inImagem.i1 = inImagem.j1 = 0;
    fscanf(imagem, " %d", &inImagem.i2);
    fscanf(imagem, " %d", &inImagem.j2);
    fscanf(imagem, " %d", &c);

    /* Alocando espaço para as matrizes. */
    matrizO = (int ***)malloc(sizeof(int **) * inImagem.i2);
    matrizF = (int ***)malloc(sizeof(int **) * inImagem.i2);
    matrizR = (int ***)malloc(sizeof(int **) * inImagem.i2);
    for(int i = 0; i < inImagem.i2; i++) {
        matrizO[i] = (int **)malloc(sizeof(int *) * inImagem.j2);
        matrizF[i] = (int **)malloc(sizeof(int *) * inImagem.j2);
        matrizR[i] = (int **)malloc(sizeof(int *) * inImagem.j2);
        for(int j = 0; j < inImagem.j2; j++) {
            matrizO[i][j] = (int *)malloc(sizeof(int) * 3);
            matrizF[i][j] = (int *)malloc(sizeof(int) * 3);
            matrizR[i][j] = (int *)malloc(sizeof(int) * 3);
        }
    }

    /* Lendo as matrizes no arquivo. */
    for(int i = 0; i < inImagem.i2; i++) {
        for(int j = 0; j < inImagem.j2; j++) {
            for(int k = 0; k < 3; k++) {
                fscanf(imagem, "%d", &matrizO[i][j][k]);
            }
        }
    }
    for(int i = 0; i < inImagem.i2; i++) {
        for(int j = 0; j < inImagem.j2; j++) {
            for(int k = 0; k < 3; k++) {
                fscanf(imagem, "%d", &matrizF[i][j][k]);
            }
        }
    }

    imprimeMatriz(matrizO, inImagem.i2);
    imprimeMatriz(matrizF, inImagem.i2);

    /* Testando. */
    strassen(matrizO, matrizF, matrizR, &inImagem, &inImagem, inImagem.i2);
    imprimeMatriz(matrizR, inImagem.i2);

    /* Liberando memória e fechando o arquivo. */
    for(int i = 0; i < inImagem.i2; i++) {
        for(int j = 0; j < inImagem.j2; j++) {
            free(matrizO[i][j]);
            free(matrizF[i][j]);
            free(matrizR[i][j]);
        }
        free(matrizO[i]);
        free(matrizF[i]);
        free(matrizR[i]);
    }
    free(matrizO);
    free(matrizF);
    free(matrizR);
    fclose(imagem);
    return 0;
}
