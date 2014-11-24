#ifndef YFWINDOW_H
#define YFWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QMouseEvent>
class HeadWidget;
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
    HeadWidget *headwidget;
    QWidget *centralWidget;
    QVBoxLayout *layout;
    void setrejectSizeChange(bool noChange=false);
    void setrejectMove(bool nomove=false);
signals:
    void switchmaxwin();
public slots:
    void handlemaxwin();
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void resizeEvent(QResizeEvent *);
    void SetCursorStyle(int direction);
    //virtual void paintEvent(QPaintEvent *);
private:
    QPoint m_oldpos;
    int changetype;
};
class HeadWidget:public QWidget{
    Q_OBJECT
public:
    explicit HeadWidget(QWidget *parent);
    QLabel *iconLabel;
    QLabel *titlelabel;
    QPushButton *closebtn;
    QPushButton *minbtn;
    QPushButton *maxbtn;
    QWidget *other;
    void setHeadHeight(int height);
    void SetHeadTitle(QByteArray title);
    void setHeadIcon(char *fileName);
protected:
   // virtual void resizeEvent(QResizeEvent *);
};

#endif // YFWINDOW_H
