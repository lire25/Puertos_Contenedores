#include "almacen.h"

#include "jornada.h"

int almac_tiene_cont(tPuerto* puerto, unsigned zona_id, tContenedor pedido)
{

    tPila *pila = lista_ver_pos(&puerto->zonas_alm, zona_id);
    tContenedor cont;

    unsigned cant_reu = 0; //cantidad de reubicaciones
    unsigned max_reu = (puerto->config.cant_zonas_alm-1)* puerto->config.cap_pila; //maximo de reubicaciones que se puede hacer

    tPila pila_aux;

    int res = 0;

    for (int i = 0; i < puerto->config.cant_zonas_alm; i++)
    {
        if (i != zona_id)
            max_reu -= puerto->almac_cant[i];
    }

    pila_crear(&pila_aux);

    while (!pila_es_vacia(pila) && cant_reu < max_reu && !res)
    {

        pila_sacar(pila, &cont, sizeof(tContenedor));

        pila_poner(&pila_aux, &cont, sizeof(tContenedor));
        cant_reu++;

        res = (cont.buque_id == pedido.buque_id && cont.cont_id == pedido.cont_id);

    }

    while (!pila_es_vacia(&pila_aux))
    {
        pila_sacar(&pila_aux, &cont, sizeof(tContenedor));
        pila_poner(pila, &cont, sizeof(tContenedor));
    }

    pila_vaciar(&pila_aux);

    return res ? 1+cant_reu : 0;

}

int almac_tiene_espacio(const tPuerto* puerto)
{

    for (int i = 0; i < puerto->config.cant_zonas_alm; i++)
    {
        if (puerto->almac_cant[i] < puerto->config.cap_pila)
            return 1;
    }
    return 0;

}
int almac_limpiar(void *dato, void *arg, unsigned pos)
{
    tPila *pila = (tPila*)dato;
    pila_vaciar(pila);
    return 1;
}

void almac_mostrar_zona(const void* p, unsigned pos, FILE *fp)
{

    fprintf(fp, "Z%u: [", pos+1);

    jornada_mostrar_cont_pila((tPila*)p, fp);

    fprintf(fp, "] ");

}
int almac_zona_entregar(void *dato, void *arg, unsigned pos)
{

    tPila *pila = (tPila*)dato;
    tPuerto *puerto = (tPuerto*)arg;
    tCamion sig_camion;
    tContenedor aux;

    if (cola_es_vacia(&puerto->camiones_fila)) return 0;
    if (pila_es_vacia(pila)) return 1;

    cola_ver_primero(&puerto->camiones_fila, &sig_camion, sizeof(tCamion));
    pila_ver_tope(pila, &aux, sizeof(tContenedor));

    if (
        sig_camion.pedido.buque_id == aux.buque_id &&
        sig_camion.pedido.cont_id == aux.cont_id
    )
    {
        pila_sacar(pila, &aux, sizeof(tContenedor));
        cola_sacar(&puerto->camiones_fila, &sig_camion, sizeof(tCamion));
        puerto->almac_cant[pos]--;
        printf("\n-SE ENTREGA C%u-%u al camion K%u\n", aux.buque_id,aux.cont_id, sig_camion.id);
        return 0;
    }

    return 1;

}
