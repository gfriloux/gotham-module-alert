#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <Eina.h>
#include <Ecore.h>

#include "alert.h"

static const char *name = "Alert";

void *
module_register(Gotham *gotham)
{
   Module_Alert *alert;

   if ((gotham->me->type != GOTHAM_CITIZEN_TYPE_ALFRED) &&
       (gotham->me->type != GOTHAM_CITIZEN_TYPE_BOTMAN))
     return NULL;

   alert = calloc(1, sizeof(Module_Alert));
   EINA_SAFETY_ON_NULL_RETURN_VAL(alert, NULL);

   DBG("alert[%p]", alert);

   alert->gotham = gotham;

   if (gotham->me->type == GOTHAM_CITIZEN_TYPE_BOTMAN) botman_register(alert);
   else alfred_register(alert);

   return alert;
}

void
module_unregister(void *data)
{
   Module_Alert *alert = data;

   DBG("alert[%p]", alert);

   free(alert);
}


const char *
module_init(void)
{
   eina_init();
   _module_alert_log_dom = eina_log_domain_register("module_alert",
                                                    EINA_COLOR_RED);
   return name;
}

void
module_shutdown(void)
{
   eina_log_domain_unregister(_module_alert_log_dom);
   eina_shutdown();
}

