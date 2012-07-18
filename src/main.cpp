#include <QtGui/QApplication>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include <QtCore/QDebug>
#include <QtGui/QIcon>

#include "navitconf/gui/NavitConfigurator.h"

/**
 * Set the translated language depending on definition in the operation system.
 * The *.qm files stored in the program folder are used for other language than
 * English. The file qt_DE.qm is copied from /usr/share/qt4/translations/
 * to the program folder.
 *
 * Follow these steps to create a new language translation:
 * 1. Add a language file name (e.g. NavitConfigurator_fr.ts) to the following
 *    line in the pro-file to activate further language translations
 *    separated by spaces:
 * <pre>
 * TRANSLATIONS = NavitConfigurator_de.ts
 * </pre>
 * 2. Then create the language translation file (e.g. NavitConfigurator_fr.ts) with
 * <pre>
 * lupdate NavitConfigurator.pro
 * # delete not used translations with
 * lupdate -noobsolete NavitConfigurator.pro
 * </pre>
 *    Every call like <code>tr("translate this text")</code> in the source code
 *    will have its own line in each *.ts file.
 * 3. Edit the *.ts files and create the *.qm files
 *    (e.g. NavitConfigurator_fr.qm) with
 * <pre>
 * lrelease NavitConfigurator.pro
 * </pre>
 *
 * Tipp: Install Qt-Lingust for editing the ts-file:
 * <pre>
 * sudo apt-get install qt4-dev-tools
 * linguist
 * </pre>
 *
 * @see http://doc.qt.nokia.com/4.7/internationalization.html
 */
void inline setLanguage() {
	// get language acronym
	QString local(QLocale::system().name());
	int underscorePosition = local.indexOf('_');
	if (underscorePosition >= 0) { // e.g. cut _DE from de_DE
		local = local.mid(0, underscorePosition);
	}
	// set language for Qt library
    static QTranslator qtTranslator;
    qtTranslator.load("qt_" + local);
    QApplication::installTranslator(&qtTranslator);
    // set language for application
    static QTranslator myappTranslator;
    myappTranslator.load("NavitConfigurator_" + local);
    QApplication::installTranslator(&myappTranslator);
}

int main(int argc, char* argv[]) {
    // use resource file containing all icons build into the application file
    Q_INIT_RESOURCE(NavitConfigurator);
    QApplication app(argc, argv);
    // Set the organization name for the automatically created settings file
    // stored in $HOME/.config/NavitConfigurator.conf
    app.setOrganizationName("NavitConfigurator");
    setLanguage();
    NavitConfigurator w;
    // start program and exit after application window has been closed
    return app.exec();
}
