/********************************************************************************
** Form generated from reading UI file 'editstudentdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITSTUDENTDIALOG_H
#define UI_EDITSTUDENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditStudentDialog
{
public:
    QLabel *label_5;
    QLineEdit *lineEdit_name;
    QLabel *label_3;
    QComboBox *comboBox_gender;
    QLineEdit *lineEdit_phone;
    QSpinBox *spinBox_age;
    QLabel *label;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_4;
    QLineEdit *lineEdit_address;
    QLineEdit *lineEdit_studentId;
    QLabel *label_7;
    QLabel *label_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_confirm;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;
    QComboBox *comboBox_class;

    void setupUi(QDialog *EditStudentDialog)
    {
        if (EditStudentDialog->objectName().isEmpty())
            EditStudentDialog->setObjectName("EditStudentDialog");
        EditStudentDialog->resize(520, 420);
        EditStudentDialog->setMinimumSize(QSize(520, 420));
        EditStudentDialog->setMaximumSize(QSize(520, 420));
        label_5 = new QLabel(EditStudentDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(50, 190, 90, 30));
        label_5->setMinimumSize(QSize(90, 30));
        label_5->setMaximumSize(QSize(90, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        label_5->setFont(font);
        lineEdit_name = new QLineEdit(EditStudentDialog);
        lineEdit_name->setObjectName("lineEdit_name");
        lineEdit_name->setGeometry(QRect(150, 110, 300, 30));
        lineEdit_name->setMinimumSize(QSize(300, 30));
        lineEdit_name->setMaximumSize(QSize(300, 30));
        lineEdit_name->setFont(font);
        label_3 = new QLabel(EditStudentDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 110, 90, 30));
        label_3->setMinimumSize(QSize(90, 30));
        label_3->setMaximumSize(QSize(90, 30));
        label_3->setFont(font);
        comboBox_gender = new QComboBox(EditStudentDialog);
        comboBox_gender->addItem(QString());
        comboBox_gender->addItem(QString());
        comboBox_gender->addItem(QString());
        comboBox_gender->setObjectName("comboBox_gender");
        comboBox_gender->setGeometry(QRect(150, 150, 300, 30));
        comboBox_gender->setMinimumSize(QSize(300, 30));
        comboBox_gender->setMaximumSize(QSize(300, 30));
        comboBox_gender->setFont(font);
        lineEdit_phone = new QLineEdit(EditStudentDialog);
        lineEdit_phone->setObjectName("lineEdit_phone");
        lineEdit_phone->setGeometry(QRect(150, 270, 300, 30));
        lineEdit_phone->setMinimumSize(QSize(300, 30));
        lineEdit_phone->setMaximumSize(QSize(300, 30));
        lineEdit_phone->setFont(font);
        spinBox_age = new QSpinBox(EditStudentDialog);
        spinBox_age->setObjectName("spinBox_age");
        spinBox_age->setGeometry(QRect(150, 190, 300, 30));
        spinBox_age->setMinimumSize(QSize(300, 30));
        spinBox_age->setMaximumSize(QSize(300, 30));
        spinBox_age->setFont(font);
        spinBox_age->setMaximum(100);
        label = new QLabel(EditStudentDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 20, 300, 30));
        label->setMinimumSize(QSize(300, 30));
        label->setMaximumSize(QSize(300, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        label->setFont(font1);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_6 = new QLabel(EditStudentDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 230, 90, 30));
        label_6->setMinimumSize(QSize(90, 30));
        label_6->setMaximumSize(QSize(90, 30));
        label_6->setFont(font);
        label_8 = new QLabel(EditStudentDialog);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(50, 310, 90, 30));
        label_8->setMinimumSize(QSize(90, 30));
        label_8->setMaximumSize(QSize(90, 30));
        label_8->setFont(font);
        label_4 = new QLabel(EditStudentDialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(50, 150, 90, 30));
        label_4->setMinimumSize(QSize(90, 30));
        label_4->setMaximumSize(QSize(90, 30));
        label_4->setFont(font);
        lineEdit_address = new QLineEdit(EditStudentDialog);
        lineEdit_address->setObjectName("lineEdit_address");
        lineEdit_address->setGeometry(QRect(150, 310, 300, 30));
        lineEdit_address->setMinimumSize(QSize(300, 30));
        lineEdit_address->setMaximumSize(QSize(300, 30));
        lineEdit_address->setFont(font);
        lineEdit_studentId = new QLineEdit(EditStudentDialog);
        lineEdit_studentId->setObjectName("lineEdit_studentId");
        lineEdit_studentId->setGeometry(QRect(150, 70, 300, 30));
        lineEdit_studentId->setMinimumSize(QSize(300, 30));
        lineEdit_studentId->setMaximumSize(QSize(300, 30));
        lineEdit_studentId->setFont(font);
        label_7 = new QLabel(EditStudentDialog);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(50, 270, 90, 30));
        label_7->setMinimumSize(QSize(90, 30));
        label_7->setMaximumSize(QSize(90, 30));
        label_7->setFont(font);
        label_2 = new QLabel(EditStudentDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 70, 90, 30));
        label_2->setMinimumSize(QSize(90, 30));
        label_2->setMaximumSize(QSize(90, 30));
        label_2->setFont(font);
        layoutWidget = new QWidget(EditStudentDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(90, 360, 341, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_confirm = new QPushButton(layoutWidget);
        pushButton_confirm->setObjectName("pushButton_confirm");
        pushButton_confirm->setMinimumSize(QSize(90, 30));
        pushButton_confirm->setMaximumSize(QSize(90, 30));
        pushButton_confirm->setFont(font);

        horizontalLayout->addWidget(pushButton_confirm);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font);

        horizontalLayout->addWidget(pushButton_cancel);

        comboBox_class = new QComboBox(EditStudentDialog);
        comboBox_class->setObjectName("comboBox_class");
        comboBox_class->setGeometry(QRect(150, 230, 300, 30));
        comboBox_class->setMinimumSize(QSize(300, 30));
        comboBox_class->setMaximumSize(QSize(300, 30));
        comboBox_class->setFont(font);
        QWidget::setTabOrder(lineEdit_studentId, lineEdit_name);
        QWidget::setTabOrder(lineEdit_name, comboBox_gender);
        QWidget::setTabOrder(comboBox_gender, spinBox_age);
        QWidget::setTabOrder(spinBox_age, comboBox_class);
        QWidget::setTabOrder(comboBox_class, lineEdit_phone);
        QWidget::setTabOrder(lineEdit_phone, lineEdit_address);
        QWidget::setTabOrder(lineEdit_address, pushButton_confirm);
        QWidget::setTabOrder(pushButton_confirm, pushButton_cancel);

        retranslateUi(EditStudentDialog);

        QMetaObject::connectSlotsByName(EditStudentDialog);
    } // setupUi

    void retranslateUi(QDialog *EditStudentDialog)
    {
        EditStudentDialog->setWindowTitle(QCoreApplication::translate("EditStudentDialog", "Dialog", nullptr));
        label_5->setText(QCoreApplication::translate("EditStudentDialog", "\345\271\264\351\276\204\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("EditStudentDialog", "\345\247\223\345\220\215\357\274\232", nullptr));
        comboBox_gender->setItemText(0, QCoreApplication::translate("EditStudentDialog", "\347\224\267", nullptr));
        comboBox_gender->setItemText(1, QCoreApplication::translate("EditStudentDialog", "\345\245\263", nullptr));
        comboBox_gender->setItemText(2, QCoreApplication::translate("EditStudentDialog", "\345\205\266\344\273\226", nullptr));

        label->setText(QCoreApplication::translate("EditStudentDialog", "\344\277\256\346\224\271\345\255\246\347\224\237", nullptr));
        label_6->setText(QCoreApplication::translate("EditStudentDialog", "\347\217\255\347\272\247\357\274\232", nullptr));
        label_8->setText(QCoreApplication::translate("EditStudentDialog", "\345\234\260\345\235\200\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("EditStudentDialog", "\346\200\247\345\210\253\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("EditStudentDialog", "\347\224\265\350\257\235\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("EditStudentDialog", "\345\255\246\345\217\267\357\274\232", nullptr));
        pushButton_confirm->setText(QCoreApplication::translate("EditStudentDialog", "\347\241\256\350\256\244\344\277\256\346\224\271", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("EditStudentDialog", "\345\217\226\346\266\210\344\277\256\346\224\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditStudentDialog: public Ui_EditStudentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITSTUDENTDIALOG_H
