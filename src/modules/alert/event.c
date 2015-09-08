#include "alert.h"

Eina_Bool
event_citizen_command(void *data,
                      int type EINA_UNUSED,
                      void *ev)
{
   Module_Alert *alert = data;
   Gotham_Citizen_Command *command = ev;

   if (strcmp(command->name, ".alert"))
     return EINA_TRUE;

   command->handled = EINA_TRUE;

   if (info->gotham->me->type == GOTHAM_CITIZEN_TYPE_ALFRED)
     {
        if ( (command->citizen->type == GOTHAM_CITIZEN_TYPE_BOTMAN) &&
             (command->parameters)                                  &&
             (!strcmp(command->parameters, "state"))
           )
          alfred_forward(alert, command);
        else if (!command->command[1])
          {
             alfred_command_list(alert, command);
          }
        else if ((command->command[1]) && (!strcmp(command->command[1], "add")))
          {
             alfred_command_add(alert, command);
          }
        else if ((command->command[1]) && (!strcmp(command->command[1], "del")))
          {
             alfred_command_del(alert, command);
          }
     }


   return EINA_TRUE;
}
