#ifndef YFTOOLS_H
#define YFTOOLS_H
#include <QMessageBox>
#include <QApplication>
void YF_alert(QString text)
{
    QMessageBox::about(qApp->activeWindow(),"提示",text);
}
#endif // YFTOOLS_H
