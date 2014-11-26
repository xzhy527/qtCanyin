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
    QLabel *toptiplabel;
    QTableView *tableView;
    QFrame *funframe;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QPushButton *refreshbtn;
    QComboBox *comboBox;
    QTableView *tableView_2;
    QPushButton *settlebtn;
    QPushButton *createbtn;
    QPushButton *querybtn;
    QPushButton *settingbtn;
    QLabel *orderIDlabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1024, 768);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        toptiplabel = new QLabel(centralWidget);
        toptiplabel->setObjectName(QStringLiteral("toptiplabel"));
        toptiplabel->setGeometry(QRect(0, 2, 1024, 25));
        toptiplabel->setFrameShape(QFrame::Box);
        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(0, 64, 771, 708));
        tableView->setFrameShape(QFrame::Box);
        tableView->setFrameShadow(QFrame::Plain);
        funframe = new QFrame(centralWidget);
        funframe->setObjectName(QStringLiteral("funframe"));
        funframe->setGeometry(QRect(0, 28, 770, 34));
        funframe->setFrameShape(QFrame::Box);
        funframe->setFrameShadow(QFrame::Plain);
        lineEdit = new QLineEdit(funframe);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(60, 2, 225, 29));
        label_2 = new QLabel(funframe);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(6, 8, 54, 19));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        refreshbtn = new QPushButton(funframe);
        refreshbtn->setObjectName(QStringLiteral("refreshbtn"));
        refreshbtn->setGeometry(QRect(636, 0, 75, 34));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(refreshbtn->sizePolicy().hasHeightForWidth());
        refreshbtn->setSizePolicy(sizePolicy);
        refreshbtn->setAutoExclusive(false);
        comboBox = new QComboBox(funframe);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(712, 0, 57, 34));
        comboBox->setFont(font);
        comboBox->setAutoFillBackground(true);
        comboBox->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
        comboBox->setDuplicatesEnabled(true);
        comboBox->setFrame(true);
        tableView_2 = new QTableView(centralWidget);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setGeometry(QRect(772, 170, 252, 550));
        settlebtn = new QPushButton(centralWidget);
        settlebtn->setObjectName(QStringLiteral("settlebtn"));
        settlebtn->setGeometry(QRect(772, 720, 252, 49));
        QFont font1;
        font1.setPointSize(20);
        font1.setBold(true);
        font1.setWeight(75);
        settlebtn->setFont(font1);
        settlebtn->setAutoFillBackground(false);
        createbtn = new QPushButton(centralWidget);
        createbtn->setObjectName(QStringLiteral("createbtn"));
        createbtn->setGeometry(QRect(772, 28, 252, 39));
        createbtn->setFont(font);
        createbtn->setAutoFillBackground(false);
        querybtn = new QPushButton(centralWidget);
        querybtn->setObjectName(QStringLiteral("querybtn"));
        querybtn->setGeometry(QRect(772, 68, 252, 39));
        querybtn->setFont(font);
        querybtn->setAutoFillBackground(false);
        settingbtn = new QPushButton(centralWidget);
        settingbtn->setObjectName(QStringLiteral("settingbtn"));
        settingbtn->setGeometry(QRect(772, 108, 252, 39));
        settingbtn->setFont(font);
        settingbtn->setAutoFillBackground(false);
        orderIDlabel = new QLabel(centralWidget);
        orderIDlabel->setObjectName(QStringLiteral("orderIDlabel"));
        orderIDlabel->setGeometry(QRect(772, 148, 252, 23));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        orderIDlabel->setFont(font2);
        orderIDlabel->setFrameShape(QFrame::Box);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\231\272\350\203\275\351\244\220\351\245\256\347\256\241\347\220\206\347\263\273\347\273\237", 0));
        toptiplabel->setText(QApplication::translate("MainWindow", "\346\254\242\350\277\216\344\275\277\347\224\250\346\234\254\347\263\273\347\273\237", 0));
        label_2->setText(QApplication::translate("MainWindow", "\345\277\253\346\215\267\347\240\201", 0));
        refreshbtn->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260\345\220\214\346\255\245", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\274\251\345\233\276", 0)
         << QApplication::translate("MainWindow", "\345\210\227\350\241\250", 0)
         << QApplication::translate("MainWindow", "\347\256\200\345\255\227", 0)
        );
        settlebtn->setText(QApplication::translate("MainWindow", "\347\273\223\347\256\227\346\211\223\345\215\260", 0));
        createbtn->setText(QApplication::translate("MainWindow", "\346\226\260\345\242\236\346\265\201\346\260\264\345\215\225", 0));
        querybtn->setText(QApplication::translate("MainWindow", "\346\234\200\350\277\221\346\265\201\346\260\264\345\215\225\346\237\245\350\257\242", 0));
        settingbtn->setText(QApplication::translate("MainWindow", "\345\212\237\350\203\275\344\270\216\350\256\276\347\275\256", 0));
        orderIDlabel->setText(QApplication::translate("MainWindow", "\346\265\201\346\260\264\345\215\225\345\217\267\357\274\23220142325553335", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
