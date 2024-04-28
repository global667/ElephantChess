#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QTabWidget>
#include <QWidget>
#include <utility>
#include <QProgressDialog>
#include <QtZlib/zlib.h>

class AboutView final : public QTabWidget
{
    Q_OBJECT
public:
    explicit AboutView(QWidget *parent = nullptr);

private:
    //QTabWidget *tabview;
    QWidget *tabwidget1;
    QWidget *tabwidget2;
    QWidget *tabwidget3;

signals:
};

class DownloadView final : public QWidget {
    Q_OBJECT
public:
    explicit DownloadView( QWidget *parent = nullptr, QUrl link = QUrl("https://www.elephant-chess.com/wp-content/uploads/2024/04/pikafish.zip")) {
        this->parent = parent;
        progressbar = new QProgressDialog(this);
        progressbar->show();
        url = new QUrl(std::move(link));
        request = new QNetworkRequest(*url);
        manager = new QNetworkAccessManager();
        reply = manager->get(*request);
        QObject::connect(reply, SIGNAL(finished()), this,SLOT(downloadFinished()));
        QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        QObject::connect(this, SIGNAL(downloaded(QString)), parent, SLOT(onDownloaded(QString)));
        progressbar->setWindowTitle("Downloading Pikafish");
        progressbar->setLabelText("Downloading Pikafish");
        progressbar->setMinimum(0);
        progressbar->setValue(0);
        progressbar->setWindowModality(Qt::WindowModal);
    };

private:
    QNetworkRequest *request;
    QNetworkAccessManager *manager;
    QUrl * url;
    QNetworkReply *reply;
    QObject *parent;
    QProgressDialog * progressbar;

signals:
    void downloaded(QString filename) const;

private slots:
        void downloadFinished() const {
            if (reply->error()) {
                qDebug() << "Error: " << reply->errorString();
            } else {
               /* QDir dir;
                if (!dir.exists("engines")) {
                    dir.mkdir("engines");
                }
                */
                //QFile file("engines/pikafish.zip");
                QFile file(QDir::currentPath() + "./pikafish.zip");
                qDebug() << QDir::currentPath();
                if (file.open(QFile::WriteOnly)) {
                    file.write(reply->readAll());
                    file.close();
                    qDebug() << "File downloaded";
                } else {
                    qDebug() << "Error: Unable to open file for writing";
                }
            }
            reply->deleteLater();
    QProcess process;
    process.setWorkingDirectory(QDir::currentPath());
    //process.setArguments({"tar","-x", "pikafish.zip"});
    process.start("cmd.exe start /c tar -x pikafish.zip");
    process.waitForFinished(-1);
    //QFile::remove("pikafish.zip");

            emit downloaded("pikafish.exe");
        }

    void downloadProgress(const qint64 bytesReceived, const qint64 bytesTotal) const {
            qDebug() << "Downloaded " << bytesReceived << " of " << bytesTotal << " bytes";
            progressbar->setMaximum(static_cast<int>(bytesTotal));
            progressbar->setValue(static_cast<int>(bytesReceived));
        }
};



#endif // ABOUTVIEW_H
