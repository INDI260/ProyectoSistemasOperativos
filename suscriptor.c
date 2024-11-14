/********************************************************************************************
MEMBRETE
Integrantes del grupo: Sara García, Alejandro Caicedo, Sergio, Nicolás
Materia: Sistemas Operativos
Tópico: Proyecto
Descripción del archivo: Este archivo contiene toda la lógica del suscriptor, es decir,
el que se suscribe a algunos temas de intrés y recibe las noticias de sus intereses respectivos
*********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "suscriptor.h"


// Función que permite al suscriptor elegir los temas a los que se quiere suscribir.
void suscripciones(char *pipeSSC) {
    // Crea una tubería FIFO (named pipe) para la suscripción.
    mkfifo(pipeSSC, 0666);

    // Abre el pipe de suscripción en modo escritura.
    int pipe_fd = open(pipeSSC, O_WRONLY);
    if (pipe_fd == -1) {  // Si no se puede abrir el pipe, se muestra un error y se termina el programa.
        perror("Error al abrir el pipe de suscriptor para escritura");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        // Se ingresan los temas separados por coma
        printf("Ingrese sus temas de interés y separelos por comas (Arte(A), Politica(P), Ciencia(C), Espectaculo(E), Sucesos(S)):\n");
        char tema[BUFFER_SIZE];
        fgets(tema, sizeof(tema), stdin);  //Lee la entrada del usuario.

        // Elimina el salto de línea al final de la cadena de texto.
        tema[strcspn(tema, "\n")] = '\0';

        // Si el usuario no ingresó ningún tema, solicita que ingrese al menos uno.
        if (tema[0] == '\0') {
            printf("Debe suscribirse a al menos un tema.\n");
            continue;  // Vuelve al inicio del ciclo para pedir al usuario que ingrese un tema válido.
        }

        // Envia los temas al pipe.
        if (write(pipe_fd, tema, strlen(tema) + 1) == -1) {  // El +1 es para incluir el terminador de cadena '\0'.
            perror("Error al enviar los temas");
            close(pipe_fd);  // Cierra el pipe en caso de error.
            exit(EXIT_FAILURE);
        }

        // Al enviarse los temas se termina el ciclo
        break;
    }

    close(pipe_fd);  // Se cierra el pipe
}


// Función que recibe las noticias del publicador y las muestra en pantalla.
void noticias(char *pipeSSC) {
    // Abre el pipe de suscripción en modo lectura.
    int pipe_fd = open(pipeSSC, O_RDONLY);
    if (pipe_fd == -1) {  // Si no se puede abrir el pipe se muestra un error y se termina la ejecucion del programa.
        perror("Error al abrir el pipe para recibir noticias");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    printf("Esperando noticias...\n");

    // Lee las noticias del pipe hasta que se termine de leer.
    while (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        // Muestra las noticias recibidas.
        if(sizeof(buffer) > 10)
          printf("Noticia recibida: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));  // Limpia el buffer después de mostrar la noticia.
    }

    close(pipe_fd);  // Cierra el pipe.
}
