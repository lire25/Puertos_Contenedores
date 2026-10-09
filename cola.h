#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TODO_OK 1
#define ERROR_MEMORIA 2
#define VACIA_COLA 3
#define MINIMO(X, Y) ((X) < (Y)? (X): (Y))

typedef struct Snodo
{
    void *dato;
    unsigned tamDato;
    struct Snodo *sig;
}tNodo;

typedef struct
{
    tNodo *Pri;
    tNodo *Ult;
}tCola;

void crearCola(tCola *pc);
void vaciarCola(tCola *pc);
int colaLlena(const tCola *pc, unsigned cantbyte);
int colaVacia(const tCola *pc);
int ponerEnCola(tCola *pc, const void *dato, unsigned cantbyte);
int sacarDeCola(tCola *pc, void *dato, unsigned cantbyte);
int verPrimero(const tCola pc, void *dato, unsigned cantbyte);

#endif // COLA_H_INCLUDED
