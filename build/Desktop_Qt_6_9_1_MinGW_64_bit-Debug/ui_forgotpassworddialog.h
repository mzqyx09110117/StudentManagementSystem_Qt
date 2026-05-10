/********************************************************************************
** Form generated from reading UI file 'forgotpassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORGOTPASSWORDDIALOG_H
#define UI_FORGOTPASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ForgotPasswordDialog
{
public:
    QRadioButton *radioButton_question1;
    QRadioButton *radioButton_question2;
    QLabel *label;
    QLabel *label_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_confirmUser;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_confirmPassword;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_back;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_answer;
    QLineEdit *lineEdit_newPassword;
    QLineEdit *lineEdit_confirm;
    QLineEdit *lineEdit_username;

    void setupUi(QDialog *ForgotPasswordDialog)
    {
        if (ForgotPasswordDialog->objectName().isEmpty())
            ForgotPasswordDialog->setObjectName("ForgotPasswordDialog");
        ForgotPasswordDialog->resize(520, 400);
        ForgotPasswordDialog->setMinimumSize(QSize(520, 400));
        ForgotPasswordDialog->setMaximumSize(QSize(520, 400));
        radioButton_question1 = new QRadioButton(ForgotPasswordDialog);
        radioButton_question1->setObjectName("radioButton_question1");
        radioButton_question1->setGeometry(QRect(170, 100, 300, 30));
        radioButton_question1->setMinimumSize(QSize(300, 30));
        radioButton_question1->setMaximumSize(QSize(300, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        radioButton_question1->setFont(font);
        radioButton_question2 = new QRadioButton(ForgotPasswordDialog);
        radioButton_question2->setObjectName("radioButton_question2");
        radioButton_question2->setGeometry(QRect(170, 140, 300, 30));
        radioButton_question2->setMinimumSize(QSize(300, 30));
        radioButton_question2->setMaximumSize(QSize(300, 30));
        radioButton_question2->setFont(font);
        label = new QLabel(ForgotPasswordDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 20, 300, 30));
        label->setMinimumSize(QSize(300, 30));
        label->setMaximumSize(QSize(300, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        label->setFont(font1);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(ForgotPasswordDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 100, 90, 30));
        label_2->setMinimumSize(QSize(90, 30));
        label_2->setMaximumSize(QSize(90, 30));
        label_2->setFont(font);
        layoutWidget = new QWidget(ForgotPasswordDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(40, 320, 431, 32));
        layoutWidget->setFont(font);
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_confirmUser = new QPushButton(layoutWidget);
        pushButton_confirmUser->setObjectName("pushButton_confirmUser");
        pushButton_confirmUser->setMinimumSize(QSize(90, 30));
        pushButton_confirmUser->setMaximumSize(QSize(90, 30));
        pushButton_confirmUser->setFont(font);

        horizontalLayout->addWidget(pushButton_confirmUser);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_confirmPassword = new QPushButton(layoutWidget);
        pushButton_confirmPassword->setObjectName("pushButton_confirmPassword");
        pushButton_confirmPassword->setMinimumSize(QSize(90, 30));
        pushButton_confirmPassword->setMaximumSize(QSize(90, 30));
        pushButton_confirmPassword->setFont(font);

        horizontalLayout->addWidget(pushButton_confirmPassword);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_back = new QPushButton(layoutWidget);
        pushButton_back->setObjectName("pushButton_back");
        pushButton_back->setMinimumSize(QSize(90, 30));
        pushButton_back->setMaximumSize(QSize(90, 30));
        pushButton_back->setFont(font);

        horizontalLayout->addWidget(pushButton_back);

        label_3 = new QLabel(ForgotPasswordDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 60, 90, 30));
        label_3->setMinimumSize(QSize(90, 30));
        label_3->setMaximumSize(QSize(90, 30));
        label_3->setFont(font);
        label_4 = new QLabel(ForgotPasswordDialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 180, 90, 30));
        label_4->setMinimumSize(QSize(90, 30));
        label_4->setMaximumSize(QSize(90, 30));
        label_4->setFont(font);
        label_5 = new QLabel(ForgotPasswordDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(40, 220, 90, 30));
        label_5->setMinimumSize(QSize(90, 30));
        label_5->setMaximumSize(QSize(90, 30));
        label_5->setFont(font);
        label_6 = new QLabel(ForgotPasswordDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(40, 260, 90, 30));
        label_6->setMinimumSize(QSize(90, 30));
        label_6->setMaximumSize(QSize(90, 30));
        label_6->setFont(font);
        lineEdit_answer = new QLineEdit(ForgotPasswordDialog);
        lineEdit_answer->setObjectName("lineEdit_answer");
        lineEdit_answer->setGeometry(QRect(170, 180, 300, 30));
        lineEdit_answer->setMinimumSize(QSize(300, 30));
        lineEdit_answer->setMaximumSize(QSize(300, 30));
        lineEdit_answer->setFont(font);
        lineEdit_newPassword = new QLineEdit(ForgotPasswordDialog);
        lineEdit_newPassword->setObjectName("lineEdit_newPassword");
        lineEdit_newPassword->setGeometry(QRect(170, 220, 300, 30));
        lineEdit_newPassword->setMinimumSize(QSize(300, 30));
        lineEdit_newPassword->setMaximumSize(QSize(300, 30));
        lineEdit_newPassword->setSizeIncrement(QSize(0, 0));
        lineEdit_newPassword->setBaseSize(QSize(0, 0));
        lineEdit_newPassword->setFont(font);
        lineEdit_newPassword->setEchoMode(QLineEdit::EchoMode::Password);
        lineEdit_confirm = new QLineEdit(ForgotPasswordDialog);
        lineEdit_confirm->setObjectName("lineEdit_confirm");
        lineEdit_confirm->setGeometry(QRect(170, 260, 300, 30));
        lineEdit_confirm->setMinimumSize(QSize(300, 30));
        lineEdit_confirm->setMaximumSize(QSize(300, 30));
        lineEdit_confirm->setFont(font);
        lineEdit_confirm->setEchoMode(QLineEdit::EchoMode::Password);
        lineEdit_username = new QLineEdit(ForgotPasswordDialog);
        lineEdit_username->setObjectName("lineEdit_username");
        lineEdit_username->setGeometry(QRect(170, 60, 300, 30));
        lineEdit_username->setMinimumSize(QSize(300, 30));
        lineEdit_username->setMaximumSize(QSize(300, 30));
        lineEdit_username->setFont(font);
        QWidget::setTabOrder(lineEdit_username, radioButton_question1);
        QWidget::setTabOrder(radioButton_question1, radioButton_question2);
        QWidget::setTabOrder(radioButton_question2, lineEdit_answer);
        QWidget::setTabOrder(lineEdit_answer, lineEdit_newPassword);
        QWidget::setTabOrder(lineEdit_newPassword, lineEdit_confirm);
        QWidget::setTabOrder(lineEdit_confirm, pushButton_confirmUser);
        QWidget::setTabOrder(pushButton_confirmUser, pushButton_confirmPassword);
        QWidget::setTabOrder(pushButton_confirmPassword, pushButton_back);

        retranslateUi(ForgotPasswordDialog);

        QMetaObject::connectSlotsByName(ForgotPasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *ForgotPasswordDialog)
    {
        ForgotPasswordDialog->setWindowTitle(QCoreApplication::translate("ForgotPasswordDialog", "Dialog", nullptr));
        radioButton_question1->setText(QCoreApplication::translate("ForgotPasswordDialog", "\350\257\267\345\205\210\347\241\256\350\256\244\347\224\250\346\210\267\345\220\215", nullptr));
        radioButton_question2->setText(QCoreApplication::translate("ForgotPasswordDialog", "\350\257\267\345\205\210\347\241\256\350\256\244\347\224\250\346\210\267\345\220\215", nullptr));
        label->setText(QCoreApplication::translate("ForgotPasswordDialog", "\345\255\246\347\224\237\347\256\241\347\220\206\345\277\230\350\256\260\345\257\206\347\240\201\347\263\273\347\273\237", nullptr));
        label_2->setText(QCoreApplication::translate("ForgotPasswordDialog", "\345\257\206\344\277\235\351\227\256\351\242\230\357\274\232", nullptr));
        pushButton_confirmUser->setText(QCoreApplication::translate("ForgotPasswordDialog", "\347\241\256\350\256\244\347\224\250\346\210\267", nullptr));
        pushButton_confirmPassword->setText(QCoreApplication::translate("ForgotPasswordDialog", "\347\241\256\350\256\244\344\277\256\346\224\271", nullptr));
        pushButton_back->setText(QCoreApplication::translate("ForgotPasswordDialog", "\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
        label_3->setText(QCoreApplication::translate("ForgotPasswordDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("ForgotPasswordDialog", "\345\257\206\344\277\235\347\255\224\346\241\210\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("ForgotPasswordDialog", "\346\226\260\345\257\206\347\240\201\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("ForgotPasswordDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        lineEdit_newPassword->setPlaceholderText(QCoreApplication::translate("ForgotPasswordDialog", "\345\255\227\346\257\215\343\200\201\346\225\260\345\255\227\343\200\201\347\254\246\345\217\267\350\207\263\345\260\221\344\270\244\347\247\215\357\274\2148--40\344\275\215", nullptr));
        lineEdit_confirm->setPlaceholderText(QCoreApplication::translate("ForgotPasswordDialog", "\350\257\267\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        lineEdit_username->setInputMask(QString());
        lineEdit_username->setPlaceholderText(QCoreApplication::translate("ForgotPasswordDialog", "\350\257\267\345\205\210\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ForgotPasswordDialog: public Ui_ForgotPasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORGOTPASSWORDDIALOG_H
