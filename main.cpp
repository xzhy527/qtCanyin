#include "mainwindow.h"
#include "yf.h"
#include "loginview.h"
int main(int argc, char *argv[])
{
    YF a(argc,argv);
    LoginView lview;
    MainWindow *w;
    if(lview.exec()==QDialog::Accepted){
        //lview.deleteLater();
        w=new MainWindow();
        w->show();
    }
    qDebug()<<"45666";
    return a.exec();
}
