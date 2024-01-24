#ifndef GOL_H_
#define GOL_H_

int gol_quit;

void trata_sigint(int signum);

int conta_vizinhos(int *grid, int posx, int posy, int rows, int cols);

int *inicializa_grid(int rows, int cols);

void atualiza_grid(int *grid, int *buffer, int rows, int cols);

void imprime_grid(int *grid, int rows, int cols);

void reseta_cursor(int rows, int cols);

void restaura_cursor(int rows);

#endif // GOL_H_
