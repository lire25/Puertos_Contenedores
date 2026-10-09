#include "cola.h"
void crearCola(tCola* pc)
{
    pc->Pri = NULL;
}

void vaciarCola(tCola* pc)
{
    tNodo *elim;
    while ((*pc)->Pri)
    {
        elim = pc->Pri;
        pc->Pri = elim->sig;
        free(elim->dato);
        free(elim);
    }
    pc->Ult = NULL;
}

int colaLlena(const tCola* pc, unsigned cantbyte)
{
    return TODO_OK;
}

int colaVacia(const tCola* pc)
{
    return NULL == pc->Pri;
}

int ponerEnCola(tCola* pc, const void* dato, unsigned cantbyte)
{
    tNodo *nue = (tNodo)malloc(sizeof(tNodo))
    if(!*nue)
    {
        fprintf(stderr, "Error - no se pudo reservar memoria para el nodo \n");
        return ERROR_MEMORIA;
    }
    nue->dato = malloc(cantbyte);
    if(nue->dato)
    {
        fprintf(stderr, "Error - no se pudo resercar memoria para la informacion \n");
        free(nue);
        return TODO_OK;
    }

    memcpy(nue->dato, dato, cantbyte);
    nue->tamDato = cantbyte;
    nue->sig = NULL;
    if(NULL == pc->Pri)
        {pc->Pri = nue;}
    else
        {pc->Ult->sig = nue;}
    pc->Ult = nue;

    return TODO_OK;
}

int sacarDeCola(tCola* pc, void* dato, unsigned cantbyte)
{
    tNodo *elim;
    if(NULL == pc->Pri)
        return VACIA_COLA;
    elim =  pc->Pri;
    pc->Pri = elim->sig;
    memcpy(dato, elim->dato, MINIMO(cantbyte, elim->tamDato));
    free(elim->dato);
    free(elim);
    if(NULL == pc->Pri)
        pc->Ult = NULL;
    return  TODO_OK;
}

int verPrimero(const tCola pc, void* dato, unsigned cantbyte)
{
    if (NULL == pc->Pri)
        return VACIA_COLA;
    memcpy(dato, pc->Pri->dato, MINIMO(cantbyte, pc->Pri->tamDato))
    return TODO_OK;
}
