#ifndef OPERDADOR_H_INCLUDED
#define OPERDADOR_H_INCLUDED

#include "puerto.h"

#define OPERADOR_LINEA_MAXTAM 128
#define OPERADOR_ARGUMENTO_MAXTAM 32

enum {
    OPERACION_ESPERAR,
    OPERACION_VER,
    OPERACION_ENTREGAR,
    OPERACION_REUBICAR,
    OPERACION_DESCARGAR,
    OPERACION_CANTIDAD
};

#define OPERADOR_NOMBRE_MAXTAM 32

typedef struct {
    char nombre[OPERADOR_NOMBRE_MAXTAM];
    unsigned puntaje;
    unsigned cant_jornadas;
} tOperador;

typedef struct {
    char nombre[OPERADOR_NOMBRE_MAXTAM];
    unsigned arch_pos;
} tOpIndice;

void op_pedir_operacion(tPuerto *puerto, tConf conf);
int op_tipo_de_operacion(const char* nombre_operacion);

int op_crear_operador(const char *op_arch, const char *nombre, tArbol *arbol);
void op_cargar_arbolindice(const char *op_arch, tArbol *arbol);
int op_operador_pos(const char *nombre, tArbol *arbol_op);
int op_cargar_operador(const char *op_arch, tOperador *operador, const char *nombre, tArbol *arbol_op);
void op_mostrar_operador(const char *op_arch, const char *jor_arch, const char *nombre, tArbol *arbol_op);
void op_registrar_jornada(const char *op_arch, const char *jor_arch, const char *nombre, tArbol *arbol_op, tJornadaRegistro jreg);

#endif // OPERDADOR_H_INCLUDED
