/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QLabel *label_3;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_1;
    QLineEdit *lineEdit_username;
    QLabel *label_2;
    QLineEdit *lineEdit_password;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_register;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_login;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_forgot;
    QRadioButton *radioButton_showPwd;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(520, 300);
        LoginDialog->setMinimumSize(QSize(520, 300));
        LoginDialog->setMaximumSize(QSize(520, 300));
        label_3 = new QLabel(LoginDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(110, 20, 300, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(16);
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_3->setWordWrap(false);
        layoutWidget = new QWidget(LoginDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(60, 70, 396, 101));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_1 = new QLabel(layoutWidget);
        label_1->setObjectName("label_1");
        label_1->setMinimumSize(QSize(90, 30));
        label_1->setMaximumSize(QSize(90, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(12);
        label_1->setFont(font1);

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        lineEdit_username = new QLineEdit(layoutWidget);
        lineEdit_username->setObjectName("lineEdit_username");
        lineEdit_username->setMinimumSize(QSize(300, 30));
        lineEdit_username->setMaximumSize(QSize(300, 30));
        lineEdit_username->setFont(font1);

        gridLayout->addWidget(lineEdit_username, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(90, 30));
        label_2->setMaximumSize(QSize(90, 30));
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_password = new QLineEdit(layoutWidget);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setMinimumSize(QSize(300, 30));
        lineEdit_password->setMaximumSize(QSize(300, 30));
        lineEdit_password->setFont(font1);
        lineEdit_password->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(lineEdit_password, 1, 1, 1, 1);

        layoutWidget1 = new QWidget(LoginDialog);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(60, 230, 391, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_register = new QPushButton(layoutWidget1);
        pushButton_register->setObjectName("pushButton_register");
        pushButton_register->setMinimumSize(QSize(90, 30));
        pushButton_register->setMaximumSize(QSize(90, 30));
        pushButton_register->setFont(font1);

        horizontalLayout->addWidget(pushButton_register);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_login = new QPushButton(layoutWidget1);
        pushButton_login->setObjectName("pushButton_login");
        pushButton_login->setMinimumSize(QSize(90, 30));
        pushButton_login->setMaximumSize(QSize(90, 30));
        pushButton_login->setFont(font1);

        horizontalLayout->addWidget(pushButton_login);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_forgot = new QPushButton(layoutWidget1);
        pushButton_forgot->setObjectName("pushButton_forgot");
        pushButton_forgot->setMinimumSize(QSize(90, 30));
        pushButton_forgot->setMaximumSize(QSize(90, 30));
        pushButton_forgot->setFont(font1);

        horizontalLayout->addWidget(pushButton_forgot);

        radioButton_showPwd = new QRadioButton(LoginDialog);
        radioButton_showPwd->setObjectName("radioButton_showPwd");
        radioButton_showPwd->setGeometry(QRect(210, 180, 100, 30));
        radioButton_showPwd->setMinimumSize(QSize(100, 30));
        radioButton_showPwd->setMaximumSize(QSize(100, 30));
        radioButton_showPwd->setFont(font1);
        QWidget::setTabOrder(lineEdit_username, lineEdit_password);
        QWidget::setTabOrder(lineEdit_password, pushButton_login);
        QWidget::setTabOrder(pushButton_login, radioButton_showPwd);
        QWidget::setTabOrder(radioButton_showPwd, pushButton_register);
        QWidget::setTabOrder(pushButton_register, pushButton_forgot);

        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("LoginDialog", "\345\255\246\347\224\237\347\256\241\347\220\206\347\231\273\351\231\206\347\263\273\347\273\237", nullptr));
        label_1->setText(QCoreApplication::translate("LoginDialog", "\350\264\246    \345\217\267\357\274\232", nullptr));
        lineEdit_username->setText(QString());
        lineEdit_username->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\350\276\223\345\205\245\350\264\246\345\217\267", nullptr));
        label_2->setText(QCoreApplication::translate("LoginDialog", "\345\257\206    \347\240\201\357\274\232", nullptr));
        lineEdit_password->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        pushButton_register->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        pushButton_login->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        pushButton_forgot->setText(QCoreApplication::translate("LoginDialog", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        radioButton_showPwd->setText(QCoreApplication::translate("LoginDialog", "\346\230\276\347\244\272\345\257\206\347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
