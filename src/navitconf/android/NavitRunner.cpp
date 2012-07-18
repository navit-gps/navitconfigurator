/*
 * NavitRunner.cpp
 *
 *  Created on: 04.06.2012
 *      Author: remo
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
