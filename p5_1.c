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

        Granjero*/
#include <stdio.h>

// Prototipos
void calcularVisitas(int *diasApertura, int n, int r, int *visitas, int *numVisitas, int *contador);

// Función main
int main()
{

    int contador = 0;
    // Días de apertura de la tienda
    int diasApertura[] = { 0,3,10,18,25,35,42,50,65,78,85};
    int n = sizeof(diasApertura) / sizeof(diasApertura[0]);

    // Duración del fertilizante
    int r = 15;

    // Arreglo para almacenar los días de visita
    int visitas[100];
    int numVisitas = 0;

    // Calcular los días de visita
    calcularVisitas(diasApertura, n, r, visitas, &numVisitas, &contador);

    // Imprimir los resultados
    printf("Dias de visita:\n");
    for (int i = 0; i < numVisitas; i++)
    {
        printf("%d ", visitas[i]);
    }
    printf("\n");
    printf("\n\nContador: %d", contador);

    return 0;
}

// Función para encontrar los días de visita
void calcularVisitas(int *diasApertura, int n, int r, int *visitas, int *numVisitas, int *contador)
{
    (*contador)++;
    (*contador)++;
    int ultimoDiaVisita = diasApertura[0];      // Día inicial de visita
    visitas[(*numVisitas)++] = ultimoDiaVisita; // Guardar el primer día de visita

    (*contador)++;
    (*contador)++;

    for (int i = 0; i < n;)
    {
        (*contador)++;

        int siguienteVisita = i; // Posición del día de la siguiente visita
        (*contador)++;
        // Buscar el último día dentro del rango permitido
        while (siguienteVisita < n && diasApertura[siguienteVisita] - ultimoDiaVisita <= r)
        {
            (*contador)++;
            siguienteVisita++;
        }
        (*contador)++;

        siguienteVisita--; // Ajustar al último día válido
        (*contador)++;

        if (siguienteVisita >= n || diasApertura[siguienteVisita] == ultimoDiaVisita)
        {
            break; // No quedan días válidos
        }
        (*contador)++;
        (*contador)++;
        (*contador)++;

        // Agregar el día a la lista de visitas
        ultimoDiaVisita = diasApertura[siguienteVisita];
        visitas[(*numVisitas)++] = ultimoDiaVisita;
        i = siguienteVisita + 1; // Avanzar al siguiente rango
    }
    (*contador)++;
    (*contador)++;

}
