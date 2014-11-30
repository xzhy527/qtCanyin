#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include "YF/YFTools.h"
#include <QCompleter>
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    this->setGeometry(-1300,0,500,500);
    this->setWindowTitle("智能餐饮管理系统");
    this->setObjectName("MainWindow");
    this->setAutoFillBackground(true);
    QColor color(66,90,82);
    this->setPalette(QPalette(color));
    ui->setupUi(this);
    //YF::post("queryDishes");
    ui->toptiplabel->setText(tr("正在下载菜单数据........."));
    if(!this->isMaximized())this->setWindowState(Qt::WindowMaximized);
    //多线程创建和定义
    m_task=new YFTask;
    m_task->moveToThread(&m_thread);
    m_thread.start();
    //信号和槽连接<<network<<下载图片<<更新提示信息;
    connect(YF::self(),SIGNAL(replyjsonlist(QJsonArray)),this,SLOT(replyjsonArray(QJsonArray)));
    connect(this,SIGNAL(downloadimage(QUrl)),m_task,SLOT(httpdownload(QUrl)),Qt::QueuedConnection);
    connect(this,SIGNAL(updatetoptipmessage(QString,int)),this,SLOT(showNotification(QString,int)));
    connect(ui->createbtn,SIGNAL(clicked()),this,SLOT(createtempsales()));
    connect(ui->orderIDCombobox,SIGNAL(activated(int)),this,SLOT(orderIDchange(int)));
    connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(searchdishes(QString)));


    //延时调用初始画面
    QTimer::singleShot(100,this,SLOT(initview()));
}

MainWindow::~MainWindow()
{
    delete ui;
    m_thread.quit();
    m_thread.wait();
//    delete m_thread;
    m_task->deleteLater();


}
//初始化视图
void MainWindow::initview()
{
   qDebug()<<"初始化数据库"<<YF::initdatabaseconnect();
   dishesModel=new QStandardItemModel(this);
   orderModel=new QSqlTableModel(this);
   orderModel->setTable("t_order");
   orderModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
   viewtype=wordbutton;
   connect(ui->tableView_2->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(resizeGridClomn(int,int,int)));
   ui->tableView->setModel(dishesModel);
   ui->tableView_2->setModel(orderModel);
   ui->tableView->setObjectName("dishestableview");
   ui->tableView_2->setObjectName("ordertableview");
   m_colmncount=8;
   m_colmnwidth=110;
   m_colmnheight=110;
//  orderModel->setColumnCount(5);
//  QStringList titlelabels;
//  titlelabels<<"项目"<<"单价"<<"数量"<<"金额"<<"ID";
//  orderModel->setHorizontalHeaderLabels(titlelabels);
   orderModel->setHeaderData(0,Qt::Horizontal,"项目");
   orderModel->setHeaderData(1,Qt::Horizontal,"单价");
   orderModel->setHeaderData(2,Qt::Horizontal,"数量");
   orderModel->setHeaderData(3,Qt::Horizontal,"金额");
   ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
   ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
   for(int i=0;i<3;i++){
        int t_width=YF::settingvalue(ui->tableView_2->objectName()+"/colmnswidth_"+i).toInt();
        ui->tableView_2->setColumnWidth(i,t_width);
   }
   ui->tableView_2->setColumnHidden(4,true);
   ui->tableView_2->setColumnHidden(5,true);
   ui->tableView_2->setColumnHidden(6,true);
   ui->tableView_2->setColumnHidden(7,true);
   ui->tableView_2->setColumnHidden(8,true);
   ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableView->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setItemDelegate(&disheimage);
   loadDishesLocalData();
   createtempsales();
   dishessearchModel=new QSqlQueryModel;
   ui->lineEdit->setCompleter(new QCompleter);
   ui->lineEdit->completer()->setModel(dishessearchModel);
}
/**
 * @brief 创建临时消费订单
 */
void MainWindow::createtempsales()
{
    loadcacheorder();
    QDateTime nowtime=QDateTime::currentDateTime();
    QString salesid=nowtime.toString("yyMMddhhmmss");
    salesid="8"+salesid.right(salesid.length()-1)+"5"+QString::number(orderindex);
    QString dispalytext=QString::number(orderindex)+"  "+nowtime.toString("hh:mm:ss");
    ui->orderIDCombobox->insertItem(0,dispalytext);
    ui->orderIDCombobox->setItemData(0,salesid,Qt::DecorationRole);
    ui->orderIDCombobox->setCurrentText(dispalytext);
    ui->orderIDCombobox->setDuplicatesEnabled(false);
    //orderModel->removeRows(0,orderModel->rowCount());
    orderModel->setFilter("sid='"+salesid+"'");
    orderModel->select();
    switchViewtype(-viewtype);
}

void MainWindow::orderIDchange(int index)
{
    QString salesid = ui->orderIDCombobox->itemData(index,Qt::DecorationRole).toString();
    if(salesid=="ordertemplate"){
        salesid=ui->orderIDCombobox->itemData(index,Qt::EditRole).toString();
        QSqlQuery query("select * from t_template where name='"+salesid+"'");
        query.exec();
        if(query.next()){
           createtempsales();
           do {
               orderModel->insertRow(0);
               orderModel->setData(orderModel->index(0,0),query.value("dishesname"));
               orderModel->setData(orderModel->index(0,1),query.value("bprice"));
               orderModel->setData(orderModel->index(0,2),query.value("num"));
               orderModel->setData(orderModel->index(0,3),query.value("total"));
               orderModel->setData(orderModel->index(0,4),query.value("dishesid"));
               orderModel->setData(orderModel->index(0,5),ui->orderIDCombobox->currentData(Qt::DecorationRole));
               orderModel->setData(orderModel->index(0,6),YF_getdatetime());
               orderModel->setData(orderModel->index(0,7),"template");
               orderModel->submitAll();
           } while (query.next());
           switchViewtype(-viewtype);
        }
        return;
    }

    orderModel->setFilter("sid='"+salesid+"'");
    orderModel->select();
    switchViewtype(-viewtype);
}
/**
 * @brief 从本地数据库加载数据并存到本地文件中
 */
void MainWindow::loadDishesLocalData()
{
    QSqlQuery query("select * from t_dishes");
    query.exec();
    QJsonArray dishesJsonArrayData;
    QSqlRecord record=query.record();
    while(query.next()){
        QJsonObject jsonobj;
        for(int i=0;i<record.count();i++){
            jsonobj.insert(record.fieldName(i),query.value(i).toString());
        }
        dishesJsonArrayData.append(jsonobj);
    }

    dishesJsonListData=dishesJsonArrayData;
    switchViewtype(-viewtype);

}
/**
 * @brief 处理临存和上次的订单以及加载模板
 */
void MainWindow::loadcacheorder()
{
    ui->orderIDCombobox->clear();
    orderindex=1;
    QString datestr=QDateTime::currentDateTime().addDays(-1).toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query;
    query.exec("delete from t_order where cd<='"+datestr+"'");
    query.exec("select name from t_template group by name");
    while(query.next()){
        ui->orderIDCombobox->insertItem(0,query.value("name").toString());
        ui->orderIDCombobox->setItemData(0,"ordertemplate",Qt::DecorationRole);
    }
    query.exec("select sid from t_order group by sid");
    while(query.next()){
        QString IDstr=query.value("sid").toString();
        IDstr=QString::number(orderindex++)+"  "+IDstr.mid(6,2)+":"+IDstr.mid(8,2)+":"+IDstr.mid(10,2);
        ui->orderIDCombobox->insertItem(0,IDstr);
        ui->orderIDCombobox->setItemData(0,query.value("sid"),Qt::DecorationRole);
    }
}
/**
 * @brief 寻找已点订单,如果没有找到或ID和Name都为空的数据返回-1,ID和name选其一
 * @param id
 * @param name
 * @return 已点订单的ID
 */
int MainWindow::findordered(int id, QString name)
{
      if(orderModel->rowCount()<1)return -1;
      if(id<0&&name.isEmpty())return -1;
      bool re_ID=id<0?true:false;
      bool re_name=name.isEmpty()?true:false;
      int i=0;
      for(;i<orderModel->rowCount();i++){
         int orderid = orderModel->index(i,4).data().toInt();
         QString ordername = orderModel->index(i,0).data().toString();
         if(!re_ID&&orderid==id)re_ID=true;
         if(!re_name&&name==ordername)re_name=true;
         if(re_ID&&re_name) return i;
      }
      return -1;
}
/**
 * @brief 增加点菜数量
 * @param dishesobject
 * @param addnum 增量
 * @param autoupdatecheck 是否自动保存
 */
void MainWindow::increasedishes(QJsonObject dishesobject, int addnum, bool autoupdatecheck)
{
    int id = dishesobject.value("id").toVariant().toInt();
    int r_index= findordered(id,dishesobject.value("name").toString());
    //qDebug()<<"寻找已经点菜品"<<r_index;
    if(r_index>=0){
        //计算数量
        int newcount=orderModel->index(r_index,2).data().toInt()+addnum;
        orderModel->setData(orderModel->index(r_index,2),newcount);
        //计算金额
        double newmoney=dishesobject.value("bprice").toVariant().toDouble()*newcount;
        orderModel->setData(orderModel->index(r_index,3),newmoney);
        orderModel->submitAll();
        qDebug()<<"新数量:"<<newcount<<"新金额"<<newmoney;
    }else{
        //首次添加，价格和单价都为bprice，应该从远程数据库再添加
        r_index=0;
        orderModel->insertRow(0);
        orderModel->setData(orderModel->index(0,0),dishesobject.value("name"));
        orderModel->setData(orderModel->index(0,1),dishesobject.value("bprice").toVariant().toDouble());
        orderModel->setData(orderModel->index(0,2),addnum);
        orderModel->setData(orderModel->index(0,3),dishesobject.value("bprice").toVariant().toDouble()*addnum);
        orderModel->setData(orderModel->index(0,4),id);
        orderModel->setData(orderModel->index(0,5),ui->orderIDCombobox->currentData(Qt::DecorationRole));
        orderModel->setData(orderModel->index(0,6),YF_getdatetime());
        orderModel->setData(orderModel->index(0,7),"normal");
        orderModel->submitAll();
        if(orderModel->lastError().type()!= QSqlError::NoError){
            YF::popErrorMessage(this,orderModel->lastError().text());
        }

    }
    orderModel->select();
    if(autoupdatecheck)changedishedcheck(id,orderModel->index(r_index,2).data().toInt());
}
/**
 * @brief 删减菜品订单数量
 * @param dishesobject
 * @param num 变化量
 */
void MainWindow::decreasedishes(QJsonObject dishesobject,int num,bool autoupdatecheck)
{
   int id = dishesobject.value("id").toVariant().toInt();
   int r_index= findordered(id);
   decreasedishes(r_index,num,autoupdatecheck);
}
void MainWindow::decreasedishes(int r_index,int num,bool autoupdatecheck){
    if(r_index<0)return;
    int id=orderModel->index(r_index,4).data().toInt();
    int count=0;
    //计算数量
    count=orderModel->index(r_index,2).data().toInt()-num;
    if(count<=0){
       orderModel->removeRow(r_index);
    }else{
        orderModel->setData(orderModel->index(r_index,2),count);
        //计算金额
        double sum=orderModel->index(r_index,1).data().toDouble()*count;
        orderModel->setData(orderModel->index(r_index,3),sum);
    }
    orderModel->submitAll();
    orderModel->select();
    if(autoupdatecheck)changedishedcheck(id,count);
}

void MainWindow::insertNotification(QJsonObject json)
{
    Q_UNUSED(json);
}

/**
 * @brief 通过modelindex获取具体餐单数据
 * @param index
 * @return 当前model的josn数据
 */
QJsonObject MainWindow::fetchDishesID(QModelIndex index){
    if(viewtype==scaleview||viewtype==wordbutton){
        QVariant tempdata=index.data();
        return tempdata.toJsonObject();
    }
    else{
        return dishesModel->index(index.row(),8).data().toJsonObject();
    }
}
/**
 * @brief MainWindow::changedishedcheck
 * @param id 要改变dishesmode中数据的ID值
 * @param num 已点数量变到值
 */
void MainWindow::changedishedcheck(int id, int num)
{
    qDebug()<<"要改变图标的菜单ID"<<id<<num;
    int t_r=0;
    int t_c=0;
    bool finded=false;
    QJsonObject dishesobj;
    if(viewtype==scaleview||viewtype==wordbutton){
        for(t_r=0;t_r<dishesModel->rowCount();t_r++){
            for(t_c=0;t_c<dishesModel->columnCount();t_c++){
                dishesobj=dishesModel->index(t_r,t_c).data().toJsonObject();
                int t_id=dishesobj.value("id").toVariant().toInt();
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
           qDebug()<<"找到了:r"<<t_r<<"l" <<t_c;
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
            int t_id=dishesobj.value("id").toVariant().toInt();
            if(t_id==id){
                finded=true;
                break;
            }
        }
        if(!finded)return;
        dishesModel->setData(dishesModel->index(t_r,5),num);
    }
}
/**
 * @brief MainWindow::changedishedcheck
 * @param index dishesmodel的index
 * @param changenum 改变量
 * @return 无返回值
 */
void MainWindow::changedishedcheck(QModelIndex index, int changenum)
{
    if(viewtype==scaleview||viewtype==wordbutton){
        QJsonObject jsonobj=index.data().toJsonObject();
        int newchecknum=jsonobj.value("checkednum").toInt()+changenum;
        if(newchecknum>0){
            jsonobj.insert("checkednum",newchecknum);
            jsonobj.insert("selected",true);
        }else{
            jsonobj.insert("checkednum",0);
            jsonobj.insert("selected",false);
        }
        dishesModel->setData(index,jsonobj);
    }else{
        int rowindex=index.row();
        int newchecknum=dishesModel->index(rowindex,5).data().toInt()+changenum;
        if(newchecknum>0){
            dishesModel->setData(dishesModel->index(rowindex,5),newchecknum);
        }else{
            dishesModel->setData(dishesModel->index(rowindex,5),0);
        }
    }

}

void MainWindow::switchViewtype(int newviewtype)
{
    //QJsonArray dishesJsonListData=YF::settingvalue("dishesJsonListData").toJsonArray();
    ui->toptiplabel->setText(tr("正在切换显示模式........"));
    if(viewtype==newviewtype)return;
    dishesModel->clear();
    //if(newviewtype==-100)newviewtype=viewtype;
    if(newviewtype<0)newviewtype=qAbs(newviewtype);
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
            int t_orderindex=findordered(jsonobj.value("id").toVariant().toInt());
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
            int t_width=YF::settingvalue(ui->tableView->objectName()+"/colmnswidth_"+i).toInt();
            ui->tableView->setColumnWidth(i,t_width);
        }
        ui->tableView->setColumnHidden(8,true);
        //解析数据
        for(int i=0;i<dishesJsonListData.size();++i){
            QJsonObject jsonobj=dishesJsonListData.at(i).toObject();
            int t_orderindex=findordered(jsonobj.value("id").toVariant().toInt());
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
//    QJsonObject json;
//    json.insert("table","toptip");
//    json.insert("type",0);
//    json.insert("content",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+":这是一个测试内容");
//    json.insert("fromer","系统");
//    json.insert("lasttime","2014-11-28 05:10:10");
//    json.insert("times","10");
//     qDebug()<<YF::sql(json);
    emit updatetoptipmessage("视图更新完成!!!",0);
}
/**
 * @brief 网络数据到底更新本地资源
 * @param jsonlist
 */
void MainWindow::replyjsonArray(QJsonArray jsonlist)
{
    emit updatetoptipmessage("网络数据下载成功,正在解析数据........");
    YF::sql("delete from t_dishes");
    for(int i=0;i<jsonlist.count();i++){
        QJsonObject jsonobj=jsonlist.at(i).toObject();
        jsonobj.insert("table","t_dishes");
        QString t_pym=YF_getSpell(jsonobj.value("name").toVariant().toString());
        jsonobj.insert("genpy",t_pym);

        YF::sql(jsonobj);
    }
    ui->refreshbtn->setText("更新同步");
    ui->refreshbtn->setEnabled(true);
    ui->toptiplabel->setText(tr("菜单数据下载完成!!!"));
    dishesJsonListData=jsonlist;
    //下载菜单的图片数据
    emit updatetoptipmessage("更新菜品图片存储.......",0);
    foreach (QJsonValue t_json,jsonlist) {
        QString urlstring=t_json.toObject().value("picaddr").toString();
        if(!urlstring.isEmpty()){
            //qDebug()<<"发射的图片地址"<<urlstring;
            //发射更新本地图片数据
            //emit downloadimage(QUrl(urlstring));
        }
    }
    switchViewtype(-viewtype);
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
    increasedishes(fetchDishesID(index),1,false);
    changedishedcheck(index,1);
}
void MainWindow::resizeGridClomn(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    YF::setsetting(sender()->parent()->objectName()+"/colmnswidth_"+logicalIndex,newSize);
}

void MainWindow::showNotification(QString text, int msgtype)
{
    if(msgtype==0&&!text.isEmpty()){
        ui->toptiplabel->setText(text);
        //2秒钟重新调用通知,读取数据库的提示通知.
        QTimer::singleShot(3000,this,SLOT(showNotification()));
        return;
    }
    QSqlQuery query;
    QString nowstr=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//    qDebug()<<query.exec("select * from toptip");
    qDebug()<<query.exec("select * from toptip where times>0 and lasttime>='"+nowstr+"' order by id desc limit 0,1");
    if(query.next()){
        QString messagetext=query.value("content").toString();
        ui->toptiplabel->setText(messagetext);
        QString id=query.value("id").toString();
        qDebug()<<"查到值,数据:"<<messagetext<<id<<query.value("times")<<query.value("lasttime");
        query.exec("update toptip set times=times-1 where id="+id);
        QTimer::singleShot(5000,this,SLOT(showNotification()));
    }
    query.clear();



}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!event->modifiers().testFlag(Qt::ControlModifier))return;
    if(focusWidget()==ui->tableView_2&&ui->tableView_2->currentIndex().row()>=0){
        int t_index=ui->tableView_2->currentIndex().row();
        int t_count=orderModel->index(t_index,2).data().toInt();
        double t_price=orderModel->index(t_index,1).data().toDouble();
        ui->tableView_2->selectRow(t_index);
        if(event->key()==Qt::Key_Plus){
                //订单自加运算
                orderModel->setData(orderModel->index(t_index,2),++t_count);
                t_price=t_price*t_price;
                orderModel->setData(orderModel->index(t_index,3),t_price);
                orderModel->submit();
        }
        else if(event->key()==Qt::Key_Minus){
            //订单自减运算
            int dishesid=orderModel->index(t_index,4).data().toInt();
            if(t_count<=1){
                changedishedcheck(dishesid,0);
                orderModel->removeRow(t_index);
                orderModel->submitAll();
                orderModel->select();
                ui->tableView_2->setCurrentIndex(orderModel->index(orderModel->rowCount()-1,0));
            }else{
                orderModel->setData(orderModel->index(t_index,2),--t_count);
                t_price=t_price*t_count;
                orderModel->setData(orderModel->index(t_index,3),t_price);
                changedishedcheck(orderModel->index(t_index,4).data().toInt(),t_count);
                orderModel->submit();
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
    qDebug()<<YF_getSpell(ui->lineEdit->text());
}



YFTask::YFTask()
{
    QDir::current().mkpath("temp/images");
    network=new QNetworkAccessManager();
    connect(network,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));

}

void YFTask::httpdownload(QUrl url)
{
    mutex.lock();
    //qDebug()<<"开始下载的ID"<<QThread::currentThreadId()<<"文件名称:"<<url.fileName();
    network->get(QNetworkRequest(url));
    //qDebug()<<"http下载结束";
    mutex.unlock();
}

void YFTask::finished(QNetworkReply *repy)
{
   QString filename = repy->url().fileName();
   QFile file("temp/images/"+filename);
   file.open(QFile::WriteOnly|QFile::Truncate);
   file.write(repy->readAll());
   file.close();
   repy->deleteLater();
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQuery query;
    qDebug()<<"query地址:"<<&query;
    qDebug()<<"查询缓存订单:"<<YF::sql("select * from t_order",&query);
    qDebug()<<"query地址:"<<&query<<query.next()<<query.lastQuery();
    while(query.next())qDebug()<<"值:"<<query.value("id");
}

void MainWindow::searchdishes(QString text)
{
    qDebug()<<text;
    if(text.isEmpty())return;
    QString sqltext;
    QByteArray bytes=text.left(1).toLatin1();
    qDebug()<<bytes.count();
    QChar qch(bytes.at(0));
    if(qch.isNumber()){
        sqltext="select barcode from t_dishes where barcode like '%"+text+"%'";
    }
    if(qch.isLetter()){
        sqltext="select pinyin,name from t_dishes where pinyin like '%"+text+"%'";
    }

    dishessearchModel->setQuery(sqltext);
    qDebug()<<dishessearchModel->query().value("name");
    QFont font;
    font.setBold(true);
    font.setPixelSize(24);
    font.setFamily("宋体");
    ui->lineEdit->completer()->popup()->setFont(font);
    //ui->lineEdit->completer()->popup()->setCurrentIndex(dishessearchModel->index(0,0));
    ui->lineEdit->completer()->setMaxVisibleItems(20);
    ui->lineEdit->completer()->setCompletionColumn(0);
}
