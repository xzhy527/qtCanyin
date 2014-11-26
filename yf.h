#ifndef YF_H
#define YF_H
#include <QString>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QUrl>
#include <qjsondocument.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QPalette>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
class YF : public QApplication
{
    Q_OBJECT

public:
    YF(int &argc, char **argv);
    ~YF();
    static YF* self();
    static QNetworkAccessManager *networkaccess;
    static QString websiteurl;
    static void setsetting(QString key,QVariant value,QString filename="setting.config");
    static QVariant getsettingvalue(QString key,QString filename="setting.config");
    static void post(QString url,const QByteArray & data="");
    static void post(QString url,QJsonObject json);
    static QByteArray get(QString url,bool async=true);
    static void popErrorMessage(QWidget* parent=0,QString messagetext="");
signals:
    void replyjsondocument(QJsonDocument);
    void replyjsonobject(QJsonObject);
    void replyjsonlist(QJsonArray);
public slots:
    void networkfinished(QNetworkReply*);
private:
    QJsonDocument *jsondocument;
    QNetworkReply *syncreply;
};

#endif // YF_H
