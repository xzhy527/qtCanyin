#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "yf.h"
#include "YF/dishesscale.h"
namespace Ui {
class MainWindow;
}
class DownloadDishesImages;
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
    QJsonArray dishesJsonListData;
    DownloadDishesImages *downloadthread;
    void initview();
    int findordered(QString id="",QString name="");
    void increasedishes(QJsonObject dishesobject);
    void decreasedishes(QJsonObject QJsonObject,int num=1);
    QJsonObject fetchDishesID(QModelIndex data);
    void changedishedcheck(QString id,int num);
    void switchViewtype(int newviewtype=2);
public slots:
    void replyjsonArray(QJsonArray);
private slots:
    void on_refreshbtn_clicked();
    void on_comboBox_activated(int index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void resizeGridClomn(int logicalIndex, int oldSize, int newSize);
    void showNotification(QString text="",int msgtype=0);
    // QWidget interface
    void on_settingbtn_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *);
};
class DownloadDishesImages:public QThread{

public:
    DownloadDishesImages(){}
    // QThread interface
protected:
    virtual void run();
};

#endif // MAINWINDOW_H
