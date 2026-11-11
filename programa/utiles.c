#include "utiles.h"

#include <stdio.h>
#include <stdlib.h>

int diff_numeros_enteros(const void *a, const void *b)
{

    int n1 = *(int*)a;
    int n2 = *(int*)b;

    return n2 - n1;

}

void texto_remover_espacio(char *texto)
{

    int len = strlen(texto);
    if (len <= 0)
        return;

    for (int i = len; i > 0; i--)
    {
        if (texto[i-1] != ' ')
            continue;
        len--;
        for (int j = i-1; j < len; j++)
            texto[j] = texto[j+1];
    }

    texto[len] = '\0';

}
void texto_remover_caracter(char *texto, char car)
{
    int len = strlen(texto);
    if (len <= 0)
        return;

    for (int i = len; i > 0; i--)
    {
        if (texto[i-1] != car)
            continue;
        len--;
        for (int j = i-1; j < len; j++)
            texto[j] = texto[j+1];
    }

    texto[len] = '\0';
}
tLista texto_separado(char *texto, char del, unsigned short max_tam)
{

    tLista lista;
    lista_crear(&lista);
    int len = strlen(texto);
    char *buffer = malloc(sizeof(char)*max_tam);
    unsigned cursor = 0;

    for (int i = 0; i <= len; i++)
    {
        if (texto[i] != del && texto[i] != '\0') continue;
        if (i == cursor) continue;
        memcpy(buffer, texto+cursor, i-cursor+1);
        buffer[i-cursor] = '\0';
        lista_poner_final(
                          &lista,
                          buffer,
                          minimo(i-cursor+1,max_tam)
                          );
        cursor = i+1;
    }

    free(buffer);

    return lista;

}
int texto_separado_pila(tPila* pila, char *texto, char del, unsigned short max_tam)
{

    int len = strlen(texto);
    char *buffer = malloc(sizeof(char)*max_tam);
    if (!buffer) return 0;
    unsigned cursor = 0;
    int cant = 0;

    for (int i = 0; i <= len; i++)
    {
        if (texto[i] != del && texto[i] != '\0') continue;
        if (i == cursor) continue;
        memcpy(buffer, texto+cursor, i-cursor+1);
        buffer[i-cursor] = '\0';
        pila_poner(pila, buffer, minimo(i-cursor+1,max_tam));
        cursor = i+1;
        cant++;
    }

    free(buffer);

    return cant;

}
unsigned texto_encontrar(const char *texto, char car)
{

    int len = strlen(texto);
    for (unsigned i = 0; i < len; i++)
    {
        if (texto[i] == car)
            return i;
    }

    return len;

}

int texto_reemplazar(char *texto, char viejo, char nuevo)
{

    int prim = -1;

    int len = strlen(texto);
    for (unsigned i = 0; i < len; i++)
    {
        if (texto[i] == viejo)
        {
            texto[i] = nuevo;
            if (prim == -1) prim = i;
        }
    }

    return prim;

}

void limpiar_pantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
