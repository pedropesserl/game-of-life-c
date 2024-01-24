#include <stdio.h>
#include <stdlib.h>
#include "libterm.h"

int gol_quit = 0;

void trata_sigint(int signum) {
    (void) signum;
    gol_quit = 1;
}

int conta_vizinhos(int *grid, int posx, int posy, int rows, int cols) {
    int soma = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // wrap around
            soma += grid[ ((posx + i + rows) % rows)*cols + (posy + j + cols) % cols ];
        }
    }
    soma -= grid[posx*cols + posy];
    return soma;
}

int *inicializa_grid(int rows, int cols) {
    int *grid = (int*)malloc(rows * cols * sizeof(int));
    if (!grid) {
        fprintf(stderr, "ERRO: compra mais RAM");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
             grid[i*cols + j] = rand() % 2;
        }
    }
    return grid;
}

void atualiza_grid(int *grid, int *buffer, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int n = conta_vizinhos(grid, i, j, rows, cols);
            if (n < 2 || n > 3) {
                buffer[i*cols + j] = 0;
            } else {
                if (n == 3) {
                    buffer[i*cols + j] = 1;
                } else {
                    if (grid[i*cols + j] == 0) {
                        buffer[i*cols + j] = 0;
                    } else {
                        buffer[i*cols + j] = 1;
                    }
                }
            }
        }
    }
}

void imprime_grid(int *grid, int rows, int cols) {
    // impressao subpixel
    for (int i = 0; i < rows; i += 2) {
        for (int j = 0; j < cols; j++) {
            if (grid[i*cols + j] == 0) {
                if (grid[(i+1)*cols + j] == 0) {
                    printf(" ");
                } else {
                    printf("▄");
                }
            } else {
                if (grid[(i+1)*cols + j] == 0) {
                    printf("▀");
                } else {
                    printf("█");
                }
            }
        }
        printf("\n");
    }
    
    cursor_up(rows/2);
    cursor_left(cols);
}

void reseta_cursor(int rows, int cols) {
    cursor_up(rows);
    cursor_left(cols);
}

void restaura_cursor(int rows) {
    cursor_down(rows/2);
    show_cursor();
}
