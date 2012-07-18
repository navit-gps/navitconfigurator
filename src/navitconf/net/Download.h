#include <QObject>
#include <QWidget>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

/**
 * Download helper class.
 * Needs: QT += network
 */
class Download : public QObject {
    Q_OBJECT
public:
    /**
     * @param parent The parent object who wants to listen to the signals.
     */
	Download(QObject* parent);
	~Download();
	/**
	 * Downloads a file given by the url and saves it after
	 * the download to the given filename.
	 * @param url      The URL for a file to download.
	 * @param filename The local file name.
	 */
	void download(const QString& url, const QString& filename);

signals:
	/**
	 * @param percent The progress as a number between 0.0 and 1.0
	 */
	void downloadProgress(float percent);
	/**
	 * Signals a successfully written file.
	 * @param filename The file name of the written file.
	 */
	void downloadFinished(const QString& filename);
	/**
	 * Signals something goes wrong.
	 * @param msg A message describing the error.
	 */
	void downloadError(const QString& msg);

private slots:
	void downloadProgress(qint64 recieved, qint64 total);
	void downloadFinished(QNetworkReply* data);
	void error();

private:
	QObject* parent;
	QString filename;
	/** The used Qt-Manager for downloading a file */
	QNetworkAccessManager manager;
	QNetworkReply* reply;

};
