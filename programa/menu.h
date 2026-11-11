#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "operador.h" //OPERADOR_NOMBRE_MAXTAM
#include "puerto.h"

#define MENU_CMD_MAXTAM 8
#define MENU_CMD_MAXCANT 16

enum {
    MENUESTADO_CONSOLA,
    MENUESTADO_PIDENOMBRE,
    MENUESTADO_SIMULACION
};

typedef struct {
    char nombre[MENU_CMD_MAXTAM];
    void (*func)(void*); //la funcion que se activa al llamar el comando (el parametro es un puntero al menu tMenu* y los argumentos en texto plano)
} tMenuComando;

typedef struct {

    char req_salir;
    unsigned estado;
    tMenuComando comandos[MENU_CMD_MAXCANT];
    unsigned cant_comandos;
    char operario_actual[OPERADOR_NOMBRE_MAXTAM];
    tArbol operarios_abb;
    tPuerto puerto;
    tConf conf;

} tMenu;

void menu_iniciar(tMenu *menu);
void menu_procesar(tMenu *menu);
void menu_terminar(tMenu *menu);
void menu_agregar_comando(tMenu *menu, const char *nombre, void (*func)(void*));

#endif // MENU_H_INCLUDED
