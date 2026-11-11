#ifndef UTILES_H_INCLUDED
#define UTILES_H_INCLUDED

#include "../estructuras/estructuras.h"

int diff_numeros_enteros(const void *a, const void *b);

void texto_remover_espacio(char *texto);
void texto_remover_caracter(char *texto, char car);
tLista texto_separado(char *texto, char del, unsigned short max_tam);
int texto_separado_pila(tPila* pila, char *texto, char del, unsigned short max_tam);
unsigned texto_encontrar(const char *texto, char car);
int texto_reemplazar(char *texto, char viejo, char nuevo);

#endif // UTILES_H_INCLUDED
