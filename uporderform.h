#ifndef UPORDERFORM_H
#define UPORDERFORM_H
#include <QWidget>
#include "yf.h"
namespace Ui {
class UporderForm;
}

class UporderForm : public QWidget
{
    Q_OBJECT

public:
    explicit UporderForm(QWidget *parent = 0);
    explicit UporderForm(QString orderID,QWidget *parent = 0);
    ~UporderForm();
public:
    QString orderid;
private slots:
    double readorderinfo(QString orderID);
    void sattle();
    void calc(QString realmomey);
private:
    Ui::UporderForm *ui;
    QStandardItemModel *orderModel;
    void init();
};

#endif // UPORDERFORM_H
