#include <iostream>
#include <windows.h>

using namespace std;// linea para simplificar las lineas cada que se usen los elementos de la biblioteca C++.

// Definir el tamaño del laberinto
const int Filas = 5;
const int Columnas = 10;

//Proceso que imprime el laberinto en la consola
void dibujarLaberinto(char laberinto[Filas][Columnas]) {
    for (int i = 0; i < Filas; i++) {
        for (int j = 0; j < Columnas; j++) {
            if (laberinto[i][j] == '#') {
                printf("#");
            } else if (laberinto[i][j] == '.') {
                printf(".");
            } else if (laberinto[i][j] == 'E') {
                printf("E");
            } else if (laberinto[i][j] == 'S') {
                printf("S");
            }
        }
        printf("\n");
    }
}

//Proceso que hace el camino interior y paredes del laberinto
void inicializarLaberinto(char laberinto[Filas][Columnas]) {
    // Llena el laberinto con paredes
    for (int i = 0; i < Filas; i++) {
        for (int j = 0; j < Columnas; j++) {
            laberinto[i][j] = '#';
        }
    }

    // Crea caminos en el laberinto
    laberinto[1][0] = 'E'; // Entrada
    laberinto[1][1] = '.';
    laberinto[1][2] = '.';
    laberinto[1][3] = '.';
    laberinto[1][5] = '.';
    laberinto[1][6] = '.';
    laberinto[1][7] = '.';
    laberinto[1][8] = '.';
    laberinto[2][1] = '.';
    laberinto[2][5] = '.';
    laberinto[2][8] = '.';
    laberinto[3][1] = '.';
    laberinto[3][2] = '.';
    laberinto[3][3] = '.';
    laberinto[3][4] = '.';
    laberinto[3][5] = '.';
    laberinto[3][7] = '.';
    laberinto[3][8] = '.';
    laberinto[3][9] = 'S'; // Salida
}

// Función para establecer el cursor en una posición específica
void cursor(int x, int y) {
    COORD posicion;//estructura de tipo COORD que almacena las coordenadas asignadas
    posicion.X = x;
    posicion.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicion);//coloca el cursor en las coordenadas especificadas
}


// Inicialización del contador de veces que se desmarca el camino
int retroceso = 0;


// Función de tipo booleana para mover el asterisco en el laberinto automáticamente utilizando recursividad hasta que llegue a 'S'
bool mover_asterisco(int x, int y, char laberinto[Filas][Columnas]) {
    // Verifica si está fuera de los límites horizontales del laberinto o en una pared
    if (x < 0 || x >= Columnas || y < 0 || y >= Filas || laberinto[y][x] == '#' || laberinto[y][x] == '*')
        return false;

    // Verifica si llegó al borde derecho del laberinto y si es asi termina el recorrido
    if (laberinto[y][x] == 'S') {
        cursor(x, y);
        printf("S");
        return true;
    }

    // Marca con un asterisco la posición actual como parte del camino
    cursor(x, y);
    printf("*");
    laberinto[y][x] = '*';


    // Intenta moverse en las cuatro direcciones (derecha, abajo, izquierda, arriba), dejando un lapso de 1 segundo entre cada condicion
    if (mover_asterisco(x + 1, y, laberinto)) return true; // Derecha
    Sleep(1000);
    if (mover_asterisco(x, y + 1, laberinto)) return true; // Abajo
    Sleep(1000);
    if (mover_asterisco(x - 1, y, laberinto)) return true; // Izquierda
    Sleep(1000);
    if (mover_asterisco(x, y - 1, laberinto)) return true; // Arriba
    Sleep(1000);

    // Si no hay salida en ninguna dirección, desmarca la posición
    cursor(x, y);
    printf(".");
    laberinto[y][x] = '.';
    retroceso++;

    // Retorna falso para indicar que no encontró salida desde esta posición
    return false;
}

//Función principal
int main() {
    // Coloca el cursor al inicio del laberinto (0, 0)
    cursor(0, 0);
    char laberinto[Filas][Columnas];

    //Llama a las funciones para inicializar e imprimir el laberinto en la consola
    inicializarLaberinto(laberinto);
    dibujarLaberinto(laberinto);

    // Inicia el movimiento del asterisco desde una posición inicial que es 1,1
    if (!mover_asterisco(1, 1, laberinto)) {//si la función devuelve false, significa que no hay salida e imprime un mensaje
        cursor(0, Filas);
        printf("No hay camino disponible hasta el otro lado del laberinto.\n");
    } else {
        cursor(0, Filas);//si no se cumple la función de arriba, quiere decir que el asterisco si llegó  's' e imprime un mensaje
        printf("¡Llegaste al otro lado del laberinto!\n");
    }

    // Muestra el contador de retrocesos
    printf("Veces que se desmarcó el camino: %d\n", retroceso);

    return 0;
}
