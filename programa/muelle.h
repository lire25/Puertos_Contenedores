#ifndef MUELLE_H_INCLUDED
#define MUELLE_H_INCLUDED

#include "puerto.h"

//maneja funciones especificas para el muelle del puerto

int muelle_recibir_buque(tPuerto *puerto);
void muelle_mostrar(const void *dato, unsigned pos, FILE *fp);
int muelle_limpiar(const void *dato, void *arg, unsigned pos);

#endif // MUELLE_H_INCLUDED
