#include "operador.h"

#include <string.h>

#include "../estructuras/estructuras.h"
#include "utiles.h"

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

    if (lista_es_vacia(args)) return 0;

    char arg_texto[ OPERADOR_ARGUMENTO_MAXTAM ];
    lista_sacar_comienzo(args, arg_texto, sizeof(char)*OPERADOR_ARGUMENTO_MAXTAM);
    arg_texto[OPERADOR_ARGUMENTO_MAXTAM-1] = '\0';

    return sscanf(arg_texto, formato, dest) == 1;

}

//----BASE DE DATOS DE OPERADORES----

int op_comparar(const void *a, const void *b)
{

    tOpIndice *i1 = (tOpIndice*)a;
    tOpIndice *i2 = (tOpIndice*)b;

    return strcmp(i1->nombre, i2->nombre);

}
int op_crear_operador(const char *op_arch, const char *nombre, tArbol *arbol)
{

    tOpIndice ind;
    tOperador operador;
    unsigned pos;
    if (op_operador_pos(nombre, arbol) >= 0) //si ya existe no hace falta crearlo
        return 0;
    FILE *fp = fopen(op_arch, "ab");
    if (!fp)
        return 0;
    strcpy_s(operador.nombre, OPERADOR_NOMBRE_MAXTAM, nombre);
    operador.cant_jornadas = 0;
    operador.puntaje = 0;

    fwrite(&operador, sizeof(tOperador), 1, fp);

    fseek(fp, 0, SEEK_END);
    pos = ftell(fp) / sizeof(tOperador) - 1;
    ind.arch_pos = pos;
    strcpy_s(ind.nombre, OPERADOR_NOMBRE_MAXTAM, nombre);

    fclose(fp);

    arbol_poner(arbol, &ind, sizeof(tOpIndice), op_comparar);

    return 1;

}
void op_cargar_arbolindice(const char *op_arch, tArbol *arbol)
{

    FILE *fp = fopen(op_arch, "rb");
    tOpIndice ind;
    tOperador operador;
    unsigned pos = 0;

    if (!fp)
        return;

    while (fread(&operador, sizeof(tOperador), 1, fp))
    {
        ind.arch_pos = pos;
        strcpy(ind.nombre, operador.nombre);
        arbol_poner(arbol, &ind, sizeof(tOpIndice), op_comparar);
        pos++;
    }

    fclose(fp);

}
int op_operador_pos(const char *nombre, tArbol *arbol_op)
{

    tOpIndice ind;

    strcpy(ind.nombre, nombre);

    if (!arbol_buscar(arbol_op, &ind, sizeof(tOpIndice), op_comparar))
        return -1;

    return ind.arch_pos;

}
int op_cargar_operador(const char *op_arch, tOperador *operador, const char *nombre, tArbol *arbol_op)
{

    FILE *fp;
    int pos = op_operador_pos(nombre, arbol_op);

    if (pos < 0)
        return 0;

    fp = fopen(op_arch, "rb");

    if (!fp)
        return 0;

    fseek(fp, sizeof(tOperador)*pos, SEEK_SET);
    fread(operador, sizeof(tOperador), 1, fp);

    fclose(fp);

    return 1;

}
void op_mostrar_operador(const char *op_arch, const char *jor_arch, const char *nombre, tArbol *arbol_op)
{


    FILE *fp;
    tOperador operador;
    tJornadaRegistro jreg;
    int pos = op_operador_pos(nombre, arbol_op);
    int i = 0;
    if (!op_cargar_operador(op_arch, &operador, nombre, arbol_op))
        return 0;

    printf("[%u] : %s\n", pos, operador.nombre);
    printf("Jornadas realizadas: %u\n", operador.cant_jornadas);
    printf("Puntaje acumulado: %u\n", operador.puntaje);

    fp = fopen(jor_arch, "rb");

    while (fread(&jreg, sizeof(tJornadaRegistro), 1, fp))
    {
        if (jreg.operador == pos)
        {
            printf("\n---JORNADA %i---\n", i+1);
            jornada_mostrar_registro(&jreg, stdout);
            i++;
        }
    }

    fclose(fp);

    return 1;

}
void op_registrar_jornada(const char *op_arch, const char *jor_arch, const char *nombre, tArbol *arbol_op, tJornadaRegistro jreg)
{

    FILE *fp;
    tOperador operador;
    int pos = op_operador_pos(nombre, arbol_op);
    if (!op_cargar_operador(op_arch, &operador, nombre, arbol_op))
        return 0;

    //guardo la jornada
    fp = fopen(jor_arch, "ab");

    if (!fp)
        return;

    jreg.operador = pos;

    fwrite(&jreg, sizeof(tJornadaRegistro), 1, fp);

    fclose(fp);

    //actualizo el operador
    operador.cant_jornadas++;
    operador.puntaje += jornada_puntaje(&jreg);

    fp = fopen(op_arch, "r+b");
    if (!fp)
        return;

    fseek(fp, pos*sizeof(tOperador), SEEK_SET);
    fwrite(&operador, sizeof(tOperador), 1, fp);

    fclose(fp);

}

//----RANKING----
int op_comparar_pos(const void *a, const void *b) //se usa para ordenar por puntaje (arch_pos reemplaza a puntaje)
{

    tOpIndice *i1 = (tOpIndice*)a;
    tOpIndice *i2 = (tOpIndice*)b;

    return (int)(i2->arch_pos) - (int)(i1->arch_pos);

}
void op_cargar_puntaje(void *dato, void *arg, unsigned pos) //carga los puntajes de una lista de indices
{

    tOpIndice *ind = (tOpIndice*)dato;
    FILE *fp = (FILE*)arg;
    tOperador operador;

    //leo el operador
    fseek(fp, ind->arch_pos * sizeof(tOperador), SEEK_SET);
    fread(&operador, sizeof(tOperador), 1, fp);

    //reemplazo arch_pos en el indice por el puntaje
    ind->arch_pos = operador.puntaje;
    //esto va a convertir la lista de [nombre,pos archivo] a [nombre, puntaje]. que se necesita para el ranking

}
void op_mostrar_puntaje(void *dato, unsigned pos, FILE *fp)
{
    tOpIndice *i1 = (tOpIndice*)dato;
    fprintf(fp, "%u - %s : %u\n", pos, i1->nombre, i1->arch_pos);
}
void op_mostrar_ranking(const char *op_arch, tArbol *arbol_op)
{

    FILE *fp;
    tLista lista;
    lista_crear(&lista);

    fp = fopen(op_arch, "r");

    if (!fp || arbol_es_vacio(&arbol_op))
    {
        printf("\n-NO EXISTE REGISTRO DE OPERADORES\n\n");
        return;
    }

    //convierto el abb de operadores en una lista (duplicando sus valores)
    arbol_a_lista(arbol_op, &lista, ARBOLORDEN_INORDER);

    //les cargo el puntaje, reemplazando arch_pos
    lista_recorrer(&lista, op_cargar_puntaje, fp);

    fclose(fp);

    //ordeno por puntaje
    lista_ordenar(&lista, op_comparar_pos);

    lista_mostrar_pos(&lista, op_mostrar_puntaje, stdout);

    lista_vaciar(&lista);

}
