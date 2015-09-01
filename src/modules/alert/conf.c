#include <cJSON.h>
#include "alert.h"

void
alert_conf_botman_load(Module_Alert *alert)
{
   cJSON *json,
         *p;

   json = gotham_modules_conf_load(MODULE_ALERT_CONF);
   if (!json) return;

   p = json->child;
   while (p)
     {
        cJSON *name,
              *interval,
              *command;
        Module_Alert_Command *mac;

        name = cJSON_GetObjectItem(p, "name");
        EINA_SAFETY_ON_NULL_GOTO(name, end_loop);

        interval = cJSON_GetObjectItem(p, "interval");
        EINA_SAFETY_ON_NULL_GOTO(interval, end_loop);

        command = cJSON_GetObjectItem(p, "command");
        EINA_SAFETY_ON_NULL_GOTO(command, end_loop);

        mac = alert_command_new(alert->gotham, name->valuestring, interval->valueint, command->valuestring);
        EINA_SAFETY_ON_NULL_GOTO(mac, end_loop);

        DBG("name[%s] interval[%f] command[%s]", mac->name, mac->interval, mac->command);

        alert->commands = eina_list_append(alert->commands, mac);

end_loop:
        p = p->next;
     }


   cJSON_Delete(json);
}
