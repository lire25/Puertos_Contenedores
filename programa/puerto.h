#ifndef PUERTO_H_INCLUDED
#define PUERTO_H_INCLUDED

#include "jornada.h"

typedef struct {
    tJornada jornada_actual;
    tJornadaRegistro jregistro;
    tLista zonas_alm;
    tCola camiones_fila;
    tCola buques_fila;
    tLista muelles;
    tConf config;
    unsigned *almac_cant;
    unsigned minuto_actual;
} tPuerto;

#define MUELLE_VACIO BUQUE_ID_INVALIDA

void puerto_crear(const char *puerto_arch, tPuerto *puerto, tConf conf);
void puerto_destruir(tPuerto *puerto);
int puerto_jornada_finalizada(const tPuerto *puerto);
void puerto_finalizar_jornada(tPuerto *puerto);

void puerto_mostrar_estado(tPuerto*);
void puerto_ejecutar_minuto(tPuerto *puerto);
void puerto_avanzar_minuto(tPuerto* puerto, unsigned minutos);

int puerto_hay_operacion(tPuerto* puerto);
int puerto_entregar(tPuerto*);
int puerto_reubicar(tPuerto *puerto, unsigned zona_orig, unsigned zona_dest);
int puerto_descargar(tPuerto *puerto, unsigned muelle_orig, unsigned zona_dest);
void puerto_ver(tPuerto *puerto);

#endif // PUERTO_H_INCLUDED
