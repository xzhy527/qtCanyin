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
class YF : public QApplication
{
    Q_OBJECT

public:
    YF(int &argc, char **argv);
    ~YF();
    static YF* self();
    static QNetworkAccessManager *networkaccess;
    static QString websiteurl;

    static void post(QString url,const QByteArray & data="");
    static void post(QString url,QJsonObject json);
signals:
    void replyjsondocument(QJsonDocument);
    void replyjsonobject(QJsonObject);
    void replyjsonlist(QJsonArray);
public slots:
    void networkfinished(QNetworkReply*);
private:
    QJsonDocument *jsondocument;
};
//一封测试
#endif // YF_H
