#ifndef ALMACEN_H_INCLUDED
#define ALMACEN_H_INCLUDED

#include "puerto.h"

int almac_tiene_cont(tPuerto* puerto, unsigned zona_id, tContenedor pedido);
int almac_tiene_espacio(const tPuerto*);
int almac_limpiar(void *dato, void *arg, unsigned pos);
void almac_mostrar_zona(const void* p, unsigned pos, FILE *fp);
int almac_zona_entregar(void *dato, void *arg, unsigned pos);

#endif // ALMACEN_H_INCLUDED
