#include "yf.h"
#include <QSplashScreen>
#include <QLabel>
#include <QThread>
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

void YF::setsetting(QString key, QVariant value,QString filename)
{
    QSettings config(filename,QSettings::IniFormat);
    config.setValue(key,value);
}

QVariant YF::getsettingvalue(QString key,QString filename)
{
   QSettings config(filename,QSettings::IniFormat);
   return config.value(key);
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

QByteArray YF::get(QString url,bool async)
{
    QByteArray bytes;
    QNetworkRequest request(url);
    if(!async){
      YF::self()->syncreply=YF::self()->networkaccess->get(request);
      QEventLoop eventLoop;
      connect(YF::self()->syncreply,SIGNAL(finished()),&eventLoop,SLOT(quit()));
     // connect(YF::self()->networkaccess,SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
      eventLoop.exec();
      bytes=YF::self()->syncreply->readAll();
      YF::self()->syncreply->deleteLater();
    }else{
       YF::self()->networkaccess->get(request);
    }
    return bytes;
}

void YF::popErrorMessage(QWidget* parent, QString messagetext)
{
    QLabel *errorlabel=new QLabel();
    errorlabel->setObjectName("popErrorMessage");
    errorlabel->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    if(parent!=NULL){
        QRect rect(parent->x()+parent->width()/2-100,parent->y()+parent->height()/2-25,200,50);
        errorlabel->setGeometry(rect);
    }
    errorlabel->activateWindow();
    errorlabel->setText(messagetext);
    errorlabel->setAlignment(Qt::AlignCenter);
    errorlabel->show();
    for (int var = 100; var>=0; --var) {
        errorlabel->setWindowOpacity(var*0.01);
        if(var>80){
           QThread::msleep(50);
        }
        QThread::msleep(20);
        processEvents();
    }
    errorlabel->deleteLater();
}
void YF::networkfinished(QNetworkReply *re_t)
{
    if(YF::self()->syncreply==re_t)return;
    QNetworkReply::NetworkError networkreplyerror=re_t->error();
    if(networkreplyerror!=0){
        //取活动的窗口界面
        QWidget *curr_widget=activeWindow();
        YF::popErrorMessage(curr_widget,"网络连接出现错误");
        return;
    }
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
    re_t->close();
    re_t->deleteLater();
}
