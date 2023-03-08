#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <termios.h>
#include <signal.h>

int ROWS;
int COLS;

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

void reseta_cursor(int rows, int cols) {
    // mover para cima
    printf("\033[%dA", rows);
    // mover para a esquerda
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

void restaura_cursor() {
    // reposicionar cursor
    printf("\033[%dB\n", ROWS/2);
    // mostrar cursor
    printf("\033[?25h");
}

int main() {
    srand(time(0));

    printf("Entre com o número de linhas e o número de colunas:\n");
    scanf("%d %d", &ROWS, &COLS);

    // tratar CTRL+C
    struct sigaction sa;
    prepara_sigaction(&sa);
    sigaction(SIGINT, &sa, NULL);

    printf("Aperte q para sair.\n\n");
    // esconder cursor
    printf("\033[?25l");

    int grid[ROWS][COLS];
    // preencher grid
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            grid[i][j] = rand() % 2;

    int buffer[ROWS][COLS];
    
    static struct termios atualconfig, novaconfig;
    prepara_terminal(&atualconfig, &novaconfig);

    while(!quit && (!kb_hit() || fgetc(stdin) != 'q')) {
        usleep(100 * 1000);
        imprime_grid(grid);
        atualiza_grid(grid, buffer);
        copia_matriz(grid, buffer);
    };

    restaura_configs(&atualconfig);

    restaura_cursor();

    return 0;
}
