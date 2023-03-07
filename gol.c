#include <stdio.h>
#include <unistd.h>
#define ROWS 10
#define COLS 10

int conta_vizinhos(int grid[ROWS][COLS], int posx, int posy) {
    int soma = 0;
    
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            // wrap around
            soma += grid[ (posx + i + ROWS) % ROWS ][ (posy + j + COLS) % COLS ];

    soma -= grid[posx][posy];

    return soma;
}

void atualiza_grid(int grid[ROWS][COLS], int buffer[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) {
            int n = conta_vizinhos(grid, i, j);

            if (n < 2 || n > 3) 
                buffer[i][j] = 0;
            else
                if (n == 3)
                    buffer[i][j] = 1;
                else
                    if (grid[i][j] == 0)
                        buffer[i][j] = 0;
                    else
                        buffer[i][j] = 1;
        }
}

void reseta_cursor(int lins, int cols) {
    // mover para cima
    printf("\033[%dA", lins);
    //mover para a esquerda
    printf("\033[%dD", cols);
}

void imprime_grid(int grid[ROWS][COLS]) {
    // impressao subpixel
    for (int i = 0; i < ROWS; i += 2) {
        for (int j = 0; j < COLS; j++)
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

        printf("\n");
    }
    
    reseta_cursor(ROWS/2, COLS);
}

void copia_matriz(int a[ROWS][COLS], int b[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            a[i][j] = b[i][j];
}

int main() {
    printf("Aperte Ctrl+C para sair.\n\n");
    /* printf("\033[?25l"); */

    int grid[ROWS][COLS] = {
        {0, 1, 0},
        {0, 0, 1},
        {1, 1, 1}
    };

    int buffer[ROWS][COLS];

    while(1) {
        usleep(100 * 1000);
        imprime_grid(grid);
        atualiza_grid(grid, buffer);
        copia_matriz(grid, buffer);
    };

    return 0;
}
