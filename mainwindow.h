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
    int m_colmncount;
    int m_colmnwidth;
    int m_colmnheight;
private:
    Ui::MainWindow *ui;
    QStandardItemModel *dishesModel;
    QStandardItemModel *orderModel;
    int viewtype;
    DishesScale disheimage;
    void initview();
    int findordered(QString id="",QString name="");
public slots:
    void replyjsonArray(QJsonArray);
private slots:
    void on_refreshbtn_clicked();
    void on_comboBox_activated(int index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_activated(const QModelIndex &index);
    void on_tableView_entered(const QModelIndex &index);
    void on_tableView_pressed(const QModelIndex &index);

};

#endif // MAINWINDOW_H
