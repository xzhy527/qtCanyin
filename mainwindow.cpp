#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setObjectName("MainWindow");
    this->setAutoFillBackground(true);
    QColor color(66,90,82);
    this->setPalette(QPalette(color));
    dishesModel=new QStandardItemModel(this);
    orderModel=new QStandardItemModel(this);
    viewtype=scaleview;
    ui->setupUi(this);
    ui->tableView->setModel(dishesModel);
    ui->tableView_2->setModel(orderModel);
    connect(YF::self(),SIGNAL(replyjsonlist(QJsonArray)),this,SLOT(replyjsonArray(QJsonArray)));
    YF::post("queryDishes");
    initview();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//初始化视图
void MainWindow::initview()
{
   m_colmncount=5;
   m_colmnwidth=110;
   m_colmnheight=110;
   orderModel->setColumnCount(5);
   orderModel->setHeaderData(0,Qt::Horizontal,"项目");
   orderModel->setHeaderData(1,Qt::Horizontal,"单价");
   orderModel->setHeaderData(2,Qt::Horizontal,"数量");
   orderModel->setHeaderData(3,Qt::Horizontal,"金额");
   //orderModel->setHeaderData(4,Qt::Horizontal,"ID");
   ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableView_2->setColumnWidth(0,150);
   ui->tableView_2->setColumnWidth(1,30);
   ui->tableView_2->setColumnWidth(2,30);
   ui->tableView_2->setColumnWidth(3,40);
   ui->tableView_2->setColumnHidden(4,true);
   ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

int MainWindow::findordered(QString id, QString name)
{
    if(id.isEmpty()&&name.isEmpty())return -1;
    QList<QStandardItem *> dd;
    if(!id.isEmpty()){
      dd=orderModel->findItems(id,Qt::MatchExactly,4);
      return dd.count()>0?dd.at(0)->row():-1;
    }
    if(!name.isEmpty()){
        dd=orderModel->findItems(tr("酱香牛肉"),Qt::MatchExactly,0);
        return dd.count()>0?dd.at(0)->row():-1;
    }
    return -1;
}



void MainWindow::replyjsonArray(QJsonArray jsonlist)
{
    dishesModel->clear();
    ui->tableView->setModel(dishesModel);
    if(viewtype==scaleview||viewtype==wordbutton){
        ui->tableView->verticalHeader()->setVisible(false);
        ui->tableView->horizontalHeader()->setVisible(false);
        ui->tableView->setShowGrid(false);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->horizontalHeader()->setStretchLastSection(false);
        dishesModel->setColumnCount(m_colmncount);
        ui->tableView->setItemDelegate(&disheimage);
        int cindex=0;
        int rindex=-1;
        for(int i=0;i<jsonlist.size();++i){
            if(cindex==0){
               rindex++;
               dishesModel->insertRow(rindex);
               ui->tableView->setRowHeight(rindex,m_colmnheight);
            }
            ui->tableView->setColumnWidth(cindex,m_colmnwidth);
            qDebug()<<cindex<<rindex;
            QJsonObject jsonobj=jsonlist.at(i).toObject();
            jsonobj.insert("viewtype",viewtype);
            qDebug()<<"是否已经下订单:"<<findordered(QString::number(jsonobj.value("ID").toInt()));
            if(findordered(QString::number(jsonobj.value("id").toInt()))>=0){
                jsonobj.insert("selected",true);
            }
            dishesModel->setData(dishesModel->index(rindex,cindex),jsonobj);
            cindex=cindex>=m_colmncount-1?0:cindex+1;
        }
    }else{
        //这是列表表格显示状态
        ui->tableView->verticalHeader()->setVisible(true);
        ui->tableView->horizontalHeader()->setVisible(true);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        dishesModel->setColumnCount(8);
        dishesModel->setHeaderData(0,Qt::Horizontal,"快捷码");
        dishesModel->setHeaderData(1,Qt::Horizontal,"分类");
        dishesModel->setHeaderData(2,Qt::Horizontal,"类别");
        dishesModel->setHeaderData(3,Qt::Horizontal,"菜品商品");
        dishesModel->setHeaderData(4,Qt::Horizontal,"价格");
        dishesModel->setHeaderData(5,Qt::Horizontal,"供应量");
        dishesModel->setHeaderData(6,Qt::Horizontal,"备注信息");
        ui->tableView->setColumnHidden(7,true);
        for(int i=0;i<jsonlist.size();++i){
            QJsonObject jsonobj=jsonlist.at(i).toObject();
            dishesModel->insertRow(i);
            dishesModel->setData(dishesModel->index(i,0),jsonobj.take("barcode").toVariant());
            dishesModel->setData(dishesModel->index(i,1),jsonobj.take("sort").toVariant());
            dishesModel->setData(dishesModel->index(i,2),jsonobj.take("classname").toVariant());
            dishesModel->setData(dishesModel->index(i,3),jsonobj.take("name").toVariant());
            dishesModel->setData(dishesModel->index(i,4),jsonobj.take("bprice").toVariant());
            dishesModel->setData(dishesModel->index(i,5),jsonobj.take("supply").toVariant());
            dishesModel->setData(dishesModel->index(i,6),jsonobj.take("desc").toVariant());
            dishesModel->setData(dishesModel->index(i,7),jsonobj.take("id").toVariant());
        }
    }
}







void MainWindow::on_refreshbtn_clicked()
{
   232323232

    YF::post("queryDishes");
}

void MainWindow::on_comboBox_activated(int index)
{
   viewtype=index;
   YF::post("queryDishes");
}
//表格项目双击事件
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int id=0;
    QString name;
    double bprice;
    if(viewtype==scaleview||viewtype==wordbutton){
        qDebug()<<index.data();
        QVariant tempdata=index.data();
        QJsonObject json= tempdata.toJsonObject();

        name=json.value("name").toString();
        bprice=json.value("bprice").toDouble();
        id=json.value("id").toInt();
        //if(json.value("selected").isUndefined()||json.value("selected").isNull()){
            json.insert("selected",true);
        //}
        qDebug()<<"添加的数据是:"<<json;
        dishesModel->setData(index,json);
    }
    else if(viewtype==listtable){
        name=dishesModel->data(index.sibling(index.row(),3)).toString();
        bprice=dishesModel->data(index.sibling(index.row(),4)).toDouble();
        id=dishesModel->data(index.sibling(index.row(),7)).toInt();
    }
    if(id>0){
        //从服务器上获取该产品的基本信息
    }
    qDebug()<<"ID此时的值"<<id;
    int r_index=findordered(QString::number(id),name);
    if(r_index>=0){
        //计算数量
        double count=orderModel->index(r_index,2).data().toDouble()+1;
        orderModel->setData(orderModel->index(r_index,2),count);
        //计算金额
        count=orderModel->index(r_index,3).data().toDouble()+bprice;
        orderModel->setData(orderModel->index(r_index,3),count);
    }else{
        //首次添加，价格和单价都为bprice，应该从远程数据库再添加
        orderModel->insertRow(0);
        orderModel->setData(orderModel->index(0,0),name);
        orderModel->setData(orderModel->index(0,1),bprice);
        orderModel->setData(orderModel->index(0,2),1);
        orderModel->setData(orderModel->index(0,3),bprice);
        orderModel->setData(orderModel->index(0,4),id);

    }
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    qDebug()<<"activate:"<<index.data();
}

void MainWindow::on_tableView_entered(const QModelIndex &index)
{
    qDebug()<<"entered:"<<index.data();
}

void MainWindow::on_tableView_pressed(const QModelIndex &index)
{
    qDebug()<<"on_tableView_pressed:"<<index.data();
}
