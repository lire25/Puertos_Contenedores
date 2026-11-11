#include "muelle.h"

int muelle_recibir_buque(tPuerto *puerto)
{

    int res = 0;
    tBuque buque_muelle_aux;
    tPila pila_aux;
    pila_crear(&pila_aux);
    unsigned pos = 0;

    //voy sacando todos los muelles de la lista, apilando en una pila auxiliar,
    //hasta encontrar un muelle vacio, ahi copio el buque que estaba esperando
    while (!lista_es_vacia(&puerto->muelles))
    {
        int estado = lista_sacar_comienzo(&puerto->muelles,&buque_muelle_aux, sizeof(tBuque));
        if (buque_muelle_aux.id == BUQUE_ID_INVALIDA && !res) //si el muelle esta vacio
        {
            cola_sacar(&puerto->buques_fila, &buque_muelle_aux, sizeof(tBuque));
            res = 1;
        }
        pila_poner(&pila_aux, &buque_muelle_aux, sizeof(tBuque));
        pos++;
    }
    while (!pila_es_vacia(&pila_aux))
    {
        pila_sacar(&pila_aux, &buque_muelle_aux, sizeof(tBuque));
        lista_poner_comienzo(&puerto->muelles, &buque_muelle_aux, sizeof(tBuque));
    }

    pila_vaciar(&pila_aux);

    return res;
}


void muelle_mostrar(const void *dato, unsigned pos, FILE *fp)
{

    const tBuque *buque = (tBuque*)dato;
    tContenedor prox_cont;

    if (buque->id == BUQUE_ID_INVALIDA)
    {
        fprintf( fp, "M%u : vacio\n", pos+1 );
    }
    else
    {
        pila_ver_tope(&buque->contenedores, &prox_cont, sizeof(tContenedor));
        fprintf(
                fp,
                "M%u : B%u -> proximo contenedor C%u-%u\n",
                pos+1, buque->id, prox_cont.buque_id, prox_cont.cont_id
        );
    }
}

int muelle_limpiar(const void *dato, void *arg, unsigned pos)
{
    tBuque *buque = (tBuque*)dato;
    if (buque->id != BUQUE_ID_INVALIDA)
    {
        pila_vaciar(&buque->contenedores);
        buque->id = BUQUE_ID_INVALIDA;
    }
    return 1;
}
