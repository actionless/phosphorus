/* Utilities
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

#include <glib.h>

G_BEGIN_DECLS

gboolean ph_file_is_image (GSList *supported_formats, const gchar *file);
void ph_get_plugin_dirs (gchar **plugin_dir, gchar **plugin_data_dir);
void ph_get_user_dir (gchar **user_dir);

G_END_DECLS

