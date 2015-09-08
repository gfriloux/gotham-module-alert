#include "alert.h"

void
botman_register(Module_Alert *alert)
{
   conf_load_botman(alert);
}

void
botman_unregister(Module_Alert *alert)
{
   Module_Alert_Command *mac;

   EINA_LIST_FREE(alert->commands, mac)
     {
        free((char *)mac->name);
        free((char *)mac->command);
        ecore_timer_del(mac->timer);
        free(mac);
     }
}
