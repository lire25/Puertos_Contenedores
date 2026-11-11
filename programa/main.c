#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../estructuras/estructuras.h"
#include "configuracion.h"
#include "puerto.h"
#include "operador.h"

#define ARCH_CONF "conf.txt"
#define ARCH_OPERADORES "operadores.dat"
#define ARCH_JORNADAS "jornadas.dat"

/*

    TODO:
    -registro de operadores
    -menu
    -puerto.txt

*/

/*
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
*/

void finalizar_jornada(tPuerto *puerto, tConf conf, const char* op_nombre, tArbol *arbol_op);

int main()
{

    char op_nombre[OPERADOR_NOMBRE_MAXTAM];
    tArbol arbol_operadores;
    int salir = 0;
    tConf conf = conf_leer_de_archivo(ARCH_CONF);
    tPuerto puerto;

    puerto_crear(&puerto, conf);
    puerto_ejecutar_minuto(&puerto);

    strcpy(op_nombre, "MATEO");
    op_cargar_arbolindice(ARCH_OPERADORES, &arbol_operadores);
    op_crear_operador(ARCH_OPERADORES, op_nombre, &arbol_operadores);
    op_mostrar_operador(ARCH_OPERADORES,ARCH_JORNADAS, op_nombre, &arbol_operadores);

    while (!salir)
    {

        //avanzar hasta siguiente

        while (!puerto_hay_operacion(&puerto))
        {

            if (puerto_jornada_finalizada(&puerto))
                finalizar_jornada(&puerto, conf, op_nombre, &arbol_operadores);
            else
                puerto_avanzar_minuto(&puerto, 1);

        }

        puerto_mostrar_estado(&puerto);
        op_pedir_operacion(&puerto, conf);

    }

    return 0;

}

void finalizar_jornada(tPuerto *puerto, tConf conf, const char* op_nombre, tArbol *arbol_op)
{

    system("cls");

    op_registrar_jornada(ARCH_OPERADORES, ARCH_JORNADAS, op_nombre, arbol_op, puerto->jregistro);

    puerto_finalizar_jornada(puerto);
    printf("\nENTER PARA REINTENTAR\n");
    fgetc(stdin);
    system("cls");
    puerto_crear(puerto, conf);
    puerto_ejecutar_minuto(puerto);

    printf("\n~~~~~~~~~~~~~~~NUEVA JORNADA~~~~~~~~~~~~~~~\n");
    lista_mostrar(&puerto->jornada_actual.buques_info, jornada_mostrar_buques, stdout);
    lista_mostrar(&puerto->jornada_actual.camiones_info, jornada_mostrar_camiones, stdout);

    printf("---\n\n");

}
