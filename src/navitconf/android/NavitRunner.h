/*
 * NavitRunner.h
 *
 *  Created on: 04.06.2012
 *      Author: remo
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
