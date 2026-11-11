#include "menu.h"

#include <stdbool.h>
#include "configuracion.h"
#include "utiles.h"

#define ARCH_CONF "conf.txt"
#define ARCH_OPERADORES "operadores.dat"
#define ARCH_JORNADAS "jornadas.dat"
#define ARCH_PUERTO "puerto.txt"

#define MENU_LINEA_MAXTAM 256
#define MENU_ARG_MAXTAM 64

void menu_cmd_iniciar(void *_menu);
void menu_cmd_ver(void *_menu);
void menu_cmd_salir(void *_menu);

void menu_iniciar(tMenu *menu)
{

    menu->req_salir = false;

    menu->conf = conf_leer_de_archivo(ARCH_CONF);

    menu->estado = MENUESTADO_PIDENOMBRE;
    menu->cant_comandos = 0;

    menu_agregar_comando(menu, "SALIR", menu_cmd_salir);
    menu_agregar_comando(menu, "VER", menu_cmd_ver);
    menu_agregar_comando(menu, "INICIAR", menu_cmd_iniciar);

    arbol_crear(&menu->operarios_abb);
    op_cargar_arbolindice(ARCH_OPERADORES, &menu->operarios_abb); //crear abb de operadores segun el archivo

}
void menu_procesar(tMenu *menu)
{

    switch (menu->estado)
    {
    case MENUESTADO_CONSOLA:
        menu_procesar_consola(menu);
        break;
    case MENUESTADO_PIDENOMBRE:
        menu_procesar_nombre(menu);
        break;
    case MENUESTADO_SIMULACION:
        menu_procesar_simulacion(menu);
        break;
    }

}
void menu_terminar(tMenu *menu)
{

    arbol_vaciar(&menu->operarios_abb);

}

void menu_procesar_nombre(tMenu *menu)
{

    int nombre_valido = 0;

    do {

        printf("Ingrese nombre del operador:");
        fflush(stdin);
        fgets(menu->operario_actual, OPERADOR_NOMBRE_MAXTAM, stdin);
        fflush(stdin);
        texto_remover_caracter(menu->operario_actual, '\n');
        nombre_valido = *menu->operario_actual != '\0';
        if (nombre_valido)
        {
            op_crear_operador(ARCH_OPERADORES, menu->operario_actual, &menu->operarios_abb);
            menu->estado = MENUESTADO_CONSOLA;
        }
        else
            printf("\n--Nombre invalido--\n\n");

    } while (!nombre_valido);
}

void menu_procesar_simulacion(tMenu *menu)
{

    while (!puerto_hay_operacion(&menu->puerto))
    {

        if (puerto_jornada_finalizada(&menu->puerto))
        {
            menu_finalizar_simulacion(menu);
            return;
        }
        else
            puerto_avanzar_minuto(&menu->puerto, 1);

    }

    puerto_mostrar_estado(&menu->puerto);
    op_pedir_operacion(&menu->puerto, menu->conf);

}

void menu_procesar_consola(tMenu *menu)
{

    char linea[MENU_LINEA_MAXTAM] = "";
    char cmd_texto[MENU_CMD_MAXTAM] = "";
    unsigned sep_pos;
    int encuentra_func = false;

    printf("\nCOMANDOS:\n");
    printf("\tINICIAR - Comienza la simulacion de una jornada\n");
    printf("\tVER - Muestra los rankings de operadores\n");
    printf("\tSALIR - Terminar el programa\n");
    printf(">");

    fflush(stdin);

    fgets(linea, MENU_CMD_MAXTAM, stdin);

    texto_remover_caracter(linea, '\n');
    sep_pos = texto_encontrar(linea, ' ');

    strcpy_s(cmd_texto, MENU_CMD_MAXTAM, linea);

    for (int i = 0; i < menu->cant_comandos; i++)
    {
        if (!strcasecmp(menu->comandos[i].nombre, cmd_texto))
        {
            menu->comandos[i].func(menu);
            encuentra_func = true;
            break;
        }
    }

    if (!encuentra_func)
        printf("\nCOMANDO %s NO VALIDO\n\n", cmd_texto);

}

void menu_agregar_comando(tMenu *menu, const char *nombre, void (*func)(void*))
{

    unsigned pos = menu->cant_comandos++;
    tMenuComando cmd;
    strcpy(cmd.nombre, nombre);
    cmd.func = func;

    menu->comandos[pos] = cmd;

}

void menu_cmd_iniciar(void *_menu)
{

    tMenu *menu = (tMenu*)_menu;
    menu->estado = MENUESTADO_SIMULACION;

    limpiar_pantalla();

    puerto_crear(ARCH_PUERTO, &menu->puerto, menu->conf);
    puerto_ejecutar_minuto(&menu->puerto);

}
void menu_cmd_ver(void *_menu)
{

    tMenu *menu = (tMenu*)_menu;

    op_mostrar_ranking(ARCH_OPERADORES, &menu->operarios_abb);

}
void menu_cmd_salir(void *_menu)
{

    tMenu *menu = (tMenu*)_menu;

    menu->req_salir = true;

}

void menu_finalizar_simulacion(tMenu *menu)
{

    limpiar_pantalla();

    tJornadaRegistro jornada_res = puerto_estadisticas(&menu->puerto);

    op_registrar_jornada(ARCH_OPERADORES, ARCH_JORNADAS, menu->operario_actual, &menu->operarios_abb, jornada_res);

    puerto_finalizar_jornada(&menu->puerto);
    printf("\nENTER PARA CONTINUAR\n");
    fgetc(stdin);
    fflush(stdin);
    limpiar_pantalla();

    menu->estado = MENUESTADO_CONSOLA;

}

int menu_pide_salida(const tMenu *menu)
{
    return menu->req_salir;
}
