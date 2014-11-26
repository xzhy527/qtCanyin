/********************************************************************************
** Form generated from reading UI file 'loginview.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINVIEW_H
#define UI_LOGINVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LoginView
{
public:
    QLabel *logoimage;
    QLabel *Store_label;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *weburlediter;
    QLineEdit *loginname_editer;
    QLineEdit *password_editer;
    QPushButton *loginbtn;
    QPushButton *quitebtn;
    QLabel *loginmessage;

    void setupUi(QDialog *LoginView)
    {
        if (LoginView->objectName().isEmpty())
            LoginView->setObjectName(QStringLiteral("LoginView"));
        LoginView->resize(582, 195);
        logoimage = new QLabel(LoginView);
        logoimage->setObjectName(QStringLiteral("logoimage"));
        logoimage->setGeometry(QRect(14, 2, 255, 165));
        logoimage->setPixmap(QPixmap(QString::fromUtf8("logon.jpg")));
        Store_label = new QLabel(LoginView);
        Store_label->setObjectName(QStringLiteral("Store_label"));
        Store_label->setGeometry(QRect(16, 166, 245, 27));
        Store_label->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(LoginView);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(274, 18, 54, 23));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);
        label_4 = new QLabel(LoginView);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(274, 60, 54, 23));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        label_4->setFont(font1);
        label_5 = new QLabel(LoginView);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(274, 96, 54, 29));
        label_5->setFont(font1);
        weburlediter = new QLineEdit(LoginView);
        weburlediter->setObjectName(QStringLiteral("weburlediter"));
        weburlediter->setGeometry(QRect(336, 14, 221, 33));
        weburlediter->setFont(font);
        loginname_editer = new QLineEdit(LoginView);
        loginname_editer->setObjectName(QStringLiteral("loginname_editer"));
        loginname_editer->setGeometry(QRect(336, 54, 221, 33));
        loginname_editer->setFont(font);
        password_editer = new QLineEdit(LoginView);
        password_editer->setObjectName(QStringLiteral("password_editer"));
        password_editer->setGeometry(QRect(336, 94, 221, 33));
        password_editer->setFont(font);
        password_editer->setEchoMode(QLineEdit::Password);
        loginbtn = new QPushButton(LoginView);
        loginbtn->setObjectName(QStringLiteral("loginbtn"));
        loginbtn->setGeometry(QRect(298, 138, 125, 37));
        loginbtn->setCheckable(true);
        loginbtn->setChecked(true);
        quitebtn = new QPushButton(LoginView);
        quitebtn->setObjectName(QStringLiteral("quitebtn"));
        quitebtn->setGeometry(QRect(442, 138, 125, 37));
        loginmessage = new QLabel(LoginView);
        loginmessage->setObjectName(QStringLiteral("loginmessage"));
        loginmessage->setGeometry(QRect(272, 178, 305, 16));
        loginmessage->setAlignment(Qt::AlignCenter);

        retranslateUi(LoginView);

        QMetaObject::connectSlotsByName(LoginView);
    } // setupUi

    void retranslateUi(QDialog *LoginView)
    {
        LoginView->setWindowTitle(QApplication::translate("LoginView", "Dialog", 0));
        logoimage->setText(QString());
        Store_label->setText(QApplication::translate("LoginView", "\351\225\277\346\230\245\346\237\263\350\220\245\350\267\257\345\272\227", 0));
        label_3->setText(QApplication::translate("LoginView", "\346\234\215\345\212\241\345\231\250", 0));
        label_4->setText(QApplication::translate("LoginView", "\347\224\250\346\210\267\345\220\215", 0));
        label_5->setText(QApplication::translate("LoginView", "\345\257\206  \347\240\201", 0));
        weburlediter->setText(QString());
        loginname_editer->setText(QString());
        password_editer->setText(QString());
        loginbtn->setText(QApplication::translate("LoginView", "\347\231\273\345\275\225", 0));
        quitebtn->setText(QApplication::translate("LoginView", "\351\200\200\345\207\272", 0));
        loginmessage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginView: public Ui_LoginView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINVIEW_H
