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

void op_pedir_operacion(tPuerto *puerto, tConf conf);
int op_tipo_de_operacion(const char* nombre_operacion);

#endif // OPERDADOR_H_INCLUDED
