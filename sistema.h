/********************************************************************************
MEMBRETE
Integrantes del grupo: Sara García, Alejandro Caicedo, Sergio, Nicolás
Materia: Sistemas Operativos
Tópico: Proyecto
Descripción del archivo: Declarar las funciones que se usarán en "sistema.c"
para mantener el orden de todas las operaciones que se realizarán allí.
********************************************************************************/

#ifndef SISTEMA_H 
#define SISTEMA_H

#include <semaphore.h>

#define BUFFER_SIZE 100        
#define SUSCRIPTORESMAX 10
#define TOPICOSMAX 5        

typedef struct {
    char temas[TOPICOSMAX][BUFFER_SIZE];  
    int num_temas;
} Suscriptor;

int validarTema(char tema_noticia, Suscriptor suscriptor);
void suscripciones(char *pipeSSC, Suscriptor suscriptores[], int *num_suscriptores, sem_t *semaforoSub);
void noticias(char *pipePSC, char *pipeSSC, Suscriptor suscriptores[], int num_suscriptores);


#endif