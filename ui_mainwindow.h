/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QTableView *tableView;
    QFrame *frame;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QPushButton *refreshbtn;
    QComboBox *comboBox;
    QTableView *tableView_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_5;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1024, 768);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 2, 1024, 25));
        label->setFrameShape(QFrame::Box);
        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(0, 64, 771, 708));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 28, 770, 34));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Plain);
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(60, 2, 225, 29));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(6, 8, 54, 19));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        refreshbtn = new QPushButton(frame);
        refreshbtn->setObjectName(QStringLiteral("refreshbtn"));
        refreshbtn->setGeometry(QRect(630, 2, 75, 30));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(refreshbtn->sizePolicy().hasHeightForWidth());
        refreshbtn->setSizePolicy(sizePolicy);
        refreshbtn->setAutoExclusive(false);
        comboBox = new QComboBox(frame);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(708, 2, 57, 31));
        comboBox->setFont(font);
        comboBox->setAutoFillBackground(true);
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
        comboBox->setDuplicatesEnabled(true);
        comboBox->setFrame(true);
        tableView_2 = new QTableView(centralWidget);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setGeometry(QRect(772, 170, 252, 550));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(772, 720, 252, 49));
        QFont font1;
        font1.setPointSize(20);
        font1.setBold(true);
        font1.setWeight(75);
        pushButton->setFont(font1);
        pushButton->setAutoFillBackground(false);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(772, 28, 252, 39));
        pushButton_2->setFont(font);
        pushButton_2->setAutoFillBackground(false);
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(772, 68, 252, 39));
        pushButton_3->setFont(font);
        pushButton_3->setAutoFillBackground(false);
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(772, 108, 252, 39));
        pushButton_4->setFont(font);
        pushButton_4->setAutoFillBackground(false);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(772, 148, 252, 23));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        label_5->setFont(font2);
        label_5->setFrameShape(QFrame::Box);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        label_2->setText(QApplication::translate("MainWindow", "\345\277\253\346\215\267\347\240\201", 0));
        refreshbtn->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\274\251\345\233\276", 0)
         << QApplication::translate("MainWindow", "\345\210\227\350\241\250", 0)
         << QApplication::translate("MainWindow", "\347\256\200\345\255\227", 0)
        );
        pushButton->setText(QApplication::translate("MainWindow", "\347\273\223\347\256\227\346\211\223\345\215\260", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "\346\226\260\345\242\236\346\265\201\346\260\264\345\215\225", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "\346\234\200\350\277\221\346\265\201\346\260\264\345\215\225\346\237\245\350\257\242", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "\345\212\237\350\203\275\344\270\216\350\256\276\347\275\256", 0));
        label_5->setText(QApplication::translate("MainWindow", "\346\265\201\346\260\264\345\215\225\345\217\267\357\274\23220142325553335", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
