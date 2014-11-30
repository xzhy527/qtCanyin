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
    QSqlDatabase::database().close();
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

QVariant YF::settingvalue(QString key,QString filename)
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

bool YF::initdatabaseconnect()
{
    QDir::current().mkpath("Data");
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
//    db.setHostName("Data/data.db");
    db.setDatabaseName("Data/data.db");
    db.setUserName("changchunxiantong");
    db.setPassword("xiaozhaoyang13756913399lisongyan13610709360");
    if(!db.open())return false;
    //表结构同网络相同
    //db.exec("drop table t_dishes");
    db.exec("create table t_dishes (dishesid integer primary key,id integer\
    ,barcode,pinyin,name,sort,picaddr,desrc,logo,mprice float,bprice float\
    ,rate float,classname,validity int,cost float,ischeck int,expirydate int,\
    ext1,ext2,ext3,genpy)");
    //id,信息类型,内容,发件人,最后提示时间,提示次数
    db.exec("create table db_toptip(id integer primary key,type,content,fromer,lasttime,times)");
    db.exec("create table t_sales(id integer primary key,saleid,salename,bprice,mprice,rate,sort,classname,quantity,rmbsum,waiter,creattime,state)");
    db.exec("create table t_order(mz,dj,sl,je,did,sid,cd,sw,id integer primary key)");
    db.exec("create table t_template(id int primary key,name,dishesname,dishesid,bprice,num,total)");
    return db.open();
}
/**
 * @brief 通过json对象进行数据库查询,
 * @param json
 *          table:表名
 *          otpion 默认insert 选项:update,delete,select,drop
 * @return
 */
bool YF::sql(QVariant sqlvariant, QSqlQuery* r_query)
{
    QSqlQuery query;
    if(r_query!=0){query=*r_query;qDebug()<<""<<r_query;}
    if(sqlvariant.isNull())return false;
    if(sqlvariant.canConvert(QMetaType::QJsonObject)){
        QJsonObject json=sqlvariant.toJsonObject();
        QString table=json.take("table").toString();
        if(table.isEmpty())return false;
        QString option=json.take("option").toString();
        option=option.isEmpty()?"insert":option;
        QString where=json.take("where").toVariant().toString();
        QString columns=json.take("columns").toString();
        QStringList cstrs;//节选列表
        QString lsqltext="";//左sql语句
        QString rsqltext="";//右sql语句
        QStringList jsonkeys=json.keys();
        //查询表结构
        query.exec("select * from "+table+" limit 0,1");
        //query.exec("select top 1 * from "+table);
        QSqlRecord record=query.record();
        //处理节选字段
        for(int i=0;i<jsonkeys.count();i++){
            QString keyname=jsonkeys.at(i);
            QString fieldname=keyname.split(":").at(0);
            if(record.indexOf(fieldname))cstrs<<keyname;
        }
        //判断where条件是否存在和解析字段
        if(option!="insert"&&option!="drop"&&option!="delete"){
            if(!where.isEmpty()){
                if(where.contains("=")){
                    rsqltext=" where "+where;
                }else{
                    if(record.indexOf(where)>-1&&!json.value(where).isUndefined()){
                        rsqltext=" where "+where+"'"+json.value(where).toVariant().toString()+"'";
                    }
                }
            }else{
                if(!json.value("id").isUndefined()){
                    rsqltext=" where id="+json.value("id").toVariant().toString() ;
                }
            }
        }
        if(option=="insert"){
            lsqltext="insert into "+table+" (";
            rsqltext=" values (";
            for(int m=0;m<cstrs.count();m++){
                QString fieldname=cstrs.at(m);
                lsqltext+=fieldname+",";
                rsqltext+="?,";
            }
            lsqltext=lsqltext.left(lsqltext.length()-1)+")";
            rsqltext=rsqltext.left(rsqltext.length()-1)+")";
            //goto sqlcall;
        }
        else if(option=="update"){
            lsqltext="update "+table+" set ";
            for(int m=0;m<cstrs.count();m++)lsqltext+=cstrs.at(m)+"=?,";
            lsqltext=lsqltext.left(lsqltext.length()-1);
        }
        else if(option=="delete"){
            lsqltext="delete "+table+rsqltext;
            return query.exec(lsqltext);
        }
        else if(option=="select"){
            columns=columns.isEmpty()?"*":columns;
            lsqltext="select "+columns+" from "+table;
            for(int m=0;m<cstrs.count();m++){
                QStringList filearray=cstrs.at(m).split(":");
                QString fieldname=filearray.at(0);
                QString mark=filearray.size()>1?filearray.at(1):"=";
                mark=mark.isEmpty()?"=":mark;
                if(rsqltext.isEmpty()){
                    rsqltext=" where "+fieldname+"=?";
                }else{
                    rsqltext=" and "+fieldname+"=?";
                }
            }
        }
        query.prepare(lsqltext+rsqltext);
        for(int i=0;i<cstrs.size();i++){
            QString str=cstrs.at(i);
            query.bindValue(i,json.value(str).toVariant());
        }
        bool re_t=query.exec();
        qDebug()<<"执行的语句:"<<query.executedQuery();
        if(!re_t){
            qDebug()<<query.lastQuery()<<"执行sql语句发生错误!"<<query.lastError().text();
        }
        return re_t;
    }
    else if(sqlvariant.canConvert(QMetaType::QString)){
        bool re_t=query.exec(sqlvariant.toString());

        qDebug()<<"执行的语句:"<<query.executedQuery();
        query.next();
        qDebug()<<"执行的语句:"<<query.value(0);
        return re_t;
    }
    return false;
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
