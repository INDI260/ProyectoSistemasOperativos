/********************************************************************************************
MEMBRETE
Integrantes del grupo: Sara García, Alejandro Caicedo, Sergio, Nicolás
Materia: Sistemas Operativos
Tópico: Proyecto
Descripción del archivo: Este archivo contiene toda la lógica del publicador, es decir,
el que recibe las noticias del publicador, las procesa y las envia a los respectivos suscriptores
*********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>
#include "sistema.h"
 

// Función que valida si el tema de la noticia coincide con los temas que el suscriptor sigue
int validarTema(char tema_noticia, Suscriptor suscriptor) {
    // Compara el primer caracter del tema de la noticia con los temas del suscriptor
    for (int i = 0; i < suscriptor.num_temas; i++) {
        if (suscriptor.temas[i][0] == tema_noticia) {
            return 1;  // Si hay coincidencia, retorna 1 (verdadero)
        }
    }
    return 0;  // Si no hay coincidencia, retorna 0 (falso)
}

// Función para procesar las suscripciones de los suscriptores
void suscripciones(char *pipeSSC, Suscriptor suscriptores[], int *num_suscriptores, sem_t *semaforoSub) {
    int pipe_fd = open(pipeSSC, O_RDONLY);  // Abrir el pipe para recibir las suscripciones
    if (pipe_fd == -1) {
        mkfifo(pipeSSC, S_IFIFO|0640);  // Si no se puede abrir el pipe
        pipe_fd = open(pipeSSC, O_RDONLY);
    }

    printf("Entra a la función de procesamiento de suscripciones\n");

    Suscriptor nuevo_suscriptor = {0};  // Inicializar un nuevo suscriptor vacío
    char buffer[BUFFER_SIZE];

    // Leer la línea con los temas desde el pipe
    if (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        // Eliminar salto de línea si existe al final de la cadena
        buffer[strcspn(buffer, "\n")] = '\0';

        // Verificar si el buffer tiene algún contenido
        if (strlen(buffer) > 0) {
            // Separar los temas recibidos por comas
            char *tema = strtok(buffer, ",");
            while (tema != NULL) {
                // Eliminar posibles espacios al inicio del tema
                while (*tema == ' ') tema++;

                // Agregar el tema al suscriptor si hay espacio
                if (nuevo_suscriptor.num_temas < TOPICOSMAX) {
                    strncpy(nuevo_suscriptor.temas[nuevo_suscriptor.num_temas++], tema, BUFFER_SIZE);
                    printf("Tema recibido: '%s'\n", tema);
                }
                tema = strtok(NULL, ",");  // Obtener el siguiente tema
            }

            // Almacenar el nuevo suscriptor en la lista de suscriptores
            sem_wait(semaforoSub);  // Adquirir el semáforo para proteger el acceso a la lista
            suscriptores[*num_suscriptores] = nuevo_suscriptor;
            (*num_suscriptores)++;  // Incrementar el contador de suscriptores
            sem_post(semaforoSub);  // Liberar el semáforo
        } else {
            printf("No se recibió ningún tema para suscribir.\n");
        }
    }

    printf("Numero de suscriptores.%d\n: ", *num_suscriptores);
    close(pipe_fd);  // Cerrar el pipe de suscripciones
}

// Función para procesar las noticias recibidas desde el publicador y enviarlas a los suscriptores
void noticias(char *pipePSC, char *pipeSSC, Suscriptor suscriptores[], int num_suscriptores) {
    int pipe_fd = open(pipePSC, O_RDONLY);  // Abrir el pipe para recibir las noticias
    if (pipe_fd == -1) {
        perror("Error al abrir el pipe de noticias");
        exit(EXIT_FAILURE);  // Si no se puede abrir el pipe, termina el programa
    }

    int sub_fd = open(pipeSSC, O_WRONLY);  // Abrir el pipe de salida para enviar las noticias a los suscriptores
    if (sub_fd == -1) {
        perror("Error al abrir el pipe de salida para los suscriptores");
        close(pipe_fd);
        exit(EXIT_FAILURE);  // Si no se puede abrir el pipe, termina el programa
    }

    char buffer[BUFFER_SIZE];
    while (read(pipe_fd, buffer, sizeof(buffer)) > 0) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Eliminar el salto de línea al final de la noticia
        printf("Noticia recibida: %s\n", buffer);

        char tema_noticia = buffer[0];  // Extraer el primer carácter del buffer como tema de la noticia
        // Enviar la noticia a los suscriptores que estén interesados en este tema
        for (int i = 0; i < num_suscriptores; i++) {
            printf("Comparando tema_noticia '%c' con tema del suscriptor '%c'\n", tema_noticia, suscriptores[i].temas[0][0]);
            if (validarTema(tema_noticia, suscriptores[i])) {  // Compara el tema con los temas de cada suscriptor
                // Enviar la noticia al suscriptor
                if (write(sub_fd, buffer, strlen(buffer)) == -1) {
                    perror("Error al enviar noticia a suscriptor");
                }
                // Enviar un salto de línea adicional al suscriptor
                if (write(sub_fd, "\n", 1) == -1) {
                    perror("Error al enviar salto de línea");
                }
                break;  // Solo enviar la noticia a un suscriptor
            }
        }
    }

    close(pipe_fd);  // Cerrar el pipe de noticias
    close(sub_fd);   // Cerrar el pipe de salida para suscriptores
}