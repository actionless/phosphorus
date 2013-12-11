/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * callbacks.c
 * Copyright (C) 2013 Jente Hidskes <jthidskes@outlook.com>
 *
 * Phosphorus is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Phosphorus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "phosphorus.h"
#include "background.h"
#include "ui.h"

void on_apply_button_clicked (GtkButton *button, gpointer user_data) {
	GList *sel_items;
	GtkTreeIter iter;
	GtkTreePath *path;
	GtkTreeModel *model;

	cfg.config_changed = 1;
	sel_items = gtk_icon_view_get_selected_items (GTK_ICON_VIEW (icon_view));
	model = gtk_icon_view_get_model (GTK_ICON_VIEW (icon_view));
	path = g_list_nth_data (sel_items, 0); /* We are only interested in the first item, since we set selection mode to GTK_SELECTION_SINGLE */
	g_list_free_full (sel_items, (GDestroyNotify) gtk_tree_path_free);
	if (!gtk_tree_model_get_iter (model, &iter, path)) {
		g_fprintf (stderr, "Error: can not determine activated wallpaper (from apply button).\n");
		return;
	}
	gtk_tree_model_get (model, &iter, 1, &cfg.set_wp, -1);

	if (set_background () < 0)
		g_fprintf (stderr, "Error: applying background failed.\n");
}

void on_radio_button_clicked (GtkToggleButton *button, gpointer user_data) {
	if (gtk_toggle_button_get_active (button) == TRUE) {
		unsigned int mode = GPOINTER_TO_INT(user_data);

		if (cfg.sort != mode) {
			cfg.sort = mode;
			cfg.config_changed = 1;
		}
	}
}

void on_prefs_dlg_rmv_btn_clicked (GtkButton *button, gpointer user_data) {
	GtkTreeSelection *selection = (GtkTreeSelection *)user_data;
	GtkTreeModel *model;
	GtkTreeIter iter;

	if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
		const char *filename = NULL;

		gtk_tree_model_get (model, &iter, 0, &filename, -1);
		for (GSList *l = cfg.dirs; l; l = l->next) {
			if (g_strcmp0 (filename, (char *)l->data) == 0) {
				cfg.dirs = g_slist_remove (cfg.dirs, l->data);
				cfg.config_changed = 1;
			}
		}
		gtk_tree_selection_select_iter (selection, &iter);
		gtk_list_store_remove (prefs_liststore, &iter);
	} else
		g_fprintf (stderr, "Error: can't get selected directory to delete from tree view.\n");
}

void on_prefs_dlg_add_btn_clicked (GtkButton *button, gpointer user_data) {
	GtkWidget *dialog;
	GtkTreeIter iter;

	dialog = gtk_file_chooser_dialog_new (_("Pick a directory"), (GtkWindow *) user_data,
			GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, _("Cancel"), GTK_RESPONSE_CANCEL,
			_("Open"), GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename;

		cfg.config_changed = 1;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		cfg.dirs = g_slist_append (cfg.dirs, g_strdup (filename));
		gtk_list_store_insert_with_values (prefs_liststore, &iter, -1, 0, filename, -1);
		g_free (filename);
	}

	gtk_widget_destroy (dialog);
}

void on_prefs_button_clicked (GtkButton *button, gpointer user_data) {
	GtkWidget *dialog;
	int result;

	dialog = prefs_dialog_open ((GtkWindow *) user_data, cfg.sort);
	result = gtk_dialog_run (GTK_DIALOG (dialog));
	switch (result) {
		case GTK_RESPONSE_OK:
			//FIXME: reload config and images?
		case GTK_RESPONSE_CANCEL:
		default:
			break;
	}

	gtk_widget_destroy (dialog);
}

void on_combo_changed (GtkComboBox *combo, gpointer user_data) {
	cfg.config_changed = 1;
	cfg.wp_mode = gtk_combo_box_get_active (combo);
}

void on_item_activated (GtkIconView *view, GtkTreePath *path, gpointer user_data) {
	GtkTreeModel *model;
	GtkTreeIter iter;

	cfg.config_changed = 1;
	model = gtk_icon_view_get_model (GTK_ICON_VIEW (icon_view));
	if (!gtk_tree_model_get_iter (model, &iter, path)) {
		g_fprintf (stderr, "Error: can not determine activated wallpaper (from item activated).\n");
		return;
	}
	gtk_tree_model_get (model, &iter, 1, &cfg.set_wp, -1);

	if (set_background () < 0)
		g_fprintf (stderr, "Error: applying background failed.\n");
}

void on_color_button_clicked (GtkColorButton *button, gpointer user_data) {
	cfg.config_changed = 1;
	gtk_color_chooser_get_rgba (GTK_COLOR_CHOOSER (button), &cfg.bg_color);
}
