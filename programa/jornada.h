#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include "../estructuras/estructuras.h"
#include "configuracion.h"

#define BUQUE_ID_INVALIDA 4294967295
#define JORNADA_REGISTRO_MAXTAM 256

typedef struct {

    unsigned buque_id;
    unsigned cont_id;

} tContenedor;

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

typedef struct {
    tLista historial;
    unsigned operador;
    unsigned tiempo_usado;
    unsigned tiempo_jornada;
    unsigned buques_des;
    unsigned conts_entr;
    unsigned reub_cont;
    unsigned cam_pend;
} tJornadaRegistro; //leva cuenta de valores a mostrar en el resumen final

/*
Tiempo utilizado: 21 min de 30
Buques descargados: 2 -> 10 puntos
Contenedores entregados: 5 -> 50 puntos
Reorganizaciones: 1
Camiones pendientes: 0 -> 0 puntos de penalización
Puntuación final: 60 puntos
*/

int jornada_mostrar_buques(const void *dato, FILE *fp);
int jornada_mostrar_camiones(const void *dato, FILE *fp);

void jornada_generar( tJornada* jornada, tConf config );
void jornada_destruir(tJornada* jornada);
//int jornada_de_archivo(tJornada* jornada, char* arch_nombre, tConf *conf);
int jornada_mostrar_cont_pila(tPila *pila, FILE *fp); //muestra una pila de contenedores (C4,C3,C2,C1)

void jornada_iniciar_registro(tJornadaRegistro *jreg, tConf conf);
unsigned jornada_puntaje(const tJornadaRegistro *jreg);
void jornada_registrar_evento(tJornadaRegistro *jreg, const char *evento);
void jornada_mostrar_historial(tJornadaRegistro *jreg);
void jornada_mostrar_registro(const tJornadaRegistro *jreg, FILE *fp);
void jornada_destruir_registro(tJornadaRegistro *jreg);

#endif // JORNADA_H_INCLUDED
