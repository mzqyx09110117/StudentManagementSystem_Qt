/********************************************************************************
** Form generated from reading UI file 'changepassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEPASSWORDDIALOG_H
#define UI_CHANGEPASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChangePasswordDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_confirm;
    QLineEdit *lineEdit_password;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_confirmPassword;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *ChangePasswordDialog)
    {
        if (ChangePasswordDialog->objectName().isEmpty())
            ChangePasswordDialog->setObjectName("ChangePasswordDialog");
        ChangePasswordDialog->resize(520, 400);
        ChangePasswordDialog->setMinimumSize(QSize(520, 400));
        ChangePasswordDialog->setMaximumSize(QSize(520, 400));
        label = new QLabel(ChangePasswordDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 50, 300, 30));
        label->setMinimumSize(QSize(300, 30));
        label->setMaximumSize(QSize(300, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(16);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(ChangePasswordDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 120, 90, 30));
        label_2->setMinimumSize(QSize(90, 30));
        label_2->setMaximumSize(QSize(90, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(12);
        label_2->setFont(font1);
        label_3 = new QLabel(ChangePasswordDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 190, 90, 30));
        label_3->setMinimumSize(QSize(90, 30));
        label_3->setMaximumSize(QSize(90, 30));
        label_3->setFont(font1);
        lineEdit_confirm = new QLineEdit(ChangePasswordDialog);
        lineEdit_confirm->setObjectName("lineEdit_confirm");
        lineEdit_confirm->setGeometry(QRect(170, 190, 300, 30));
        lineEdit_confirm->setMinimumSize(QSize(300, 30));
        lineEdit_confirm->setMaximumSize(QSize(300, 30));
        lineEdit_confirm->setFont(font1);
        lineEdit_confirm->setEchoMode(QLineEdit::EchoMode::Password);
        lineEdit_password = new QLineEdit(ChangePasswordDialog);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setGeometry(QRect(170, 120, 300, 30));
        lineEdit_password->setMinimumSize(QSize(300, 30));
        lineEdit_password->setMaximumSize(QSize(300, 30));
        lineEdit_password->setFont(font1);
        lineEdit_password->setEchoMode(QLineEdit::EchoMode::Password);
        layoutWidget = new QWidget(ChangePasswordDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(89, 270, 341, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_confirmPassword = new QPushButton(layoutWidget);
        pushButton_confirmPassword->setObjectName("pushButton_confirmPassword");
        pushButton_confirmPassword->setMinimumSize(QSize(90, 30));
        pushButton_confirmPassword->setMaximumSize(QSize(90, 30));
        pushButton_confirmPassword->setFont(font1);

        horizontalLayout->addWidget(pushButton_confirmPassword);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font1);

        horizontalLayout->addWidget(pushButton_cancel);

        QWidget::setTabOrder(lineEdit_password, lineEdit_confirm);
        QWidget::setTabOrder(lineEdit_confirm, pushButton_confirmPassword);
        QWidget::setTabOrder(pushButton_confirmPassword, pushButton_cancel);

        retranslateUi(ChangePasswordDialog);

        QMetaObject::connectSlotsByName(ChangePasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *ChangePasswordDialog)
    {
        ChangePasswordDialog->setWindowTitle(QCoreApplication::translate("ChangePasswordDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ChangePasswordDialog", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        label_2->setText(QCoreApplication::translate("ChangePasswordDialog", "\346\226\260\345\257\206\347\240\201\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("ChangePasswordDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        lineEdit_confirm->setPlaceholderText(QCoreApplication::translate("ChangePasswordDialog", "\350\257\267\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        lineEdit_password->setPlaceholderText(QCoreApplication::translate("ChangePasswordDialog", "\345\255\227\346\257\215\343\200\201\346\225\260\345\255\227\343\200\201\347\254\246\345\217\267\350\207\263\345\260\221\344\270\244\347\247\215\357\274\2148--40\344\275\215", nullptr));
        pushButton_confirmPassword->setText(QCoreApplication::translate("ChangePasswordDialog", "\347\241\256\350\256\244\344\277\256\346\224\271", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("ChangePasswordDialog", "\345\217\226\346\266\210\344\277\256\346\224\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChangePasswordDialog: public Ui_ChangePasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEPASSWORDDIALOG_H
