#include <cJSON.h>
#include "alert.h"

void
conf_load_botman(Module_Alert *alert)
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

void
_conf_load_alfred_watchers(Module_Alert *alert,
                           cJSON *watchers)
{
   int size,
       i;

   size = cJSON_GetArraySize(watchers);

   alert->watchers = eina_array_new(2);

   for (i = 0; i < size; i++)
     {
        cJSON *watcher = cJSON_GetArrayItem(watchers, i);
        DBG("Adding %s", watcher->valuestring);
        eina_array_push(alert->watchers, eina_stringshare_add(watcher->valuestring));
     }
}

void
conf_load_alfred(Module_Alert *alert)
{
   cJSON *json,
         *watchers;

   json = gotham_modules_conf_load(MODULE_ALERT_CONF);
   EINA_SAFETY_ON_NULL_RETURN(json);

   watchers = cJSON_GetObjectItem(json, "watchers");
   _conf_load_alfred_watchers(alert, watchers);

   cJSON_Delete(json);
}

void
conf_save(Module_Alert *alert)
{
   cJSON *json,
         *watchers;
   unsigned int i;
   Eina_Array_Iterator iterator;
   const char *watcher;

   if (alert->gotham->me->type != GOTHAM_CITIZEN_TYPE_ALFRED) return;

   json = cJSON_CreateObject();
   EINA_SAFETY_ON_NULL_RETURN(json);

   watchers = cJSON_CreateArray();
   EINA_SAFETY_ON_NULL_GOTO(watchers, free_json);

   cJSON_AddItemToObject(json, "watchers", watchers);

   EINA_ARRAY_ITER_NEXT(alert->watchers, i, watcher, iterator)
     cJSON_AddItemReferenceToArray(watchers, cJSON_CreateString(watcher));
   gotham_modules_conf_save(MODULE_ALERT_CONF, json);

free_json:
   cJSON_Delete(json);
}
