#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../estructuras/estructuras.h"
#include "configuracion.h"
#include "puerto.h"
#include "operador.h"

#include "menu.h"

/*

    TODO:
    -menu
    -puerto.txt

*/

int main()
{

    tMenu menu;
    menu_iniciar(&menu);

    while (!menu.req_salir)
    {

        menu_procesar(&menu);

    }

    menu_terminar(&menu);

    return 0;

}

