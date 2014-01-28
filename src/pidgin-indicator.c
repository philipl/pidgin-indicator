/*
 * Ubuntu Indicator plugin for Pidgin
 *
 * Copyright (C) 2014 Philip Langdale <philipl@overt.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 */

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

#include "docklet.h"

static AppIndicator *sIndicator = NULL;
static PurpleStatusPrimitive sStatus = PURPLE_STATUS_OFFLINE;

static void
indicator_update_icon(PurpleStatusPrimitive status, gboolean connecting, gboolean pending) {
        const gchar *icon_name = NULL;

        sStatus = status;
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
  indicator_update_icon(sStatus, FALSE, FALSE);
}

static struct indicator_docklet_ui_ops ui_ops =
{
  indicator_update_icon,
  indicator_blank_icon,
};

static void
indicator_build_menu(PurplePlugin *plugin, AppIndicator *indicator) {
  GtkMenu *menu = GTK_MENU(docklet_menu());
  GList *items = gtk_container_get_children(GTK_CONTAINER(menu));

  app_indicator_set_menu(indicator, menu);
  app_indicator_set_secondary_activate_target(indicator, GTK_WIDGET(items->data));
}

static gboolean
indicator_load(PurplePlugin *plugin) {
  indicator_docklet_init(plugin, &ui_ops);

  AppIndicator *indicator =
    app_indicator_new_with_path("pidgin", PIDGIN_STOCK_TRAY_AVAILABLE,
                                APP_INDICATOR_CATEGORY_APPLICATION_STATUS,
                                DATADIR G_DIR_SEPARATOR_S "pixmaps" G_DIR_SEPARATOR_S "pidgin" G_DIR_SEPARATOR_S "tray");

  sIndicator = indicator;

  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
  indicator_update_icon(purple_savedstatus_get_type(purple_savedstatus_get_current()),
                        FALSE, FALSE);

  void *plugins_handle = purple_plugins_get_handle();
  purple_signal_connect(plugins_handle, "plugin-load", plugin->handle,
                        PURPLE_CALLBACK(indicator_build_menu), indicator);
  purple_signal_connect(plugins_handle, "plugin-unload", plugin->handle,
                        PURPLE_CALLBACK(indicator_build_menu), indicator);
  indicator_build_menu(NULL, indicator);

  plugin->extra = indicator;

  pidgin_blist_visibility_manager_add();

  return TRUE;
}

static gboolean
indicator_unload(PurplePlugin *plugin) {
  indicator_docklet_uninit(plugin);
  purple_signals_disconnect_by_handle(plugin->handle);

  if (plugin->extra) {
    g_object_unref(G_OBJECT(plugin->extra));
    plugin->extra = NULL;
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
  NULL, // Localised below
  VERSION,
  NULL, // Localised below
  NULL, // Localised below
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
  info.name = _("Ubuntu Indicator");
  info.summary = _("Indicator icon for Ubuntu Unity");
  info.description = _("Show a Unity Indicator icon in Ubuntu");
}

#pragma GCC visibility push(default)
PURPLE_INIT_PLUGIN(Indicator, init_plugin, info)
#pragma GCC visibility pop
