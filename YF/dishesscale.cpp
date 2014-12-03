#include "dishesscale.h"
#include <QJsonObject>
#include <QStyledItemDelegate>
#include <QFont>
#include <QString>
#include <QUrl>
#include <QLinearGradient>
#include <QTableView>
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
    QRect rect=option.rect;
    QBrush brush;
   QTableView *tableview = qobject_cast<QTableView *>(option.styleObject);

   if(tableview->currentIndex()==index){
       brush.setColor(Qt::blue);
   }else{
       brush.setColor(QColor(169,195,221,255));
   }
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    //qDebug()<<"绘图区域:"<<rect;
    QJsonObject jsonobj=json.toJsonObject();
    if(jsonobj.count()>1){
        if(jsonobj.value("viewtype").toInt()==2){
            //简字显示类型

            //缩小矩形
            rect.adjust(0,0,-2,-2);
            painter->drawRect(rect);
            QFont font;
            font.setBold(true);
            font.setPixelSize(16);
            painter->setFont(font);
            QString text=QString::number(jsonobj.value("bprice").toVariant().toDouble()) + tr("元");
            painter->drawText(rect.x(),rect.y()+30,rect.width(),25,Qt::AlignCenter,jsonobj.value("name").toString());
            painter->drawText(rect.x(),rect.y()+60,rect.width(),25,Qt::AlignCenter,text);
            QString quicktext=jsonobj.value("quick").toVariant().toString();

            if(!quicktext.isEmpty()){
                QRect quickrect(rect.x()+rect.width()-37,rect.y()+1,35,25);
                QLinearGradient linergradient(quickrect.topLeft(),quickrect.topRight());
                linergradient.setColorAt(0.0,Qt::black);
                linergradient.setColorAt(0.5,Qt::white);
                linergradient.setColorAt(1.0,Qt::black);
                linergradient.setSpread(QGradient::PadSpread);
                painter->setBrush(linergradient);
                //painter->setBrush(brush);
                painter->drawRoundRect(quickrect,10,10);
                painter->drawText(quickrect,Qt::AlignCenter,quicktext);
            }
            int imagewidth=30;
            int imageheight=30;
            int imageleft=rect.x()+(rect.width()-imagewidth)/2;
            int imagetop=rect.y()+rect.height()-imageheight;
            QPoint imagepoint(imageleft,imagetop);
            if(jsonobj.value("selected").toBool()){
                QImage img(":/images/checked2.png");
                painter->drawImage(imagepoint,img.scaled(imagewidth,imageheight,Qt::KeepAspectRatio));
            }

        }else{
            //缩略图模式
            rect.adjust(2,2,-2,-2);
            if(!jsonobj.value("picaddr").toString().isEmpty()){
               QString filepath=QUrl(jsonobj.value("picaddr").toString()).fileName();
               painter->drawImage(rect,QImage("temp/images/"+filepath));
            }else{
                painter->drawImage(rect,QImage(":/images/checked.png"));
            }

//            painter->drawRoundRect(rect,4,4);
//            b.setColor(Qt::red);
//            b.setStyle(Qt::SolidPattern);
//            painter->setBrush(b);
//            painter->drawRoundRect(rect.x(),rect.y()+rect.height(),rect.width(),20,4,4);
//            //QByteArray dd=YF::get("http://www.skycn.com/static/ad/201411/skcy78080_1105.jpg");
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

