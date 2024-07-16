#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  // Para Sleep() y manipulación de consola

#define ROWS 21
#define COLS 21

int maze[ROWS][COLS];

// Dirección de movimientos (derecha, abajo, izquierda, arriba)
int dir[4][2] = {
    {0, 1},   // derecha
    {1, 0},   // abajo
    {0, -1},  // izquierda
    {-1, 0}   // arriba
};

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printMaze() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == 1 && j == 1) {
                printf("E"); // Entrada
            } else if (i == ROWS - 2 && j == COLS - 2) {
                printf("S"); // Salida
            } else if (maze[i][j] == 1) {
                printf("#"); // Pared
            } else if (maze[i][j] == 2) {
                printf("."); // Camino recorrido
            } else {
                printf(" "); // Camino
            }
        }
        printf("\n");
    }
}

void generateMaze(int x, int y) {
    maze[x][y] = 0;

    int directions[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = directions[r];
        directions[r] = directions[i];
        directions[i] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int newX = x + dir[directions[i]][0] * 2;
        int newY = y + dir[directions[i]][1] * 2;

        if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS && maze[newX][newY] == 1) {
            maze[x + dir[directions[i]][0]][y + dir[directions[i]][1]] = 0;
            generateMaze(newX, newY);
        }
    }
}

int isValid(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0);
}

int solveMaze(int x, int y, int *steps) {
    if (x == ROWS - 2 && y == COLS - 2) {
        maze[x][y] = 2;
        gotoxy(0, 0);  // Mover el cursor al inicio del laberinto
        printMaze();
        return 1;
    }

    maze[x][y] = 2;
    gotoxy(0, 0);  // Mover el cursor al inicio del laberinto
    printMaze();
    Sleep(100);  // Delay para mostrar el proceso (100 milisegundos)

    for (int i = 0; i < 4; i++) {
        int newX = x + dir[i][0];
        int newY = y + dir[i][1];

        if (isValid(newX, newY)) {
            (*steps)++;
            if (solveMaze(newX, newY, steps)) {
                return 1;
            }
            (*steps)--;
        }
    }

    maze[x][y] = 0;
    return 0;
}

int main() {
    srand(time(NULL));

    // Inicializar el laberinto con paredes
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = 1;
        }
    }

    // Generar el laberinto aleatorio
    generateMaze(1, 1);

    int steps = 0;
    if (solveMaze(1, 1, &steps)) {
        printf("Laberinto resuelto en %d movimientos.\n", steps);
    } else {
        printf("No se pudo resolver el laberinto.\n");
    }

    return 0;
}
