#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "yf.h"
#include "YF/dishesscale.h"
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <QAction>
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
    YFTask *m_task;
    QThread m_thread;
private:
    Ui::MainWindow *ui;
    QStandardItemModel *dishesModel;
    QSqlQueryModel *dishessearchModel;
    QSqlTableModel *orderModel;
    int viewtype;
    int orderindex;//订单序列
    DishesScale disheimage;   //菜品代理
    QMenu *ordermenu;
    QAction *deleteAction;
    QAction *copyAction;
    QList<QAction*> orderActions;//订单的菜单
    QJsonArray dishesJsonListData;
    int findordered(int id=-1,QString name="");
    void increasedishes(QJsonObject dishesobject,int num=1,bool autoupdatecheck=true);
    void decreasedishes(QJsonObject QJsonObject,int num=1,bool autoupdatecheck=true);
    QJsonObject fetchDishesID(QModelIndex data=QModelIndex());
    void changedishedcheck(int id, int num);
    void changedishedcheck(QModelIndex index, int num);
    void switchViewtype(int newviewtype=2);
    void decreasedishes(int id, int num, bool autoupdatecheck);
    void insertNotification(QJsonObject json);
private slots:
    void replyjsonArray(QJsonArray);
    void on_refreshbtn_clicked();
    void on_comboBox_activated(int index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void resizeGridClomn(int logicalIndex, int oldSize, int newSize);
    void on_settingbtn_clicked();
    void initview();
    void createtempsales();
    void orderIDchange(int index);
    void loadDishesLocalData(QString sqltext="", bool isapppend=false);
    void loadDishesLocalData(QSqlQuery query,bool isapppend=false);
    void loadcacheorder(QString orderid="");
    void on_pushButton_clicked();
    void searchdishes(QString text);
    void customContext(QPoint);
    void searchfinished();
    QModelIndex finddishes(QString propertyname,QVariant value);
    void actionhandle();
    void countordertotal(QString sid="");
protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *);
signals:
    void downloadimage(QUrl url);
    void updatetoptipmessage(QString messagetext,int msgtype=0);
public:
    bool eventFilter(QObject *, QEvent *);
};
class YFTask : public QObject {
    Q_OBJECT
public:
   explicit YFTask();
   explicit YFTask(Ui::MainWindow *ui);
    ~YFTask() {network->deleteLater();}
public slots:
    void httpdownload(QUrl url);
    void finished(QNetworkReply*);
    void showNotification(QString text="", int msgtimes=0);
private:
    QNetworkAccessManager *network;
    Ui::MainWindow *ui;
    QMutex mutex;
};
#endif // MAINWINDOW_H
