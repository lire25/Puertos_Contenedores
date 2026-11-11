#include "estructuras.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void lista_crear(tLista* lista)
{
    *lista = NULL;
}
int lista_vaciar(tLista* p)
{

    int cant = 0;
    tNodo* aux;
    while (*p)
    {
        aux = *p;
        p = &aux->sig;
        free(aux->info);
        free(aux);
        cant++;
    }

    return cant;

}
int lista_es_llena(const tLista* lista, unsigned cant_bytes)
{

    tNodo *nue = malloc(sizeof(tNodo));
    if (nue == NULL) return 0;

    nue->info = malloc(cant_bytes);
    if (!nue->info)
    {
        free(nue);
        return 0;
    }

    free(nue->info);
    free(nue);

    return 1;

}
int lista_es_vacia(const tLista* p)
{

    return *p == NULL;

}
int lista_poner_final(tLista* p, const void *dato, unsigned tam)
{

    tNodo *nue = malloc(sizeof(tNodo));
    if (nue == NULL) return ESTD_SIN_MEMORIA;
    nue->info = malloc(tam);
    if (nue->info == NULL)
    {
        free(nue);
        return ESTD_SIN_MEMORIA;
    }

    memcpy(nue->info, dato, tam);
    nue->tam = tam;
    nue->sig = NULL;

    while (*p)
        p = &(*p)->sig;

    *p = nue;

    return ESTD_OK;

}
int lista_ver_final(const tLista* p, void *dato, unsigned tam)
{

    if (*p == NULL)
        return ESTD_ERROR;

    while (*p)
        p = &(*p)->sig;

    memcpy(dato, (*p)->info, minimo((*p)->tam,tam));

    return ESTD_OK;

}
int lista_sacar_final(tLista* p, void *dato, unsigned tam)
{

    if (*p == NULL)
        return ESTD_ERROR;

    while (*p)
        p = &(*p)->sig;

    memcpy(dato, (*p)->info, minimo((*p)->tam,tam));

    free((*p)->info);
    free(*p);
    *p = NULL;

    return ESTD_OK;

}
int lista_poner_comienzo(tLista* p, const void *dato, unsigned tam)
{


    tNodo *nue = malloc(sizeof(tNodo));
    if (nue == NULL) return ESTD_SIN_MEMORIA;
    nue->info = malloc(tam);
    if (nue->info == NULL)
    {
        free(nue);
        return ESTD_SIN_MEMORIA;
    }

    memcpy(nue->info, dato, tam);
    nue->tam = tam;
    nue->sig = *p;
    *p = nue;

    return ESTD_OK;

}
int lista_ver_comienzo(const tLista* p, void *dato, unsigned tam)
{

    if (*p == NULL)
        return ESTD_ERROR;

    memcpy(dato, (*p)->info, minimo((*p)->tam, tam));

    return ESTD_OK;

}
int lista_sacar_comienzo(tLista* p, void *dato, unsigned tam)
{

    if (*p == NULL) return ESTD_ERROR;

    tNodo *aux = *p;
    *p = aux->sig;
    memcpy(dato, aux->info, minimo(aux->tam, tam));

    return ESTD_OK;

}
int lista_mostrar(const tLista* p, fListaMostrar mostrar_func, FILE *fp)
{

    if (mostrar_func==NULL)
        return ESTD_ERROR;

    int cant = 0;

    while (*p)
    {
        mostrar_func((*p)->info, fp);
        p = &(*p)->sig;
        cant++;
    }

    return cant;

}
int lista_mostrar_pos(const tLista *p, fListaMostrarPos mostrar_func, FILE *fp)
{

    if (!mostrar_func)
        return ESTD_ERROR;

    int pos = 0;

    while (*p)
    {
        mostrar_func((*p)->info, pos, fp);
        p = &(*p)->sig;
        pos++;
    }

    return pos;

}
int lista_mostrar_revez(const tLista* p, fListaMostrar mostrar_func, FILE *fp)
{
    if (*p)
    {
        int n = lista_mostrar_revez(&(*p)->sig, mostrar_func, fp);
        mostrar_func((*p)->info, fp);
        return n+1;
    }
    return 0;
}
int lista_mostrar_vaciar(tLista* p, fListaMostrar mostrar_func, FILE *fp)
{

    int cant = 0;
    tNodo* aux;
    while (*p)
    {
        aux = *p;
        p = &aux->sig;
        mostrar_func((*p)->info, fp);
        free(aux->info);
        free(aux);
        cant++;
    }

    return cant;

}
int lista_mostrar_revez_vaciar(tLista* p, fListaMostrar mostrar_func, FILE *fp)
{
    if (*p)
    {
        int n = lista_mostrar_revez(&(*p)->sig, mostrar_func, fp);
        mostrar_func((*p)->info, fp);
        free((*p)->info);
        free(*p);
        *p = NULL;
        return n+1;
    }
    return 0;
}
int lista_poner_ordenado(tLista* p, void *dato, unsigned tam, fListaComparar compfunc, fListaAcumular acumfunc)
{

    tNodo *nue;

    while (*p && compfunc((*p)->info,dato) < 0)
        p = &(*p)->sig;

    if (*p && compfunc((*p)->info,dato) == 0)
    {
        if (acumfunc)
            if (!acumfunc(&(*p)->info, &(*p)->tam, dato, tam))
                return ESTD_SIN_MEMORIA;
        return ESTD_DUP;
    }

    nue = malloc(sizeof(tNodo));
    if (nue == NULL) return ESTD_SIN_MEMORIA;
    nue->info = malloc(tam);
    if (nue->info == NULL)
    {
        free(nue);
        return ESTD_SIN_MEMORIA;
    }

    memcpy(nue->info, dato, tam);
    nue->tam = tam;
    nue->sig = *p;
    *p = nue;
    return ESTD_OK;

}
void lista_ordenar(tLista* p, fListaComparar compfunc)
{

    tLista *pri = p;
    tLista *q;
    tNodo *aux;

    if (*p == NULL)
        return;

    while ((*p)->sig)
    {
        if ( compfunc((*p)->info, (*p)->sig->info) > 0 )
        {
            q = pri;
            aux = (*p)->sig;
            (*p)->sig = aux->sig;
            while(compfunc((*q)->info, aux->info) <= 0)
                q = &(*q)->sig;
            aux->sig = *q;
            *q = aux;
        }
        else
            p = &(*p)->sig;
    }

}

int lista_recorrer(tLista *p, fListaRecorrer recorrer, void *arg)
{

    int i = 0;

    while (*p)
    {

        if (!recorrer((*p)->info, arg, i))
            return i;
        p = &(*p)->sig;
        i++;
    }

    return i;

}
int lista_recorrer_filtrado(tLista* p, fListaComparar fcomp, fListaRecorrer frec, void *dato)
{

    if (frec == NULL || fcomp == NULL) return -1;

    int i = 0;

    while (*p)
    {

        if (fcomp((*p)->info, dato) != 0) continue;

        if (!frec((*p)->info, (*p)->tam, i))
            return i;
        p = &(*p)->sig;
        i++;
    }

    return i;

}
void* lista_ver_pos(tLista *p, unsigned pos)
{

    int i = 0;
    while (*p)
    {
        if (i == pos)
        {
            return (*p)->info;
        }
        p = &(*p)->sig;
        i++;
    }

    return NULL;

}
