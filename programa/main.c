#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../estructuras/estructuras.h"
#include "configuracion.h"
#include "puerto.h"
#include "operador.h"

/*

    TODO:
    -historial de operaciones
    -registro de operadores

*/

/*
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
*/

void finalizar_jornada(tPuerto*, tConf);

int main()
{

    tConf conf = conf_leer_de_archivo("conf.txt");

    tPuerto puerto;
    puerto_crear(&puerto, conf);
    puerto_iniciar_jornada(&puerto, conf);


    lista_mostrar(&puerto.jornada_actual.buques_info, jornada_mostrar_buques, stdout);
    lista_mostrar(&puerto.jornada_actual.camiones_info, jornada_mostrar_camiones, stdout);

    printf("\n---\n\n");

    int salir = 0;

    puerto_ejecutar_minuto(&puerto);

    while (!salir)
    {

        //avanzar hasta siguiente

        while (!puerto_hay_operacion(&puerto))
        {

            if (puerto_jornada_finalizada(&puerto))
                finalizar_jornada(&puerto, conf);
            else
                puerto_avanzar_minuto(&puerto, 1);

        }

        puerto_mostrar_estado(&puerto);
        op_pedir_operacion(&puerto, conf);

    }

    return 0;

}

void finalizar_jornada(tPuerto *puerto, tConf conf)
{

    puerto_reiniciar_jornada(puerto, conf);
    puerto_ejecutar_minuto(puerto);

    printf("\n~~~~~~~~~~~~~~~NUEVA JORNADA~~~~~~~~~~~~~~~\n");
    lista_mostrar(&puerto->jornada_actual.buques_info, jornada_mostrar_buques, stdout);
    lista_mostrar(&puerto->jornada_actual.camiones_info, jornada_mostrar_camiones, stdout);

    printf("---\n\n");

}
