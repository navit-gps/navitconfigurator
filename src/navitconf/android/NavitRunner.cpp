/**
 * NavitConfigurator, makes it easy to configure Navit
 * Copyright (C) 2012 Raimar Bühmann
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

#include "NavitRunner.h"

#include <QtCore/QVector>

static const char* NAVIT_INTEND_NAME        = "org.navitproject.navit/org.navitproject.navit.Navit";
static const char* NAVIT_XML_PATH_ON_DEVICE = "sdcard/navit/navit.xml";

NavitRunner::NavitRunner(const Document& document)
:
	Android(NAVIT_INTEND_NAME),
	document(document)
{
}

NavitRunner::~NavitRunner() {
}

QString NavitRunner::uploadDocument() const {
	return Android::pushFile(document.getFilename(), NAVIT_XML_PATH_ON_DEVICE);
}
