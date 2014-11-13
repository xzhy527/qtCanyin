#include "yf.h"
QNetworkAccessManager* YF::networkaccess=0;
QString YF::websiteurl="";
YF::YF(int &argc, char **argv):QApplication(argc,argv)
{
    qApp->setOrganizationName(QLatin1String("ccxt"));
    qApp->setApplicationName("CanYin");
    qApp->setApplicationVersion(QLatin1String("1.0"));
    this->networkaccess=new QNetworkAccessManager(this);
    connect(this->networkaccess,SIGNAL(finished(QNetworkReply*)),this,SLOT(networkfinished(QNetworkReply*)));
}

YF::~YF()
{

}

YF *YF::self()
{
    return (static_cast<YF*>(QCoreApplication::instance()));
}

void YF::post(QString urltext, const QByteArray &data)
{
    qDebug()<<YF::websiteurl+"/"+urltext;

    QUrl url(YF::websiteurl+urltext);
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    //request.setRawHeader("Content-Type","application/Json");
    YF::self()->networkaccess->post(request,data);
    //re_t->setRawHeader(QByteArray("Last-Modified"), QByteArray("Sun, 06 Nov 1994 08:49:37 GMT"));
    //re_t->setRawHeader(QByteArray("Content-Type"),QByteArray("application/Json"));
    // re_t->setRawHeader("Content-Type","application/Json");
}

void YF::post(QString url, QJsonObject json)
{
    //此处应该有更好的解决方案
//    QJsonDocument jsondocument;
//    jsondocument.setObject(json);
      QByteArray data;
      QStringList list=json.keys();
      foreach(QString keyname,list) {
        data.append(keyname+"="+json.take(keyname).toString()+"&");
      }
      YF::self()->post(url,data);
}

void YF::networkfinished(QNetworkReply *re_t)
{
    QJsonParseError json_error;
    QJsonDocument jsondocument=QJsonDocument::fromJson(re_t->readAll(),&json_error);

    if(json_error.error == QJsonParseError::NoError&&!jsondocument.isNull()&&!jsondocument.isEmpty()){
        if(jsondocument.isObject()){
           QJsonObject obj=jsondocument.object();
           emit self()->replyjsonobject(obj);
        }
        if(jsondocument.isArray())
           emit this->replyjsonlist(jsondocument.array());
        emit this->replyjsondocument(jsondocument);

    }
}
