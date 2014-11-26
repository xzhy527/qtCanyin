#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QTimer>
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    this->setWindowTitle("智能餐饮管理系统");
    this->setObjectName("MainWindow");
    this->setAutoFillBackground(true);
    QColor color(66,90,82);
    this->setPalette(QPalette(color));

    ui->setupUi(this);
    connect(YF::self(),SIGNAL(replyjsonlist(QJsonArray)),this,SLOT(replyjsonArray(QJsonArray)));
    YF::post("queryDishes");
    ui->toptiplabel->setText(tr("正在下载菜单数据........."));
    initview();
    if(!this->isMaximized())this->setWindowState(Qt::WindowMaximized);
    downloadthread=new DownloadDishesImages;

}

MainWindow::~MainWindow()
{
    delete ui;
}
//初始化视图
void MainWindow::initview()
{
   dishesModel=new QStandardItemModel(this);
   orderModel=new QStandardItemModel(this);
   viewtype=scaleview;
   connect(ui->tableView_2->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(resizeGridClomn(int,int,int)));
   ui->tableView->setModel(dishesModel);
   ui->tableView_2->setModel(orderModel);
   ui->tableView->setObjectName("dishestableview");
   ui->tableView_2->setObjectName("ordertableview");
   m_colmncount=5;
   m_colmnwidth=110;
   m_colmnheight=110;
   orderModel->setColumnCount(5);
   QStringList titlelabels;
   titlelabels<<"项目"<<"单价"<<"数量"<<"金额"<<"ID";
   orderModel->setHorizontalHeaderLabels(titlelabels);
   ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
   for(int i=0;i<3;i++){
        int t_width=YF::getsettingvalue(ui->tableView_2->objectName()+"/colmnswidth_"+i).toInt();
        ui->tableView_2->setColumnWidth(i,t_width);
   }
   ui->tableView_2->setColumnHidden(4,true);
   ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableView->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setItemDelegate(&disheimage);

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
        dd=orderModel->findItems(name,Qt::MatchExactly,0);
        return dd.count()>0?dd.at(0)->row():-1;
    }
    return -1;
}

void MainWindow::increasedishes(QJsonObject dishesobject)
{
    QString id = QString::number(dishesobject.value("id").toInt());
    int r_index= findordered(id);
    if(r_index>=0){
        //计算数量
        double count=orderModel->index(r_index,2).data().toDouble()+1;
        orderModel->setData(orderModel->index(r_index,2),count);
        //计算金额
        count=orderModel->index(r_index,3).data().toDouble()+dishesobject.value("bprice").toDouble();
        orderModel->setData(orderModel->index(r_index,3),count);
    }else{
        //首次添加，价格和单价都为bprice，应该从远程数据库再添加
        r_index=0;
        orderModel->insertRow(0);
        orderModel->setData(orderModel->index(0,0),dishesobject.value("name"));
        orderModel->setData(orderModel->index(0,1),dishesobject.value("bprice").toDouble());
        orderModel->setData(orderModel->index(0,2),1);
        orderModel->setData(orderModel->index(0,3),dishesobject.value("bprice").toDouble());
        orderModel->setData(orderModel->index(0,4),id);
    }
    changedishedcheck(id,orderModel->index(r_index,2).data().toInt());
}
void MainWindow::decreasedishes(QJsonObject dishesobject,int num)
{
   QString id = QString::number(dishesobject.value("id").toInt());
   int r_index= findordered(id);
   int count=0;
   if(r_index>=0){
       //计算数量
       count=orderModel->index(r_index,2).data().toDouble()-num;
       if(count<=0){
          orderModel->removeRow(r_index);
       }else{
           orderModel->setData(orderModel->index(r_index,2),count);
           //计算金额
           double sum=orderModel->index(r_index,3).data().toDouble()-dishesobject.value("bprice").toDouble();
           orderModel->setData(orderModel->index(r_index,3),sum);
       }
   }
   changedishedcheck(id,count);
}

QJsonObject MainWindow::fetchDishesID(QModelIndex index){
    if(viewtype==scaleview||viewtype==wordbutton){
        QVariant tempdata=index.data();
        return tempdata.toJsonObject();
    }
    else{
        return dishesModel->index(index.row(),8).data().toJsonObject();
    }
}

void MainWindow::changedishedcheck(QString id, int num)
{
    int t_r=0;
    int t_c=0;
    bool finded=false;
    QJsonObject dishesobj;
    if(viewtype==scaleview||viewtype==wordbutton){
        for(;t_r<dishesModel->rowCount();t_r++){
            for(;t_c<dishesModel->columnCount();t_c++){
                dishesobj=dishesModel->index(t_r,t_c).data().toJsonObject();
                QString t_id=QString::number(dishesobj.value("id").toInt());
                if(t_id==id){
                    finded=true;
                    break;
                }
            }
            if(finded)break;
        }
        if(!finded)return;
        ui->tableView->setCurrentIndex(dishesModel->index(t_r,t_c));
        //ui->tableView->setFocus();
        if(num>0){
           dishesobj.insert("checkednum",num);
           dishesobj.insert("selected",true);
           dishesModel->setData(dishesModel->index(t_r,t_c),dishesobj);
        }else{
            dishesobj.insert("checkednum",0);
            dishesobj.insert("selected",false);
            dishesModel->setData(dishesModel->index(t_r,t_c),dishesobj);
        }
    }
    else if(viewtype==listtable){
        for(t_r=0;t_r<dishesModel->rowCount();t_r++){
            dishesobj=dishesModel->index(t_r,8).data().toJsonObject();
            QString t_id=QString::number(dishesobj.value("id").toInt());
            if(t_id==id){
                finded=true;
                break;
            }
        }
        if(!finded)return;
        dishesModel->setData(dishesModel->index(t_r,5),num);
    }
}

void MainWindow::switchViewtype(int newviewtype)
{
    ui->toptiplabel->setText(tr("正在切换显示模式........"));
    if(viewtype==newviewtype)return;
    dishesModel->clear();
    if(newviewtype==-1)newviewtype=viewtype;
    if(newviewtype==scaleview||newviewtype==wordbutton){
        ui->tableView->verticalHeader()->setVisible(false);
        ui->tableView->horizontalHeader()->setVisible(false);
        ui->tableView->setShowGrid(false);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->horizontalHeader()->setStretchLastSection(false);
        disconnect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(resizeGridClomn(int,int,int)));
        dishesModel->setColumnCount(m_colmncount);
        int cindex=0;
        int rindex=-1;
        for(int i=0;i<dishesJsonListData.size();++i){
            if(cindex==0){
               rindex++;
               dishesModel->insertRow(rindex);
               ui->tableView->setRowHeight(rindex,m_colmnheight);
            }
            ui->tableView->setColumnWidth(cindex,m_colmnwidth);
            QJsonObject jsonobj=dishesJsonListData.at(i).toObject();
            jsonobj.insert("viewtype",newviewtype);
            //qDebug()<<"是否已经下订单:"<<findordered(QString::number(jsonobj.value("ID").toInt()));
            int t_orderindex=findordered(QString::number(jsonobj.value("id").toInt()));
            if(t_orderindex>=0){
                jsonobj.insert("selected",true);
                jsonobj.insert("checkednum",orderModel->index(t_orderindex,2).data().toInt());
            }
            dishesModel->setData(dishesModel->index(rindex,cindex),jsonobj);
            cindex=cindex>=m_colmncount-1?0:cindex+1;
        }
    }else{
        ui->tableView->setShowGrid(true);
        ui->tableView->verticalHeader()->setVisible(true);
        ui->tableView->horizontalHeader()->setVisible(true);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        connect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(resizeGridClomn(int,int,int)));
        dishesModel->setColumnCount(9);
        QStringList titlelables;
        titlelables<<"快捷码"<<"分类"<<"类别"<<"菜品商品"<<"价格"<<"已点单"<<"供应量"<<"备注信息";
        dishesModel->setHorizontalHeaderLabels(titlelables);
        for(int i=0;i<8;i++){
            int t_width=YF::getsettingvalue(ui->tableView->objectName()+"/colmnswidth_"+i).toInt();
            ui->tableView->setColumnWidth(i,t_width);
        }
        ui->tableView->setColumnHidden(8,true);
        //解析数据
        for(int i=0;i<dishesJsonListData.size();++i){
            QJsonObject jsonobj=dishesJsonListData.at(i).toObject();
            int t_orderindex=findordered(QString::number(jsonobj.value("id").toInt()));
            if(t_orderindex>=0){
                jsonobj.insert("selected",true);
                jsonobj.insert("checkednum",orderModel->index(t_orderindex,2).data().toInt());
            }
            dishesModel->insertRow(i);
            dishesModel->setData(dishesModel->index(i,0),jsonobj.value("barcode").toVariant());
            dishesModel->setData(dishesModel->index(i,1),jsonobj.value("sort").toVariant());
            dishesModel->setData(dishesModel->index(i,2),jsonobj.value("classname").toVariant());
            dishesModel->setData(dishesModel->index(i,3),jsonobj.value("name").toVariant());
            dishesModel->setData(dishesModel->index(i,4),jsonobj.value("bprice").toVariant());
            dishesModel->setData(dishesModel->index(i,5),jsonobj.value("checkednum").toVariant());
            dishesModel->setData(dishesModel->index(i,6),jsonobj.value("supply").toVariant());
            dishesModel->setData(dishesModel->index(i,7),jsonobj.value("desc").toVariant());
            dishesModel->setData(dishesModel->index(i,8),jsonobj);
        }
    }
    viewtype=newviewtype;
    //QTimer::singleShot(1,this,&ui->toptiplabel->setText("fdsfdsa"));
    //QThread::sleep(1);
    ui->toptiplabel->setText("视图切换完成!!!");
}
void MainWindow::replyjsonArray(QJsonArray jsonlist)
{
    ui->refreshbtn->setText("更新同步");
    ui->refreshbtn->setEnabled(true);
    ui->toptiplabel->setText(tr("菜单数据下载完成!!!"));
    dishesJsonListData=jsonlist;
    switchViewtype(-1);


}
void MainWindow::on_refreshbtn_clicked()
{
    ui->refreshbtn->setText("更新...");
    ui->refreshbtn->setEnabled(false);
    YF::post("queryDishes");
    ui->toptiplabel->setText(tr("正在下载菜单数据........."));
}

void MainWindow::on_comboBox_activated(int index)
{
   switchViewtype(index);
}
//表格项目双击事件
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    increasedishes(fetchDishesID(index));
}


void MainWindow::resizeGridClomn(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    YF::setsetting(sender()->parent()->objectName()+"/colmnswidth_"+logicalIndex,newSize);
}

void MainWindow::showNotification(QString text, int msgtype)
{
    if(msgtype==0)
    ui->toptiplabel->setText(text);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!event->modifiers().testFlag(Qt::ControlModifier))return;
    if(focusWidget()==ui->tableView_2&&ui->tableView_2->currentIndex().row()>=0){
        int t_index=ui->tableView_2->currentIndex().row();
        int t_count=orderModel->index(t_index,2).data().toInt();
        double t_total=orderModel->index(t_index,3).data().toDouble();
        if(event->key()==Qt::Key_Plus){
                orderModel->setData(orderModel->index(t_index,2),++t_count);
                t_total+=orderModel->index(t_index,1).data().toDouble();
                orderModel->setData(orderModel->index(t_index,3),t_total);
        }
        else if(event->key()==Qt::Key_Minus){
            if(t_count<=1){
                changedishedcheck(orderModel->index(t_index,4).data().toString(),0);
                orderModel->removeRow(t_index);
                ui->tableView_2->setCurrentIndex(orderModel->index(orderModel->rowCount()-1,0));
            }else{
                orderModel->setData(orderModel->index(t_index,2),--t_count);
                t_total-=orderModel->index(t_index,1).data().toDouble();
                orderModel->setData(orderModel->index(t_index,3),t_total);
                changedishedcheck(orderModel->index(t_index,4).data().toString(),orderModel->index(t_index,2).data().toInt());
            }
        }
    }
    if(focusWidget()==ui->tableView&&ui->tableView->currentIndex().row()>=0){
        if(event->key()==Qt::Key_Plus){
            on_tableView_doubleClicked(ui->tableView->currentIndex());
        }
        else if(event->key()==Qt::Key_Minus)
        {
             decreasedishes(fetchDishesID(ui->tableView->currentIndex()));
        }

    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
   QRect mrect=this->rect();
   QRect rect=ui->toptiplabel->rect();
   rect.setRight(mrect.right());
   ui->toptiplabel->setGeometry(rect);
   rect=ui->createbtn->rect();
   ui->createbtn->move(mrect.right()-ui->createbtn->width(),ui->createbtn->y());
   ui->querybtn->move(mrect.right()-ui->querybtn->width(),ui->querybtn->y());
   ui->orderIDlabel->move(mrect.right()-ui->orderIDlabel->width(),ui->orderIDlabel->y());
   ui->settingbtn->move(mrect.right()-ui->settingbtn->width(),ui->settingbtn->y());
   ui->settlebtn->move(mrect.right()-ui->settlebtn->width(),mrect.bottom()-ui->settlebtn->height());
   rect=ui->tableView_2->rect();
   rect.setLeft(ui->createbtn->x());
   rect.setRight(mrect.right());
   rect.setTop(ui->orderIDlabel->y()+ui->orderIDlabel->height()+1);
   rect.setBottom(ui->settlebtn->y()-1);
   ui->tableView_2->setGeometry(rect);
   ui->funframe->resize(ui->createbtn->x()-1,ui->funframe->height());
   ui->tableView->resize(ui->createbtn->x()-1,mrect.height()-ui->tableView->y()-1);
   ui->comboBox->move(ui->funframe->width()-ui->comboBox->width()-1,ui->comboBox->y());
   ui->refreshbtn->move(ui->comboBox->x()-ui->refreshbtn->width()-1,ui->refreshbtn->y());
}

void MainWindow::on_settingbtn_clicked()
{
//    QUiLoader uiloader;
//    qDebug()<<YF::get("http://www.baidu.com",false);
    //downloadthread->start();
    DownloadDishesImages downimgage;
    downimgage.start();
}




void DownloadDishesImages::run()
{
   qDebug()<<"开始执行线程";
   qDebug()<<YF::get("http://blog.sina.com.cn/s/blog_4edd2a190100tcrf.html",false);
}
