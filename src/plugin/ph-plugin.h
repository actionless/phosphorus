/* PhPlugin
 *
 * Copyright (C) 2015-2016 Jente Hidskes
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define PH_TYPE_PLUGIN ph_plugin_get_type ()

G_DECLARE_INTERFACE (PhPlugin, ph_plugin, PH, PLUGIN, GObject)

struct _PhPluginInterface {
	GTypeInterface interface;

	/* Virtual public methods */
	void (*load) (PhPlugin *plugin);
	void (*unload) (PhPlugin *plugin);
	void (*set_background) (PhPlugin *plugin, const gchar *filepath);
};

void ph_plugin_load (PhPlugin *plugin);
void ph_plugin_unload (PhPlugin *plugin);
void ph_plugin_set_background (PhPlugin *plugin, const gchar *filepath);

G_END_DECLS

