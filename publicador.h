/********************************************************************************
MEMBRETE
Integrantes del grupo: Sara García, Alejandro Caicedo, Sergio, Nicolás
Materia: Sistemas Operativos
Tópico: Proyecto
Descripción del archivo: Declarar las funciones que se usarán en "publicador.c"
para mantener el orden de todas las operaciones que se realizarán allí.
********************************************************************************/

#ifndef PUBLICADOR_H 
#define PUBLICADOR_H

#include <stdbool.h>

bool validarFormatoNoticia(const char* noticia);
void publicarNoticia(int pipe, const char* noticia);
void ejecutarPublicador(const char* pipePSC, const char* archivoNoticias, int tiempo);

#endif