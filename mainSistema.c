/********************************************************************************************
MEMBRETE
Integrantes del grupo: Sara García, Alejandro Caicedo, Sergio, Nicolás
Materia: Sistemas Operativos
Tópico: Proyecto
Descripción del archivo: En este archivo se procesan los argumentos que el usuario introduce 
al ejecutar el programa.
*********************************************************************************************/

//Librerias necesarias
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sistema.h"

// Semáforo para proteger la lista de suscriptores
sem_t semaforoSub;

int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;  // Pipes de noticias y suscripciones
    int timeF = 0;  // Tiempo de finalización (en segundos)

    // Se reciben los argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) pipePSC = argv[i + 1];  // Pipe para recibir noticias
        else if (strcmp(argv[i], "-s") == 0) pipeSSC = argv[i + 1];  // Pipe para recibir suscripciones
        else if (strcmp(argv[i], "-t") == 0) timeF = atoi(argv[i + 1]);  // Tiempo de espera
    }

    // Se verifica que estén los paramteros 
    if (pipePSC == NULL || pipeSSC == NULL || timeF <= 0) {
        fprintf(stderr, "Uso: %s -p pipePSC -s pipeSSC -t timeF\n", argv[0]);
        exit(EXIT_FAILURE);  // Terminar si los parámetros no son válidos
    }

    // Se inicializa el semaforo
    if (sem_init(&semaforoSub, 0, 1) != 0) {
        perror("Error al inicializar el semáforo");
        exit(EXIT_FAILURE);
    }

    Suscriptor suscriptores[SUSCRIPTORESMAX] = {0};  // Arreglo de suscriptores
    int num_suscriptores = 0;  // Contador de suscriptores

    // Procesar suscripciones y noticias
    suscripciones(pipeSSC, suscriptores, &num_suscriptores, &semaforoSub);
    noticias(pipePSC, pipeSSC, suscriptores, num_suscriptores);

    // Esperar el tiempo especificado antes de finalizar
    sleep(timeF);
    printf("Sistema finalizando...\n");

    // Destruir el semáforo
    sem_destroy(&semaforoSub);

    return 0;  // Finalizar el programa
}
