#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libterm.h"
#include "gol.h"

#define ROWS 10
#define COLS 10

int main() {
    prepare_sigint(trata_sigint);

    printf("Aperte q para sair.\n\n");
    hide_cursor();

    int grid[ROWS][COLS] = {
        {0, 1, 0,},
        {0, 0, 1,},
        {1, 1, 1,},
    };
    int buffer[ROWS][COLS];
    
    static struct termios atualconfig;
    disable_canonical_stdin(&atualconfig);

    while(!gol_quit) {
        usleep(100 * 1000);
        imprime_grid((int*)grid, ROWS, COLS);
        atualiza_grid((int*)grid, (int*)buffer, ROWS, COLS);
        memcpy((int*)grid, (int*)buffer, ROWS * COLS * sizeof(int));

        if (kb_hit() && fgetc(stdin) == 'q') {
            gol_quit = 1;
        }
    };

    set_stdin_flush(&atualconfig);

    restaura_cursor(ROWS);

    return 0;
}
