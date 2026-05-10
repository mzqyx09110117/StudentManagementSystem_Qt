/********************************************************************************
** Form generated from reading UI file 'changequestionsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEQUESTIONSDIALOG_H
#define UI_CHANGEQUESTIONSDIALOG_H

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

class Ui_ChangeQuestionsDialog
{
public:
    QLabel *label;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_confirmquestions;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;
    QLineEdit *lineEdit_question1;
    QLabel *label_5;
    QLineEdit *lineEdit_answer1;
    QLineEdit *lineEdit_question2;
    QLabel *label_8;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *lineEdit_answer2;

    void setupUi(QDialog *ChangeQuestionsDialog)
    {
        if (ChangeQuestionsDialog->objectName().isEmpty())
            ChangeQuestionsDialog->setObjectName("ChangeQuestionsDialog");
        ChangeQuestionsDialog->resize(520, 400);
        ChangeQuestionsDialog->setMinimumSize(QSize(520, 400));
        ChangeQuestionsDialog->setMaximumSize(QSize(520, 400));
        label = new QLabel(ChangeQuestionsDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 30, 300, 30));
        label->setMinimumSize(QSize(300, 30));
        label->setMaximumSize(QSize(300, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(16);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        layoutWidget = new QWidget(ChangeQuestionsDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(90, 320, 341, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_confirmquestions = new QPushButton(layoutWidget);
        pushButton_confirmquestions->setObjectName("pushButton_confirmquestions");
        pushButton_confirmquestions->setMinimumSize(QSize(90, 30));
        pushButton_confirmquestions->setMaximumSize(QSize(90, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(12);
        pushButton_confirmquestions->setFont(font1);

        horizontalLayout->addWidget(pushButton_confirmquestions);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font1);

        horizontalLayout->addWidget(pushButton_cancel);

        lineEdit_question1 = new QLineEdit(ChangeQuestionsDialog);
        lineEdit_question1->setObjectName("lineEdit_question1");
        lineEdit_question1->setGeometry(QRect(169, 100, 300, 30));
        lineEdit_question1->setMinimumSize(QSize(300, 30));
        lineEdit_question1->setMaximumSize(QSize(300, 30));
        lineEdit_question1->setFont(font1);
        label_5 = new QLabel(ChangeQuestionsDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(49, 100, 90, 30));
        label_5->setMinimumSize(QSize(90, 30));
        label_5->setMaximumSize(QSize(90, 30));
        label_5->setFont(font1);
        lineEdit_answer1 = new QLineEdit(ChangeQuestionsDialog);
        lineEdit_answer1->setObjectName("lineEdit_answer1");
        lineEdit_answer1->setGeometry(QRect(170, 150, 300, 30));
        lineEdit_answer1->setMinimumSize(QSize(300, 30));
        lineEdit_answer1->setMaximumSize(QSize(300, 30));
        lineEdit_answer1->setFont(font1);
        lineEdit_question2 = new QLineEdit(ChangeQuestionsDialog);
        lineEdit_question2->setObjectName("lineEdit_question2");
        lineEdit_question2->setGeometry(QRect(169, 200, 300, 30));
        lineEdit_question2->setMinimumSize(QSize(300, 30));
        lineEdit_question2->setMaximumSize(QSize(300, 30));
        lineEdit_question2->setFont(font1);
        label_8 = new QLabel(ChangeQuestionsDialog);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(49, 200, 90, 30));
        label_8->setMinimumSize(QSize(90, 30));
        label_8->setMaximumSize(QSize(90, 30));
        label_8->setFont(font1);
        label_6 = new QLabel(ChangeQuestionsDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 150, 90, 30));
        label_6->setMinimumSize(QSize(90, 30));
        label_6->setMaximumSize(QSize(90, 30));
        label_6->setFont(font1);
        label_7 = new QLabel(ChangeQuestionsDialog);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(50, 250, 90, 30));
        label_7->setMinimumSize(QSize(90, 30));
        label_7->setMaximumSize(QSize(90, 30));
        label_7->setFont(font1);
        lineEdit_answer2 = new QLineEdit(ChangeQuestionsDialog);
        lineEdit_answer2->setObjectName("lineEdit_answer2");
        lineEdit_answer2->setGeometry(QRect(170, 250, 300, 30));
        lineEdit_answer2->setMinimumSize(QSize(300, 30));
        lineEdit_answer2->setMaximumSize(QSize(300, 30));
        lineEdit_answer2->setFont(font1);

        retranslateUi(ChangeQuestionsDialog);

        QMetaObject::connectSlotsByName(ChangeQuestionsDialog);
    } // setupUi

    void retranslateUi(QDialog *ChangeQuestionsDialog)
    {
        ChangeQuestionsDialog->setWindowTitle(QCoreApplication::translate("ChangeQuestionsDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ChangeQuestionsDialog", "\344\277\256\346\224\271\345\257\206\344\277\235\351\227\256\351\242\230", nullptr));
        pushButton_confirmquestions->setText(QCoreApplication::translate("ChangeQuestionsDialog", "\347\241\256\350\256\244\344\277\256\346\224\271", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("ChangeQuestionsDialog", "\345\217\226\346\266\210\344\277\256\346\224\271", nullptr));
        lineEdit_question1->setPlaceholderText(QCoreApplication::translate("ChangeQuestionsDialog", "\344\276\213\345\246\202\357\274\232\344\275\240\346\234\200\345\226\234\346\254\242\347\232\204\351\242\234\350\211\262\346\230\257\344\273\200\344\271\210\357\274\237", nullptr));
        label_5->setText(QCoreApplication::translate("ChangeQuestionsDialog", "\345\257\206\344\277\235\351\227\256\351\242\2301\357\274\232", nullptr));
        lineEdit_answer1->setPlaceholderText(QCoreApplication::translate("ChangeQuestionsDialog", "\350\257\267\350\276\223\345\205\245\347\255\224\346\241\2101", nullptr));
        lineEdit_question2->setPlaceholderText(QCoreApplication::translate("ChangeQuestionsDialog", "\344\276\213\345\246\202\357\274\232\344\275\240\347\254\254\344\270\200\345\217\252\345\256\240\347\211\251\347\232\204\345\220\215\345\255\227\346\230\257\344\273\200\344\271\210\357\274\237", nullptr));
        label_8->setText(QCoreApplication::translate("ChangeQuestionsDialog", "\345\257\206\344\277\235\351\227\256\351\242\2302\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("ChangeQuestionsDialog", "\345\257\206\344\277\235\347\255\224\346\241\2101\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("ChangeQuestionsDialog", "\345\257\206\344\277\235\347\255\224\346\241\2102\357\274\232", nullptr));
        lineEdit_answer2->setPlaceholderText(QCoreApplication::translate("ChangeQuestionsDialog", "\350\257\267\350\276\223\345\205\245\347\255\224\346\241\2102", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChangeQuestionsDialog: public Ui_ChangeQuestionsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEQUESTIONSDIALOG_H
