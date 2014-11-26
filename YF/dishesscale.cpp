#include "dishesscale.h"
#include <QJsonObject>
#include <QStyledItemDelegate>
#include <QFont>
#include <QString>
DishesScale::DishesScale(QObject *parent) :
    QItemDelegate(parent)
{
}

void DishesScale::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter,option,index);
    QVariant json=index.data();
    if(!json.isValid()||json.isNull()){
        return;
    }
    QJsonObject jsonobj=json.toJsonObject();
    if(jsonobj.count()>1){
        QRect rect=option.rect;
        if(jsonobj.value("viewtype").toInt()==2){
            //简字显示类型
            QRect s_rect(rect.x()+30,rect.y()+75,30,30);
            if(jsonobj.value("selected").toBool()){
                QImage img(":/images/checked2.png");
                painter->drawImage(s_rect,img.scaled(50,50,Qt::KeepAspectRatio));
            }
//            else{
//                painter->drawRect(s_rect);
//            }


            rect.adjust(0,0,-2,-1);
            painter->drawRect(rect);
            QFont font;
            font.setBold(true);
            font.setPixelSize(16);
            painter->setFont(font);
            QString text=QString::number(jsonobj.value("bprice").toDouble()) + tr("元");
            painter->drawText(rect.x(),rect.y()+30,rect.width(),25,Qt::AlignCenter,jsonobj.value("name").toString());
            painter->drawText(rect.x()+25,rect.y()+75,text);


        }else{
            //缩略图模式
            QBrush b;
            painter->setBrush(b);
            painter->drawImage(rect,QImage(":/images/checked.png"));
            painter->drawRoundRect(rect,4,4);
            b.setColor(Qt::red);
            b.setStyle(Qt::SolidPattern);
            painter->setBrush(b);

            //
            painter->drawRoundRect(rect.x(),rect.y()+rect.height(),rect.width(),20,4,4);
            //QByteArray dd=YF::get("http://www.skycn.com/static/ad/201411/skcy78080_1105.jpg");
            //QItemDelegate::paint(painter,option,index);
        }


//        QLabel testlabe("nihaodsfadsfdas");
//        QStyleOption sty;
//        testlabe.setGeometry(rect);
//        QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel,&sty,painter,&testlabe);
    }else{
       //列表表格显示类型
       QItemDelegate::paint(painter,option,index);
    }
}

