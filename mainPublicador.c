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
#include "publicador.h"

int main(int argc, char* argv[]) {
    //Variable que almacena la ruta de la tubería
    const char* pipePSC = NULL;
    //Variable que almacena la ruta del archivo
    const char* archivoNoticias = NULL;
    //Variable para almacenar el tiempo de publicación entre cada noticia
    int tiempo = 0;

    // Se procesan los argumentos de la línea de comandos
    int opt;
    while ((opt = getopt(argc, argv, "p:f:t:")) != -1) {
        switch (opt) {
            //optarg equivale al valor asociado con cada opción
            //Ruta del pipe
            case 'p':
                pipePSC = optarg;
                break;
            //Ruta del archivo
            case 'f':
                archivoNoticias = optarg;
                break;
            //Tiempo de espera entre noticias
            case 't':
                tiempo = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Uso incorrecto de los argumentos.\n");
                return 1;
        }
    }

    //Si los argumentos son inválidos se muestra mensaje de error
    if (pipePSC == NULL || archivoNoticias == NULL || tiempo <= 0) {
        fprintf(stderr, "Faltan argumentos necesarios.\n");
        return 1;
    }

    //Se ejecutar el publicador con los argumentos proporcionados
    ejecutarPublicador(pipePSC, archivoNoticias, tiempo);

    return 0;
}