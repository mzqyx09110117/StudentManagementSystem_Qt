/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

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

class Ui_RegisterDialog
{
public:
    QLabel *label_1;
    QRadioButton *radioButton_admin;
    QRadioButton *radioButton_teacher;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_register;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_back;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *lineEdit_username;
    QLabel *label_3;
    QLineEdit *lineEdit_password;
    QLabel *label_4;
    QLineEdit *lineEdit_confirm;
    QLabel *label_5;
    QLineEdit *lineEdit_question1;
    QLabel *label_6;
    QLineEdit *lineEdit_answer1;
    QLabel *label_8;
    QLineEdit *lineEdit_question2;
    QLabel *label_7;
    QLineEdit *lineEdit_answer2;
    QLabel *label_9;
    QLineEdit *lineEdit_teacherName;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(520, 480);
        RegisterDialog->setMinimumSize(QSize(520, 480));
        RegisterDialog->setMaximumSize(QSize(520, 480));
        label_1 = new QLabel(RegisterDialog);
        label_1->setObjectName("label_1");
        label_1->setGeometry(QRect(110, 20, 300, 30));
        label_1->setMinimumSize(QSize(300, 30));
        label_1->setMaximumSize(QSize(300, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(16);
        label_1->setFont(font);
        label_1->setAlignment(Qt::AlignmentFlag::AlignCenter);
        radioButton_admin = new QRadioButton(RegisterDialog);
        radioButton_admin->setObjectName("radioButton_admin");
        radioButton_admin->setGeometry(QRect(130, 370, 100, 30));
        radioButton_admin->setMinimumSize(QSize(100, 30));
        radioButton_admin->setMaximumSize(QSize(100, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(12);
        radioButton_admin->setFont(font1);
        radioButton_teacher = new QRadioButton(RegisterDialog);
        radioButton_teacher->setObjectName("radioButton_teacher");
        radioButton_teacher->setGeometry(QRect(290, 370, 100, 30));
        radioButton_teacher->setMinimumSize(QSize(100, 30));
        radioButton_teacher->setMaximumSize(QSize(100, 30));
        radioButton_teacher->setFont(font1);
        layoutWidget = new QWidget(RegisterDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(80, 410, 361, 41));
        layoutWidget->setFont(font1);
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_register = new QPushButton(layoutWidget);
        pushButton_register->setObjectName("pushButton_register");
        pushButton_register->setMinimumSize(QSize(90, 30));
        pushButton_register->setMaximumSize(QSize(90, 30));
        pushButton_register->setFont(font1);

        horizontalLayout->addWidget(pushButton_register);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_back = new QPushButton(layoutWidget);
        pushButton_back->setObjectName("pushButton_back");
        pushButton_back->setMinimumSize(QSize(90, 30));
        pushButton_back->setMaximumSize(QSize(90, 30));
        pushButton_back->setFont(font1);

        horizontalLayout->addWidget(pushButton_back);

        layoutWidget1 = new QWidget(RegisterDialog);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(40, 60, 441, 301));
        layoutWidget1->setFont(font1);
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(90, 30));
        label_2->setMaximumSize(QSize(90, 30));
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        lineEdit_username = new QLineEdit(layoutWidget1);
        lineEdit_username->setObjectName("lineEdit_username");
        lineEdit_username->setMinimumSize(QSize(300, 30));
        lineEdit_username->setMaximumSize(QSize(300, 30));
        lineEdit_username->setFont(font1);

        gridLayout->addWidget(lineEdit_username, 0, 1, 1, 1);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(90, 30));
        label_3->setMaximumSize(QSize(90, 30));
        label_3->setFont(font1);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        lineEdit_password = new QLineEdit(layoutWidget1);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setMinimumSize(QSize(300, 30));
        lineEdit_password->setMaximumSize(QSize(300, 30));
        lineEdit_password->setFont(font1);
        lineEdit_password->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(lineEdit_password, 1, 1, 1, 1);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(90, 30));
        label_4->setMaximumSize(QSize(90, 30));
        label_4->setFont(font1);

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        lineEdit_confirm = new QLineEdit(layoutWidget1);
        lineEdit_confirm->setObjectName("lineEdit_confirm");
        lineEdit_confirm->setMinimumSize(QSize(300, 30));
        lineEdit_confirm->setMaximumSize(QSize(300, 30));
        lineEdit_confirm->setFont(font1);
        lineEdit_confirm->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(lineEdit_confirm, 2, 1, 1, 1);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName("label_5");
        label_5->setMinimumSize(QSize(90, 30));
        label_5->setMaximumSize(QSize(90, 30));
        label_5->setFont(font1);

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        lineEdit_question1 = new QLineEdit(layoutWidget1);
        lineEdit_question1->setObjectName("lineEdit_question1");
        lineEdit_question1->setMinimumSize(QSize(300, 30));
        lineEdit_question1->setMaximumSize(QSize(300, 30));
        lineEdit_question1->setFont(font1);

        gridLayout->addWidget(lineEdit_question1, 3, 1, 1, 1);

        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName("label_6");
        label_6->setMinimumSize(QSize(90, 30));
        label_6->setMaximumSize(QSize(90, 30));
        label_6->setFont(font1);

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        lineEdit_answer1 = new QLineEdit(layoutWidget1);
        lineEdit_answer1->setObjectName("lineEdit_answer1");
        lineEdit_answer1->setMinimumSize(QSize(300, 30));
        lineEdit_answer1->setMaximumSize(QSize(300, 30));
        lineEdit_answer1->setFont(font1);

        gridLayout->addWidget(lineEdit_answer1, 4, 1, 1, 1);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName("label_8");
        label_8->setMinimumSize(QSize(90, 30));
        label_8->setMaximumSize(QSize(90, 30));
        label_8->setFont(font1);

        gridLayout->addWidget(label_8, 5, 0, 1, 1);

        lineEdit_question2 = new QLineEdit(layoutWidget1);
        lineEdit_question2->setObjectName("lineEdit_question2");
        lineEdit_question2->setMinimumSize(QSize(300, 30));
        lineEdit_question2->setMaximumSize(QSize(300, 30));
        lineEdit_question2->setFont(font1);

        gridLayout->addWidget(lineEdit_question2, 5, 1, 1, 1);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName("label_7");
        label_7->setMinimumSize(QSize(90, 30));
        label_7->setMaximumSize(QSize(90, 30));
        label_7->setFont(font1);

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        lineEdit_answer2 = new QLineEdit(layoutWidget1);
        lineEdit_answer2->setObjectName("lineEdit_answer2");
        lineEdit_answer2->setMinimumSize(QSize(300, 30));
        lineEdit_answer2->setMaximumSize(QSize(300, 30));
        lineEdit_answer2->setFont(font1);

        gridLayout->addWidget(lineEdit_answer2, 6, 1, 1, 1);

        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName("label_9");
        label_9->setMinimumSize(QSize(90, 30));
        label_9->setMaximumSize(QSize(90, 30));
        label_9->setFont(font1);

        gridLayout->addWidget(label_9, 7, 0, 1, 1);

        lineEdit_teacherName = new QLineEdit(layoutWidget1);
        lineEdit_teacherName->setObjectName("lineEdit_teacherName");
        lineEdit_teacherName->setMinimumSize(QSize(300, 30));
        lineEdit_teacherName->setMaximumSize(QSize(300, 30));
        lineEdit_teacherName->setFont(font1);

        gridLayout->addWidget(lineEdit_teacherName, 7, 1, 1, 1);

        QWidget::setTabOrder(lineEdit_username, lineEdit_password);
        QWidget::setTabOrder(lineEdit_password, lineEdit_confirm);
        QWidget::setTabOrder(lineEdit_confirm, lineEdit_question1);
        QWidget::setTabOrder(lineEdit_question1, lineEdit_answer1);
        QWidget::setTabOrder(lineEdit_answer1, lineEdit_question2);
        QWidget::setTabOrder(lineEdit_question2, lineEdit_answer2);
        QWidget::setTabOrder(lineEdit_answer2, lineEdit_teacherName);
        QWidget::setTabOrder(lineEdit_teacherName, radioButton_admin);
        QWidget::setTabOrder(radioButton_admin, radioButton_teacher);
        QWidget::setTabOrder(radioButton_teacher, pushButton_register);
        QWidget::setTabOrder(pushButton_register, pushButton_back);

        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        label_1->setText(QCoreApplication::translate("RegisterDialog", "\345\255\246\347\224\237\347\256\241\347\220\206\346\263\250\345\206\214\347\263\273\347\273\237", nullptr));
        radioButton_admin->setText(QCoreApplication::translate("RegisterDialog", "\347\256\241\347\220\206\345\221\230", nullptr));
        radioButton_teacher->setText(QCoreApplication::translate("RegisterDialog", "\346\231\256\351\200\232\346\225\231\345\270\210", nullptr));
        pushButton_register->setText(QCoreApplication::translate("RegisterDialog", "\346\263\250\345\206\214", nullptr));
        pushButton_back->setText(QCoreApplication::translate("RegisterDialog", "\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
        label_2->setText(QCoreApplication::translate("RegisterDialog", "\350\264\246      \345\217\267\357\274\232", nullptr));
        lineEdit_username->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "4--20\344\275\215", nullptr));
        label_3->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206      \347\240\201\357\274\232", nullptr));
        lineEdit_password->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "\345\255\227\346\257\215\343\200\201\346\225\260\345\255\227\343\200\201\347\254\246\345\217\267\350\207\263\345\260\221\344\270\244\347\247\215\357\274\2148--40\344\275\215", nullptr));
        label_4->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        lineEdit_confirm->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "\350\257\267\345\206\215\346\254\241\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        label_5->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\344\277\235\351\227\256\351\242\2301\357\274\232", nullptr));
        lineEdit_question1->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "\344\276\213\345\246\202\357\274\232\344\275\240\346\234\200\345\226\234\346\254\242\347\232\204\351\242\234\350\211\262\346\230\257\344\273\200\344\271\210\357\274\237", nullptr));
        label_6->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\344\277\235\347\255\224\346\241\2101\357\274\232", nullptr));
        lineEdit_answer1->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "\350\257\267\350\276\223\345\205\245\347\255\224\346\241\2101", nullptr));
        label_8->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\344\277\235\351\227\256\351\242\2302\357\274\232", nullptr));
        lineEdit_question2->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "\344\276\213\345\246\202\357\274\232\344\275\240\347\254\254\344\270\200\345\217\252\345\256\240\347\211\251\347\232\204\345\220\215\345\255\227\346\230\257\344\273\200\344\271\210\357\274\237", nullptr));
        label_7->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\344\277\235\347\255\224\346\241\2102\357\274\232", nullptr));
        lineEdit_answer2->setPlaceholderText(QCoreApplication::translate("RegisterDialog", "\350\257\267\350\276\223\345\205\245\347\255\224\346\241\2102", nullptr));
        label_9->setText(QCoreApplication::translate("RegisterDialog", "\346\225\231\345\270\210\345\247\223\345\220\215\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
