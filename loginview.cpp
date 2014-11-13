#include "loginview.h"
#include "ui_loginview.h"
#include <QDebug>

LoginView::LoginView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginView)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    connect(YF::self(),SIGNAL(replyjsonobject(QJsonObject)),this,SLOT(readreply(QJsonObject)));
    ui->loginname_editer->setFocus();
}
LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_quitebtn_clicked()
{
    qApp->quit();
}

void LoginView::on_loginbtn_clicked()
{
    if(ui->loginname_editer->text().length()<1||ui->password_editer->text().length()<1)
        return;
    QJsonObject json;
    json.insert("loginName",ui->loginname_editer->text());
    json.insert("loginPWD",ui->password_editer->text());
    YF::websiteurl=ui->weburlediter->text()+"/";
    if(!YF::websiteurl.startsWith("http://",Qt::CaseInsensitive)){
        YF::websiteurl="http://"+YF::websiteurl;
    }
    YF::self()->post("validateLogin",json);

}

void LoginView::readreply(QJsonObject json)
{

    if(json.take("statusCode").toString().toInt()<=0){
       QString message=json.take("message").toString();
       ui->loginmessage->setText(message);
    }else{
        qDebug()<<json.take("statusCode").toString().toInt();
        this->accept();
    }

}

void LoginView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
            move_point = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
}

void LoginView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
         move(event->globalPos() - move_point);
         event->accept();
     }
}
