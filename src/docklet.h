#ifndef _DOCKLET_H_
#define _DOCKLET_H_

#include <config.h>
#include <plugin.h>

struct indicator_docklet_ui_ops
{
  void (*update_icon)(PurpleStatusPrimitive, gboolean, gboolean);
  void (*blank_icon)(void);
};

GtkWidget *docklet_menu(void);
void indicator_docklet_init(PurplePlugin *plugin, struct indicator_docklet_ui_ops *ops);
void indicator_docklet_uninit(PurplePlugin *plugin);

#endif // _DOCKLET_H_
