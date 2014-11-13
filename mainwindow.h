#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "yf.h"
#include "YF/dishesscale.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT




public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum Viewtype{scaleview=0,listtable=1,wordbutton=2};
private:
    Ui::MainWindow *ui;
    QStandardItemModel *dishesModel;
    QStandardItemModel *orderModel;
    int viewtype;
    DishesScale disheimage;
    // QWidget interface
public slots:
    void replyjsonArray(QJsonArray);
private slots:
    void on_refreshbtn_clicked();
    void on_comboBox_activated(int index);
    void on_tableView_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
