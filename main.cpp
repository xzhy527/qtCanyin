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

    YF::websiteurl="http://www.3blade.com/";
    LoginView lview;
    MainWindow *w;
    w=new MainWindow();
    w->show();
//    if(lview.exec()==QDialog::Accepted){
//        //lview.deleteLater();
//        w=new MainWindow();
//        w->show();
//    }
//    YFWindow window;
//    window.headwidget->setHeadHeight(30);
//    window.headwidget->SetHeadTitle("qq聊天界面");
//    window.show();
    return a.exec();
}
