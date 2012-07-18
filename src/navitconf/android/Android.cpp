/*
 * Android.cpp
 *
 *  Created on: 04.06.2012
 *      Author: remo
 */

#include "Android.h"

#include <QDebug>

const static char* const ADB_PROGAM_NAME = "adb";

Android::Android(const char* const INTEND_NAME)
:
	INTEND_NAME(INTEND_NAME)
{
}

Android::~Android() {
}

QString Android::install(const QString& apkFilename) const {
	QStringList params;
	params.append("install");
	params.append("-r"); // reinstall
	params.append(apkFilename);
	return exec(ADB_PROGAM_NAME, params); // install
}

QString Android::run(bool restart) const {
	QStringList params;
	params.append("shell");
	params.append("am");
	params.append("start");
	if (restart) {
		params.append("--activity-clear-top");
	}
	params.append("-a"); // action
	params.append("android.intent.action.MAIN");
	params.append("-n"); // component
	params.append(INTEND_NAME); // org.navitproject.navit/org.navitproject.navit.Navit
	return exec(ADB_PROGAM_NAME, params); // start
}

QString Android::pushFile(const QString& localFilename, const QString& targetFilename) const {
	QStringList params;
	params.append("push");
	params.append(localFilename);
	params.append(targetFilename);
	return exec(ADB_PROGAM_NAME, params);
}

QString Android::pullFile(const QString& localFilename, const QString& sourceFilename) const {
	QStringList params;
	params.append("pull");
	params.append(localFilename);
	params.append(sourceFilename);
	return exec(ADB_PROGAM_NAME, params);
}

QString Android::pressKeys(QVector<KeyCode> keyCodes) const {
	QString result;
	for (int i = 0; i < keyCodes.size(); i++) {
		result.append(exec(ADB_PROGAM_NAME, getKeyEventParams(keyCodes[i])));
	}
	return result;
}

QStringList Android::getKeyEventParams(int keyCode) const {
	QStringList params;
	params.append("shell");
	params.append("input");
	params.append("keyevent");
	params.append(QString::number(keyCode));
	return params;
}

QString Android::exec(const QString& programName, const QStringList& params) const {
	QProcess p;
	p.start(programName, params);
	p.waitForFinished(20000); // timeout 20 seconds
	QString result;
	result.append(programName);
	for (int i = 0; i < params.size(); i++) {
		result.append(' ');
		result.append(params[i]);
	}
	result.append('\n');
	result.append(p.readAllStandardOutput()); // out
	result.append(p.readAllStandardError());
	return result;
}
