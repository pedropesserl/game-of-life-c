#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "libterm.h"
#include "gol.h"

int main() {
    srand(time(0));

    int rows, cols;
    printf("Entre com o número de linhas e o número de colunas:\n");
    scanf("%d %d", &rows, &cols);

    // tratar CTRL+C
    prepare_sigint(trata_sigint);

    printf("Aperte q para sair.\n\n");
    hide_cursor();

    int *grid = inicializa_grid(rows, cols);
    int *buffer = (int*)malloc(rows * cols * sizeof(int));
    if (!buffer) {
        fprintf(stderr, "ERRO: compra mais RAM");
        exit(1);
    }
    
    static struct termios atualconfig;
    disable_canonical_stdin(&atualconfig);

    while(!gol_quit) {
        usleep(100 * 1000);
        imprime_grid(grid, rows, cols);
        atualiza_grid(grid, buffer, rows, cols);
        memcpy(grid, buffer, rows * cols * sizeof(int));

        if (kb_hit() && fgetc(stdin) == 'q') {
            gol_quit = 1;
        }
    };

    set_stdin_flush(&atualconfig);

    restaura_cursor(rows);

    free(grid);
    free(buffer);

    return 0;
}
