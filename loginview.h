#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include "yf.h"
namespace Ui {
class LoginView;
}
class QPoint;
class LoginView : public QDialog
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = 0);
    ~LoginView();

private slots:
    void on_quitebtn_clicked();
    void on_loginbtn_clicked();
    void readreply(QJsonObject);
private:
    Ui::LoginView *ui;
    QPoint move_point;
    // QWidget interface
protected:
   virtual void mousePressEvent(QMouseEvent *event);
   virtual void mouseMoveEvent(QMouseEvent *event);

};

#endif // LOGINVIEW_H
