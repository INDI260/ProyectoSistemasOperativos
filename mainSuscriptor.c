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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>
#include "suscriptor.h"

// Función principal que maneja la ejecución del programa.
int main(int argc, char *argv[]) {

  char *pipeSSC = NULL;  // Variable para almacenar la ruta del pipe.

    // Procesa los argumentos de la línea de comandos.
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {  // Si se encuentra la opción -s, asigna el valor al pipeSSC.
            pipeSSC = argv[i + 1];
        }
    }

    // Verifica que se haya recibido el pipe por línea de comandos.
    if (pipeSSC == NULL) {
        fprintf(stderr, "Uso: %s -s pipeSSC\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  

    // Llama a las funciones de suscripción y recepción de noticias.
    suscripciones(pipeSSC);
    noticias(pipeSSC);

    return 0;
}