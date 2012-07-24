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

#ifndef NAVITRUNNER_H_
#define NAVITRUNNER_H_

#include <navitconf/file/Document.h>

#include "Android.h"

/**
 * Uploads the current navit.xml file and starts Navit on the Android device using.
 */
class NavitRunner: public Android {
public:
	/**
	 * @param document The navit.xml file using for upload.
	 */
	explicit NavitRunner(const Document& document);
	virtual ~NavitRunner();
	/**
	 * Uploads the navit.xml file to the Android device at the target <code>/sdcard/navit/navit.xml</code>
	 */
	QString uploadDocument() const;
private:
	const Document& document;
};

#endif /* NAVITRUNNER_H_ */
