#include "configuracion.h"

#include <string.h>
#include "utiles.h"

#define CONF_PARAMETROS_NOMBRES {\
"JORNADA","MUELLES","ZONAS","PILAS","BUQUES","MAX_CONT_BUQUE", \
"CAMIONES", "TIEMPO_DES", "TIEMPO_REU", "TIEMPO_ENT" \
}
#define CONF_PARAMETROS_PUNTEROS(conf) { \
&conf->duracion_jornada_minutos, &conf->cant_muelles, &conf->cant_zonas_alm, \
&conf->cap_pila,&conf->max_buques, &conf->max_cont_por_buque, &conf->max_camiones, \
&conf->tiempo_desc_cont, &conf->tiempo_reu_cont, &conf->tiempo_carga_camion \
}

unsigned* conf_obtener_parametro(tConf *conf, const char* texto)
{

    char parametros[][64] = CONF_PARAMETROS_NOMBRES;
    unsigned* punteros[] = CONF_PARAMETROS_PUNTEROS(conf);
    unsigned cant_parametros = sizeof(parametros) / 64ull;

    for (int i = 0; i < cant_parametros; i++)
    {
        if (strcmp(parametros[i], texto) == 0)
            return punteros[i];
    }

    return NULL;

}
int conf_leer_linea(tConf *conf, char *linea)
{

    char parametros[][64] = CONF_PARAMETROS_NOMBRES;
    unsigned* punteros[] = CONF_PARAMETROS_PUNTEROS(conf);
    unsigned cant_parametros = sizeof(parametros) / 64ull;

    char param[64];
    unsigned valor;
    unsigned *val_ptr;
    unsigned del_pos;

    texto_remover_espacio(linea);

    del_pos = texto_reemplazar(linea, ':', '\0');
    if (del_pos == -1) return;

    strcpy_s(param, 64, linea);
    sscanf(linea+del_pos+1, "%u", &valor);

    val_ptr = conf_obtener_parametro(conf, param);
    *val_ptr = valor;

    return 1;

}
tConf conf_leer_de_archivo(const char *arch_nombre)
{

    tConf conf = {0};
    FILE *arch;
    char linea[512];
    char param[64];
    unsigned valor;
    unsigned del_pos;
    unsigned* val_ptr;

    conf.cant_muelles = 1;
    conf.cant_zonas_alm = 3;
    conf.cap_pila = 3;
    conf.duracion_jornada_minutos = 30;
    conf.max_buques = 2;
    conf.max_camiones = 5;
    conf.max_cont_por_buque = 3;
    conf.tiempo_carga_camion = 2;
    conf.tiempo_desc_cont = 3;
    conf.tiempo_reu_cont = 1;

    arch = fopen(arch_nombre, "r");

    if (!arch)
    {
        conf_guardar_configuracion(&conf, arch_nombre);
        return conf;
    }

    while (!feof(arch))
    {

        fgets(linea, 512, arch);

        conf_leer_linea(&conf, linea);

    }

    fclose(arch);

    return conf;

}
void conf_guardar_configuracion(tConf* conf, const char *arch_nombre)
{

    FILE *arch = fopen(arch_nombre, "w");

    if (!arch) return;

    char parametros[][64] = CONF_PARAMETROS_NOMBRES;
    unsigned* punteros[] = CONF_PARAMETROS_PUNTEROS(conf);
    unsigned cant_parametros = sizeof(parametros) / 64ull;

    for (int i = 0; i < cant_parametros; i++)
        fprintf(arch, "%s : %u\n", parametros[i], *(punteros[i]));

    fclose(arch);

}
