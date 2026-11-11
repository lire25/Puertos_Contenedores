#include "estructuras.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


void pila_crear(tPila* pila)
{
    *pila = NULL;
}
void pila_vaciar(tPila* p)
{

    tNodo *aux;

    while (*p)
    {
        aux = *p;
        *p = aux->sig;
        free(aux->info);
        free(aux);
    }

}
int pila_poner(tPila* pila, const void *dato, unsigned tam)
{

    tNodo *nue = malloc(sizeof(tNodo));

    if (nue == NULL)
        return ESTD_SIN_MEMORIA;

    nue->info = malloc(tam);
    if (nue->info == NULL)
    {
        free(nue);
        return ESTD_SIN_MEMORIA;
    }

    memcpy(nue->info, dato, tam);
    nue->tam = tam;
    nue->sig = *pila;

    *pila = nue;

    return ESTD_OK;

}
int pila_sacar(tPila* pila, void *dest, unsigned tam)
{

    tNodo *aux = *pila;
    if (aux == NULL) return ESTD_ERROR; //pila vacia

    *pila = aux->sig;

    memcpy(dest, aux->info, minimo(tam, aux->tam));
    free(aux->info);
    free(aux);
    return ESTD_OK;

}
int pila_ver_tope(const tPila* pila, void *dest, unsigned tam)
{

    if (*pila == NULL)
        return ESTD_ERROR;

    memcpy(dest, (*pila)->info, minimo(tam, (*pila)->tam));

    return ESTD_OK;

}
int pila_es_vacia(const tPila* pila)
{

    return *pila == NULL;

}
int pila_es_llena(const tPila*, unsigned tam)
{

    tNodo* aux1 = malloc(sizeof(tNodo));
    void* aux2 = malloc(tam);

    free(aux1);
    free(aux2);

    return aux1 == NULL || aux2 == NULL;

}
void pila_mostrar(tPila* p, fListaMostrar mostrar, FILE* fp)
{

    while (*p)
    {
        mostrar((*p)->info, fp);
        p = &(*p)->sig;
    }

}
