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

//TODO: Arrumar descrição
/*
Descrição: função responsável por multiplicar as matrizes O e F da estrutura 'matrizes', e salvar o resultado na matriz R.
Entrada: ponteiro da estrutura 'matrizes' que contém as matrizes que serão multiplicadas, ponteiro para a estrutura 'intervalo' que contém o intervalo do quadrante.
Saída: nada.
*/
void multiplicarMatrizes(int **matrizO, int **matrizF, int **matrizR, intervalo *inO, intervalo *inF, intervalo *inR) {
    int elemento; //Vai armazenar o valor de cada elemento da matriz resultante.

    /* Fazendo a multiplicação. */
    for(int iO = inO->i1, iR = inR->i1; iO < inO->i2; iO++, iR++) { //Percorrendo as linhas.
        for(int jF = inF->j1, jR = inR->j1; jF < inF->j2; jF++, jR++) { //Percorrendo as colunas.
            elemento = 0;
            for(int linha = inF->i1, coluna = inO->j1; linha < inF->i2; linha++, coluna++) { //Percorrendo os elementos que irão para a soma (como são de tamanhos iguais, só uma verificação basta).
                elemento += matrizO[iO][coluna] * matrizF[linha][jF];
            }
            matrizR[iR][jR] = elemento;
        }
    }
}

//TODO: Fazer a descrição.
void strassen(int **matrizO, int **matrizF, int **matrizR, intervalo *inO, intervalo *inF, intervalo *inR, int n) {
    intervalo inAE, inBF, inCG, inDH; //Intervalos dos quadrantes A e E, B e F, C e G, D e H.
    intervalo in0new; //Intervalo das matrizes de operações básicas e de produtos (0 até newN).
    int **P1, **P2, **P3, **P4, **P5, **P6, **P7; //Os sete produtos.
    int **bas1, **bas2, **bas3, **bas4, **bas5, **bas6, **bas7, **bas8, **bas9, **bas10; //Matrizes resultantes das 10 operações básicas.
    int **q1, **q2, **q3, **q4; //Matrizes resultantes das operações nos sete produtos.
    int newN = n/2; //Nova dimensão (newN x newN).
    if(n <= 64) {
        multiplicarMatrizes(matrizO, matrizF, matrizR, inO, inF, inR);
    }else {
        /* Setando as linhas dos quadrantes. */
        inAE.i1 = inBF.i1 = inR->i1;
        inAE.i2 = inBF.i2 = inCG.i1 = inDH.i1 = (inR->i1 + inR->i2)/2;
        inCG.i2 = inDH.i2 = inR->i2;
        in0new.i1 = 0;
        in0new.i2 = newN;

        /* Setando as colunas dos quadrantes. */
        inAE.j1 = inCG.j1 = inR->j1;
        inAE.j2 = inCG.j2 = inBF.j1 = inDH.j1 = (inR->j1 + inR->j2)/2;
        inBF.j2 = inDH.j2 = inR->j2;
        in0new.j1 = 0;
        in0new.j2 = newN;

        /* Alocando espaço para os produtos e operações básicas. */
        {
            P1 = (int **)malloc(newN * sizeof(int *));
            P2 = (int **)malloc(newN * sizeof(int *));
            P3 = (int **)malloc(newN * sizeof(int *));
            P4 = (int **)malloc(newN * sizeof(int *));
            P5 = (int **)malloc(newN * sizeof(int *));
            P6 = (int **)malloc(newN * sizeof(int *));
            P7 = (int **)malloc(newN * sizeof(int *));
            bas1 = (int **)malloc(newN * sizeof(int *));
            bas2 = (int **)malloc(newN * sizeof(int *));
            bas3 = (int **)malloc(newN * sizeof(int *));
            bas4 = (int **)malloc(newN * sizeof(int *));
            bas5 = (int **)malloc(newN * sizeof(int *));
            bas6 = (int **)malloc(newN * sizeof(int *));
            bas7 = (int **)malloc(newN * sizeof(int *));
            bas8 = (int **)malloc(newN * sizeof(int *));
            bas9 = (int **)malloc(newN * sizeof(int *));
            bas10 = (int **)malloc(newN * sizeof(int *));
        }
        for(int i = 0; i < newN; i++) {
            P1[i] = (int *)malloc(newN * sizeof(int));
            P2[i] = (int *)malloc(newN * sizeof(int));
            P3[i] = (int *)malloc(newN * sizeof(int));
            P4[i] = (int *)malloc(newN * sizeof(int));
            P5[i] = (int *)malloc(newN * sizeof(int));
            P6[i] = (int *)malloc(newN * sizeof(int));
            P7[i] = (int *)malloc(newN * sizeof(int));
            bas1[i] = (int *)malloc(newN * sizeof(int));
            bas2[i] = (int *)malloc(newN * sizeof(int));
            bas3[i] = (int *)malloc(newN * sizeof(int));
            bas4[i] = (int *)malloc(newN * sizeof(int));
            bas5[i] = (int *)malloc(newN * sizeof(int));
            bas6[i] = (int *)malloc(newN * sizeof(int));
            bas7[i] = (int *)malloc(newN * sizeof(int));
            bas8[i] = (int *)malloc(newN * sizeof(int));
            bas9[i] = (int *)malloc(newN * sizeof(int));
            bas10[i] = (int *)malloc(newN * sizeof(int));
        }

        /* Calculando as matrizes resultantes das operações básicas (+ ou -). */
        for(int linha1 = inAE.i1, linha2 = inCG.i1, i = 0; linha1 < inAE.i2; linha1++, linha2++, i++) {
            for(int coluna1 = inAE.j1, coluna2 = inBF.j1, j = 0; coluna1 < inAE.j2; coluna1++, coluna2++, j++) {
                bas1[i][j] = matrizF[linha1][coluna2] - matrizF[linha2][coluna2]; //F - H
                bas2[i][j] = matrizO[linha1][coluna1] + matrizO[linha1][coluna2]; //A + B
                bas3[i][j] = matrizO[linha2][coluna1] + matrizO[linha2][coluna2]; //C + D
                bas4[i][j] = matrizF[linha2][coluna1] - matrizF[linha1][coluna1]; //G - E
                bas5[i][j] = matrizO[linha1][coluna1] + matrizO[linha2][coluna2]; //A + D
                bas6[i][j] = matrizF[linha1][coluna1] + matrizF[linha2][coluna2]; //E + H
                bas7[i][j] = matrizO[linha1][coluna2] - matrizO[linha2][coluna2]; //B - D
                bas8[i][j] = matrizF[linha2][coluna1] + matrizF[linha2][coluna2]; //G + H
                bas9[i][j] = matrizO[linha1][coluna1] - matrizO[linha2][coluna1]; //A - C
                bas10[i][j] = matrizF[linha1][coluna1] + matrizF[linha1][coluna2]; //E + F
            }
        }

        /* Dividindo o problema em 7 subproblemas (calculando os produtos). */
        strassen(matrizO, bas1, P1, &inAE, &in0new, &in0new, newN);
        strassen(bas2, matrizF, P2, &in0new, &inDH, &in0new, newN);
        strassen(bas3, matrizF, P3, &in0new, &inAE, &in0new, newN);
        strassen(matrizO, bas4, P4, &inDH, &in0new, &in0new, newN);
        strassen(bas5, bas6, P5, &in0new, &in0new, &in0new, newN);
        strassen(bas7, bas8, P6, &in0new, &in0new, &in0new, newN);
        strassen(bas9, bas10, P7, &in0new, &in0new, &in0new, newN);

        /* Alocando espaço para os quadrantes finais. */
        {
            q1 = (int **)malloc(newN * sizeof(int *));
            q2 = (int **)malloc(newN * sizeof(int *));
            q3 = (int **)malloc(newN * sizeof(int *));
            q4 = (int **)malloc(newN * sizeof(int *));
        }
        for(int i = 0; i < newN; i++) {
            q1[i] = (int *)malloc(newN * sizeof(int));
            q2[i] = (int *)malloc(newN * sizeof(int));
            q3[i] = (int *)malloc(newN * sizeof(int));
            q4[i] = (int *)malloc(newN * sizeof(int));
        }

        /* Calculando os quadrantes. */
        for(int i = in0new.i1; i < in0new.i2; i++) {
            for(int j = in0new.j1; j < in0new.j2; j++) {
                q1[i][j] = P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
                q2[i][j] = P1[i][j] + P2[i][j];
                q3[i][j] = P3[i][j] + P4[i][j];
                q4[i][j] = P1[i][j] + P5[i][j] - P3[i][j] - P7[i][j];
            }
        }

        /* Passando os quadrantes para a matrizR. */
        for(int i = inR->i1; i < inR->i2; i++) {
            for(int j = inR->j1; j < inR->j2; j++) {
                if(i < newN && j < newN) {
                    //Primeiro quadrante.
                    matrizR[i][j] = q1[i][j];
                }else if(i < newN && j >= newN) {
                    //Segundo quadrante.
                    matrizR[i][j] = q2[i][j-newN];
                }else if(i >= newN && j < newN) {
                    //Terceiro quadrante.
                    matrizR[i][j] = q3[i-newN][j];
                }else {
                    //Quarto quadrante.
                    matrizR[i][j] = q4[i-newN][j-newN];
                }
            }
        }

        /* Liberando memória. */
        for(int i = 0; i < newN; i++) {
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
            free(q1[i]);
            free(q2[i]);
            free(q3[i]);
            free(q4[i]);
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
            free(q1);
            free(q2);
            free(q3);
            free(q4);
        }
    }
}

int main(void) {
    FILE *imagem = fopen("C:\\Users\\Henrique Zucato\\Documents\\01-Universidade\\4-Periodo\\CTCO04-Projeto-E-Analise-De-Algoritmos\\Trabalho-01-Multiplicacao-Matrizes\\Codigos\\ppm\\Exemplo1.txt", "r");
    char codificacao[3]; //Codificação da imagem.
    int n; //Dimensão da imagem (como será n x n, só estou usando uma variável).
    int c; //Maior pixel.
    int **matrizO, **matrizF;

    if(!imagem) {
        printf("\nErro na abertura do arquivo.");
        return 1;
    }

    fgets(codificacao, 3, imagem);
    printf("%s", codificacao);
    if(strcmp(codificacao, "P3") != 0) {
        printf("\nArquivo em outra codificação.");
        return 1;
    }

    fscanf(imagem, "%d ", &n);
    fscanf(imagem, "%d ", &n);

    n = n * 3;

    printf("\nDimensão: %d x %d", n, n);

    fgets(&c, 1, imagem);

    matrizO = (int **)malloc(n * sizeof(int *));
    matrizF = (int **)malloc(n * sizeof(int *));
    for(int i = 0; i < n; i++) {
        matrizO[i] = (int *)malloc(n * sizeof(int));
        matrizF[i] = (int *)malloc(n * sizeof(int));
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            fscanf(imagem, "%d ", &matrizO[i][j]);
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            fscanf(imagem, "%d ", &matrizF[i][j]);
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d ", matrizO[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d ", matrizF[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < n; i++) {
        free(matrizO[i]);
        free(matrizF[i]);
    }
    free(matrizO);
    free(matrizF);

    fclose(imagem);
    return 0;
}
