#ifndef YFWINDOW_H
#define YFWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QMouseEvent>
class Titlewidget;
class YFWindow;

class YFWindow : public QFrame
{
    Q_OBJECT
public:
    enum E_Direction{
        eTop=5,
        eBottom=7,
        eRight=6,
        eLeft=3,
        eTopRight=11,
        eBottomLeft=10,
        eLeftTop=8,
        eRightBottom=13,
    };
    explicit YFWindow(QWidget *parent = 0);
    Titlewidget *headwidget;
    QWidget *centralWidget;
    QVBoxLayout *layout;
    void SetCursorStyle(int direction);
signals:

public slots:


    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void resizeEvent(QResizeEvent *);
private:
    QPoint m_oldpos;
    int changetype;


};
class Titlewidget:public QWidget{
    Q_OBJECT
public:
    explicit Titlewidget(QWidget *parent);
    QLabel *titlelabel;
    QPushButton *closebtn;
    QPushButton *minbtn;
    QPushButton *maxbtn;
    QWidget *other;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *);
};

#endif // YFWINDOW_H
