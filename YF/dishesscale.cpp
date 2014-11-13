#include "dishesscale.h"
#include <QJsonObject>
#include <QStyledItemDelegate>
#include <QFont>
DishesScale::DishesScale(QObject *parent) :
    QItemDelegate(parent)
{
}

void DishesScale::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    if(index.column()!=0){
//        QItemDelegate::paint(painter,option,index);
//        return;
//    }
    QVariant json=index.data();
    if(!json.isValid()||json.isNull()){
        //QItemDelegate::paint(painter,option,index);
        return;
    }
    QJsonObject jsonobj=json.toJsonObject();
    if(jsonobj.count()>1){
        QRect rect=option.rect;
        painter->drawRect(rect.x()+2,rect.y(),100,100);
        QFont font;
        font.setBold(true);
        font.setPixelSize(16);
        painter->setFont(font);

        QString text=QString::number(jsonobj.take("bprice").toDouble())+"元";
        painter->drawText(rect.x()+20,rect.y()+50,jsonobj.take("name").toString());
        painter->drawText(rect.x()+25,rect.y()+75,text);

//        QLabel testlabe("nihaodsfadsfdas");
//        QStyleOption sty;

//        testlabe.setGeometry(rect);
//        QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel,&sty,painter,&testlabe);



    }else{
       QItemDelegate::paint(painter,option,index);
    }
   // qDebug()<<"当前创建的列号:"<<index.column()<<index.data();


}
