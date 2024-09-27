# Algoritmo de Strassen
## Organização dos arquivos

- ppm: pasta com os arquivos contendo as matrizes. Os arquivos `.in` possuem a matriz da imagem e em seguida a matriz do filtro, enquanto os arquivos `.out` possuem a matriz do resultado da multiplicação.
- main.c: código do programa.

## O problema da multiplicação de matrizes
Seja a multiplicação Z = X.Y, onde Z, X e Y são matrizes quadradas `n x n`. Cada elemento Zij vai ser igual à somatória, de k = 1 até n, de `Xik . Ykj`.

A consequência disso é que a complexidade para calcular essa multiplicação é `O(n³)`.

## Ideia Inicial
Uma das primeiras ideias para reduzir essa complexidade foi dividir cada matriz em 4 quadrantes, dessa forma a matriz X fica igual à (A, B /C, D), e a matriz Y fica igual à (E, F /G, H). Com isso a matriz resultante Z é igual à (AE + BG, AF + BH /CE + DG, CF + DH).

Infelizmente essa forma ainda tem complexidade `O(n³)`.

## Algoritmo de Strassen
Foi então que Volker Strassen, em 1969, apresentou uma maneira de realizar essa multiplicação em tempo de execução menor que cúbico. 

A ideia é reduzir o número de chamadas recursivas para 7, ainda realizando contas básicas de subtração e adição, que requisitam `O(n²)`. Porem o fato da recursão ser 7 e não 8, faz com que a complexidade total desse algoritmo seja `O(n^(2.8))` (segundo o teorema mestre).

A ideia das 7 recursões é calcular 7 produtos, usando os quadrantes das matrizes, e no fim montar a matriz resultante total. Os produtos são:
- P1 = A(F - H)
- P2 = (A + B)H
- P3 = (C + D)E
- P4 = D(G - E)
- P5 = (A + D)(E + H)
- P6 = (B - D)(G + H)
- P7 = (A - C)(E + F)

E no final a matriz resultante será igual à (P5 + P4 - P2 + P6, P1 + P2 /P3 + P4, P1 + P5 - P3 - P7).

Para confirmar isso basta realizar as operações acima e fazer as operações básicas, no final teremos a mesma matriz resultante da divisão em quadrantes.