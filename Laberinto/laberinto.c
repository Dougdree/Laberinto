#include <stdio.h>   // Biblioteca estándar de entrada y salida
#include <stdlib.h>  // Biblioteca estándar de funciones generales (como rand)
#include <time.h>    // Biblioteca para manipulación de tiempo (para srand)
#include <windows.h> // Biblioteca para funciones específicas de Windows (como Sleep y SetConsoleCursorPosition)

#define ROWS 21      // Definir el número de filas del laberinto
#define COLS 21      // Definir el número de columnas del laberinto

int maze[ROWS][COLS]; // Declarar el laberinto como un arreglo bidimensional

// Direcciones de movimiento (derecha, abajo, izquierda, arriba)
int dir[4][2] = {
    {0, 1},   // derecha
    {1, 0},   // abajo
    {0, -1},  // izquierda
    {-1, 0}   // arriba
};

// Función para mover el cursor a una posición específica en la consola
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Función para imprimir el laberinto en la consola
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

// Función para generar el laberinto usando un algoritmo de laberinto aleatorio
void generateMaze(int x, int y) {
    maze[x][y] = 0; // Marcar la celda actual como camino

    int directions[4] = {0, 1, 2, 3}; // Arreglo para las direcciones
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4; // Seleccionar una dirección aleatoria
        int temp = directions[r];
        directions[r] = directions[i];
        directions[i] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int newX = x + dir[directions[i]][0] * 2;
        int newY = y + dir[directions[i]][1] * 2;

        if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS && maze[newX][newY] == 1) {
            maze[x + dir[directions[i]][0]][y + dir[directions[i]][1]] = 0; // Marcar el camino intermedio como libre
            generateMaze(newX, newY); // Llamar recursivamente para continuar generando el laberinto
        }
    }
}

// Función para verificar si una celda es válida para moverse
int isValid(int x, int y) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0);
}

// Función para resolver el laberinto utilizando un algoritmo de backtracking
int solveMaze(int x, int y, int *steps) {
    if (x == ROWS - 2 && y == COLS - 2) { // Si se llega a la salida
        maze[x][y] = 2; // Marcar la celda como parte del camino solucionado
        gotoxy(0, 0);  // Mover el cursor al inicio del laberinto
        printMaze();   // Imprimir el laberinto
        return 1;
    }

    maze[x][y] = 2; // Marcar la celda actual como parte del camino recorrido
    gotoxy(0, 0);  // Mover el cursor al inicio del laberinto
    printMaze();   // Imprimir el laberinto
    Sleep(100);    // Retardo para mostrar el proceso (100 milisegundos)

    for (int i = 0; i < 4; i++) {
        int newX = x + dir[i][0];
        int newY = y + dir[i][1];

        if (isValid(newX, newY)) { // Verificar si la nueva posición es válida
            (*steps)++;
            if (solveMaze(newX, newY, steps)) { // Llamada recursiva para continuar resolviendo el laberinto
                return 1;
            }
            (*steps)--;
        }
    }

    maze[x][y] = 0; // Si no se encuentra solución, desmarcar la celda
    return 0;
}

int main() {
    srand(time(NULL)); // Inicializar la semilla aleatoria

    // Inicializar el laberinto con paredes
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = 1;
        }
    }

    // Generar el laberinto aleatorio
    generateMaze(1, 1);

    int steps = 0; // Contador de pasos
    if (solveMaze(1, 1, &steps)) { // Resolver el laberinto
        printf("Laberinto resuelto en %d movimientos.\n", steps);
    } else {
        printf("No se pudo resolver el laberinto.\n");
    }

    return 0;
}

