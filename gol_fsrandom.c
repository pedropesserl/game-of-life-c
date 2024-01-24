#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "libterm.h"
#include "gol.h"

int main() {
    srand(time(0));

    // pegar tamanho do terminal
    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);
    term_rows = (term_rows - 2) * 2;

    // tratar CTRL+C
    prepare_sigint(trata_sigint);

    printf("Aperte q para sair.\n");
    hide_cursor();

    int *grid = inicializa_grid(term_rows, term_cols);
    int *buffer = (int*)malloc(term_rows * term_cols * sizeof(int));
    if (!buffer) {
        fprintf(stderr, "ERRO: compra mais RAM");
        exit(1);
    }

    static struct termios atualconfig;
    disable_canonical_stdin(&atualconfig);

    while(!gol_quit) {
        usleep(100 * 1000);
        imprime_grid(grid, term_rows, term_cols);
        atualiza_grid(grid, buffer, term_rows, term_cols);
        memcpy(grid, buffer, term_rows * term_cols * sizeof(int));

        if (kb_hit() && fgetc(stdin) == 'q') {
            gol_quit = 1;
        }
    };

    set_stdin_flush(&atualconfig);

    restaura_cursor(term_rows);

    free(grid);
    free(buffer);

    return 0;
}
