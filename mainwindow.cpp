#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include "YF/YFTools.h"
#include <QCompleter>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    this->setGeometry(-1300,0,500,500);
    this->setWindowTitle("智能餐饮管理系统");
    this->setObjectName("MainWindow");
    this->setAutoFillBackground(true);
    QColor color(66,90,82);
    this->setPalette(QPalette(color));
    ui->setupUi(this);
    this->setVisible(true);
    //YF::post("queryDishes");
    ui->toptiplabel->setText(tr("正在初始化启动参数........."));
    if(!this->isMaximized())this->setWindowState(Qt::WindowMaximized);
    //多线程创建和定义
    m_task=new YFTask(ui);
    m_task->moveToThread(&m_thread);
    m_thread.start();
    //信号和槽连接<<network<<下载图片<<更新提示信息;
    connect(YF::self(),SIGNAL(replyjsonlist(QJsonArray)),this,SLOT(replyjsonArray(QJsonArray)));
    connect(this,SIGNAL(downloadimage(QUrl)),m_task,SLOT(httpdownload(QUrl)),Qt::QueuedConnection);
    connect(this,SIGNAL(updatetoptipmessage(QString,int)),m_task,SLOT(showNotification(QString,int)),Qt::QueuedConnection);
    connect(ui->createbtn,SIGNAL(clicked()),this,SLOT(createtempsales()));    
    connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(searchdishes(QString)));
    connect(ui->label,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customContext(QPoint)));
    connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(searchfinished()));
      connect(ui->orderIDCombobox,SIGNAL(activated(int)) ,this,SLOT(orderIDchange(int)));
    ui->lineEdit->installEventFilter(this);
    ui->label->setContextMenuPolicy(Qt::CustomContextMenu);
    //延时调用初始画面
    QTimer::singleShot(100,this,SLOT(initview()));
}
MainWindow::~MainWindow()
{
    delete ui;
    m_thread.quit();
    m_thread.wait();
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
   if(YF::settingvalue("dishesmodel_RowHeight").toString().isEmpty())
       YF::setsetting("dishesmodel_RowHeight",110);
   if(YF::settingvalue("dishesmodel_columncount").toString().isEmpty())
       YF::setsetting("dishesmodel_columncount",8);
   if(YF::settingvalue("dishesmodel_Rowwidth").toString().isEmpty())
       YF::setsetting("dishesmodel_Rowwidth",110);
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
   //设置联想词
   dishessearchModel=new QSqlQueryModel;
   ui->lineEdit->setCompleter(new QCompleter);
   ui->lineEdit->completer()->setModel(dishessearchModel);
   ui->lineEdit->completer()->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
   QTableView *dispalysearchview=new QTableView;
   dispalysearchview->horizontalHeader()->setHidden(true);
   dispalysearchview->setModel(dishessearchModel);
   dispalysearchview->resizeRowsToContents();
   dispalysearchview->resizeColumnsToContents();
   dispalysearchview->setMinimumHeight(150);
   dispalysearchview->verticalHeader()->hide();
   ui->lineEdit->completer()->setPopup(dispalysearchview);
   //设置自定义菜单
   ordermenu=new QMenu(ui->label);
   deleteAction=new QAction(tr("删除此订单"),ordermenu);
   copyAction=new QAction(tr("复制此订单"),ordermenu);
   ordermenu->insertAction(0,deleteAction);
   ordermenu->insertAction(0,copyAction);
   connect(deleteAction,SIGNAL(triggered()),this,SLOT(actionhandle()));
   connect(copyAction,SIGNAL(triggered()),this,SLOT(actionhandle()));
}
/**
 * @brief 创建临时消费订单
 */
void MainWindow::createtempsales()
{
    loadcacheorder();
    orderindex++;
    QDateTime nowtime=QDateTime::currentDateTime();
    QString salesid=nowtime.toString("yyMMddhhmmss");
    salesid="8"+salesid.right(salesid.length()-1)+"5"+QString::number(orderindex);
    QString dispalytext=QString::number(orderindex)+"  "+nowtime.toString("hh:mm:ss");
    ui->orderIDCombobox->insertItem(0,dispalytext);
    ui->orderIDCombobox->setItemData(0,salesid,Qt::DecorationRole);
    ui->orderIDCombobox->setCurrentText(dispalytext);
    ui->orderIDCombobox->setDuplicatesEnabled(false);
    orderModel->setFilter("sid='"+salesid+"'");
    orderModel->select();
    switchViewtype(-viewtype);
    countordertotal();
}
/**
 * @brief 订单号发生改变重新加载菜单数据
 * @param index
 */
void MainWindow::orderIDchange(int index)
{
    QString salesid = ui->orderIDCombobox->itemData(index,Qt::DecorationRole).toString();
    //如果是模板选择项,生成一个新的订单
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
    countordertotal();
    switchViewtype(-viewtype);
}
/**
 * @brief 从本地数据库加载数据并存到本地文件中
 */
void MainWindow::loadDishesLocalData(QString sqltext,bool isapppend)
{
    QSqlQuery query;
    if(sqltext.isEmpty()){
      query.exec("select * from t_dishes");
    }else{
      query.exec(sqltext);
    }
    loadDishesLocalData(query,isapppend);
}

void MainWindow::loadDishesLocalData(QSqlQuery query, bool isapppend)
{
    QJsonArray dishesJsonArrayData;
    QSqlRecord record=query.record();
    while(query.next()){
        QJsonObject jsonobj;
        for(int i=0;i<record.count();i++){
            jsonobj.insert(record.fieldName(i),query.value(i).toString());
        }
        if(isapppend){
           this->dishesJsonListData.append(jsonobj);
        }else{
           dishesJsonArrayData.append(jsonobj);
        }

    }
    if(!isapppend){
       dishesJsonListData=dishesJsonArrayData;
    }
    emit updatetoptipmessage("数据加载完成");
}
/**
 * @brief 处理临存和上次的订单以及加载模板
 */
void MainWindow::loadcacheorder(QString orderid)
{
    ui->orderIDCombobox->clear();
    orderindex=0;
    QString datestr=QDateTime::currentDateTime().addDays(-1).toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query;
    query.exec("delete from t_order where cd<='"+datestr+"'");
    query.exec("select name from t_template group by name");
    while(query.next()){
        ui->orderIDCombobox->insertItem(0,query.value("name").toString());
        ui->orderIDCombobox->setItemData(0,"ordertemplate",Qt::DecorationRole);
    }
    orderActions.clear();
    query.exec("select sid from t_order group by sid");
    while(query.next()){
        QString siddata;
        QString IDstr=siddata=query.value("sid").toString();
        int t_index=IDstr.mid(13,IDstr.length()-13).toInt();
        if(t_index>orderindex)orderindex=t_index;
        IDstr=QString::number(t_index)+"  "+IDstr.mid(6,2)+":"+IDstr.mid(8,2)+":"+IDstr.mid(10,2);
        ui->orderIDCombobox->insertItem(0,IDstr);
        ui->orderIDCombobox->setItemData(0,query.value("sid"),Qt::DecorationRole);       
    }
    if(!orderid.isEmpty()){
        int t_index=ui->orderIDCombobox->findData(orderid,Qt::DecorationRole);
        ui->orderIDCombobox->setCurrentIndex(t_index);
        emit ui->orderIDCombobox->activated(t_index);
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
    int newcount=addnum;
    //qDebug()<<"寻找已经点菜品"<<r_index;
    if(r_index>=0){
        //计算数量
        newcount=orderModel->index(r_index,2).data().toInt()+addnum;
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
    countordertotal();
    if(dishesobject==fetchDishesID()){
        changedishedcheck(ui->tableView->currentIndex(),newcount);
        return;
    }
    if(autoupdatecheck)changedishedcheck(id,newcount);
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
//    qDebug()<<"减少菜品数量"<<r_index;
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
    countordertotal();
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
    if(index.row()==-1)index=ui->tableView->currentIndex();
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
    QModelIndex dishesmodelindex=finddishes("id",id);
    if(dishesmodelindex.row()<0)return;
    changedishedcheck(dishesmodelindex,num);
}
/**
 * @brief MainWindow::changedishedcheck
 * @param index dishesmodel的index
 * @param changenum 改变量
 * @return 无返回值
 */
void MainWindow::changedishedcheck(QModelIndex index, int num)
{

    if(viewtype==scaleview||viewtype==wordbutton){
        QJsonObject jsonobj=index.data().toJsonObject();
        if(num>0){
            jsonobj.insert("checkednum",num);
            jsonobj.insert("selected",true);
        }else{
            jsonobj.insert("checkednum",0);
            jsonobj.insert("selected",false);
        }
        dishesModel->setData(index,jsonobj);
    }else{
        int rowindex=index.row();
        if(num>0){
            dishesModel->setData(dishesModel->index(rowindex,5),num);
        }else{
            dishesModel->setData(dishesModel->index(rowindex,5),0);
        }
    }
}
//切换回显菜单
void MainWindow::switchViewtype(int newviewtype)
{
    emit updatetoptipmessage(tr("正在切换显示模式........"));
    if(viewtype==newviewtype)return;
    dishesModel->clear();
    //if(newviewtype==-100)newviewtype=viewtype;
    int colmncount=YF::settingvalue("dishesmodel_columncount").toInt();
    int rowheight=YF::settingvalue("dishesmodel_RowHeight").toInt();
    int rowwidth=YF::settingvalue("dishesmodel_Rowwidth").toInt();
    //修正单元视图
    int tableviewwidth=ui->tableView->width();
    int unitcont=tableviewwidth/rowwidth;
    colmncount=unitcont;
    //滚动条的20px需要计算
    rowwidth=(tableviewwidth-20)/unitcont;
    //加载视图部分
    if(newviewtype<0)newviewtype=qAbs(newviewtype);
    if(newviewtype==scaleview||newviewtype==wordbutton){
        ui->tableView->verticalHeader()->setVisible(false);
        ui->tableView->horizontalHeader()->setVisible(false);
        ui->tableView->setShowGrid(false);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->horizontalHeader()->setStretchLastSection(false);
        //ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        disconnect(ui->tableView->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(resizeGridClomn(int,int,int)));
        dishesModel->setColumnCount(colmncount);
        int cindex=0;
        int rindex=-1;
        int quickindex=0;
        for(int i=0;i<dishesJsonListData.size();++i){
            if(cindex==0){
               rindex++;
               dishesModel->insertRow(rindex);
               ui->tableView->setRowHeight(rindex,rowheight);
            }
            ui->tableView->setColumnWidth(cindex,rowwidth);
            QJsonObject jsonobj=dishesJsonListData.at(i).toObject();
            jsonobj.insert("viewtype",newviewtype);
            //qDebug()<<"是否已经下订单:"<<findordered(QString::number(jsonobj.value("ID").toInt()));
            int t_orderindex=findordered(jsonobj.value("id").toVariant().toInt());
            if(t_orderindex>=0){
                jsonobj.insert("selected",true);
                jsonobj.insert("checkednum",orderModel->index(t_orderindex,2).data().toInt());
            }
            if(quickindex<9)jsonobj.insert("quick",++quickindex);
            dishesModel->setData(dishesModel->index(rindex,cindex),jsonobj);
            cindex=cindex>=colmncount-1?0:cindex+1;
        }
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int keyindex=event->key();
    //不带修饰器的
    if(focusWidget()==ui->tableView&&ui->tableView->currentIndex().row()==0&&keyindex==Qt::Key_Up){
        ui->lineEdit->setFocus();
        return;
    }
    if(focusWidget()==ui->tableView&&ui->tableView->currentIndex().column()==dishesModel->columnCount()-1&&keyindex==Qt::Key_Right){
        ui->tableView_2->setFocus();
        return;
    }
    if(focusWidget()==ui->tableView_2&&keyindex==Qt::Key_Left){
        ui->tableView->setFocus();
        return;
    }

    //携带修饰器的处理
    if(!event->modifiers().testFlag(Qt::ControlModifier))return;
    //快捷键部分
    if(keyindex==Qt::Key_D){
        ui->tableView->setFocus();
        return;
    }
    if(keyindex==Qt::Key_G){
        ui->tableView->setFocus();
        return;
    }

    if(keyindex==Qt::Key_F){
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
    if(keyindex==Qt::Key_P){
        //打印结算
        return;
    }
    if(keyindex==Qt::Key_O){
        //设置与功能说明
       return;
    }
    if(keyindex==Qt::Key_Q){
        //查询功能
        return;
    }
    if(keyindex==Qt::Key_N){
        createtempsales();
        return;
    }
    //快速点菜切换处理
    if(keyindex==Qt::Key_Enter||keyindex==Qt::Key_Return){
        if(focusWidget()==ui->lineEdit){
            ui->tableView->setFocus();
        }
        if(focusWidget()==ui->tableView){
            increasedishes(fetchDishesID(ui->tableView->currentIndex()));
            ui->lineEdit->clear();
            ui->lineEdit->setFocus();
        }
        return;
    }
    //操作点菜功能
    if(focusWidget()==ui->tableView_2&&ui->tableView_2->currentIndex().row()>=0){
        int t_index=ui->tableView_2->currentIndex().row();
        if(keyindex==Qt::Key_Left){
            ui->tableView->setFocus();
            return;
        }
        ui->tableView_2->selectRow(t_index);
        if(event->key()==Qt::Key_Plus){
            QModelIndex modelindex=finddishes("id",orderModel->index(t_index,4).data());
            increasedishes(fetchDishesID(modelindex),1);
        }
        else if(event->key()==Qt::Key_Minus){
            decreasedishes(t_index,1,true);
            ui->tableView_2->setCurrentIndex(orderModel->index(orderModel->rowCount()-1,0));
        }
    }
    if(focusWidget()==ui->tableView&&ui->tableView->currentIndex().row()>=0){
        if(keyindex==Qt::Key_Plus){
            on_tableView_doubleClicked(ui->tableView->currentIndex());
        }
        else if(keyindex==Qt::Key_Minus)
        {
             decreasedishes(fetchDishesID(ui->tableView->currentIndex()));
        }
        else if(keyindex>48&&keyindex<=57){
            QModelIndex dishesmodelindex=finddishes("quick",keyindex-48);
            if(dishesmodelindex.row()>-1){
                increasedishes(fetchDishesID(dishesmodelindex));
            }
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
   rect=ui->totallable->rect();
   rect.setLeft(ui->createbtn->x());
   rect.setRight(mrect.right());
   rect.setTop(ui->settlebtn->y()-rect.height()-1);
   rect.setBottom(ui->settlebtn->y()-1);
   ui->totallable->setGeometry(rect);
   rect=ui->tableView_2->rect();
   rect.setLeft(ui->createbtn->x());
   rect.setRight(mrect.right());
   rect.setTop(ui->orderIDlabel->y()+ui->orderIDlabel->height()+1);
   rect.setBottom(ui->totallable->y()-1);
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

YFTask::YFTask(Ui::MainWindow *t_ui)
{
    YFTask();
    this->ui=t_ui;
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

void YFTask::showNotification(QString text, int msgtimes)
{
    static QMutex notificationmutex;
    notificationmutex.lock();
    QSqlQuery query;
    if(!text.isEmpty()){
        ui->toptiplabel->setText("["+YF_getdatetime()+"]:"+text);
        QThread::sleep(1);
        //toptiplable->repaint();
        //2秒钟重新调用通知,读取数据库的提示通知.
        if(msgtimes>0){
            query.prepare("insert into toptip (type,content,fromer,lasttime,times) values (?,?,?,?,?)");
            query.bindValue(0,0);
            query.bindValue(1,text);
            query.bindValue(2,"系统");
            query.bindValue(3,YF_getdatetime(QDateTime::currentDateTime(),1));
            query.bindValue(4,msgtimes);
            query.exec();
        }
        QTimer::singleShot(3000,this,SLOT(showNotification()));
        notificationmutex.unlock();
        return;
    }
    QString nowstr=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    query.exec("select * from toptip where times>0 and lasttime>='"+nowstr+"' order by id desc limit 0,1");
    if(query.next()){
        QString messagetext=query.value("content").toString();
        ui->toptiplabel->setText(messagetext);
        QString id=query.value("id").toString();
        qDebug()<<"查到值,数据:"<<messagetext<<id<<query.value("times")<<query.value("lasttime");
        query.exec("update toptip set times=times-1 where id="+id);
        query.exec("delte from toptip where type=0 and times=0");
        QTimer::singleShot(5000,this,SLOT(showNotification()));
    }
    query.clear();
    notificationmutex.unlock();
}

void MainWindow::on_pushButton_clicked()
{
    //qDebug()<<(QVariant(1)==QVariant("1.0"));
}

void MainWindow::searchdishes(QString text)
{
    if(text.isEmpty())return;
    QString sqltext;
    QByteArray bytes=text.left(1).toLocal8Bit();
    if(bytes.count()<2){
        QChar qch(bytes.at(0));
        if(qch.isNumber()){
            sqltext="select barcode,name,id from t_dishes where barcode like '%"+text+"%'";
        }
        if(qch.isLetter()){
            sqltext="select pinyin,name,id from t_dishes where pinyin like '%"+text+"%'";
        }
    }else{
        sqltext="select name,id from t_dishes where name like '%"+text+"%'";
    }
    dishessearchModel->setQuery(sqltext);
    int cls=dishessearchModel->columnCount();
    //qDebug()<<dishessearchModel->query().value("name");
    QFont font;
    font.setBold(true);
    font.setPixelSize(20);
    font.setFamily("宋体");
    QTableView *t_tableviw=qobject_cast<QTableView*>(ui->lineEdit->completer()->popup());
    t_tableviw->setFont(font);
    t_tableviw->resizeRowsToContents();
    t_tableviw->resizeColumnsToContents();
    t_tableviw->horizontalHeader()->setStretchLastSection(true);
    t_tableviw->setWordWrap(false);
    if(cls-2>0)t_tableviw->setColumnHidden(cls-2,false);
    if(cls-1>0)t_tableviw->setColumnHidden(cls-1,true);
}
/**
 * @brief 这是自定义菜单
 * @param pos
 */
void MainWindow::customContext(QPoint )
{
    QSqlQuery query;
    if(ui->label==sender()&&orderModel->rowCount()>0){
        QPoint lablelglobalpoint=ui->label->mapToGlobal(ui->label->pos());
        lablelglobalpoint.ry()+=ui->label->height();
        QString sid=ui->orderIDCombobox->currentData(Qt::DecorationRole).toString();
        QMenu *movemenu=0;
         QMenu *returmenu=0;
        query.exec("select sid from t_order where sid<>'"+sid+"' group by sid");

        if(query.next()){
           movemenu=new QMenu(tr("转移此订单"),this);
           ordermenu->addMenu(movemenu);
           do {
               QString IDstr=query.value("sid").toString();
               int t_index=IDstr.mid(13,IDstr.length()-13).toInt();
               QString actiontitle=QString::number(t_index)+"  "+IDstr.mid(6,2)+":"+IDstr.mid(8,2)+":"+IDstr.mid(10,2);
               QAction *t_action=new QAction(actiontitle,movemenu);
               t_action->setData(IDstr);
               t_action->setObjectName("join");
               movemenu->insertAction(0,t_action);
               connect(t_action,SIGNAL(triggered()),this,SLOT(actionhandle()));
           } while (query.next());
        }
        query.exec("select sw from t_order where sid='"+sid+"' and sw like 'move:%' group by sw");
        if(query.next()){
            returmenu=new QMenu(tr("分离此订单"),this);
            ordermenu->addMenu(returmenu);
            do {
                QString IDstr=query.value("sw").toString();
                IDstr=IDstr.replace("move:","");
                int t_index=IDstr.mid(13,IDstr.length()-13).toInt();
                QString actiontitle=QString::number(t_index)+"  "+IDstr.mid(6,2)+":"+IDstr.mid(8,2)+":"+IDstr.mid(10,2);
                QAction *t_action=new QAction(actiontitle,returmenu);
                t_action->setData(IDstr);
                t_action->setObjectName("split");
                returmenu->insertAction(0,t_action);
                connect(t_action,SIGNAL(triggered()),this,SLOT(actionhandle()));
            } while (query.next());
        }
        ordermenu->exec(lablelglobalpoint);
        if(returmenu!=0){
            returmenu->deleteLater();
        }
        if(movemenu!=0){
            movemenu->deleteLater();
        }

    }
}
void MainWindow::searchfinished()
{
    QString text=ui->lineEdit->text();
    if(text.isEmpty())return;
    int t_c=dishessearchModel->columnCount()-1;
    int t_r=ui->lineEdit->completer()->popup()->currentIndex().row();
    QString topid=dishessearchModel->index(t_r,t_c).data().toString();
//    qDebug()<<topid;
    if(topid.isEmpty())topid="-1";
    loadDishesLocalData("select * from t_dishes where id="+topid);
    QString ids;
    for(int i=0;i<dishessearchModel->rowCount();i++){
        QString thisid=dishessearchModel->index(i,t_c).data().toString();
        if(topid!=thisid)ids+=thisid+",";
    }
//    qDebug()<<ids.left(ids.length()-1);
    loadDishesLocalData("select * from t_dishes where id in ("+ids.left(ids.length()-1)+")",true);
    loadDishesLocalData("select * from t_dishes where id not in ("+ids+topid+")",true);
    switchViewtype(-viewtype);
    ui->tableView->setCurrentIndex(dishesModel->index(0,0));
}

QModelIndex MainWindow::finddishes(QString propertyname, QVariant value)
{
    if(propertyname.isEmpty())return QModelIndex();
    if(viewtype==scaleview||viewtype==wordbutton){
        int t_r=0;
        int t_c=0;
        for(t_r=0;t_r<dishesModel->rowCount();t_r++){
            for(t_c=0;t_c<dishesModel->columnCount();t_c++){
                 QJsonObject jsonobj=dishesModel->index(t_r,t_c).data().toJsonObject();
                // qDebug()<<t_r<<t_c<<jsonobj;
                 if(jsonobj.value(propertyname).toVariant()==value){
                     return dishesModel->index(t_r,t_c);
                 }
            }
        }
    }else{
        for(int i=0;i<dishesModel->rowCount();i++){
            QJsonObject jsonobj=dishesModel->index(i,8).data().toJsonObject();
            if(jsonobj.value(propertyname).toVariant()==value){
                return dishesModel->index(i,8);
            }
        }

    }
    return QModelIndex();
}

void MainWindow::actionhandle()
{
    QAction *t_action=qobject_cast<QAction*>(sender());
    QString t_title=t_action->text();
    QSqlQuery query;
    QString sid=ui->orderIDCombobox->currentData(Qt::DecorationRole).toString();
    if(t_title=="删除此订单"){
        query.exec("delete from t_order where sid='"+sid+"'");
        loadcacheorder();
        return;
    }else if(t_title=="复制此订单"){
        createtempsales();
        QString newsid=ui->orderIDCombobox->currentData(Qt::DecorationRole).toString();
        query.exec("insert into t_order(mz,dj,sl,je,did,cd,sw,sid)select mz,dj,sl,je,did,'"+YF_getdatetime()+"','normal','"+newsid+"'"+" from t_order where sid='"+sid+"'");
        loadcacheorder(newsid);
    }else{
        if(t_action->objectName()=="join"){
            if(t_action->data()==ui->orderIDCombobox->currentData(Qt::DecorationRole))return;
            QString newsid=t_action->data().toString();
            QString sqltext="update t_order set sid=?,sw=? where sid=?";
            query.prepare(sqltext);
            query.bindValue(0,newsid);
            query.bindValue(1,"move:"+sid);
            query.bindValue(2,sid);
            query.exec();
            loadcacheorder(newsid);
        }else if(t_action->objectName()=="split"){
            QString newid=t_action->data().toString();
            QString sqltext="update t_order set sid=?,sw=? where sid=? and sw=?";
            query.prepare(sqltext);
            query.bindValue(0,newid);
            query.bindValue(1,"back:"+sid);
            query.bindValue(2,sid);
            query.bindValue(3,"move:"+t_action->data().toString());
            query.exec();
            loadcacheorder(sid);
        }

    }


}

void MainWindow::countordertotal(QString sid)
{
    QSqlQuery query;
    if(sid.isEmpty()){
        sid=ui->orderIDCombobox->currentData(Qt::DecorationRole).toString();
    }
    query.exec("select sum(je) from t_order where sid='"+sid+"'");
    if(query.next()){
        ui->totallable->setText("总金额:"+QString::number(query.value(0).toDouble())+"元");
    }
}
bool MainWindow::eventFilter(QObject *target, QEvent *event){

    if(target==ui->lineEdit){
        if(event->type()==QEvent::FocusIn){
            QPalette p=QPalette();
            p.setColor(QPalette::Base,Qt::gray);
            ui->lineEdit->setPalette(p);
        }
        if (event->type()==QEvent::FocusOut)    // 这里指 lineEdit1 控件的失去焦点事件
        {
          QPalette p=QPalette();
          p.setColor(QPalette::Base,Qt::white);
          ui->lineEdit->setPalette(p);
        }

        if(event->type()==QEvent::KeyPress){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            int keyindex=keyEvent->key();
            if(keyEvent->modifiers().testFlag(Qt::ControlModifier)&&keyindex>48&&keyindex<=57){
                qDebug()<<"ctrl事件在lineedit";
                QModelIndex dishesmodelindex=finddishes("quick",keyindex-48);
                if(dishesmodelindex.row()>-1){
                    increasedishes(fetchDishesID(dishesmodelindex));
                    ui->lineEdit->clear();
                }
                return true;
           }
           if(ui->lineEdit->text().isEmpty()&&keyindex==Qt::Key_Right){
               ui->tableView_2->setFocus();
               return true;
           }
           if(ui->lineEdit->text().isEmpty()&&keyindex==Qt::Key_Down){
               ui->tableView->setFocus();
               return true;
           }
        }
    }
    return qApp->eventFilter(target,event);
}
