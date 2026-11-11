#include "operador.h"

#include <string.h>

#include "../estructuras/estructuras.h"
#include "utiles.h"

int op_mostrar_texto(void *dato, FILE *fp);
void op_pedir_operacion(tPuerto *puerto, tConf conf)
{

    printf("\nOPERADOR>");

    unsigned comando;
    unsigned arg_aux;

    unsigned zona_orig, zona_dest;
    unsigned muelle_orig;

    unsigned error = 0;

    char nombre_comando[4];

    char op_texto[OPERADOR_LINEA_MAXTAM];
    //scanf("%s", op_texto);
    fflush(stdin);
    fgets(op_texto, OPERADOR_LINEA_MAXTAM, stdin);

    texto_remover_caracter(op_texto, '\n');
    tLista cmd_args = texto_separado(op_texto, ' ', OPERADOR_ARGUMENTO_MAXTAM);

    if (lista_es_vacia(cmd_args))
    {
        printf("error en entrada de comando\n");
        return;
    }

    lista_sacar_comienzo(&cmd_args, nombre_comando, sizeof(char)*4);
    nombre_comando[3] = '\0';

    comando = op_tipo_de_operacion(nombre_comando);

    //NOTA:
    //el formato "%*[Mm]%u" o "%*[Zz]%u" lo uso
    //para aceptar argumentos que empiezen con mayuscula o minuscula

    switch (comando)
    {
    case OPERACION_ESPERAR:
        puerto_avanzar_minuto(puerto, 1);
        break;
    case OPERACION_VER:
        puerto_mostrar_estado(puerto);
        break;
    case OPERACION_ENTREGAR:
        if (puerto_entregar(puerto))
            puerto_avanzar_minuto(puerto, puerto->config.tiempo_carga_camion);
        break;
    case OPERACION_REUBICAR:
        error = !op_leer_argumento(&cmd_args, "%*[Zz]%u", &zona_orig);
        error |= !op_leer_argumento(&cmd_args, "%*[Zz]%u", &zona_dest);
        if (!error)
        {
            if (puerto_reubicar(puerto, zona_orig-1, zona_dest-1))
                puerto_avanzar_minuto(puerto, puerto->config.tiempo_reu_cont);
        }
        else
            printf("\n\t*ERROR*: ARGUMENTOS NO RECONOCIDOS (REU Z%u Z%u)\n");
        break;
    case OPERACION_DESCARGAR:
        error = !op_leer_argumento(&cmd_args, "%*[Mm]%u", &muelle_orig);
        error |= !op_leer_argumento(&cmd_args, "%*[Zz]%u", &zona_dest);
        if (!error)
        {
            if (puerto_descargar(puerto, muelle_orig-1, zona_dest-1))
                puerto_avanzar_minuto(puerto, puerto->config.tiempo_desc_cont);
        }
        else
            printf("\n\t*ERROR*: ARGUMENTOS NO RECONOCIDOS (DES M%%u Z%%u)\n");
        break;
    default:
        printf("\n\t*ERROR*: COMANDO NO RECONOCIDO (ESP VER ENT REU DES)\n");
        break;
    }

    return 1;

}

int op_tipo_de_operacion(const char* nombre_operacion)
{

    char nombres[OPERACION_CANTIDAD][4];
    strcpy(nombres[OPERACION_ESPERAR], "ESP");
    strcpy(nombres[OPERACION_VER], "VER");
    strcpy(nombres[OPERACION_ENTREGAR], "ENT");
    strcpy(nombres[OPERACION_REUBICAR], "REU");
    strcpy(nombres[OPERACION_DESCARGAR], "DES");

    for (int i = 0; i < OPERACION_CANTIDAD; i++)
    {
        if (!strcasecmp(nombre_operacion, nombres[i])) return i;
    }

    return OPERACION_CANTIDAD;

}

int op_leer_argumento(tLista *args, const char *formato, void *dest)
{

    char arg_texto[ OPERADOR_ARGUMENTO_MAXTAM ];
    lista_sacar_comienzo(args, &arg_texto[0], sizeof(char)*OPERADOR_ARGUMENTO_MAXTAM);
    arg_texto[OPERADOR_ARGUMENTO_MAXTAM-1] = '\0';

    return sscanf(arg_texto, formato, dest) == 1;

}
