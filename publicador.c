/********************************************************************************************
MEMBRETE
Integrantes del grupo: Sara García, Alejandro Caicedo, Sergio, Nicolás
Materia: Sistemas Operativos
Tópico: Proyecto
Descripción del archivo: Este archivo contiene toda la lógica del publicador, es decir,
el que lee las noticias de un archivo y las envía a la tubería siempre y cuando tengan
el formato requerido, en caso de que no sea el formato correcto la noticia no será publicada
*********************************************************************************************/

//Librerias necesarias
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "publicador.h"

//Función para verificar si la noticia tiene el formato requerido 
bool validarFormatoNoticia(const char* noticia) {
  //Verifica que la noticia no esté vacía 
    if (noticia == NULL || strlen(noticia) == 0) return false;

    //Verifica que el primer caracter sea correcto (A, E, C, P, S)
    char tipo = noticia[0];
    if (tipo != 'A' && tipo != 'E' && tipo != 'C' && tipo != 'P' && tipo != 'S') {
        return false;
    }

    //Verifica que después de la letra haya ":"
    char* pos = strchr(noticia, ':');
    if (pos == NULL || pos == noticia || *(pos + 1) == '\0') {
        return false;
    }

    return true; 
}

//Función para enviar noticias al pipe
void publicarNoticia(int pipe, const char* noticia) {
  //Write para enviar al pipe y +1 para incluir el terminador de cadena
    write(pipe, noticia, strlen(noticia) + 1);  
}

//Función principal del publicador
void ejecutarPublicador(const char* pipePSC, const char* archivoNoticias, int tiempo) {
    //Se abre el archivo de noticias, "r" para leer
    FILE* archivo = fopen(archivoNoticias, "r");
    if (archivo == NULL) {
      //En caso de error al abrir el archivo se muestra un mensaje
        fprintf(stderr, "Error al abrir la noticia.\n");
        return;
    }

    //Abre la tuberia 
    //Modo escritura
    int pipe = open(pipePSC, O_WRONLY);
    if (pipe == -1) {
      //En caso de no poder abrir se crea
      mkfifo(pipePSC, S_IFIFO|0640);
      pipe = open(pipePSC, O_WRONLY);
    }

    //Variable para leer las líneas de cada archivo
    char linea[81];
  
  //Se lee cada línea de la noticia
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Eliminar espacios al principio de la línea 
        char* start = linea;
        while (*start == ' ' || *start == '\t') start++;

        //Elimina espacios y al final de la línea
        char* end = linea + strlen(linea) - 1;
        while (end > start && (*end == ' ' || *end == '\n' || *end == '\t')) end--;
        *(end + 1) = '\0'; 

        //Se valida la noticia antes de publicarla
        if (validarFormatoNoticia(start)) {
            //Si la noticia es válida se publica en la tubería
            publicarNoticia(pipe, start);
        } else {
            //Si no es válida se informa 
            fprintf(stderr, "La noticia tiene el formato incorrecto: %s\n", start);
        }

        //Se espera el tiempo que se especifica en los parámetros
        sleep(tiempo);
    }

    //Se cierra el archivo 
    fclose(archivo);
    //Se cierra el pipe
    close(pipe);
}