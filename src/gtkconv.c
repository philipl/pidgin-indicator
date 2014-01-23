/**
 * @file gtkconv.c GTK+ Conversation API
 * @ingroup pidgin
 */

/* pidgin
 *
 * Pidgin is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 *
 */
#define _PIDGIN_GTKCONV_C_

#include <config.h>

#include <glib/gi18n-lib.h>

#include "pidgin.h"

#include "gtkconv.h"
#include "gtkutils.h"
#include "pidginstock.h"

/**************************************************************************
 * A bunch of buddy icon functions
 **************************************************************************/

static const char *
pidgin_conv_get_icon_stock(PurpleConversation *conv)
{
	PurpleAccount *account = NULL;
	const char *stock = NULL;

	g_return_val_if_fail(conv != NULL, NULL);

	account = purple_conversation_get_account(conv);
	g_return_val_if_fail(account != NULL, NULL);

	/* Use the buddy icon, if possible */
	if (purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_IM) {
		const char *name = NULL;
		PurpleBuddy *b;
		name = purple_conversation_get_name(conv);
		b = purple_find_buddy(account, name);
		if (b != NULL) {
			PurplePresence *p = purple_buddy_get_presence(b);
			PurpleStatus *active = purple_presence_get_active_status(p);
			PurpleStatusType *type = purple_status_get_type(active);
			PurpleStatusPrimitive prim = purple_status_type_get_primitive(type);
			stock = pidgin_stock_id_from_status_primitive(prim);
		} else {
			stock = PIDGIN_STOCK_STATUS_PERSON;
		}
	} else {
		stock = PIDGIN_STOCK_STATUS_CHAT;
	}

	return stock;
}

/**************************************************************************
 * End of the bunch of buddy icon functions
 **************************************************************************/
void
pidgin_conv_present_conversation(PurpleConversation *conv)
{
	PidginConversation *gtkconv;
	GdkModifierType state;

	pidgin_conv_attach_to_conversation(conv);
	gtkconv = PIDGIN_CONVERSATION(conv);

	pidgin_conv_switch_active_conversation(conv);
	/* Switch the tab only if the user initiated the event by pressing
	 * a button or hitting a key. */
	if (gtk_get_current_event_state(&state))
		pidgin_conv_window_switch_gtkconv(gtkconv->win, gtkconv);
	gtk_window_present(GTK_WINDOW(gtkconv->win->window));
}

static void
unseen_conv_menu_cb(GtkMenuItem *item, PurpleConversation *conv)
{
	g_return_if_fail(conv != NULL);
	pidgin_conv_present_conversation(conv);
}

static void
unseen_all_conv_menu_cb(GtkMenuItem *item, GList *list)
{
	g_return_if_fail(list != NULL);
	/* Do not free the list from here. It will be freed from the
	 * 'destroy' callback on the menuitem. */
	while (list) {
		pidgin_conv_present_conversation(list->data);
		list = list->next;
	}
}

guint
pidgin_conversations_fill_menu(GtkWidget *menu, GList *convs)
{
	GList *l;
	guint ret=0;

	g_return_val_if_fail(menu != NULL, 0);
	g_return_val_if_fail(convs != NULL, 0);

	for (l = convs; l != NULL ; l = l->next) {
		PurpleConversation *conv = (PurpleConversation*)l->data;
		PidginConversation *gtkconv = PIDGIN_CONVERSATION(conv);

		GtkWidget *icon = gtk_image_new_from_stock(pidgin_conv_get_icon_stock(conv),
				gtk_icon_size_from_name(PIDGIN_ICON_SIZE_TANGO_MICROSCOPIC));
		GtkWidget *item;
		gchar *text = g_strdup_printf("%s (%d)",
				gtk_label_get_text(GTK_LABEL(gtkconv->tab_label)),
				gtkconv->unseen_count);

		item = gtk_image_menu_item_new_with_label(text);
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), icon);
		g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(unseen_conv_menu_cb), conv);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
		g_free(text);
		ret++;
	}

	if (convs->next) {
		/* There are more than one conversation. Add an option to show all conversations. */
		GtkWidget *item;
		GList *list = g_list_copy(convs);

		pidgin_separator(menu);

		item = gtk_menu_item_new_with_label(_("Show All"));
		g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(unseen_all_conv_menu_cb), list);
		g_signal_connect_swapped(G_OBJECT(item), "destroy", G_CALLBACK(g_list_free), list);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	}

	return ret;
}

