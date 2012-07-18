/*
 * Android.h
 *
 *  Created on: 04.06.2012
 *      Author: remo
 */

#ifndef ANDROID_H_
#define ANDROID_H_

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QVector>

/**
 * This class handles the communication with an Android mobile phone.
 */
class Android {
public:

	/**
	 * These key codes are used by adb to simulate key inputs.
	 * TODO: Need more key codes.
	 */
	typedef enum KeyCode {
		BACK = 4,
		ENTER = 66,
		DOWN = 19,
		UP = 20,
		LEFT = 21,
		RIGHT = 22
		// HOME = 102 ???
	} KeyCode;

	/**
	 * @param intendName The intend name of the program this class is working with.
	 */
	explicit Android(const char* intendName);
	virtual ~Android();

	/**
	 * (Re-)install the program given by apkFilename.
	 * @param apkFilename The path to the Android-APK installation file.
	 * @return The process with the result QProcess#readAllStandardOutput() + QProcess#readAllStandardError().
	 */
	QString install(const QString& apkFilename) const;

	/**
	 * Start the program with the intend name given in the constructor
	 * with a timeout of 20 seconds. The command
	 * <pre>adb shell am start --activity-clear-top -a android.intent.action.MAIN -n {@link Android#intendName}</pre>
	 * is used.
	 * @param restart If true, the extra command line option <code>--activity-clear-top</code> is added to ensure
	 *                that the Program is restarted, if it is already running.
	 *                True by default.
	 * @return The process with the result QProcess#readAllStandardOutput() + QProcess#readAllStandardError().
	 * @see #setStartingKeyCodes(const QVector<int>&)
	 */
	QString run(bool restart = true) const;

	/**
	 * Upload the given file to the Android device.
	 * @param localFilename The filename on this computer.
	 * @param targetFilename The filename on the Android device.
	 */
	QString pushFile(const QString& localFilename, const QString& targetFilename) const;

	/**
	 * Download the given file from the Android device.
	 * @param localFilename The filename on this computer.
	 * @param targetFilename The filename on the Android device.
	 */
	QString pullFile(const QString& localFilename, const QString& sourceFilename) const;

	/**
	 * The keys specified by the list of key codes are pressed one by one
	 * using <code>adb shell input keyevent {@link #KeyCode}</code>
	 */
	QString pressKeys(QVector<KeyCode> keyCodes) const;

private:
	const char* const INTEND_NAME;
	QStringList getKeyEventParams(int keyCode) const;
	QString exec(const QString& programName, const QStringList& params) const;
};

#endif /* ANDROID_H_ */
