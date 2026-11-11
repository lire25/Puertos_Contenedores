#include "estructuras.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void cola_crear(tCola *p)
{
    p->pri = NULL;
    p->ult = NULL;
}
void cola_vaciar(tCola *p)
{

    tNodo *aux;

    while (p->pri)
    {
        aux = p->pri;
        p->pri = aux->sig;
        free(aux->info);
        free(aux);
    }

    p->ult = NULL;

}
int cola_poner(tCola *p, const void *dato, unsigned tam)
{

    tNodo *nue = malloc(sizeof(tNodo));
    if (nue == NULL)
        return ESTD_SIN_MEMORIA;

    nue->info = malloc(tam);
    if (nue->info == NULL)
    {
        free(nue->info);
        free(nue);
    }

    memcpy(nue->info, dato, tam);
    nue->tam = tam;
    nue->sig = NULL;

    if (p->ult)
        p->ult->sig = nue;
    else
        p->pri = nue;
    p->ult = nue;

    return ESTD_OK;

}
int cola_sacar(tCola *p, void *dato, unsigned tam)
{

    tNodo *aux = p->pri;

    if (aux == NULL)
        return ESTD_ERROR;

    p->pri = aux->sig;

    memcpy(dato, aux->info, minimo(aux->tam, tam));

    free(aux->info);
    free(aux);

    if (p->pri == NULL) p->ult = NULL;

    return ESTD_OK;

}
int cola_es_vacia(const tCola *p)
{

    return p->pri == NULL;

}
int cola_es_llena(const tCola *p, unsigned tam)
{

    tNodo *aux1 = malloc(sizeof(tNodo));
    void *aux2 = malloc(tam);

    free(aux1);
    free(aux2);

    return aux1 == NULL || aux2 == NULL;

}
int cola_ver_primero(const tCola *p, void *dest, unsigned tam)
{

    if (p->pri == NULL)
        return ESTD_ERROR;

    memcpy(dest, p->pri->info, minimo(p->pri->tam, tam));

    return ESTD_OK;

}
