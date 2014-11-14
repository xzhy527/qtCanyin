#include "mainwindow.h"
#include "yf.h"
#include "loginview.h"
#include "YF/yfwindow.h"
#include <QFile>
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qrc);
    YF a(argc,argv);
    QFile qfile(":/css/qcss.qss");
    if(qfile.open(QFile::ReadOnly)){
        a.setStyleSheet(qfile.readAll());
        qfile.close();
    }


//    LoginView lview;
//    MainWindow *w;
//    if(lview.exec()==QDialog::Accepted){
//        //lview.deleteLater();
//        w=new MainWindow();
//        w->show();
//    }
    YFWindow window;
    window.show();
    //qDebug()<<"123456";
    return a.exec();
}
