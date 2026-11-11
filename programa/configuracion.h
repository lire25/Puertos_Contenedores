#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

typedef struct {

    unsigned duracion_jornada_minutos;
    unsigned cant_muelles;
    unsigned cant_zonas_alm;
    unsigned cap_pila;
    unsigned max_buques;
    unsigned max_cont_por_buque;
    unsigned max_camiones;
    unsigned tiempo_desc_cont;
    unsigned tiempo_reu_cont;
    unsigned tiempo_carga_camion;

} tConf;

unsigned* conf_obtener_parametro(tConf *conf, const char* texto);
tConf conf_leer_de_archivo(const char *arch_nombre);
void conf_guardar_configuracion(tConf* conf, const char *arch_nombre);

#endif // CONFIGURACION_H_INCLUDED
