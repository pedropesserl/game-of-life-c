#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXX 10
#define MAXY 10

int conta_vizinhos(int grid[MAXX][MAXY], int posx, int posy) {
    int soma = 0;
    
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            // wrap around
            soma += grid[ (posx + i + MAXX) % MAXX ][ (posy + j + MAXY) % MAXY ];

    soma -= grid[posx][posy];

    return soma;
}

void atualiza_grid(int grid[MAXX][MAXY], int buffer[MAXX][MAXY]) {
    for (int i = 0; i < MAXX; i++)
        for (int j = 0; j < MAXY; j++) {
            int n = conta_vizinhos(grid, i, j);

            if (n < 2 || n > 3) 
                buffer[i][j] = 0;
            else {
                if (n == 3) {
                    buffer[i][j] = 1;
                } else
                    if (grid[i][j] == 0)
                        buffer[i][j] = 0;
                    else
                        buffer[i][j] = 1;
            }
        }
}

void reseta_cursor(int lins, int cols) {
    // mover para cima
    printf("\033[%dA", lins);
    //mover para a esquerda
    printf("\033[%dD", cols);
}

void imprime_grid(int grid[MAXX][MAXY]) {
    // impressao subpixel
    for (int i = 0; i < MAXX; i += 2) {
        for (int j = 0; j < MAXY; j++) {
            if (grid[i][j] == 0)
                if (grid[i+1][j] == 0)
                    printf(" ");
                else
                    printf("▄");
            else
                if (grid[i+1][j] == 0)
                    printf("▀");
                else
                    printf("█");

        }
        printf("\n");
    }
    printf("\n");
    
    reseta_cursor(MAXX/2 + 1, MAXY);
}

void copia_matriz(int a[MAXX][MAXY], int b[MAXX][MAXY]) {
    for (int i = 0; i < MAXX; i++)
        for (int j = 0; j < MAXY; j++)
            a[i][j] = b[i][j];
}

int main() {
    printf("Aperte Ctrl+C para sair.\n\n");

    int grid[MAXX][MAXY] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int buffer[MAXX][MAXY];

    while(1) {
        usleep(100 * 1000);
        imprime_grid(grid);
        atualiza_grid(grid, buffer);
        copia_matriz(grid, buffer);
    };

    return 0;
}
