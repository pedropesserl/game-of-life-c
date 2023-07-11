#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <signal.h>
#include <termios.h>
#include <string.h>

void hide_cursor() {
    printf("\033[?25l");
}

void show_cursor() {
    printf("\033[?25h");
}

void cursor_up(int rows) {
    printf("\033[%dA", rows);
}

void cursor_down(int rows) {
    printf("\033[%dB", rows);
}

void cursor_right(int cols) {
    printf("\033[%dC", cols);
}

void cursor_left(int cols) {
    printf("\033[%dD", cols);
}

void cursor_to(int row, int col) {
    printf("\033[%d;%dH", row, col);
} 

void get_terminal_size(int *rows, int *cols) {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    *rows = w.ws_row;
    *cols = w.ws_col;
}

void prepare_sigint(void (*sign_handler)(int)) {
    struct sigaction sa;
    sa.sa_handler = sign_handler;
    sigemptyset(&(sa.sa_mask));
    sigaction(SIGINT, &sa, NULL);
}

int kb_hit() {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    fd_set fds; // fds: file descriptor set
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDOUT_FILENO, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void set_stdin_now(struct termios *config) {
    tcsetattr(STDIN_FILENO, TCSANOW, config);
}

void set_stdin_flush(struct termios *config) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, config);
}

struct termios disable_canonical_stdin(struct termios *currconfig) {
    tcgetattr(STDIN_FILENO, currconfig);
    struct termios new;
    // fill most of new's fields
    memcpy(&new, currconfig, sizeof(struct termios));
    // disable canonical input mode and echo of characters on screen
    new.c_lflag &= ~(ICANON | ECHO);
    // min number of characters in input: 1
    new.c_cc[VMIN] = 1;
    set_stdin_now(&new);
    return new;
}
