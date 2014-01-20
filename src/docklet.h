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
