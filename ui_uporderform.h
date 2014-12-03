/********************************************************************************
** Form generated from reading UI file 'uporderform.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPORDERFORM_H
#define UI_UPORDERFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UporderForm
{
public:
    QLabel *orderidlable;
    QFrame *frame;
    QLabel *label_2;
    QLineEdit *memberediter;
    QFrame *frame_2;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *freeediter;
    QLineEdit *rateediter;
    QLineEdit *ysediter;
    QLabel *label_4;
    QLineEdit *realediter;
    QLabel *label_7;
    QLineEdit *zlediter;
    QLabel *label_8;
    QComboBox *paytypecombox;
    QLabel *label_9;
    QLineEdit *descrediter;
    QPushButton *printebtn;
    QPushButton *sattlebtn;
    QPushButton *returnbtn;
    QTableView *tableView;

    void setupUi(QWidget *UporderForm)
    {
        if (UporderForm->objectName().isEmpty())
            UporderForm->setObjectName(QStringLiteral("UporderForm"));
        UporderForm->resize(700, 392);
        orderidlable = new QLabel(UporderForm);
        orderidlable->setObjectName(QStringLiteral("orderidlable"));
        orderidlable->setGeometry(QRect(0, 0, 700, 28));
        orderidlable->setFrameShape(QFrame::Box);
        frame = new QFrame(UporderForm);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 30, 700, 28));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Plain);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(4, 6, 74, 16));
        memberediter = new QLineEdit(frame);
        memberediter->setObjectName(QStringLiteral("memberediter"));
        memberediter->setGeometry(QRect(78, 2, 121, 22));
        frame_2 = new QFrame(UporderForm);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(0, 60, 583, 124));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Plain);
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(2, 4, 81, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);
        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(204, 4, 81, 21));
        label_5->setFont(font);
        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(408, 4, 70, 21));
        label_6->setFont(font);
        freeediter = new QLineEdit(frame_2);
        freeediter->setObjectName(QStringLiteral("freeediter"));
        freeediter->setGeometry(QRect(274, 2, 132, 25));
        rateediter = new QLineEdit(frame_2);
        rateediter->setObjectName(QStringLiteral("rateediter"));
        rateediter->setGeometry(QRect(480, 2, 100, 26));
        ysediter = new QLineEdit(frame_2);
        ysediter->setObjectName(QStringLiteral("ysediter"));
        ysediter->setGeometry(QRect(72, 2, 132, 26));
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(2, 34, 81, 21));
        label_4->setFont(font);
        realediter = new QLineEdit(frame_2);
        realediter->setObjectName(QStringLiteral("realediter"));
        realediter->setGeometry(QRect(72, 30, 130, 28));
        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(204, 34, 81, 21));
        label_7->setFont(font);
        zlediter = new QLineEdit(frame_2);
        zlediter->setObjectName(QStringLiteral("zlediter"));
        zlediter->setGeometry(QRect(274, 30, 132, 28));
        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(2, 64, 81, 21));
        label_8->setFont(font);
        paytypecombox = new QComboBox(frame_2);
        paytypecombox->setObjectName(QStringLiteral("paytypecombox"));
        paytypecombox->setGeometry(QRect(72, 62, 129, 26));
        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(2, 94, 81, 21));
        label_9->setFont(font);
        descrediter = new QLineEdit(frame_2);
        descrediter->setObjectName(QStringLiteral("descrediter"));
        descrediter->setGeometry(QRect(72, 92, 503, 26));
        printebtn = new QPushButton(UporderForm);
        printebtn->setObjectName(QStringLiteral("printebtn"));
        printebtn->setGeometry(QRect(584, 60, 116, 44));
        sattlebtn = new QPushButton(UporderForm);
        sattlebtn->setObjectName(QStringLiteral("sattlebtn"));
        sattlebtn->setGeometry(QRect(584, 104, 116, 44));
        returnbtn = new QPushButton(UporderForm);
        returnbtn->setObjectName(QStringLiteral("returnbtn"));
        returnbtn->setGeometry(QRect(584, 146, 116, 40));
        tableView = new QTableView(UporderForm);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(0, 188, 699, 205));

        retranslateUi(UporderForm);

        QMetaObject::connectSlotsByName(UporderForm);
    } // setupUi

    void retranslateUi(QWidget *UporderForm)
    {
        UporderForm->setWindowTitle(QApplication::translate("UporderForm", "Form", 0));
        orderidlable->setText(QApplication::translate("UporderForm", " \346\265\201\346\260\264\345\215\225\345\217\267\357\274\2322014255441153", 0));
        label_2->setText(QApplication::translate("UporderForm", "\344\274\232\345\221\230\345\215\241/\345\247\223\345\220\215", 0));
        label_3->setText(QApplication::translate("UporderForm", "\345\272\224\346\224\266\351\207\221\351\242\235", 0));
        label_5->setText(QApplication::translate("UporderForm", "\344\274\230\346\203\240\351\207\221\351\242\235", 0));
        label_6->setText(QApplication::translate("UporderForm", "\346\212\230\346\211\243\346\257\224\347\216\207", 0));
        label_4->setText(QApplication::translate("UporderForm", "\345\256\236\346\224\266\351\207\221\351\242\235", 0));
        label_7->setText(QApplication::translate("UporderForm", "\347\216\260\351\207\221\346\211\276\351\233\266", 0));
        label_8->setText(QApplication::translate("UporderForm", "\346\224\257\344\273\230\346\226\271\345\274\217", 0));
        label_9->setText(QApplication::translate("UporderForm", "\345\244\207\346\263\250\344\277\241\346\201\257", 0));
        printebtn->setText(QApplication::translate("UporderForm", "\346\211\223\345\215\260\345\260\217\347\245\250", 0));
        sattlebtn->setText(QApplication::translate("UporderForm", "\347\273\223\347\256\227", 0));
        returnbtn->setText(QApplication::translate("UporderForm", "\345\217\226\346\266\210\350\277\224\345\233\236", 0));
    } // retranslateUi

};

namespace Ui {
    class UporderForm: public Ui_UporderForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPORDERFORM_H
