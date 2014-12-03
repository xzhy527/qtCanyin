#include "uporderform.h"
#include "ui_uporderform.h"
#include "YF/YFTools.h"
UporderForm::UporderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UporderForm)
{
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->setupUi(this);
}

UporderForm::UporderForm(QString orderID, QWidget *parent):QWidget(parent),ui(new Ui::UporderForm)
{
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->setupUi(this);
    this->orderid=orderID;
    this->setVisible(true);
    connect(ui->sattlebtn,SIGNAL(clicked()),this,SLOT(sattle()));
    connect(ui->realediter,SIGNAL(textChanged(QString)),this,SLOT(calc(QString)));
    init();

}
void UporderForm::init()
{
    QString orderinfo="流水订单号:"+this->orderid;
    orderinfo+="  消费时间:201"+this->orderid.mid(1,1)+"-"+this->orderid.mid(2,2)+"-"+this->orderid.mid(4,2)\
            +" "+this->orderid.mid(6,2)+":"+this->orderid.mid(8,2)+":"+this->orderid.mid(10,2);
    ui->orderidlable->setText(orderinfo);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    orderModel=new QStandardItemModel;
    ui->tableView->setModel(orderModel);
    orderModel->setColumnCount(9);
    QStringList headtitles;
    headtitles<<"ID"<<"消费项目"<<"单价"<<"数量"<<"基本金额"<<"折扣比率"<<"结算方式"<<"折后金额"<<"消费时间"<<"备注说明";
    orderModel->setHorizontalHeaderLabels(headtitles);
    double total=readorderinfo(this->orderid);
    ui->ysediter->setText(QString::number(total));
    //计算优惠金额部分
    int freetotal=0;
    ui->freeediter->setText(QString::number(freetotal));
}

double UporderForm::readorderinfo(QString orderID){
    QSqlQuery query;
    query.exec("select id,mz,dj,sl,je,did,cd from t_order where sid='"+orderID+"'");
    double t_je=0.0;
    while(query.next()){
        orderModel->insertRow(0);
        orderModel->setData(orderModel->index(0,0),query.value("id"));
        orderModel->setData(orderModel->index(0,1),query.value("mz"));
        orderModel->setData(orderModel->index(0,2),query.value("dj"));
        orderModel->setData(orderModel->index(0,3),query.value("sl"));
        orderModel->setData(orderModel->index(0,4),query.value("je"));
        t_je+=query.value("je").toDouble();
        orderModel->setData(orderModel->index(0,5),"折扣比");
        orderModel->setData(orderModel->index(0,6),"正常+套餐+会员折扣+特殊优惠");
        orderModel->setData(orderModel->index(0,7),query.value("je"));
        orderModel->setData(orderModel->index(0,8),query.value("cd"));
        orderModel->setData(orderModel->index(0,9),query.value("sw"));
    }
    return t_je;
}

void UporderForm::sattle()
{
    QJsonObject sattleobj;
    sattleobj.insert("table","t_account");
    sattleobj.insert("saleid","");
    sattleobj.insert("paytyoe",ui->paytypecombox->currentText());
    sattleobj.insert("cmoney","1000");
    sattleobj.insert("rmoney",ui->ysediter->text());
    sattleobj.insert("creattime",YF_getdatetime());
    sattleobj.insert("operation","xiaoxiao");
    sattleobj.insert("descr","备注说明");
    sattleobj.insert("isnormal","0");

    if(!YF::sql(sattleobj))return;

    for(int i=0;i<orderModel->rowCount();i++){
        QJsonObject saleobj;
        saleobj.insert("table","t_sales");
        saleobj.insert("salename",orderModel->index(i,1).data().toString());
        saleobj.insert("bprice",orderModel->index(i,2).data().toString());
        saleobj.insert("mprice",orderModel->index(i,2).data().toString());
        saleobj.insert("rate",orderModel->index(i,5).data().toString());
        saleobj.insert("quantity",orderModel->index(i,3).data().toString());
        saleobj.insert("rmbsum",orderModel->index(i,7).data().toString());
        saleobj.insert("waiter","xiaoxiao");
        saleobj.insert("creattime",orderModel->index(i,8).data().toString());
        saleobj.insert("state",orderModel->index(i,6).data().toString());
        if(YF::sql(saleobj)){
            YF::sql("delete from t_order where id='"+orderModel->index(i,0).data().toString()+"'");
        }
    }
    this->close();

}

void UporderForm::calc(QString realmomey)
{
    double t_momey = realmomey.toDouble();
    double t_ys=ui->ysediter->text().toDouble();
    if(t_momey>t_ys){
        double t_m=t_momey-t_ys;
        ui->zlediter->setText(QString::number(t_m));

    }else{
        ui->zlediter->setText("0");
    }
}
UporderForm::~UporderForm()
{
    delete ui;
    delete orderModel;
}
