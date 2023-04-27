#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIELD_LENGTH 80
#define FIELD_HEIGHT 25

void initial_state(int matrix[FIELD_HEIGHT][FIELD_LENGTH]);
int check_alive(int matrix[FIELD_HEIGHT][FIELD_LENGTH], int dim_m, int dim_n);
void output(int matrix[FIELD_HEIGHT][FIELD_LENGTH]);
void initial_state(int matrix[FIELD_HEIGHT][FIELD_LENGTH]);
void game_logic(int matrix[FIELD_HEIGHT][FIELD_LENGTH], int* flag);
void copy(int target[FIELD_HEIGHT][FIELD_LENGTH], int source[FIELD_HEIGHT][FIELD_LENGTH]);
void input(int matrix[FIELD_HEIGHT][FIELD_LENGTH], int* flag);

int main(int argv, char** argc) {
    int field[FIELD_HEIGHT][FIELD_LENGTH];
    int flag = 0;
    int speed = 0;
    initial_state(field);
    if (argv > 1) {
        input(field, &flag);
        speed = atoi(argc[1]);
        if (speed < 1 || speed > 100) {
            printf("Некорректный ввод!");
            flag = 1;
        }
    } else {
        input(field, &flag);
        if (freopen("/dev/tty", "r", stdin) == NULL) {
            flag = 1;
        }
        if (flag == 0) {
            printf("Введите скорость игры(число от 1 до 100)\n");
            if (scanf("%d", &speed) != 1 || speed < 1 || speed > 100) {
                printf("Некорректный ввод!");
                flag = 1;
            }
        }
    }
    while (flag != 1) {
        game_logic(field, &flag);
        output(field);
        usleep(10000 * speed);
    }
    return 0;
}

void initial_state(int matrix[FIELD_HEIGHT][FIELD_LENGTH]) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_LENGTH; j++) {
            matrix[i][j] = 0;
        }
    }
}

void game_logic(int matrix[FIELD_HEIGHT][FIELD_LENGTH], int* flag) {
    int buffer[FIELD_HEIGHT][FIELD_LENGTH];
    int counter = 0;
    copy(buffer, matrix);
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_LENGTH; j++) {
            if (matrix[i][j] == 1) {
                counter++;
                if (check_alive(matrix, i, j) < 2 || check_alive(matrix, i, j) > 3) {
                    buffer[i][j] = 0;
                }
            } else {
                if (check_alive(matrix, i, j) == 3) {
                    buffer[i][j] = 1;
                }
            }
        }
    }
    copy(matrix, buffer);
    if (counter == 0) {
        *flag = 1;
    }
}

int check_alive(int matrix[FIELD_HEIGHT][FIELD_LENGTH], int dim_m, int dim_n) {
    int alive_cells = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int m_offset = dim_m + i;
            int n_offset = dim_n + j;
            if (m_offset == -1) {
                m_offset = FIELD_HEIGHT - 1;
            } else if (m_offset == FIELD_HEIGHT) {
                m_offset = 0;
            }
            if (n_offset == -1) {
                n_offset = FIELD_LENGTH - 1;
            } else if (n_offset == FIELD_LENGTH) {
                n_offset = 0;
            }
            if (matrix[m_offset][n_offset] == 1 && (i != 0 || j != 0)) {
                alive_cells++;
            }
        }
    }
    return alive_cells;
}

void copy(int target[FIELD_HEIGHT][FIELD_LENGTH], int source[FIELD_HEIGHT][FIELD_LENGTH]) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_LENGTH; j++) {
            target[i][j] = source[i][j];
        }
    }
}

void output(int matrix[FIELD_HEIGHT][FIELD_LENGTH]) {
    printf("\033[0d\033[2J");
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 0; j < FIELD_LENGTH; j++) {
            if (matrix[i][j] == 0) {
                printf(" ");
            } else {
                printf("o");
            }
        }
        printf("\n");
    }
}
void input(int matrix[FIELD_HEIGHT][FIELD_LENGTH], int* flag) {
    *flag = 0;
    int first = -1, second = -1;
    while (first != 0) {
        if (scanf("%d", &first) != 1 || first < 0 || first >= FIELD_HEIGHT) {
            printf("некорректный ввод!");
            *flag = 1;
            break;
        } else {
            if (first == 0)
                break;
            else {
                if (scanf("%d", &second) != 1 || second < 1 || second >= FIELD_LENGTH) {
                    *flag = 1;
                    printf("некорректный ввод!");
                    break;
                } else {
                    matrix[first][second] = 1;
                }
            }
        }
    }
}
