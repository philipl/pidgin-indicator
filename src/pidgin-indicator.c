#define PURPLE_PLUGINS

#include <config.h>

#include <glib/gi18n-lib.h>

#include <connection.h>
#include <core.h>
#include <debug.h>
#include <eventloop.h>
#include <gtkblist.h>
#include <gtkdialogs.h>
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

GtkWidget *docklet_menu(void);

static gboolean
indicator_load(PurplePlugin *plugin) {
  AppIndicator *indicator =
    app_indicator_new_with_path("pidgin", PIDGIN_STOCK_TRAY_AVAILABLE,
                                APP_INDICATOR_CATEGORY_APPLICATION_STATUS,
                                DATADIR G_DIR_SEPARATOR_S "pixmaps" G_DIR_SEPARATOR_S "pidgin" G_DIR_SEPARATOR_S "tray");

  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
  app_indicator_set_icon(indicator, PIDGIN_STOCK_TRAY_AVAILABLE);
  app_indicator_set_menu(indicator, GTK_MENU(docklet_menu()));

  plugin->extra = indicator;

  pidgin_blist_visibility_manager_add();

  return TRUE;
}

static gboolean
indicator_unload(PurplePlugin *plugin) {
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

PURPLE_INIT_PLUGIN(Indicator, init_plugin, info)
