/********************************************************************************
** Form generated from reading UI file 'subjectdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBJECTDIALOG_H
#define UI_SUBJECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SubjectDialog
{
public:
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_subjectName;
    QLabel *label_5;
    QLineEdit *lineEdit_subjectId;
    QDoubleSpinBox *doubleSpinBox_credit;
    QPushButton *pushButton_save;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_delete;
    QLabel *label_6;

    void setupUi(QDialog *SubjectDialog)
    {
        if (SubjectDialog->objectName().isEmpty())
            SubjectDialog->setObjectName("SubjectDialog");
        SubjectDialog->resize(400, 300);
        SubjectDialog->setMinimumSize(QSize(400, 300));
        SubjectDialog->setMaximumSize(QSize(400, 300));
        label_3 = new QLabel(SubjectDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 120, 80, 30));
        label_3->setMinimumSize(QSize(80, 30));
        label_3->setMaximumSize(QSize(80, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        label_4 = new QLabel(SubjectDialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(50, 160, 80, 30));
        label_4->setMinimumSize(QSize(80, 30));
        label_4->setMaximumSize(QSize(80, 30));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        lineEdit_subjectName = new QLineEdit(SubjectDialog);
        lineEdit_subjectName->setObjectName("lineEdit_subjectName");
        lineEdit_subjectName->setGeometry(QRect(150, 120, 200, 30));
        lineEdit_subjectName->setMinimumSize(QSize(200, 30));
        lineEdit_subjectName->setMaximumSize(QSize(200, 30));
        lineEdit_subjectName->setFont(font);
        label_5 = new QLabel(SubjectDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(50, 80, 80, 30));
        label_5->setMinimumSize(QSize(80, 30));
        label_5->setMaximumSize(QSize(80, 30));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        lineEdit_subjectId = new QLineEdit(SubjectDialog);
        lineEdit_subjectId->setObjectName("lineEdit_subjectId");
        lineEdit_subjectId->setGeometry(QRect(150, 80, 200, 30));
        lineEdit_subjectId->setMinimumSize(QSize(200, 30));
        lineEdit_subjectId->setMaximumSize(QSize(200, 30));
        lineEdit_subjectId->setFont(font);
        doubleSpinBox_credit = new QDoubleSpinBox(SubjectDialog);
        doubleSpinBox_credit->setObjectName("doubleSpinBox_credit");
        doubleSpinBox_credit->setGeometry(QRect(150, 160, 201, 31));
        doubleSpinBox_credit->setFont(font);
        pushButton_save = new QPushButton(SubjectDialog);
        pushButton_save->setObjectName("pushButton_save");
        pushButton_save->setGeometry(QRect(70, 230, 90, 30));
        pushButton_save->setMinimumSize(QSize(90, 30));
        pushButton_save->setMaximumSize(QSize(90, 30));
        pushButton_save->setFont(font);
        pushButton_cancel = new QPushButton(SubjectDialog);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setGeometry(QRect(240, 230, 90, 30));
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font);
        pushButton_delete = new QPushButton(SubjectDialog);
        pushButton_delete->setObjectName("pushButton_delete");
        pushButton_delete->setGeometry(QRect(70, 230, 90, 30));
        pushButton_delete->setMinimumSize(QSize(90, 30));
        pushButton_delete->setMaximumSize(QSize(90, 30));
        pushButton_delete->setFont(font);
        label_6 = new QLabel(SubjectDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 20, 300, 30));
        label_6->setMinimumSize(QSize(300, 30));
        label_6->setMaximumSize(QSize(300, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);
        QWidget::setTabOrder(lineEdit_subjectId, lineEdit_subjectName);
        QWidget::setTabOrder(lineEdit_subjectName, doubleSpinBox_credit);
        QWidget::setTabOrder(doubleSpinBox_credit, pushButton_delete);
        QWidget::setTabOrder(pushButton_delete, pushButton_save);
        QWidget::setTabOrder(pushButton_save, pushButton_cancel);

        retranslateUi(SubjectDialog);

        QMetaObject::connectSlotsByName(SubjectDialog);
    } // setupUi

    void retranslateUi(QDialog *SubjectDialog)
    {
        SubjectDialog->setWindowTitle(QCoreApplication::translate("SubjectDialog", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("SubjectDialog", "\350\257\276\347\250\213\345\220\215\347\247\260\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("SubjectDialog", "\350\257\276\347\250\213\345\255\246\345\210\206\357\274\232", nullptr));
        lineEdit_subjectName->setPlaceholderText(QCoreApplication::translate("SubjectDialog", "\350\257\267\350\276\223\345\205\245\350\257\276\347\250\213\345\220\215\347\247\260", nullptr));
        label_5->setText(QCoreApplication::translate("SubjectDialog", "\350\257\276\347\250\213\347\274\226\345\217\267\357\274\232", nullptr));
        lineEdit_subjectId->setPlaceholderText(QCoreApplication::translate("SubjectDialog", "\350\257\267\350\276\223\345\205\245\350\257\276\347\250\213\347\274\226\345\217\267", nullptr));
        pushButton_save->setText(QCoreApplication::translate("SubjectDialog", "\347\241\256\350\256\244\346\267\273\345\212\240", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("SubjectDialog", "\345\217\226\346\266\210", nullptr));
        pushButton_delete->setText(QCoreApplication::translate("SubjectDialog", "\347\241\256\350\256\244\345\210\240\351\231\244", nullptr));
        label_6->setText(QCoreApplication::translate("SubjectDialog", "\346\267\273\345\212\240\344\277\256\346\224\271\345\210\240\351\231\244\350\257\276\347\250\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SubjectDialog: public Ui_SubjectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBJECTDIALOG_H
