#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "yf.h"
#include "YF/dishesscale.h"
#include <QMutex>
#include <QMutexLocker>
namespace Ui {
class MainWindow;
}
class YFTask;
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
    YFTask *m_task;
    QThread m_thread;
private:
    Ui::MainWindow *ui;
    QStandardItemModel *dishesModel;
    QSqlQueryModel *dishessearchModel;
    QSqlTableModel *orderModel;
    int viewtype;
    int orderindex;
    DishesScale disheimage;
    QJsonArray dishesJsonListData;
    int findordered(int id=-1,QString name="");
    void increasedishes(QJsonObject dishesobject,int num=1,bool autoupdatecheck=true);
    void decreasedishes(QJsonObject QJsonObject,int num=1,bool autoupdatecheck=true);
    QJsonObject fetchDishesID(QModelIndex data);
    void changedishedcheck(int id, int num);
    void changedishedcheck(QModelIndex index,int changenum);
    void switchViewtype(int newviewtype=2);
    void decreasedishes(int id, int num, bool autoupdatecheck);
    void insertNotification(QJsonObject json);
public slots:
    void replyjsonArray(QJsonArray);
private slots:
    void on_refreshbtn_clicked();
    void on_comboBox_activated(int index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void resizeGridClomn(int logicalIndex, int oldSize, int newSize);
    void showNotification(QString text="",int msgtype=0);
    void on_settingbtn_clicked();
    void initview();
    void createtempsales();
    void orderIDchange(int index);
    void loadDishesLocalData();
    void loadcacheorder();
    void on_pushButton_clicked();
    void searchdishes(QString text);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *);
signals:
    void downloadimage(QUrl url);
    void updatetoptipmessage(QString messagetext,int msgtype=0);

};
class YFTask : public QObject {
    Q_OBJECT
public:
    YFTask();
    ~YFTask() {
        network->deleteLater();
    }
   QMutex mutex;

public slots:
    void httpdownload(QUrl url);
    void finished(QNetworkReply*);
private:
    QNetworkAccessManager *network;

};
#endif // MAINWINDOW_H
