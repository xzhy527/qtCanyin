#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    dishesModel=new QStandardItemModel(this);
    orderModel=new QStandardItemModel(this);
    viewtype=scaleview;
    ui->setupUi(this);
    ui->tableView->setModel(dishesModel);
    ui->tableView_2->setModel(orderModel);
    connect(YF::self(),SIGNAL(replyjsonlist(QJsonArray)),this,SLOT(replyjsonArray(QJsonArray)));
    YF::post("queryDishes");
    orderModel->setColumnCount(3);
    orderModel->setHeaderData(0,Qt::Horizontal,"项目");
    orderModel->setHeaderData(1,Qt::Horizontal,"数");
    orderModel->setHeaderData(2,Qt::Horizontal,"金额");
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setColumnWidth(0,170);
    ui->tableView_2->setColumnWidth(1,30);
    ui->tableView_2->setColumnWidth(2,45);
    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::replyjsonArray(QJsonArray jsonlist)
{
    dishesModel->clear();
    //ui->tableView=new QTableView(this);
    ui->tableView->setModel(dishesModel);
    switch (viewtype) {
        case scaleview:{
            qDebug()<<"进入所列图视图";
            ui->tableView->verticalHeader()->setVisible(false);
            ui->tableView->horizontalHeader()->setVisible(false);
            ui->tableView->setShowGrid(false);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            dishesModel->setColumnCount(5);

            for(int i=0;i<5;++i){
              ui->tableView->setItemDelegateForColumn(i,&disheimage);
              ui->tableView->setColumnWidth(i,110);
            }
            int cindex=0;
            int rindex=-1;
            for(int i=0;i<jsonlist.size();++i){
                if(cindex==0){
                   rindex++;
                   dishesModel->insertRow(rindex);
                   ui->tableView->setRowHeight(rindex,110);
                }
                qDebug()<<cindex<<rindex;
                QJsonObject jsonobj=jsonlist.at(i).toObject();
                dishesModel->setData(dishesModel->index(rindex,cindex),jsonobj);
                cindex=cindex>=4?0:cindex+1;

            }



        }
            break;
        case listtable:{


            ui->tableView->verticalHeader()->setVisible(true);
            ui->tableView->horizontalHeader()->setVisible(true);
            ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
            dishesModel->setColumnCount(7);
            dishesModel->setHeaderData(0,Qt::Horizontal,"快捷码");
            dishesModel->setHeaderData(1,Qt::Horizontal,"分类");
            dishesModel->setHeaderData(2,Qt::Horizontal,"类别");
            dishesModel->setHeaderData(3,Qt::Horizontal,"菜品商品");
            dishesModel->setHeaderData(4,Qt::Horizontal,"价格");
            dishesModel->setHeaderData(5,Qt::Horizontal,"供应量");
            dishesModel->setHeaderData(6,Qt::Horizontal,"备注信息");
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
            }


        }
            break;
        default:{

        }
        break;
    }

}







void MainWindow::on_refreshbtn_clicked()
{
    YF::post("queryDishes");
}

void MainWindow::on_comboBox_activated(int index)
{
   viewtype=index;
   YF::post("queryDishes");
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString name;
    double bprice;
    bool isadded=false;
    if(viewtype==scaleview){
        qDebug()<<index.data();
        QVariant tempdata=index.data();
        QJsonObject json= tempdata.toJsonObject();
        name=json.take("name").toString();
        bprice=json.take("bprice").toDouble();
    }
    else if(viewtype==listtable){
        name=dishesModel->data(index.sibling(index.row(),3)).toString();
        bprice=dishesModel->data(index.sibling(index.row(),4)).toDouble();
    }
    for(int i=0;i<orderModel->rowCount();++i){

       QString ordername=orderModel->data(orderModel->index(i,0)).toString();
       if(ordername==name){
            isadded=true;
            double count=orderModel->index(i,1).data().toDouble()+1;
            orderModel->setData(orderModel->index(i,1),count);
            count=orderModel->index(i,2).data().toDouble()+bprice;
            orderModel->setData(orderModel->index(i,2),count);

       }

    }
    if(!isadded){
        orderModel->insertRow(0);
        orderModel->setData(orderModel->index(0,0),name);
        orderModel->setData(orderModel->index(0,1),1);
        orderModel->setData(orderModel->index(0,2),bprice);



    }


}
