#include "yfwindow.h"
#include <QDebug>
YFWindow::YFWindow(QWidget *parent) :QFrame(parent,Qt::FramelessWindowHint)
{
    layout=new QVBoxLayout(this);
    headwidget=new Titlewidget(this);
    centralWidget=new QWidget();
    layout->addWidget(headwidget);
    layout->addWidget(centralWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
    this->resize(500,500);
    headwidget->titlelabel->resize(this->width(),headwidget->height());

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
        }else{
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
            move(event->globalPos() - m_oldpos);
            event->accept();
        }
    }
}

void YFWindow::mouseReleaseEvent(QMouseEvent *)
{
    changetype=0;
    SetCursorStyle(changetype);
}

void YFWindow::mouseDoubleClickEvent(QMouseEvent *)
{
    qDebug()<<headwidget->titlelabel->width();
    headwidget->titlelabel->resize(this->width(),headwidget->height());
}

void YFWindow::resizeEvent(QResizeEvent *)
{
    headwidget->titlelabel->resize(this->width(),headwidget->height());
}


Titlewidget::Titlewidget(QWidget *parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    titlelabel=new QLabel("你好的");
    titlelabel->resize(this->width(),this->height());
    closebtn=new QPushButton();
    closebtn->setIcon(QIcon(":/images/close.png"));
    closebtn->setObjectName("headclosebtn");
    connect(closebtn,SIGNAL(clicked()),parent,SLOT(close()));
    minbtn=new QPushButton();
    maxbtn=new QPushButton();
    other=new QWidget();
    //titlelabel->setStyleSheet("color:black;");
    QHBoxLayout *titlelayout=new QHBoxLayout();
    titlelayout->addWidget(titlelabel);
    titlelayout->addStretch();
    titlelayout->addWidget(closebtn);
    titlelayout->setMargin(0);
    titlelayout->setSpacing(0);
    titlelayout->setContentsMargins(0,0,0,0);
    this->setLayout(titlelayout);
    this->resize(this->width(),30);
}

void Titlewidget::resizeEvent(QResizeEvent *)
{
    this->resize(this->width(),30);
}
