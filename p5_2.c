/*INSTITUTO POLITECNICO NACIONAL
ESCUELA SUPERIOR DE COMPUTO

Analisis y Diseno de Algoritmos
Grupo: 3CM3

Alumnos:
    Rubio Haro Diego

Profesor:
    LUNA BENOSO BEJAMIN

Fecha: 29-Noviembre-2024

Practica 5: Algoritmos Greedy

        Laberinto*/

// Librerias
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*Parámetros de compilación: -lgdi32*/

// Macros
#define CELDA_SIZE 50 // Tamaño de cada celda

// Variables globales
int N, contador = 0;                       // Tamaño del laberinto
int **laberinto;                           // Matriz dinámica para el laberinto
int laberintoFijo = 0;                     // Indica si el laberinto está fijo
int marcadoEntrada = 0, marcadoSalida = 0; // Variables globales que verifican las coordenadas de entrada y salida
int entradaX = 0, entradaY = 0;            // Coordenadas de la entrada
int salidaX = 0, salidaY = 0;              // Coordenadas de la salida
// Prototipos de funciones
void InicializarLaberinto();
void LiberarLaberinto();
void DibujarLaberinto(HDC hdc);
int ResolverLaberinto(HWND hwnd);
void FijarLaberinto(HWND hwnd);
void ProcesarClick(HWND hwnd, int x, int y);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Funcion main
int main()
{
    printf("Ingrese la longitud del laberinto (N): ");
    scanf("%d", &N);
    InicializarLaberinto();

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "LaberintoWindowClass";

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "Error al registrar la clase de ventana", "Error", MB_ICONERROR);
        return 1;
    }

    // Aseguramos que el tamaño de la ventana sea adecuado
    int width = N * CELDA_SIZE + 200; // Ajuste de ancho de ventana para acomodar los botones
    int height = N * CELDA_SIZE + 60; // Ajuste de altura para acomodar los botones

    HWND hwnd = CreateWindow(wc.lpszClassName, "Laberinto con Algoritmo Greedy", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             width, height,
                             NULL, NULL, wc.hInstance, NULL);

    if (!hwnd)
    {
        MessageBox(NULL, "Error al crear la ventana", "Error", MB_ICONERROR);
        return 1;
    }

    // Crear los botones a la derecha del laberinto (al lado derecho de la ventana)
    CreateWindow("BUTTON", "Fijar Laberinto", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 N * CELDA_SIZE + 20, ((N * CELDA_SIZE) / 3) + 20, 120, 30, hwnd, (HMENU)1, wc.hInstance, NULL);

    CreateWindow("BUTTON", "Inicio", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 N * CELDA_SIZE + 20, ((N * CELDA_SIZE) / 3) + 60, 120, 30, hwnd, (HMENU)2, wc.hInstance, NULL);

    CreateWindow("BUTTON", "Fin", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 N * CELDA_SIZE + 20, ((N * CELDA_SIZE) / 3) + 100, 120, 30, hwnd, (HMENU)3, wc.hInstance, NULL);
    CreateWindow("BUTTON", "Ejecutar", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 N * CELDA_SIZE + 20, ((N * CELDA_SIZE) / 3) + 140, 120, 30, hwnd, (HMENU)4, wc.hInstance, NULL);
    // Asegúrate de que los botones sean visibles
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    printf("\n\nContador: %d\n", contador);
    return 0;
}

// Funcion que inicializa la matriz en cero
void InicializarLaberinto()
{
    laberinto = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        laberinto[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++)
        {
            laberinto[i][j] = 0; // Todas las celdas comienzan en blanco
        }
    }
}

// Funcion que libera la memoria dinamica de la matriz
void LiberarLaberinto()
{
    for (int i = 0; i < N; i++)
    {
        free(laberinto[i]);
    }
    free(laberinto);
}

// Funcion que dibuja el laberinto dados ciertos parametros
void DibujarLaberinto(HDC hdc)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int x = j * CELDA_SIZE + 10;
            int y = i * CELDA_SIZE + 10;

            // Seleccionar el color según el estado de la celda
            if (laberinto[i][j] == 1) // Marca la celda en negro
            {
                SelectObject(hdc, GetStockObject(BLACK_BRUSH)); // Pared
            }
            else if (laberinto[i][j] == 2) // Marcará el camino en verde si se completa
            {
                SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0))); // Verde para caminos visitados
            }
            else if (laberinto[i][j] == 3) // Marcará la celda en rojo para la salida
            {
                SelectObject(hdc, CreateSolidBrush(RGB(153, 0, 51))); // Rojo para la salida
            }
            else if (laberinto[i][j] == 4) // Marcará la celda en azul para la entrada
            {
                SelectObject(hdc, CreateSolidBrush(RGB(0, 51, 153))); // Azul para el inicio
            }
            else
            {
                SelectObject(hdc, GetStockObject(WHITE_BRUSH)); // Celda vacía
            }
            Rectangle(hdc, x, y, x + CELDA_SIZE, y + CELDA_SIZE);
        }
    }
}

// Funcion Euristica de Greedy Manhattan
int ResolverLaberinto(HWND hwnd)
{
    printf("\n\nInicio (%d,%d)", entradaX, entradaY);
    printf("\nFinal (%d,%d)", salidaX, salidaY);
    int x = entradaX, y = entradaY;
    laberinto[x][y] = 2; // Marcar la entrada como visitada
    laberinto[salidaX][salidaY] = 0;
    int length = 0;
    contador++;
    contador++;
    while (x != salidaX || y != salidaY)
    {
        contador++;
        contador++;
        contador++;

        length++;
        int mejorX = -1, mejorY = -1, menorDistancia = INT_MAX;

        // Probar las 4 direcciones (arriba, abajo, izquierda, derecha)
        int direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        contador++;
        contador++;

        for (int i = 0; i < 4; i++)
        {
            contador++;
            contador++;

            int nx = x + direcciones[i][0];
            int ny = y + direcciones[i][1];

            if (nx >= 0 && ny >= 0 && nx < N && ny < N && laberinto[nx][ny] == 0)
            {
                int distancia = abs(nx - salidaX) + abs(ny - salidaY);
                contador++;

                if (distancia < menorDistancia)
                {
                    contador++;
                    contador++;

                    mejorX = nx;
                    mejorY = ny;
                    menorDistancia = distancia;
                }
            }
        }

        if (mejorX == -1 || mejorY == -1)
        {
            contador++;

            MessageBox(hwnd, "No se pudo encontrar una solucion.", "Greedy", MB_OK | MB_ICONERROR);
            return 0;
        }

      
        x = mejorX;
        y = mejorY;
        laberinto[x][y] = 2; // Marcar como visitada

        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        Sleep(100);
    }

    MessageBox(hwnd, "Camino encontrado!", "Greedy", MB_OK | MB_ICONINFORMATION);
    printf("\nLongitud de camino: %d", length);
    return 1;
}

// Funcion que actua como evento del boton fijar, bloquea la tabla
void FijarLaberinto(HWND hwnd)
{
    laberintoFijo = 1;

    // Habilitar los botones de Entrada y Salida
    EnableWindow(GetDlgItem(hwnd, 2), TRUE); // Marcar Entrada
    EnableWindow(GetDlgItem(hwnd, 3), TRUE); // Marcar Salida

    // Eliminar la entrada manual de la salida
    // No necesitamos pedir las coordenadas de la salida más
    // if (!ResolverLaberinto(hwnd))
    // {
    //     MessageBox(hwnd, "Intente otro laberinto.", "Greedy", MB_OK | MB_ICONERROR);
    // }
}

/* Funcion que procesa el tipo de clic o marca que se hace en la tabla
   por ejemplo, si no esta fijo el laberinto puedes marcar y poner obstaculos, y
   si esta bloqueado solo puedes presionar los botones de marca inicio y fin del
   del recorrido*/
void ProcesarClick(HWND hwnd, int x, int y) // Agregar hwnd como parámetro
{
    int i = y / CELDA_SIZE;
    int j = x / CELDA_SIZE;

    if (i >= 0 && i < N && j >= 0 && j < N)
    {
        if (!laberintoFijo) // Si el laberinto NO está fijado
        {
            // Si la celda está vacía (valor 0), marcarla como pared (valor 1)
            if (laberinto[i][j] == 0)
            {
                laberinto[i][j] = 1; // Marcar la celda como pared (negra)
            }
            // Si la celda está marcada como pared (valor 1), desmarcarla (cambiarla a vacía)
            else if (laberinto[i][j] == 1)
            {
                laberinto[i][j] = 0; // Desmarcar la celda (vacía)
            }
        }
        else // Si el laberinto ESTÁ fijado
        {
            // Si la celda está marcada como entrada (valor 4), desmarcarla
            if (laberinto[i][j] == 4)
            {
                // Desmarcar la entrada
                laberinto[i][j] = 0; // Ponerla de vuelta en vacío
                entradaX = -1;
                entradaY = -1;
                marcadoEntrada = 0;                      // Permitir marcar la entrada de nuevo
                EnableWindow(GetDlgItem(hwnd, 2), TRUE); // Habilitar el botón "Marcar Entrada" nuevamente
            }
            // Si la celda está marcada como salida (valor 3), desmarcarla
            else if (laberinto[i][j] == 3)
            {
                // Desmarcar la salida
                laberinto[i][j] = 0; // Ponerla de vuelta en vacío
                salidaX = -1;
                salidaY = -1;
                marcadoSalida = 0;                       // Permitir marcar la salida de nuevo
                EnableWindow(GetDlgItem(hwnd, 3), TRUE); // Habilitar el botón "Marcar Salida" nuevamente
            }
            // Marcar la entrada
            else if (marcadoEntrada == 1 && laberinto[i][j] == 0)
            {
                laberinto[i][j] = 4; // Marcar como entrada
                entradaX = i;
                entradaY = j;
                marcadoEntrada = 2; // Indicar que ya se seleccionó la entrada
                // Deshabilitar el botón de entrada para evitar más marcas
                EnableWindow(GetDlgItem(hwnd, 2), FALSE);
            }
            // Marcar la salida
            else if (marcadoSalida == 1 && laberinto[i][j] == 0)
            {
                laberinto[i][j] = 3; // Marcar como salida
                salidaX = i;
                salidaY = j;
                marcadoSalida = 2; // Indicar que ya se seleccionó la salida
                // Deshabilitar el botón de salida para evitar más marcas
                EnableWindow(GetDlgItem(hwnd, 3), FALSE);
            }
        }
    }
}

// Funcion nativa de la API de 'windows.h' que permite registrar y controlar los eventos de los botones
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        DibujarLaberinto(hdc);
        EndPaint(hwnd, &ps);
        break;

    case WM_LBUTTONDOWN:
        ProcesarClick(hwnd, LOWORD(lParam), HIWORD(lParam)); // Pasar hwnd
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_COMMAND:
        // arreglo de eventos de los botones en (hwnd)*n, siendo n el boton n creado en la funcion main
        switch (LOWORD(wParam))
        {
        case 1: // Botón "Fijar Laberinto"
            FijarLaberinto(hwnd);
            break;

        case 2: // Botón "Marcar Entrada"
            if (marcadoEntrada == 0)
            {
                marcadoEntrada = 1; // Indicar que se puede marcar la entrada
            }
            break;

        case 3: // Botón "Marcar Salida"
            if (marcadoSalida == 0)
            {
                marcadoSalida = 1; // Indicar que se puede marcar la salida
            }
            break;
        case 4:
            if (!ResolverLaberinto(hwnd))
            {
                MessageBox(hwnd, "Intente otro laberinto.", "Greedy", MB_OK | MB_ICONERROR);
            }
        }
        break;

    case WM_DESTROY:
        LiberarLaberinto();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
