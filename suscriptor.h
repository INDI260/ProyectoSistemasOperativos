/********************************************************************************
MEMBRETE
Integrantes del grupo: Sara García, Alejandro Caicedo, Sergio, Nicolás
Materia: Sistemas Operativos
Tópico: Proyecto
Descripción del archivo: Declarar las funciones que se usarán en "suscriptor.c"
para mantener el orden de todas las operaciones que se realizarán allí.
********************************************************************************/

#ifndef SUSCRIPTOR_H 
#define SUSCRIPTOR_H

#define BUFFER_SIZE 100    //Tamaño máximo del buffer    
#define SUSCRIPTORESMAX 10//Numero máximo de suscriptores
#define TOPICOSMAX 5   //Número máximo de tópicos

void suscripciones(char *pipeSSC);
void noticias(char *pipeSSC);

#endif