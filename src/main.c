/* Phosphorus main
 *
 * Copyright (C) 2015 Jente Hidskes <hjdskes@gmail.com>
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * Modeled after eog's main.c
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <glib/gi18n.h>

#include "ph-application.h"

static gboolean restore_wallpaper;

static gboolean
print_version_and_exit (UNUSED const gchar *name,
			UNUSED const gchar *value,
			UNUSED gpointer     user_data,
			UNUSED GError     **error)
{
	g_print ("%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);
	exit (EXIT_SUCCESS);
	return TRUE;
}

static const GOptionEntry options[] = {
	{ "restore", 'r', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, &restore_wallpaper,
		N_("Restore the wallpaper"), NULL },
	{ "version", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, print_version_and_exit,
		N_("Show the application's version"), NULL },
	{ NULL }
};

static gboolean
parse_options (int argc, char **argv)
{
	GOptionContext *context;
	GError *error = NULL;
	gboolean succes;

	context = g_option_context_new (_("- browse and set wallpapers"));
	g_option_context_add_main_entries (context, options, GETTEXT_PACKAGE);
	/* This initialises GTK during parsing. */
	g_option_context_add_group (context, gtk_get_option_group (FALSE));

	succes = g_option_context_parse (context, &argc, &argv, &error);
	g_option_context_free (context);

	if (!succes) {
		gchar *help;

		help = g_strdup_printf (_("Run '%s --help' to see a full "
					"list of available command line options"), argv[0]);
		g_printerr ("%s\n%s\n", error->message, help);

		g_clear_error (&error);
		g_free (help);

		return FALSE;
	}

	return TRUE;
}

int
main (int argc, char **argv)
{
	PhApplication *application;
	int status;

	bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	if (!parse_options (argc, argv)) {
		return EXIT_FAILURE;
	}

	application = ph_application_new (restore_wallpaper);
	status = g_application_run (G_APPLICATION (application), argc, argv);
	g_object_unref (application);

	return status;
}

