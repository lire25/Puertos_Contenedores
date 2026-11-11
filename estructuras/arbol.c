#include "estructuras.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void arbol_crear(tArbol *arbol)
{
    *arbol = NULL;
}
int arbol_es_vacio(const tArbol *pa)
{
    return *pa == NULL;
}
int arbol_poner(tArbol *pa, void *dato, unsigned tam, fArbolComparar cmp)
{

    int diff;
    tNodoArbol *nue;

    if (*pa == NULL)
    {

        nue = malloc(sizeof(tNodoArbol));
        if (!nue)
            return ESTD_SIN_MEMORIA;
        nue->info = malloc(tam);
        if (!nue->info)
        {
            free(nue);
            return ESTD_SIN_MEMORIA;
        }
        memcpy(nue->info, dato, tam);
        nue->tam = tam;
        nue->izq = NULL;
        nue->der = NULL;
        *pa = nue;

        return 1;
    }


    diff = cmp((*pa)->info, dato);

    if (diff == 0)
        return ESTD_DUP;

    return arbol_poner( (diff < 0) ? &(*pa)->izq : &(*pa)->der, dato, tam, cmp );

}
int arbol_buscar(tArbol *pa, void *dato, unsigned tam, fArbolComparar cmp)
{

    if (!*pa)
        return ESTD_ERROR;

    int diff = cmp((*pa)->info, dato);

    if (diff == 0)
    {
        memcpy(dato, (*pa)->info, minimo((*pa)->tam, tam));
        return ESTD_OK;
    }

    if (diff < 0)
        return arbol_buscar(&(*pa)->izq, dato, tam, cmp);
    else
        return arbol_buscar(&(*pa)->der, dato, tam, cmp);

}
void arbol_mostrar(tArbol *pa, FILE *fp, eArbolOrden orden, fArbolMostrar mostrar)
{

    if (*pa == NULL)
        return;

    switch (orden)
    {
    case ARBOLORDEN_PREORDER:
        mostrar((*pa)->info, fp);
        arbol_mostrar(&(*pa)->izq, fp, orden, mostrar);
        arbol_mostrar(&(*pa)->der, fp, orden, mostrar);
        break;
    case ARBOLORDEN_INORDER:
        arbol_mostrar(&(*pa)->izq, fp, orden, mostrar);
        mostrar((*pa)->info, fp);
        arbol_mostrar(&(*pa)->der, fp, orden, mostrar);
        break;
    case ARBOLORDEN_POSTORDER:
        arbol_mostrar(&(*pa)->izq, fp, orden, mostrar);
        arbol_mostrar(&(*pa)->der, fp, orden, mostrar);
        mostrar((*pa)->info, fp);
        break;
    }

}
void arbol_vaciar(tArbol *pa)
{

    if (*pa == NULL)
        return;

    arbol_vaciar(&(*pa)->izq);
    arbol_vaciar(&(*pa)->der);

    free((*pa)->info);
    free(*pa);

    *pa = NULL;

}

int arbol_a_lista(tArbol *pa, tLista *pl, eArbolOrden orden)
{

    if (*pa == NULL)
        return ESTD_OK;

    tNodo *nue = malloc(sizeof(tNodo));
    if (!nue)
        return ESTD_SIN_MEMORIA;

    nue->info = malloc((*pa)->tam);
    if (!nue->info)
    {
        free(nue);
        return ESTD_SIN_MEMORIA;
    }
    memcpy(nue->info, (*pa)->info, (*pa)->tam);
    nue->sig = NULL;
    nue->tam = (*pa)->tam;

    switch (orden)
    {
    case ARBOLORDEN_PREORDER:
        while (*pl) pl = &(*pl)->sig;
        *pl = nue;
        arbol_a_lista(&(*pa)->izq, pl, orden);
        arbol_a_lista(&(*pa)->der, pl, orden);
        break;
    case ARBOLORDEN_INORDER:
        arbol_a_lista(&(*pa)->izq, pl, orden);
        while (*pl) pl = &(*pl)->sig;
        *pl = nue;
        arbol_a_lista(&(*pa)->der, pl, orden);
        break;
    case ARBOLORDEN_POSTORDER:
        arbol_a_lista(&(*pa)->izq, pl, orden);
        arbol_a_lista(&(*pa)->der, pl, orden);
        while (*pl) pl = &(*pl)->sig;
        *pl = nue;
        break;
    }

    return ESTD_OK;

}
