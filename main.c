/* --- Includes. --- */
#include <stdio.h>
#include <stdlib.h>

/* --- Estruturas. --- */
typedef struct matrizes {
    int n; //Dimensão das matrizes (n x n).
    int **matrizO; //Matriz original.
    int **matrizF; //Matriz filtro.
    int **matrizR; //Matriz resultante.
}matrizes;

/* --- Funções. --- */

/*
Descrição: função responsável por multiplicar as matrizes O e F da estrutura 'matrizes', e salvar o resultado na matriz R.
Entrada: ponteiro da estrutura 'matrizes' que contém as matrizes que serão multiplicadas.
Saída: nada.
*/
void multiplicarMatrizes(matrizes *m) {
    int elemento; //Vai armazenar o valor de cada elemento da matriz resultante.

    /* Fazendo a multiplicação. */
    for(int i = 0; i < m->n; i++) { //Percorrendo as linhas.
        for(int j = 0; j < m->n; j++) { //Percorrendo as colunas.
            elemento = 0;
            for(int k = 0; k < m->n; k++) { //Percorrendo os elementos que irão para a soma.
                elemento += m->matrizO[i][k] * m->matrizF[k][j];
            }
            m->matrizR[i][j] = elemento;
        }
    }
}

int main(void) {
    return 0;
}
