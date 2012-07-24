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

#include "Android.h"

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
