#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include "../estructuras/estructuras.h"
#include "configuracion.h"

typedef struct {

    unsigned buque_id;
    unsigned cont_id;

} tContenedor;

#define BUQUE_ID_INVALIDA 4294967295

typedef struct {
    unsigned id;
    unsigned minuto_arrivo;
    tPila contenedores;
} tBuque;

typedef struct {
    unsigned id;
    unsigned minuto_arrivo;
    tContenedor pedido;
} tCamion;

typedef struct {
    tLista buques_info; //los buques que llegan en la jornada
    tLista camiones_info; //los camiones que llegan en la jornada
} tJornada;

int jornada_mostrar_buques(const void *dato, FILE *fp);
int jornada_mostrar_camiones(const void *dato, FILE *fp);

void jornada_generar( tJornada* jornada, tConf config );
void jornada_destruir(tJornada* jornada);
int jornada_de_archivo(tJornada* jornada, char* arch_nombre, tConf *conf);
int jornada_mostrar_cont_pila(tPila *pila, FILE *fp); //muestra una pila de contenedores (C4,C3,C2,C1)

#endif // JORNADA_H_INCLUDED
