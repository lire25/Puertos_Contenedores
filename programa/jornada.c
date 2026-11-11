#include "jornada.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "utiles.h"

//no lo uso pero queda por las dudas :D
#define RAND_RANGOI(min, max) (min)+rand()%((max)-(min))

int jornada_ordenar_camiones(const void *a, const void *b)
{
    tCamion* c1 = (tCamion*)a;
    tCamion* c2 = (tCamion*)b;

    if (c1->id == c2->id) return 0; //es duplicado

    int diff = (int)c2->minuto_arrivo - (int)c1->minuto_arrivo;

    return diff > 0 ? -1 : 1;
}

int jornada_ordenar_buques(const void *a, const void *b)
{

    tBuque *b1 = (tBuque*)a;
    tBuque *b2 = (tBuque*)b;

    if ( b1->id == b2->id ) return 0; //es duplicado

    int diff = (int)b2->minuto_arrivo - (int)b1->minuto_arrivo;

    return (diff > 0) ? -1 : 1;

}

//cuando se genera la lista de contenedores, se ordena aleatoreamente para darse a los camiones
int jornada_ordenar_contenedores(const void *a, const void *b)
{

    return rand()%2;

}

int jornada_mostrar_conts(const void *dato, FILE *fp)
{

    tContenedor *c1 = (tContenedor*)dato;
    fprintf(fp, "\tC%u-%u\n", c1->buque_id, c1->cont_id);

    return 1;

}
int jornada_mostrar_buques(const void *dato, FILE *fp)
{

    tBuque *b1 = (tBuque*)dato;

    fprintf(fp, "#B%u | T:%u\n", b1->id, b1->minuto_arrivo);
    pila_mostrar(&b1->contenedores, jornada_mostrar_conts, fp);
    return 1;

}
int jornada_mostrar_camiones(const void *dato, FILE *fp)
{

    tCamion *c1 = (tCamion*)dato;

    fprintf(fp, "#K%u | T:%u | C%u-%u\n", c1->id, c1->minuto_arrivo, c1->pedido.buque_id, c1->pedido.cont_id);

    return 1;

}

void jornada_generar( const char *arch_puerto, tJornada* jornada, tConf config )
{

    tCamion camion;
    tBuque buque;
    tContenedor cont;

    tLista contenedores;

    FILE *fp = fopen(arch_puerto, "w");
    if (!fp)
        return;

    fprintf(fp, "[CONFIGURACION]\n");
    fprintf(
            fp, "JORNADA:%u\nMUELLES:%u\nZONAS:%u\nCAPACIDAD_PILA:%u\n\n",
            config.duracion_jornada_minutos,config.cant_muelles,config.cant_zonas_alm,
            config.cap_pila
            );

    lista_crear(&jornada->buques_info);
    lista_crear(&jornada->camiones_info);
    lista_crear(&contenedores);

    unsigned max_tiempo_buque;
    unsigned max_tiempo_camion = config.duracion_jornada_minutos - (config.tiempo_desc_cont + config.tiempo_carga_camion);
    unsigned cant_conts;

    fprintf(fp, "[BUQUES]\n");
    for (int i = 0; i < config.max_buques; i++)
    {

        buque.id = i+1;
        cant_conts = 1+rand()%config.max_cont_por_buque;
        max_tiempo_buque = config.duracion_jornada_minutos - (config.tiempo_desc_cont + config.tiempo_carga_camion)*cant_conts;
        buque.minuto_arrivo = rand()%max_tiempo_buque;
        fprintf(fp, "B%u;T%u;", buque.id, buque.minuto_arrivo);

        pila_crear(&buque.contenedores);
        for (int j = 0; j < cant_conts; j++)
        {
            cont.buque_id = i+1;
            cont.cont_id = (cant_conts-j);
            pila_poner(&buque.contenedores, &cont, sizeof(tContenedor));
            lista_poner_final(&contenedores, &cont, sizeof(tContenedor));
            if (j != 0)
                fputc(',', fp);
            fprintf(fp, "C%u-%u", cont.buque_id,j+1);
        }
        fputc('\n', fp);

        lista_poner_ordenado(&jornada->buques_info, &buque, sizeof(tBuque), jornada_ordenar_buques, NULL);

    }

    fprintf(fp, "\n[CAMIONES]\n");
    for (int i = 0; i < config.max_camiones; i++)
    {

        //si no quedan mas contenedores para retirar, no se crean mas camiones.
        //(solo pasa si hay mas camiones de lo que hay contenedores)
        if (lista_es_vacia(&contenedores))
            break;

        camion.id = i+1;
        camion.minuto_arrivo = rand()%max_tiempo_camion;
        lista_sacar_comienzo(&contenedores, &camion.pedido, sizeof(tContenedor));
        lista_poner_ordenado(&jornada->camiones_info, &camion, sizeof(tCamion), jornada_ordenar_camiones, NULL);

        fprintf(fp, "K%u;T%u;C%u-%u\n", camion.id, camion.minuto_arrivo, camion.pedido.buque_id, camion.pedido.cont_id);

    }

    lista_vaciar(&contenedores);

    fclose(fp);

}
void jornada_destruir(tJornada* jornada)
{

    lista_vaciar(&jornada->buques_info);
    lista_vaciar(&jornada->camiones_info);

}

int jornada_mostrar_cont_pila(tPila *pila, FILE *fp)
{

    tPila pila_aux;
    pila_crear(&pila_aux);
    tContenedor cont;
    unsigned pos = 0;

    while (!pila_es_vacia(pila))
    {
        pila_sacar(pila, &cont, sizeof(tContenedor));
        pila_poner(&pila_aux, &cont, sizeof(tContenedor));
        if (pos != 0)
            printf(" | ");
        fprintf(fp, "C%u-%u", cont.buque_id, cont.cont_id);
        pos++;
    }
    while (!pila_es_vacia(&pila_aux))
    {
        pila_sacar(&pila_aux, &cont, sizeof(tContenedor));
        pila_poner(pila, &cont, sizeof(tContenedor));
    }

    pila_vaciar(&pila_aux);

    return 0;

}

//----------REGISTRO DE JORNADAS----------

#define PUNTAJE_VALOR_DES 5
#define PUNTAJE_VALOR_ENT 10
//camion pendiente
#define PUNTAJE_VALOR_CAM 2

void jornada_iniciar_registro(tJornadaRegistro *jreg, tConf conf)
{

    lista_crear(&jreg->historial);

    jreg->buques_des = 0;
    jreg->cam_pend = 0;
    jreg->conts_entr = 0;
    jreg->reub_cont = 0;
    jreg->tiempo_usado = 0;
    jreg->tiempo_jornada = conf.duracion_jornada_minutos;

}
unsigned jornada_puntaje(const tJornadaRegistro *jreg)
{

    unsigned puntaje = 0;

    puntaje += jreg->buques_des * PUNTAJE_VALOR_DES;
    puntaje += jreg->conts_entr * PUNTAJE_VALOR_ENT;
    puntaje -= minimo(puntaje, jreg->cam_pend*PUNTAJE_VALOR_CAM);

    return puntaje;

}
void jornada_mostrar_registro(const tJornadaRegistro *jreg, FILE *fp)
{

    fprintf(fp, "Tiempo utilizado: %u min de %u\n", jreg->tiempo_usado, jreg->tiempo_jornada);
    fprintf(fp, "Buqes descargados: %u | +%u puntos\n", jreg->buques_des, jreg->buques_des*PUNTAJE_VALOR_DES);
    fprintf(fp, "Contenedores entregados: %u | +%u puntos\n", jreg->conts_entr, jreg->conts_entr*PUNTAJE_VALOR_ENT);
    fprintf(fp, "Camiones pendientes: %u | -%u puntos\n", jreg->cam_pend, jreg->cam_pend*PUNTAJE_VALOR_CAM);
    fprintf(fp, "Reorganizaciones: %u\n", jreg->reub_cont);
    fprintf(fp, "Puntacion Final: %u puntos\n", jornada_puntaje(jreg));

}
void jornada_registrar_evento(tJornadaRegistro *jreg, const char *evento)
{

    unsigned slen = strlen(evento);

    lista_poner_final(&jreg->historial, evento, sizeof(char)*(slen+1));

}
void jornada_mostrar_evento(const void *dato, FILE *fp)
{

    fprintf(fp, "-%s\n", (const char*)dato);

}
void jornada_mostrar_historial(tJornadaRegistro *jreg)
{

    printf("\n---HISTORIAL DE EVENTOS---\n");

    lista_mostrar(&jreg->historial, jornada_mostrar_evento, stdout);

}
void jornada_destruir_registro(tJornadaRegistro *jreg)
{
    lista_vaciar(&jreg->historial);
}
