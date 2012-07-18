#include "Download.h"

#include <QFile>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>

Download::Download(QObject* parent)
:
		QObject(0),
		parent(parent)
{
	// connect the signals this class can throw to its parent
	connect(this,   SIGNAL(downloadProgress(float)),
	        parent, SLOT(  downloadProgress(float)));
	connect(this,   SIGNAL(downloadFinished(const QString&)),
	        parent, SLOT(  downloadFinished(const QString&)));
	connect(this,   SIGNAL(downloadError(const QString&)),
	        parent, SLOT(  downloadError(const QString&)));
	// connect slot for finished downloads emitted by used QT download manager
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
    		this,     SLOT(  downloadFinished(QNetworkReply*)));
}

Download::~Download() {
}

void Download::download(const QString& url, const QString& filename) {
	QUrl qUrl(QUrl::fromEncoded(url.toLocal8Bit()));
	if (!qUrl.isValid()) {
		emit downloadError(QString("The URL is not valide: ")+url);
	} else {
		reply = manager.get(QNetworkRequest(qUrl));
		QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
		                 this,  SLOT(  downloadProgress(qint64, qint64)));
		QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
		                 this,  SLOT(  error()));
		this->filename = filename;
	}
}

void Download::downloadProgress(qint64 recieved, qint64 total) {
	emit downloadProgress(static_cast<float>(recieved) / total);
}

void Download::downloadFinished(QNetworkReply* data) {
	if (data->size() > 0) {
		QFile file(this->filename);
		if (file.open(QIODevice::WriteOnly)) {
			const QByteArray sdata = data->readAll();
			file.write(sdata);
			file.close();
			emit downloadFinished(file.fileName());
		} else {
			QString str("Writing file failed: ");
			str + file.fileName() + ": " + file.errorString();
			emit downloadError(str);
		}
	}
	reply->deleteLater();
}

void Download::error() {
	emit downloadError(reply->errorString()+".");
}
