#include "yfwindow.h"
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
YFWindow::YFWindow(QWidget *parent) :QFrame(parent,Qt::FramelessWindowHint)
{
    this->setObjectName("YFWindow");
    layout=new QVBoxLayout(this);
    headwidget=new HeadWidget(this);
    centralWidget=new QWidget();
    layout->addWidget(headwidget,0);
    layout->addWidget(centralWidget,1);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
    this->resize(500,500);
    connect(this,SIGNAL(switchmaxwin()),this,SLOT(handlemaxwin()));
    connect(headwidget->minbtn,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(headwidget->maxbtn,SIGNAL(clicked()),this,SLOT(handlemaxwin()));
    //headwidget->setVisible(false);
}

void YFWindow::setrejectSizeChange(bool noChange)
{
    this->setProperty("rejectSizeChange",noChange);
}

void YFWindow::setrejectMove(bool nomove)
{
    this->setProperty("rejectmove",nomove);
}
void YFWindow::handlemaxwin()
{
    //qDebug()<<this->property("maxwin")<<this->property("oldrect");
    bool maxwin=this->property("maxwin").toBool();
    if(!maxwin){
        this->setProperty("oldrect",geometry());
        //this->setGeometry(qApp->desktop()->availableGeometry());
        this->setWindowState(Qt::WindowMaximized);
    }else{
        this->setWindowState(Qt::WindowNoState);
        QRect rect=this->property("oldrect").toRect();
        setGeometry(rect);
    }
    this->setProperty("maxwin",!maxwin);
}
void YFWindow::SetCursorStyle(int direction)
{
    switch(direction)
        {
        case eTop:
        case eBottom:
            setCursor(Qt::SizeVerCursor);
            break;
        case eRight:
        case eLeft:
            setCursor(Qt::SizeHorCursor);
            break;
        case eTopRight:
        case eBottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case eRightBottom:
        case eLeftTop:
            setCursor(Qt::SizeFDiagCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
    }
}
void YFWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        int sh=0;
        int sv=0;
        if(event->x()<10){
            sh+=3;
        }
        if(event->x()>this->rect().width()-10){
            sh+=6;
        }
        if(event->y()<10){
            sv+=5;
        }else if(event->y()>this->rect().height()-20){
            sv+=7;
        }
        changetype=sh+sv;
        if(changetype>0){
          m_oldpos=event->globalPos();
          if(this->property("rejectSizeChange").toBool()){
             changetype=0;
          }
        }
        if(changetype==0){
           m_oldpos=event->globalPos()-frameGeometry().topLeft();
        }
        SetCursorStyle(changetype);
    }
    event->ignore();
}
void YFWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if(changetype>0){
            int ndx=event->globalX()-m_oldpos.x();
            int ndy=event->globalPos().y()-m_oldpos.y();
            QRect rectwidow=geometry();
            if(changetype==3||changetype==8||changetype==10){
                rectwidow.setLeft(rectwidow.left()+ndx);
            }
            if(changetype==6||changetype==11||changetype==13){
                rectwidow.setRight(rectwidow.right()+ndx);
            }
            if(changetype==5||changetype==8||changetype==11){
                rectwidow.setTop(rectwidow.top()+ndy);
            }
            if(changetype==7||changetype==10||changetype==13){
                rectwidow.setBottom(rectwidow.bottom()+ndy);
            }
            setGeometry(rectwidow);
            m_oldpos=event->globalPos();
            event->accept();

        }else{
            if(!this->property("rejectmove").toBool()){
               move(event->globalPos() - m_oldpos);
            }

            event->accept();
        }
    }
}
void YFWindow::mouseReleaseEvent(QMouseEvent *)
{
    changetype=0;
    SetCursorStyle(changetype);
}
void YFWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton && event->y()<headwidget->titlelabel->height()){
        emit switchmaxwin();
    }
}

void YFWindow::resizeEvent(QResizeEvent *)
{
    headwidget->titlelabel->resize(this->width(),headwidget->height());
}
HeadWidget::HeadWidget(QWidget *parent):QWidget(parent)
{
    this->setObjectName("headwidget");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    iconLabel=new QLabel();
    iconLabel->resize(0,0);
    iconLabel->setVisible(false);
    titlelabel=new QLabel();
    titlelabel->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    titlelabel->setGeometry(parent->geometry());
    closebtn=new QPushButton();
    closebtn->setIcon(QIcon(":/images/close.png"));

    minbtn=new QPushButton();
    minbtn->setVisible(false);
    maxbtn=new QPushButton();
    maxbtn->setVisible(true);
    other=new QWidget();
    other->setVisible(false);
    QHBoxLayout *titlelayout=new QHBoxLayout(this);
    titlelayout->addWidget(iconLabel,0,Qt::AlignCenter);
    titlelayout->addWidget(titlelabel,1);
    titlelayout->addWidget(other,0,Qt::AlignTop);
    titlelayout->addWidget(minbtn,0,Qt::AlignTop);
    titlelayout->addWidget(maxbtn,0,Qt::AlignTop);
    titlelayout->addWidget(closebtn,0,Qt::AlignTop);
    titlelayout->setMargin(0);
    titlelayout->setSpacing(0);
    titlelayout->setContentsMargins(0,0,0,0);
    this->setLayout(titlelayout);
    this->setMaximumHeight(30);
    this->setMinimumHeight(30);
    connect(closebtn,SIGNAL(clicked()),parent,SLOT(close()));
}
void HeadWidget::setHeadHeight(int height)
{
    this->setMaximumHeight(height);
    this->setMinimumHeight(height);
}

void HeadWidget::SetHeadTitle(QByteArray title)
{
    titlelabel->setText(title);
}

void HeadWidget::setHeadIcon(char *fileName)
{
    QImage *img=new QImage(fileName);
    img->load(fileName);
    *img=img->scaled(this->height(),this->height(),Qt::IgnoreAspectRatio);
    iconLabel->setPixmap(QPixmap::fromImage(*img));
    iconLabel->setVisible(true);
}
