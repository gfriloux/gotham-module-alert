#ifndef ALERT_H
#define ALERT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eina.h>
#include <Ecore.h>
#include <Gotham.h>

#define VARSET(_a, _b, ...) gotham_citizen_var_set(citizen, _a, _b, __VA_ARGS__)
#define VARGET(_a) gotham_citizen_var_get(citizen, _a)
#define MODULE_ALERT_CONF SYSCONF_DIR"/gotham/modules.conf.d/alert.conf"

int _module_alert_log_dom;

typedef struct _Module_Alert
{
   Gotham *gotham;

   Eina_List *commands;
} Module_Alert;

typedef struct _Module_Alert_Command
{
   Gotham *gotham;

   const char *name,
              *command;
   double interval;

   Ecore_Timer *timer;
   Ecore_Exe *exe;

   Eina_Strbuf *buf;

   struct
   {
      Ecore_Event_Handler *data,
                          *del,
                          *error;
   } ev;

} Module_Alert_Command;

#define CRI(...) EINA_LOG_DOM_CRIT(_module_alert_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_module_alert_log_dom, __VA_ARGS__)
#define NFO(...) EINA_LOG_DOM_INFO(_module_alert_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_module_alert_log_dom, __VA_ARGS__)

void alert_conf_botman_load(Module_Alert *alert);
Module_Alert_Command * alert_command_new(Gotham *gotham, const char *name, double interval, const char *command);
void module_json_answer(const char *cmd, const char *params, Eina_Bool status, Eina_Strbuf *content, Gotham *gotham, Gotham_Citizen *citizen, Eina_Bool send_to_alfred);
#endif
