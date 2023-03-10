#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <signal.h>
#include <time.h>
#include <termios.h>

int quit = 0;

void trata_signint(int signum) {
    quit = 1;
}

void prepara_sigaction(struct sigaction *sa) {
    sa->sa_handler = trata_signint;
    sigemptyset(&sa->sa_mask);
}

int kb_hit() {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);

    return FD_ISSET(STDIN_FILENO, &fds);
}

void prepara_terminal(struct termios *atualconfig, struct termios *novaconfig) {
    tcgetattr(STDIN_FILENO, atualconfig);

    struct termios nova;
    memcpy(&nova, atualconfig, sizeof(struct termios));

    novaconfig = &nova;
    // desabilitar modo de input canônico e echo dos caracteres na tela
    novaconfig->c_lflag &= ~(ICANON | ECHO);
    // número mínimo de caracteres no input: 1
    novaconfig->c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, novaconfig);
}

void restaura_configs(struct termios *atualconfig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, atualconfig);
}

int conta_vizinhos(int *grid, int posx, int posy, int rows, int cols) {
    int soma = 0;
    
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            // wrap around
            soma += *(grid + ((posx + i + rows) % rows)*cols + ((posy + j + cols) % cols));

    soma -= *(grid + posx*cols + posy);

    return soma;
}

void atualiza_grid(int *grid, int *buffer, int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            int n = conta_vizinhos(grid, i, j, rows, cols);

            if (n < 2 || n > 3) 
                *(buffer + i*cols + j) = 0;
            else
                if (n == 3)
                    *(buffer + i*cols + j) = 1;
                else
                    if (*(grid + i*cols + j) == 0)
                        *(buffer + i*cols + j) = 0;
                    else
                        *(buffer + i*cols + j) = 1;
        }
}

void reseta_cursor(int rows, int cols) {
    // mover para cima
    printf("\033[%dA", rows);
    // mover para a esquerda
    printf("\033[%dD", cols);
}

int *inicializa_grid(int rows, int cols) {
    int *grid;
    if ( !(grid = malloc(rows * cols * sizeof(int))) ) {
        fprintf(stderr, "ERRO: compra mais RAM");
        exit(1);
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
             *(grid + i*cols + j) = rand() % 2;

    return grid;
}

void imprime_grid(int *grid, int rows, int cols) {
    // impressao subpixel
    for (int i = 0; i < rows; i += 2) {
        for (int j = 0; j < cols; j++)
            if (*(grid + i*cols + j) == 0)
                if (*(grid + (i+1)*cols + j) == 0)
                    printf(" ");
                else
                    printf("▄");
            else
                if (*(grid + (i+1)*cols + j) == 0)
                    printf("▀");
                else
                    printf("█");

        printf("\n");
    }
    
    reseta_cursor(rows/2, cols);
}

void restaura_cursor(int rows) {
    // reposicionar cursor
    printf("\033[%dB\n", rows/2);
    // mostrar cursor
    printf("\033[?25h");
}

int main() {
    srand(time(0));

    // pegar tamanho do terminal
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int ROWS = (w.ws_row - 2) * 2;
    int COLS = w.ws_col;

    // tratar CTRL+C
    struct sigaction sa;
    prepara_sigaction(&sa);
    sigaction(SIGINT, &sa, NULL);

    printf("Aperte q para sair.\n");
    // esconder cursor
    printf("\033[?25l");

    int *grid;
    grid = inicializa_grid(ROWS, COLS);

    int *buffer;
    if ( !(buffer = malloc(ROWS * COLS * sizeof(int))) ) {
        fprintf(stderr, "ERRO: compra mais RAM");
        exit(1);
    }
    
    static struct termios atualconfig, novaconfig;
    prepara_terminal(&atualconfig, &novaconfig);

    while(!quit) {
        usleep(100 * 1000);
        imprime_grid(grid, ROWS, COLS);
        atualiza_grid(grid, buffer, ROWS, COLS);
        memcpy(grid, buffer, ROWS * COLS * sizeof(int));
        /* copia_matriz(grid, buffer, ROWS, COLS); */

        if (kb_hit() && fgetc(stdin) == 'q')
            quit = 1;
    };

    restaura_configs(&atualconfig);

    restaura_cursor(ROWS);

    free(grid);
    free(buffer);

    return 0;
}
