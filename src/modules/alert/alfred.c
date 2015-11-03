#include "alert.h"

void
alfred_command_list(Module_Alert *alert,
                    Gotham_Citizen_Command *command)
{
   Eina_Strbuf *buf;
   const char *p,
              *watcher;
   Eina_Array_Iterator iterator;
   unsigned int i;

   buf = eina_strbuf_new();
   eina_strbuf_append(buf, "\nList of watchers :\n");

   EINA_ARRAY_ITER_NEXT(alert->watchers, i, watcher, iterator)
     eina_strbuf_append_printf(buf, "\t%s\n", watcher);

   p = eina_strbuf_string_get(buf);
   gotham_command_send(command, p);
   eina_strbuf_free(buf);
}

void
alfred_command_add(Module_Alert *alert,
                   Gotham_Citizen_Command *command)
{
   const char *jid = NULL;
   char *watcher;
   Eina_Array_Iterator iterator;
   unsigned int i;
   Gotham_Citizen *citizen;

   if (!command->command[2]) return;

   jid = command->command[2];
   EINA_ARRAY_ITER_NEXT(alert->watchers, i, watcher, iterator)
     if (!strcmp(watcher, jid)) goto exists;

   citizen = eina_hash_find(alert->gotham->citizens, jid);
   EINA_SAFETY_ON_NULL_GOTO(citizen, not_found);

   eina_array_push(alert->watchers, eina_stringshare_add(jid));
   conf_save(alert);
   gotham_command_send(command, "A new watcher has arise");
   return;

not_found:
   gotham_command_send(command, "Unknown watcher specified");
   return;
exists:
   gotham_command_send(command, "This citizen is already a nightwatcher!");
}

Eina_Bool
_alfred_command_del_keep(void *data, void *gdata)
{
   const char *fallen = gdata,
              *found = data;

   if (strcmp(found, fallen)) return EINA_TRUE;

   eina_stringshare_del(found);
   return EINA_FALSE;
}

void
alfred_command_del(Module_Alert *alert,
                   Gotham_Citizen_Command *command)
{
   DBG("alert[%p] command[%p]", alert, command);

   if (!command->command[2]) return;

   eina_array_remove(alert->watchers, _alfred_command_del_keep,
                     (void *)command->command[2]);
   conf_save(alert);

   gotham_command_send(command, "Rest in pieces");
}


void
_alfred_forward_send(Module_Alert *alert,
                     const char *msg)
{
   const char *watcher;
   Eina_Array_Iterator iterator;
   unsigned int i;
   Gotham_Citizen *citizen;

   EINA_ARRAY_ITER_NEXT(alert->watchers, i, watcher, iterator)
     {
        citizen = eina_hash_find(alert->gotham->citizens, watcher);
        if (!citizen) continue;

        gotham_citizen_send(citizen, msg);
     }
}

void
alfred_forward(Module_Alert *alert,
               Gotham_Citizen_Command *command)
{
   Eina_Strbuf *buf;

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "Received an alert from %s ", command->citizen->jid);
   if (command->citizen->nickname) eina_strbuf_append_printf(buf, "(%s) ", command->citizen->nickname);
   eina_strbuf_append_printf(buf, ": %s", command->command[0]);

   _alfred_forward_send(alert, eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   return;
}


void
alfred_register(Module_Alert *alert)
{
   DBG("alert[%p]", alert);

   conf_load_alfred(alert);

   gotham_modules_command_add("alert", ".alert",
                              "[.alert] - "
                              "This command will list watchers that get "
                              "called when an alert signal is sent.");
   gotham_modules_command_add("alert", ".alert add",
                              "[.alert add jid] - "
                              "This command will raise a citizen to the "
                              "level of a watcher, by joining the nightwatchers.");
   gotham_modules_command_add("alert", ".alert del",
                              "[.alert del jid] - "
                              "This command will remove a fallen watcher.");
}

void
alfred_unregister(Module_Alert *alert)
{
   Eina_Array_Iterator iterator;
   unsigned int i;
   const char *watcher;

   EINA_ARRAY_ITER_NEXT(alert->watchers, i, watcher, iterator)
     eina_stringshare_del(watcher);
   eina_array_free(alert->watchers);

   gotham_modules_command_del("alert", ".alert");
   gotham_modules_command_del("alert", ".alert add");
   gotham_modules_command_del("alert", ".alert del");
}
