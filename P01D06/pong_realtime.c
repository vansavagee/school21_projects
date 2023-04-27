// I WANT TO PLAY WITH YOU
//         YOUR FRIEND, AI

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define MAX_X 80
#define MAX_Y 24
#define MAX_S 7
#define MAX_S1 73
#define Y_MIN 0
#define X_MIN 0
#define WIN_SCORE 21
#define FAST_MODE 1

int keypressed();
void draw(int racket1_kkor1, int racket1_kkor2, int racket1_kkor3, int racket2_kkor1, int racket2_kkor2,
          int racket2_kkor3, int ball_x, int ball_y, int resLeft, int resRight);
int perem();
int ball_coord(int* way, int* x, int* y, int Ly, int Ry);

int main() {
    int countLeft = 0;
    int countRight = 0;
    int way = 2;

    while (1) {
        int racket1_kkor1 = (MAX_Y) / 2 + 1;
        int racket1_kkor2 = (MAX_Y) / 2;
        int racket1_kkor3 = (MAX_Y) / 2 - 1;

        int racket2_kkor1 = (MAX_Y) / 2 + 1;
        int racket2_kkor2 = (MAX_Y) / 2;
        int racket2_kkor3 = (MAX_Y) / 2 - 1;

        int ball_x = MAX_X / 2;
        int ball_y = MAX_Y / 2;
        way = 1 + (countLeft + countRight) % 4;

        while (1) {
            // рисовка
            printf("\033[2J\033[1;1H");
            draw(racket1_kkor1, racket1_kkor2, racket1_kkor3, racket2_kkor1, racket2_kkor2, racket2_kkor3,
                 ball_x, ball_y, countLeft, countRight);
            int res = ball_coord(&way, &ball_x, &ball_y, racket1_kkor2, racket2_kkor2);
            if (res == 1) {
                // ракетки
                int peremvalue = 0;
                if (FAST_MODE == 1) {
                    int tempperem = perem();
                    if (tempperem != 0) {
                        peremvalue = tempperem;
                    }
                } else {
                    peremvalue = perem();
                }

                printf("%d", peremvalue);
                if (peremvalue == 10 && racket1_kkor3 != MAX_Y - 3) {
                    racket1_kkor1++;
                    racket1_kkor2++;
                    racket1_kkor3++;
                }
                if (peremvalue == 11 && racket1_kkor1 != 4) {
                    racket1_kkor1--;
                    racket1_kkor2--;
                    racket1_kkor3--;
                }
                if (peremvalue == 20 && racket2_kkor3 != MAX_Y - 3) {
                    racket2_kkor1++;
                    racket2_kkor2++;
                    racket2_kkor3++;
                }
                if (peremvalue == 21 && racket2_kkor1 != 4) {
                    racket2_kkor1--;
                    racket2_kkor2--;
                    racket2_kkor3--;
                }
            } else if (res == 0) {
                countLeft++;
                break;
            } else {
                countRight++;
                break;
            }
            usleep(100000);
        }
        if (countLeft == WIN_SCORE) {
            printf("\033[2J\033[1;1H");
            printf("FINAL SCORE: %d:%d\nLEFT PLAYER WON!!\nCONGRATULATIONS", countLeft, countRight);
            getchar();
            break;
        }
        if (countRight == WIN_SCORE) {
            printf("\033[2J\033[1;1H");
            printf("FINAL SCORE: %d:%d\nRIGHT PLAYER WON!!\nCONGRATULATIONS", countLeft, countRight);
            getchar();
            break;
        }
    }
}

void draw(int racket1_kkor1, int racket1_kkor2, int racket1_kkor3, int racket2_kkor1, int racket2_kkor2,
          int racket2_kkor3, int ball_x, int ball_y, int res_left, int res_right) {
    char space = ' ';  //переменая для пробела
    char top = '#';    // переменная для верха/низа
    char left = '#';   // переменная право/лево и ракеток
    char rk = '|';
    char sh = '*';  // переменная шара

    // проходим по всем полю для отрисовки
    for (int i = 0; i <= MAX_Y; i++) {
        // printf("%d", i);
        if (i == 0 || i == MAX_Y) {  // прописовка верха
            for (int j = 0; j <= MAX_X; j++) {
                printf("%c", top);
            }
            printf("\n");
        }
        // прорисовка тела

        if ((i > 0 && i < MAX_Y)) {  //прорисовка сторон
            printf("%c", left);
            for (int j = 1; j < MAX_X; j++) {
                if ((i == racket1_kkor1 || i == racket1_kkor2 || i == racket1_kkor3) && j == 7) {
                    printf("%c", rk);
                } else if ((i == racket2_kkor1 || i == racket2_kkor2 || i == racket2_kkor3) && j == 73) {
                    printf("%c", rk);
                } else if (i == ball_y && j == ball_x) {
                    printf("%c", sh);
                } else if (i == (MAX_Y - 3) && j == 3) {
                    if (res_left < 10) {
                        printf("%d", res_left);
                    } else {
                        printf("%d", res_left / 10);
                    }
                } else if (i == (MAX_Y - 3) && j == 4) {
                    if (res_left >= 10) {
                        printf("%d", res_left % 10);
                    } else {
                        printf("%c", space);
                    }
                } else if (i == (MAX_Y - 3) && j == (MAX_X - 4)) {
                    if (res_left >= 10) {
                        printf("%d", res_right / 10);
                    } else {
                        printf("%c", space);
                    }
                } else if (i == (MAX_Y - 3) && j == (MAX_X - 3)) {
                    if (res_left < 10) {
                        printf("%d", res_right);
                    } else {
                        printf("%d", res_right % 10);
                    }
                } else
                    printf("%c", space);
            }
            printf("%c\n", left);
        }
    }
}
int keypressed() {
    struct termios oldt, newt;
    char ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

int perem() {
    if (keypressed()) {
        char k = 'k';
        char m = 'm';
        char a = 'a';
        char z = 'z';
        char prin;
        prin = getchar();
        printf("%c\n\n", prin);
        if (prin == k) {
            return 21;
        } else if (prin == m) {
            return 20;
        } else if (prin == a) {
            return 11;
        } else if (prin == z) {
            return 10;
        } else {
            return 0;
        }
    }
    return 0;
}

int ball_coord(int* way, int* x, int* y, int Ly, int Ry) {
    //если кто-то из игроков забил гол
    if (*x == MAX_X - 5) {
        return 0;
    }
    if (*x == X_MIN + 5) {
        return -1;
    }
    //если мяч ударяется о рокетку (1)
    if ((*way == 2 && *x == MAX_S1 - 1 && *y == Ry + 2)) {
        *way = 4;
        return 1;
    }
    if ((*way == 3 && *x == MAX_S1 - 1 && *y == Ry - 2)) {
        *way = 1;
        return 1;
    }
    if ((*way == 1 && *x == MAX_S + 1 && *y == Ly + 2)) {
        *way = 3;
        return 1;
    }
    if ((*way == 4 && *x == MAX_S + 1 && *y == Ly - 2)) {
        *way = 2;
        return 1;
    }
    //если мяч ударяется о рокетку (2)
    if ((*way == 2) && (*x == MAX_S1 - 1) && (*y == Ry || *y == Ry - 1 || *y == Ry + 1)) {
        *way = 1;
        *y = *y - 1;
        *x = *x - 1;
        return 1;
    }
    if ((*way == 3) && (*x == MAX_S1 - 1) && (*y == Ry || *y == Ry - 1 || *y == Ry + 1)) {
        *way = 4;
        *y = *y + 1;
        *x = *x - 1;
        return 1;
    }
    if ((*way == 1) && (*x == MAX_S + 1) && (*y == Ly || *y == Ly - 1 || *y == Ly + 1)) {
        *way = 2;
        *y = *y - 1;
        *x = *x + 1;
        return 1;
    }
    if ((*way == 4) && (*x == MAX_S + 1) && (*y == Ly || *y == Ly - 1 || *y == Ly + 1)) {
        *way = 3;
        *y = *y + 1;
        *x = *x + 1;
        return 1;
    }
    //если мяч ударяется от стенки
    if (*y == Y_MIN + 1 || *y == MAX_Y - 1) {
        if (*way == 1) {
            *x = *x - 1;
            *y = *y + 1;
            *way = 4;
            return 1;
        }
        if (*way == 2) {
            *x = *x + 1;
            *y = *y + 1;
            *way = 3;
            return 1;
        }
        if (*way == 3) {
            *x = *x + 1;
            *y = *y - 1;
            *way = 2;
            return 1;
        }
        if (*way == 4) {
            *x = *x - 1;
            *y = *y - 1;
            *way = 1;
            return 1;
        }
    }
    //остальные случаи
    if (*way == 1) {
        *x = *x - 1;
        *y = *y - 1;
        return 1;
    }
    if (*way == 2) {
        *x = *x + 1;
        *y = *y - 1;
        return 1;
    }
    if (*way == 3) {
        *x = *x + 1;
        *y = *y + 1;
        return 1;
    }
    if (*way == 4) {
        *x = *x - 1;
        *y = *y + 1;
        return 1;
    }
    return 1;
}
