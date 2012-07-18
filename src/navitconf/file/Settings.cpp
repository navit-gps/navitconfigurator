/*
 * Settings.cpp
 */

#include "Settings.h"

//#include <QtGui/QApplication>

Settings& Settings::getInstance() {
	static Settings instance;
	return instance;
}

Settings::Settings()
:
		settings(),
		dir(QDir::homePath().append("/.NavitConfigurator"))
{
	if (!dir.exists()) {
		if (!dir.mkdir(dir.absolutePath())) {
			throw QString("Creating folder failed: ")+dir.path();
		}
	}
}

Settings::~Settings() {
}

QDir& Settings::getDir() {
	return dir;
}

bool Settings::hasKey(int id) {
	return settings.allKeys().contains(QString("%1").arg(id));
}

void Settings::setBool(BoolId id, bool isTrue) {
	settings.setValue(QString("%1").arg(id), isTrue);
}

bool Settings::getBool(BoolId id, bool defaultValue) {
	return settings.value(QString("%1").arg(id), defaultValue).toBool();
}

void Settings::setInt(IntId id, int value) {
	settings.setValue(QString("%1").arg(id), value);
}

int Settings::getInt(IntId id, int defaultValue) {
	return settings.value(QString("%1").arg(id), defaultValue).toInt();
}

void Settings::setString(StringId id, const QString& value) {
	settings.setValue(QString("%1").arg(id), value);
}

QString Settings::getString(StringId id, const QString& defaultValue) {
	return settings.value(QString("%1").arg(id), defaultValue).toString();
}

void Settings::setQStringList(StringListId id, const QStringList& list) {
	settings.setValue(QString("%1").arg(id), list);
}

QStringList Settings::getQStringList(StringListId id) {
	return settings.value(QString("%1").arg(id)).toStringList();
}

void Settings::setBytes(BytesId id, const QByteArray& bytes) {
	settings.setValue(QString("%1").arg(id), bytes);
}

QByteArray Settings::getBytes(BytesId id) {
	return settings.value(QString("%1").arg(id)).toByteArray();
}

QString Settings::getNavitInstallationFile() {
	return getDir().absolutePath()+"/navit.apk";
}

