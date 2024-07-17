#include <stdio.h>   // Biblioteca estándar de entrada y salida
#include <stdlib.h>  // Biblioteca estándar de funciones generales (como rand)
#include <time.h>    // Biblioteca para manipulación de tiempo (para srand)
#include <windows.h> // Biblioteca para funciones específicas de Windows (como Sleep y SetConsoleCursorPosition)

#define FILAS 13     // Definir el número de filas del laberinto
#define COLUMNAS 13  // Definir el número de columnas del laberinto

int laberinto[FILAS][COLUMNAS]; // Declarar el laberinto como un arreglo bidimensional

// Direcciones de movimiento (derecha, abajo, izquierda, arriba)
int direccion[4][2] = {
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
void imprimirLaberinto() {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == 1 && j == 1) {
                printf("E"); // Entrada
            } else if (i == FILAS - 2 && j == COLUMNAS - 2) {
                printf("S"); // Salida
            } else if (laberinto[i][j] == 1) {
                printf("#"); // Pared
            } else if (laberinto[i][j] == 2) {
                printf("."); // Camino recorrido
            } else {
                printf(" "); // Camino
            }
        }
        printf("\n");
    }
}

// Función para generar el laberinto usando un algoritmo de laberinto aleatorio
void generarLaberinto(int x, int y) {
    laberinto[x][y] = 0; // Marcar la celda actual como camino

    int direcciones[4] = {0, 1, 2, 3}; // Arreglo para las direcciones
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4; // Seleccionar una dirección aleatoria
        int temp = direcciones[r];
        direcciones[r] = direcciones[i];
        direcciones[i] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nuevaX = x + direccion[direcciones[i]][0] * 2;
        int nuevaY = y + direccion[direcciones[i]][1] * 2;

        if (nuevaX >= 0 && nuevaX < FILAS && nuevaY >= 0 && nuevaY < COLUMNAS && laberinto[nuevaX][nuevaY] == 1) {
            laberinto[x + direccion[direcciones[i]][0]][y + direccion[direcciones[i]][1]] = 0; // Marcar el camino intermedio como libre
            generarLaberinto(nuevaX, nuevaY); // Llamar recursivamente para continuar generando el laberinto
        }
    }
}

// Función para verificar si una celda es válida para moverse
int esValido(int x, int y) {
    return (x >= 0 && x < FILAS && y >= 0 && y < COLUMNAS && laberinto[x][y] == 0);
}

// Función para resolver el laberinto utilizando un algoritmo de backtracking
int resolverLaberinto(int x, int y, int *pasos) {
    if (x == FILAS - 2 && y == COLUMNAS - 2) { // Si se llega a la salida
        laberinto[x][y] = 2; // Marcar la celda como parte del camino solucionado
        gotoxy(0, 0);  // Mover el cursor al inicio del laberinto
        imprimirLaberinto();   // Imprimir el laberinto
        return 1;
    }

    laberinto[x][y] = 2; // Marcar la celda actual como parte del camino recorrido
    gotoxy(0, 0);  // Mover el cursor al inicio del laberinto
    imprimirLaberinto();   // Imprimir el laberinto
    Sleep(100);    // Retardo para mostrar el proceso (100 milisegundos)

    for (int i = 0; i < 4; i++) {
        int nuevaX = x + direccion[i][0];
        int nuevaY = y + direccion[i][1];

        if (esValido(nuevaX, nuevaY)) { // Verificar si la nueva posición es válida
            (*pasos)++;
            if (resolverLaberinto(nuevaX, nuevaY, pasos)) { // Llamada recursiva para continuar resolviendo el laberinto
                return 1;
            }
            (*pasos)--;
        }
    }

    laberinto[x][y] = 0; // Si no se encuentra solución, desmarcar la celda
    return 0;
}

int main() {
    srand(time(NULL)); // Inicializar la semilla aleatoria

    // Inicializar el laberinto con paredes
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            laberinto[i][j] = 1;
        }
    }

    // Generar el laberinto aleatorio
    generarLaberinto(1, 1);
    FILE *archivoLaberinto;
    archivoLaberinto = fopen("laberinto_archivo.txt", "w");
    if (archivoLaberinto == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            fprintf(archivoLaberinto, "%d", laberinto[i][j]);
            if (j < COLUMNAS - 1) {
                fprintf(archivoLaberinto, " "); // Agrega espacio entre columnas
            }
        }
        fprintf(archivoLaberinto, "\n"); // Agrega salto de línea después de cada fila
    }
    fclose(archivoLaberinto);

    int pasos = 0; // Contador de pasos
    if (resolverLaberinto(1, 1, &pasos)) { // Resolver el laberinto
        printf("Laberinto resuelto en %d movimientos.\n", pasos);
    } else {
        printf("No se pudo resolver el laberinto.\n");
    }

    return 0;
}

