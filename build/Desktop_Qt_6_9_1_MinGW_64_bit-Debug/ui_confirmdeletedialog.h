/********************************************************************************
** Form generated from reading UI file 'confirmdeletedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIRMDELETEDIALOG_H
#define UI_CONFIRMDELETEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfirmDeleteDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_studentId;
    QLabel *label_studentName;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_confirm;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *ConfirmDeleteDialog)
    {
        if (ConfirmDeleteDialog->objectName().isEmpty())
            ConfirmDeleteDialog->setObjectName("ConfirmDeleteDialog");
        ConfirmDeleteDialog->resize(400, 300);
        ConfirmDeleteDialog->setMinimumSize(QSize(400, 300));
        ConfirmDeleteDialog->setMaximumSize(QSize(400, 300));
        label = new QLabel(ConfirmDeleteDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 70, 300, 30));
        label->setMinimumSize(QSize(300, 30));
        label->setMaximumSize(QSize(300, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(ConfirmDeleteDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 20, 300, 30));
        label_2->setMinimumSize(QSize(300, 30));
        label_2->setMaximumSize(QSize(300, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_3 = new QLabel(ConfirmDeleteDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 120, 90, 30));
        label_3->setMinimumSize(QSize(90, 30));
        label_3->setMaximumSize(QSize(90, 30));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_4 = new QLabel(ConfirmDeleteDialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 170, 90, 30));
        label_4->setMinimumSize(QSize(90, 30));
        label_4->setMaximumSize(QSize(90, 30));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_studentId = new QLabel(ConfirmDeleteDialog);
        label_studentId->setObjectName("label_studentId");
        label_studentId->setGeometry(QRect(150, 120, 200, 30));
        label_studentId->setMinimumSize(QSize(200, 30));
        label_studentId->setMaximumSize(QSize(200, 30));
        label_studentId->setFont(font);
        label_studentId->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_studentName = new QLabel(ConfirmDeleteDialog);
        label_studentName->setObjectName("label_studentName");
        label_studentName->setGeometry(QRect(150, 170, 200, 30));
        label_studentName->setMinimumSize(QSize(200, 30));
        label_studentName->setMaximumSize(QSize(200, 30));
        label_studentName->setFont(font);
        label_studentName->setAlignment(Qt::AlignmentFlag::AlignCenter);
        widget = new QWidget(ConfirmDeleteDialog);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(60, 240, 281, 32));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_confirm = new QPushButton(widget);
        pushButton_confirm->setObjectName("pushButton_confirm");
        pushButton_confirm->setMinimumSize(QSize(90, 30));
        pushButton_confirm->setMaximumSize(QSize(90, 30));
        pushButton_confirm->setFont(font);

        horizontalLayout->addWidget(pushButton_confirm);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(widget);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font);

        horizontalLayout->addWidget(pushButton_cancel);


        retranslateUi(ConfirmDeleteDialog);

        QMetaObject::connectSlotsByName(ConfirmDeleteDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfirmDeleteDialog)
    {
        ConfirmDeleteDialog->setWindowTitle(QCoreApplication::translate("ConfirmDeleteDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ConfirmDeleteDialog", "\350\257\267\347\241\256\350\256\244\346\230\257\345\220\246\350\246\201\345\210\240\351\231\244\350\257\245\345\255\246\347\224\237", nullptr));
        label_2->setText(QCoreApplication::translate("ConfirmDeleteDialog", "\345\210\240\351\231\244\345\255\246\347\224\237", nullptr));
        label_3->setText(QCoreApplication::translate("ConfirmDeleteDialog", "\345\255\246\345\217\267\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("ConfirmDeleteDialog", "\345\247\223\345\220\215\357\274\232", nullptr));
        label_studentId->setText(QString());
        label_studentName->setText(QString());
        pushButton_confirm->setText(QCoreApplication::translate("ConfirmDeleteDialog", "\347\241\256\350\256\244\345\210\240\351\231\244", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("ConfirmDeleteDialog", "\345\217\226\346\266\210\345\210\240\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfirmDeleteDialog: public Ui_ConfirmDeleteDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMDELETEDIALOG_H
