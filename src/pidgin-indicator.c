#define PURPLE_PLUGINS

#include <config.h>

#include <glib/gi18n-lib.h>

#include <connection.h>
#include <core.h>
#include <debug.h>
#include <eventloop.h>
#include <gtkblist.h>
#include <gtkdialogs.h>
#include <gtkdocklet.h>
#include <gtkplugin.h>
#include <gtksavedstatuses.h>
#include <gtkstatusbox.h>
#include <gtkutils.h>
#include <pidgin.h>
#include <pidginstock.h>
#include <plugin.h>
#include <savedstatuses.h>
#include <version.h>

#include <libappindicator/app-indicator.h>

static AppIndicator *sIndicator = NULL;

GtkWidget *docklet_menu(void);
void indicator_docklet_init(PurplePlugin *plugin);
void indicator_docklet_uninit(PurplePlugin *plugin);

static void
indicator_update_icon(PurpleStatusPrimitive status, gboolean connecting, gboolean pending) {
        const gchar *icon_name = NULL;

        switch (status) {
                case PURPLE_STATUS_OFFLINE:
                        icon_name = PIDGIN_STOCK_TRAY_OFFLINE;
                        break;
                case PURPLE_STATUS_AWAY:
                        icon_name = PIDGIN_STOCK_TRAY_AWAY;
                        break;
                case PURPLE_STATUS_UNAVAILABLE:
                        icon_name = PIDGIN_STOCK_TRAY_BUSY;
                        break;
                case PURPLE_STATUS_EXTENDED_AWAY:
                        icon_name = PIDGIN_STOCK_TRAY_XA;
                        break;
                case PURPLE_STATUS_INVISIBLE:
                        icon_name = PIDGIN_STOCK_TRAY_INVISIBLE;
                        break;
                default:
                        icon_name = PIDGIN_STOCK_TRAY_AVAILABLE;
                        break;
        }

        if (pending)
                icon_name = PIDGIN_STOCK_TRAY_PENDING;
        if (connecting)
                icon_name = PIDGIN_STOCK_TRAY_CONNECT;

        if (icon_name) {
          app_indicator_set_icon(sIndicator, icon_name);
        }
}

static void indicator_blank_icon(void) {
  app_indicator_set_icon(sIndicator, PIDGIN_STOCK_TRAY_AVAILABLE);
}

static struct docklet_ui_ops ui_ops =
{
  NULL,
  NULL,
  indicator_update_icon,
  indicator_blank_icon,
  NULL,
  NULL
};

static gboolean
indicator_load(PurplePlugin *plugin) {
  indicator_docklet_init(plugin);
  pidgin_docklet_set_ui_ops(&ui_ops);

  AppIndicator *indicator =
    app_indicator_new_with_path("pidgin", PIDGIN_STOCK_TRAY_AVAILABLE,
                                APP_INDICATOR_CATEGORY_APPLICATION_STATUS,
                                DATADIR G_DIR_SEPARATOR_S "pixmaps" G_DIR_SEPARATOR_S "pidgin" G_DIR_SEPARATOR_S "tray");

  sIndicator = indicator;

  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
  app_indicator_set_icon(indicator, PIDGIN_STOCK_TRAY_AVAILABLE);

  GtkMenu *menu = GTK_MENU(docklet_menu());
  GList *items = gtk_container_get_children(GTK_CONTAINER(menu));

  app_indicator_set_menu(indicator, menu);
  app_indicator_set_secondary_activate_target(indicator, GTK_WIDGET(items->data));

  plugin->extra = indicator;

  pidgin_blist_visibility_manager_add();

  return TRUE;
}

static gboolean
indicator_unload(PurplePlugin *plugin) {
  indicator_docklet_uninit(plugin);

  if (plugin->extra) {
    g_object_unref(G_OBJECT(plugin->extra));
  }

  pidgin_blist_visibility_manager_remove();

  return TRUE;
}

static PurplePluginInfo info = {
  PURPLE_PLUGIN_MAGIC,
  PURPLE_MAJOR_VERSION,
  PURPLE_MINOR_VERSION,
  PURPLE_PLUGIN_STANDARD,
  PIDGIN_PLUGIN_TYPE,
  0,
  NULL,
  PURPLE_PRIORITY_DEFAULT,
  "gtk-philipl-indicator",
  "Ubuntu Indicator",
  VERSION,
  "Indicator icon for Ubuntu Unity",
  "Show a Unity Indicator icon in Ubuntu",
  "Philip Langdale <philipl@overt.org>",
  "https://github.com/philipl/pidgin-indicator",
  indicator_load,
  indicator_unload,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

static void
init_plugin(PurplePlugin *plugin) {
}

#pragma GCC visibility push(default)
PURPLE_INIT_PLUGIN(Indicator, init_plugin, info)
#pragma GCC visibility pop
