#include "menu.h"

int main()
{

    tMenu menu;
    menu_iniciar(&menu);

    while (!menu_pide_salida(&menu))
    {

        menu_procesar(&menu);

    }

    menu_terminar(&menu);

    return 0;

}

