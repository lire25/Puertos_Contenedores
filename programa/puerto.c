#include "puerto.h"
#include "../estructuras/estructuras.h"
#include "utiles.h"

#include "almacen.h"
#include "muelle.h"

void puerto_mostrar_camiones(tCola *cola, FILE *fp);

void puerto_recibir_buque(tPuerto *puerto, tConf conf);
void puerto_recibir_camion(tPuerto *puerto, tConf conf);

int puerto_almacenamiento_sacar_tope(void *dato, void *arg, unsigned pos);

void puerto_crear(const char *puerto_arch, tPuerto *puerto, tConf conf)
{

    tBuque buque_muelle_aux;
    tPila alm_aux; //almacenamiento

    cola_crear(&puerto->buques_fila);
    cola_crear(&puerto->camiones_fila);

    lista_crear(&puerto->zonas_alm);
    puerto->almac_cant = malloc(sizeof(unsigned) * conf.cant_zonas_alm);
    for (int i = 0; i < conf.cant_zonas_alm; i++)
    {
        pila_crear(&alm_aux);
        lista_poner_final(&puerto->zonas_alm, &alm_aux, sizeof(tPila));
        puerto->almac_cant[i] = 0;
    }

    lista_crear(&puerto->muelles);
    for (int i = 0; i < conf.cant_muelles; i++)
    {
        buque_muelle_aux.id = BUQUE_ID_INVALIDA; //muelle vacio
        lista_poner_final(&puerto->muelles, &buque_muelle_aux, sizeof(tBuque));
    }

    puerto->config = conf;

    puerto->minuto_actual = 0;
    jornada_generar(puerto_arch, &puerto->jornada_actual, conf);
    jornada_iniciar_registro(&puerto->jregistro, conf);

}

int puerto_jornada_finalizada(const tPuerto *puerto)
{
    return puerto->minuto_actual > puerto->config.duracion_jornada_minutos;
}
void puerto_finalizar_jornada(tPuerto *puerto)
{

    jornada_mostrar_historial(&puerto->jregistro);
    printf("\n---RESUMEN DE JORNADA---\n\n");
    jornada_mostrar_registro(&puerto->jregistro, stdout);

    puerto_destruir(puerto);

}
void puerto_destruir(tPuerto *puerto)
{

    tBuque buque;
    tCamion cam;

    //limpiar muelles
    lista_recorrer(&puerto->muelles, muelle_limpiar, NULL);
    lista_vaciar(&puerto->muelles);

    //limpiar fila de buques
    while (!cola_es_vacia(&puerto->buques_fila))
    {
        cola_sacar(&puerto->buques_fila, &buque, sizeof(tBuque));
        pila_vaciar(&buque.contenedores);
    }

    //limpiar fila de camiones
    while (!cola_es_vacia(&puerto->camiones_fila))
    {
        puerto->jregistro.cam_pend++;
        cola_sacar(&puerto->camiones_fila, &cam, sizeof(tCamion));
    }

    //limpiar almacenamiento
    lista_recorrer(&puerto->zonas_alm, almac_limpiar, NULL);
    lista_vaciar(&puerto->zonas_alm);
    free(puerto->almac_cant);

    jornada_destruir(&puerto->jornada_actual);
    jornada_destruir_registro(&puerto->jregistro);

}

void puerto_mostrar_estado(tPuerto* puerto)
{

    tCamion cam_aux;

    printf("\n-----MINUTO %u-----\n", puerto->minuto_actual);

    lista_mostrar_pos(&puerto->muelles, muelle_mostrar, stdout);

    lista_mostrar_pos(&puerto->zonas_alm, almac_mostrar_zona, stdout);
    putc('\n', stdout);

    if (!cola_es_vacia(&puerto->camiones_fila))
    {
        cola_ver_primero(&puerto->camiones_fila, &cam_aux, sizeof(tCamion));
        printf("Camion en espera: K%u -> solicita C%u-%u\n", cam_aux.id, cam_aux.pedido.buque_id,cam_aux.pedido.cont_id);
    }

}

void puerto_ejecutar_minuto(tPuerto *puerto)
{

    tBuque buque_sig;
    tCamion cam_sig;
    int band_terminado = 0; //bandera auxiliar, se usa para los while loop

    while (!lista_es_vacia(&puerto->jornada_actual.buques_info) && !band_terminado)
    {
        lista_ver_comienzo(&puerto->jornada_actual.buques_info, &buque_sig, sizeof(tBuque));
        band_terminado = buque_sig.minuto_arrivo != puerto->minuto_actual;
        if (buque_sig.minuto_arrivo == puerto->minuto_actual)
            puerto_recibir_buque(puerto, puerto->config);
    }

    band_terminado = 0;
    while (!lista_es_vacia(&puerto->jornada_actual.camiones_info) && !band_terminado)
    {
        lista_ver_comienzo(&puerto->jornada_actual.camiones_info, &cam_sig, sizeof(tCamion));
        band_terminado = cam_sig.minuto_arrivo != puerto->minuto_actual;
        if (cam_sig.minuto_arrivo == puerto->minuto_actual)
            puerto_recibir_camion(puerto, puerto->config);
    }


}
void puerto_avanzar_minuto(tPuerto* puerto, unsigned minutos)
{
    for (int i = 0; i < minutos; i++)
    {
        puerto->minuto_actual++;
        puerto_ejecutar_minuto(puerto);
    }
}

//---RECIBIR---

void puerto_recibir_buque(tPuerto *puerto, tConf conf)
{

    tBuque buque;
    int siguiente_muelle;

    lista_sacar_comienzo(&puerto->jornada_actual.buques_info, &buque, sizeof(tBuque));
    cola_poner(&puerto->buques_fila, &buque, sizeof(tBuque));

    //probar mandarlo a un muelle disponible
    muelle_recibir_buque(puerto);

    char reg[JORNADA_REGISTRO_MAXTAM];
    sprintf_s(
              reg, JORNADA_REGISTRO_MAXTAM,
              "T%u : Llega B%u",
              puerto->minuto_actual, buque.id
              );
    jornada_registrar_evento(&puerto->jregistro, reg);

}

void puerto_recibir_camion(tPuerto *puerto, tConf conf)
{

    tCamion cam;
    lista_sacar_comienzo( &puerto->jornada_actual.camiones_info, &cam, sizeof(tCamion) );
    cola_poner(&puerto->camiones_fila, &cam, sizeof(tCamion));

    puerto->jregistro.cam_pend++;

    char reg[JORNADA_REGISTRO_MAXTAM];
    sprintf_s(
              reg, JORNADA_REGISTRO_MAXTAM,
              "T%u : Llega K%u",
              puerto->minuto_actual, cam.id
              );
    jornada_registrar_evento(&puerto->jregistro, reg);

}

//-----MOSTRAR-----

void puerto_mostrar_camiones(tCola *cola, FILE *fp)
{

    tCamion cam;
    unsigned pos = 0;
    tCola cola_aux;
    cola_crear(&cola_aux);

    while (!cola_es_vacia(cola))
    {

        cola_sacar(cola, &cam, sizeof(tCamion));

        if (pos < 3)
        {
            fprintf(
                    fp,
                    "%u) K%u -> solicita C%u-%u",
                    pos+1, cam.id, cam.pedido.buque_id, cam.pedido.cont_id
                    );

            if (pos == 0) fprintf(fp, "\t<- frente");

            fputc('\n', fp);
        }

        cola_poner(&cola_aux, &cam, sizeof(tCamion));
        pos++;

    }

    while (!cola_es_vacia(&cola_aux))
    {

        cola_sacar(&cola_aux, &cam, sizeof(tCamion));
        cola_poner(cola, &cam, sizeof(tCamion));

    }

    cola_vaciar(&cola_aux);

}

//---OPERACIONES---

int puerto_hay_operacion(tPuerto* puerto)
{

    if (puerto->minuto_actual > puerto->config.duracion_jornada_minutos)
        return 0;

    tCamion sig_cam;
    tBuque muelle;

    int hay_espacio = almac_tiene_espacio(puerto); //si existe almacenamiento disp
    int cant_reu; //cantidad de reubicaciones necesarias

    unsigned duracion_maxima = puerto->config.duracion_jornada_minutos - puerto->minuto_actual; //la maxima cantidad de minutos que puede durar una operacion

    int hay_op = 0;

    if (!cola_es_vacia(&puerto->camiones_fila))
    {
        cola_ver_primero(&puerto->camiones_fila, &sig_cam, sizeof(tCamion));
        for (int i = 0; i < puerto->config.cant_zonas_alm; i++)
        {
            cant_reu = almac_tiene_cont(puerto, i, sig_cam.pedido);
            //hay operacion si: esta el contenedor pedido en una zona y el tiempo que tomaria ubicarlo y entregarlo es aceptable
            if (cant_reu)
                hay_op = puerto->config.tiempo_carga_camion + puerto->config.tiempo_reu_cont*(cant_reu-1) <= duracion_maxima;
        }
    }

    for (int i = 0; i < puerto->config.cant_muelles; i++)
    {
        muelle = *(tBuque*) lista_ver_pos(&puerto->muelles, i);
        if (muelle.id != BUQUE_ID_INVALIDA && hay_espacio)
            hay_op |= puerto->config.tiempo_desc_cont <= duracion_maxima;
    }

    return hay_op;

}

int puerto_entregar(tPuerto *puerto)
{

    tCamion cam;
    int zona;
    int pudo_entregar;

    if (cola_es_vacia(&puerto->camiones_fila))
    {
        printf("\n-NO HAY CAMIONES EN FILA\n");
        return 0;
    }

    if (puerto->minuto_actual + puerto->config.tiempo_carga_camion > puerto->config.duracion_jornada_minutos)
    {
        printf("\n\t-NO HAY SUFICIENTE TIEMPO\n");
        return 0;
    }

    cola_ver_primero(&puerto->camiones_fila, &cam, sizeof(tCamion));

    zona = lista_recorrer(&puerto->zonas_alm, almac_zona_entregar, puerto);
    pudo_entregar = zona < puerto->config.cant_zonas_alm;

    if (pudo_entregar)
    {

        puerto->jregistro.conts_entr++;
        puerto->jregistro.cam_pend--;
        puerto->jregistro.tiempo_usado = puerto->minuto_actual+puerto->config.tiempo_carga_camion;

        char reg[JORNADA_REGISTRO_MAXTAM];
        sprintf_s(
                  reg, JORNADA_REGISTRO_MAXTAM,
                  "T%u - T%u : ENT(Z%u -> C%u-%u -> K%u)",
                  puerto->minuto_actual,puerto->jregistro.tiempo_usado, zona+1, cam.pedido.buque_id,cam.pedido.cont_id, cam.id
                  );
        jornada_registrar_evento(&puerto->jregistro, reg);

    }
    else
        printf("\n-NADA QUE ENTREGAR\n");

    return pudo_entregar;

}
int puerto_reubicar(tPuerto *puerto, unsigned zona_orig, unsigned zona_dest)
{

    if (puerto->minuto_actual + puerto->config.tiempo_reu_cont > puerto->config.duracion_jornada_minutos)
    {
        printf("\n\t-NO HAY SUFICIENTE TIEMPO\n");
        return 0;
    }

    if (zona_orig >= puerto->config.cant_zonas_alm)
    {
        printf("\n\t*ERROR*: la zona origen (z%u) no existe\n", zona_orig);
        return 0;
    }

    if (zona_dest >= puerto->config.cant_zonas_alm)
    {
        printf("\n\t*ERROR*: la zona destino (z%u) no existe\n", zona_dest);
        return 0;
    }

    tPila *pila_orig, *pila_dest;
    tContenedor cont;
    pila_orig = lista_ver_pos(&puerto->zonas_alm, zona_orig);
    pila_dest = lista_ver_pos(&puerto->zonas_alm, zona_dest);

    if (pila_es_vacia(pila_orig))
    {
        printf("\n\t*ERROR*: la zona origen esta vacia\n");
        return 0;
    }

    puerto->almac_cant[zona_orig]--;
    puerto->almac_cant[zona_dest]++;

    pila_sacar(pila_orig, &cont, sizeof(tContenedor));
    pila_poner(pila_dest, &cont, sizeof(tContenedor));

    puerto->jregistro.reub_cont++;
    puerto->jregistro.tiempo_usado = puerto->minuto_actual+puerto->config.tiempo_reu_cont;

    char reg[JORNADA_REGISTRO_MAXTAM];
    sprintf_s(
              reg, JORNADA_REGISTRO_MAXTAM,
              "T%u - T%u : REU(Z%u -> C%u-%u -> Z%u)",
              puerto->minuto_actual,puerto->jregistro.tiempo_usado, zona_orig+1, cont.buque_id,cont.cont_id, zona_dest+1
              );
    jornada_registrar_evento(&puerto->jregistro, reg);

    return 1;

}
int puerto_descargar(tPuerto* puerto, unsigned muelle_orig, unsigned zona_dest)
{

    if (puerto->minuto_actual + puerto->config.tiempo_desc_cont > puerto->config.duracion_jornada_minutos)
    {
        printf("\n\t-NO HAY SUFICIENTE TIEMPO\n");
        return 0;
    }

    if (zona_dest >= puerto->config.cant_zonas_alm)
    {
        printf("\n\t*ERROR*: la zona a descargar no existe\n");
        return 0;
    }
    if (muelle_orig >= puerto->config.cant_muelles)
    {
        printf("\n\t*ERROR*: el muelle a descargar no existe\n");
        return 0;
    }

    if (puerto->almac_cant[zona_dest] >= puerto->config.cap_pila)
    {
        printf("\n-Z%u ESTA LLENA\n", zona_dest+1);
        return 0;
    }

    tBuque *muelle_buque = lista_ver_pos(&puerto->muelles, muelle_orig);
    tPila *pila_dest = lista_ver_pos(&puerto->zonas_alm, zona_dest);
    tContenedor cont;

    if (muelle_buque->id == BUQUE_ID_INVALIDA)
    {
        printf("\n\t*ERROR*: el muelle a descargar esta vacio\n");
        return 0;
    }

    pila_sacar(&muelle_buque->contenedores, &cont, sizeof(tContenedor));
    pila_poner(pila_dest, &cont, sizeof(tContenedor));

    //se vacio el buque
    if (pila_es_vacia(&muelle_buque->contenedores))
    {
        muelle_buque->id = BUQUE_ID_INVALIDA;
        //que entre el siguiente
        if (!cola_es_vacia(&puerto->buques_fila))
            muelle_recibir_buque(puerto);
    }

    puerto->almac_cant[zona_dest] += 1;

    puerto->jregistro.buques_des++;
    puerto->jregistro.tiempo_usado = puerto->minuto_actual+puerto->config.tiempo_desc_cont;

    char reg[JORNADA_REGISTRO_MAXTAM];
    sprintf_s(
              reg, JORNADA_REGISTRO_MAXTAM,
              "T%u - T%u : DES(M%u -> C%u-%u -> Z%u)",
              puerto->minuto_actual,puerto->jregistro.tiempo_usado, muelle_orig+1, cont.buque_id,cont.cont_id, zona_dest+1
              );
    jornada_registrar_evento(&puerto->jregistro, reg);

    return 1;

}
void puerto_ver(tPuerto *puerto)
{

    printf("\nCola camiones: ");

    if (cola_es_vacia(&puerto->camiones_fila))
        printf("vacia\n");
    else
    {
        putc('\n', stdout);
        puerto_mostrar_camiones(&puerto->camiones_fila, stdout);
    }

}

tJornadaRegistro puerto_estadisticas(tPuerto *puerto)
{
    return puerto->jregistro;
}
