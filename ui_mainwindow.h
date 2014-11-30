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
    QPushButton *pushButton;
    QTableView *tableView_2;
    QPushButton *settlebtn;
    QPushButton *createbtn;
    QPushButton *querybtn;
    QPushButton *settingbtn;
    QFrame *orderIDlabel;
    QLabel *label;
    QComboBox *orderIDCombobox;

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
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        lineEdit->setFont(font);
        label_2 = new QLabel(funframe);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(6, 8, 54, 19));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
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
        comboBox->setFont(font1);
        comboBox->setAutoFillBackground(true);
        comboBox->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
        comboBox->setDuplicatesEnabled(true);
        comboBox->setFrame(true);
        pushButton = new QPushButton(funframe);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(472, 0, 81, 31));
        tableView_2 = new QTableView(centralWidget);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setGeometry(QRect(772, 176, 252, 544));
        settlebtn = new QPushButton(centralWidget);
        settlebtn->setObjectName(QStringLiteral("settlebtn"));
        settlebtn->setGeometry(QRect(772, 720, 252, 49));
        QFont font2;
        font2.setPointSize(20);
        font2.setBold(true);
        font2.setWeight(75);
        settlebtn->setFont(font2);
        settlebtn->setAutoFillBackground(false);
        createbtn = new QPushButton(centralWidget);
        createbtn->setObjectName(QStringLiteral("createbtn"));
        createbtn->setGeometry(QRect(772, 28, 252, 39));
        createbtn->setFont(font1);
        createbtn->setAutoFillBackground(false);
        querybtn = new QPushButton(centralWidget);
        querybtn->setObjectName(QStringLiteral("querybtn"));
        querybtn->setGeometry(QRect(772, 68, 252, 39));
        querybtn->setFont(font1);
        querybtn->setAutoFillBackground(false);
        settingbtn = new QPushButton(centralWidget);
        settingbtn->setObjectName(QStringLiteral("settingbtn"));
        settingbtn->setGeometry(QRect(772, 108, 252, 39));
        settingbtn->setFont(font1);
        settingbtn->setAutoFillBackground(false);
        orderIDlabel = new QFrame(centralWidget);
        orderIDlabel->setObjectName(QStringLiteral("orderIDlabel"));
        orderIDlabel->setGeometry(QRect(772, 148, 252, 28));
        orderIDlabel->setFrameShape(QFrame::StyledPanel);
        orderIDlabel->setFrameShadow(QFrame::Raised);
        label = new QLabel(orderIDlabel);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 78, 26));
        QFont font3;
        font3.setPointSize(9);
        font3.setBold(true);
        font3.setWeight(75);
        label->setFont(font3);
        label->setFrameShape(QFrame::Box);
        label->setFrameShadow(QFrame::Plain);
        label->setLineWidth(2);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(false);
        label->setMargin(1);
        orderIDCombobox = new QComboBox(orderIDlabel);
        orderIDCombobox->setObjectName(QStringLiteral("orderIDCombobox"));
        orderIDCombobox->setGeometry(QRect(76, -2, 174, 28));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font4.setPointSize(16);
        font4.setBold(false);
        font4.setItalic(false);
        font4.setWeight(9);
        orderIDCombobox->setFont(font4);
        orderIDCombobox->setAcceptDrops(false);
        orderIDCombobox->setAutoFillBackground(true);
        orderIDCombobox->setStyleSheet(QString::fromUtf8("font: 75 16pt \"\351\273\221\344\275\223\";"));
        orderIDCombobox->setEditable(false);
        orderIDCombobox->setMaxVisibleItems(20);
        orderIDCombobox->setDuplicatesEnabled(true);
        orderIDCombobox->setFrame(true);
        orderIDCombobox->setModelColumn(0);
        MainWindow->setCentralWidget(centralWidget);
#ifndef QT_NO_SHORTCUT
#endif // QT_NO_SHORTCUT

        retranslateUi(MainWindow);

        orderIDCombobox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\231\272\350\203\275\351\244\220\351\245\256\347\256\241\347\220\206\347\263\273\347\273\237", 0));
        toptiplabel->setText(QApplication::translate("MainWindow", "\346\254\242\350\277\216\344\275\277\347\224\250\346\234\254\347\263\273\347\273\237", 0));
        lineEdit->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "\345\277\253\346\215\267\347\240\201", 0));
        refreshbtn->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260\345\220\214\346\255\245", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\274\251\345\233\276", 0)
         << QApplication::translate("MainWindow", "\345\210\227\350\241\250", 0)
         << QApplication::translate("MainWindow", "\347\256\200\345\255\227", 0)
        );
        pushButton->setText(QApplication::translate("MainWindow", "test", 0));
        settlebtn->setText(QApplication::translate("MainWindow", "\347\273\223\347\256\227\346\211\223\345\215\260", 0));
        createbtn->setText(QApplication::translate("MainWindow", "\346\226\260\345\242\236\346\265\201\346\260\264\345\215\225", 0));
        querybtn->setText(QApplication::translate("MainWindow", "\346\234\200\350\277\221\346\265\201\346\260\264\345\215\225\346\237\245\350\257\242", 0));
        settingbtn->setText(QApplication::translate("MainWindow", "\345\212\237\350\203\275\344\270\216\350\256\276\347\275\256", 0));
        label->setText(QApplication::translate("MainWindow", "\350\256\242\345\215\225\346\265\201\346\260\264\345\217\267", 0));
        orderIDCombobox->setCurrentText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
