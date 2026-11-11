#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include <stdio.h>

#define minimo(a,b) ((a) < (b)) ? (a) : (b)

enum {
    ESTD_ERROR,
    ESTD_OK,
    ESTD_SIN_MEMORIA,
    ESTD_DUP
};

typedef struct sNodo {
    unsigned tam;
    void *info;
    struct sNodo* sig;
} tNodo;

typedef int (*fListaMostrar)(const void*, FILE*);
typedef int (*fListaMostrarPos)(const void*, unsigned, FILE*);
typedef int (*fListaComparar)(const void*, const void*);
typedef int (*fListaAcumular)(void**, unsigned*, void*, unsigned);
typedef int (*fListaRecorrer)(void*, void *, unsigned);

//--------------------PILA--------------------

typedef tNodo* tPila;
void pila_crear(tPila*);
void pila_vaciar(tPila*);
int pila_poner(tPila*, const void *dato, unsigned tam);
int pila_sacar(tPila*, void *dest, unsigned tam);
int pila_ver_tope(const tPila*, void *dest, unsigned tam);
int pila_es_vacia(const tPila*);
int pila_es_llena(const tPila*, unsigned tam);

//--------------------COLA--------------------

typedef struct {
    tNodo* pri;
    tNodo* ult;
} tCola;

void cola_crear(tCola *p);
void cola_vaciar(tCola *p);
int cola_poner(tCola *p, const void *dato, unsigned tam);
int cola_sacar(tCola *p, void *dato, unsigned tam);
int cola_es_vacia(const tCola *p);
int cola_es_llena(const tCola *p, unsigned tam);
int cola_ver_primero(const tCola *p, void *dest, unsigned tam);

//--------------------LISTA--------------------

typedef tNodo* tLista;

void lista_crear(tLista*);
int lista_vaciar(tLista*);
int lista_es_llena(const tLista*, unsigned cant_bytes);
int lista_es_vacia(const tLista*);
int lista_poner_final(tLista* lista, const void *dato, unsigned tam);
int lista_ver_final(const tLista* lista, void *dato, unsigned tam);
int lista_sacar_final(tLista* lista, void *dato, unsigned tam);
int lista_poner_comienzo(tLista* lista, const void *dato, unsigned tam);
int lista_ver_comienzo(const tLista* lista, void *dato, unsigned tam);
int lista_sacar_comienzo(tLista* lista, void *dato, unsigned tam);
int lista_mostrar(const tLista* lista, fListaMostrar mostrar_func, FILE *fp);
int lista_mostrar_pos(const tLista*, fListaMostrarPos, FILE*);
int lista_mostrar_revez(const tLista* lista, fListaMostrar mostrar_func, FILE *fp);
int lista_mostrar_vaciar(tLista* lista, fListaMostrar mostrar_func, FILE *fp);
int lista_mostrar_revez_vaciar(tLista* lista, fListaMostrar mostrar_func, FILE *fp);
int lista_poner_ordenado(tLista* lista, void *dato, unsigned tam, fListaComparar compfunc, fListaAcumular acumfunc);
void lista_ordenar(tLista* lista, fListaComparar compfunc);
int lista_recorrer(tLista *p, fListaRecorrer recorrer, void *arg);
int lista_recorrer_filtrado(tLista*, fListaComparar, fListaRecorrer, void*);
void* lista_ver_pos(tLista *p, unsigned pos);

#endif // ESTRUCTURAS_H_INCLUDED
