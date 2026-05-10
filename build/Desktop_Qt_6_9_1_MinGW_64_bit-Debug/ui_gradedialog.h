/********************************************************************************
** Form generated from reading UI file 'gradedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRADEDIALOG_H
#define UI_GRADEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GradeDialog
{
public:
    QPushButton *pushButton_delete;
    QPushButton *pushButton_save;
    QPushButton *pushButton_cancel;
    QComboBox *comboBox_student;
    QComboBox *comboBox_subject;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_semester;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_score;
    QDateEdit *dateEdit_examDate;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QDialog *GradeDialog)
    {
        if (GradeDialog->objectName().isEmpty())
            GradeDialog->setObjectName("GradeDialog");
        GradeDialog->resize(550, 463);
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        GradeDialog->setFont(font);
        pushButton_delete = new QPushButton(GradeDialog);
        pushButton_delete->setObjectName("pushButton_delete");
        pushButton_delete->setGeometry(QRect(130, 370, 90, 30));
        pushButton_delete->setMinimumSize(QSize(90, 30));
        pushButton_delete->setMaximumSize(QSize(90, 30));
        pushButton_delete->setFont(font);
        pushButton_save = new QPushButton(GradeDialog);
        pushButton_save->setObjectName("pushButton_save");
        pushButton_save->setGeometry(QRect(130, 370, 90, 30));
        pushButton_save->setMinimumSize(QSize(90, 30));
        pushButton_save->setMaximumSize(QSize(90, 30));
        pushButton_save->setFont(font);
        pushButton_cancel = new QPushButton(GradeDialog);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setGeometry(QRect(300, 370, 90, 30));
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font);
        comboBox_student = new QComboBox(GradeDialog);
        comboBox_student->setObjectName("comboBox_student");
        comboBox_student->setGeometry(QRect(180, 80, 300, 30));
        comboBox_student->setMinimumSize(QSize(300, 30));
        comboBox_student->setMaximumSize(QSize(300, 30));
        comboBox_subject = new QComboBox(GradeDialog);
        comboBox_subject->setObjectName("comboBox_subject");
        comboBox_subject->setGeometry(QRect(180, 130, 300, 30));
        comboBox_subject->setMinimumSize(QSize(300, 30));
        comboBox_subject->setMaximumSize(QSize(300, 30));
        label = new QLabel(GradeDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(50, 80, 90, 30));
        label->setMinimumSize(QSize(90, 30));
        label->setMaximumSize(QSize(90, 30));
        label_2 = new QLabel(GradeDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 130, 90, 30));
        label_2->setMinimumSize(QSize(90, 30));
        label_2->setMaximumSize(QSize(90, 30));
        label_3 = new QLabel(GradeDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 230, 90, 30));
        label_3->setMinimumSize(QSize(90, 30));
        label_3->setMaximumSize(QSize(90, 30));
        lineEdit_semester = new QLineEdit(GradeDialog);
        lineEdit_semester->setObjectName("lineEdit_semester");
        lineEdit_semester->setGeometry(QRect(180, 230, 300, 30));
        lineEdit_semester->setMinimumSize(QSize(300, 30));
        lineEdit_semester->setMaximumSize(QSize(300, 30));
        label_4 = new QLabel(GradeDialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(50, 180, 90, 30));
        label_4->setMinimumSize(QSize(90, 30));
        label_4->setMaximumSize(QSize(90, 30));
        doubleSpinBox_score = new QDoubleSpinBox(GradeDialog);
        doubleSpinBox_score->setObjectName("doubleSpinBox_score");
        doubleSpinBox_score->setGeometry(QRect(180, 180, 300, 30));
        doubleSpinBox_score->setMinimumSize(QSize(300, 30));
        doubleSpinBox_score->setMaximumSize(QSize(300, 30));
        dateEdit_examDate = new QDateEdit(GradeDialog);
        dateEdit_examDate->setObjectName("dateEdit_examDate");
        dateEdit_examDate->setGeometry(QRect(180, 280, 300, 30));
        dateEdit_examDate->setMinimumSize(QSize(300, 30));
        dateEdit_examDate->setMaximumSize(QSize(300, 30));
        label_5 = new QLabel(GradeDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(50, 280, 90, 30));
        label_5->setMinimumSize(QSize(90, 30));
        label_5->setMaximumSize(QSize(90, 30));
        label_6 = new QLabel(GradeDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(120, 20, 300, 30));
        label_6->setMinimumSize(QSize(300, 30));
        label_6->setMaximumSize(QSize(300, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);
        QWidget::setTabOrder(comboBox_student, comboBox_subject);
        QWidget::setTabOrder(comboBox_subject, doubleSpinBox_score);
        QWidget::setTabOrder(doubleSpinBox_score, lineEdit_semester);
        QWidget::setTabOrder(lineEdit_semester, dateEdit_examDate);
        QWidget::setTabOrder(dateEdit_examDate, pushButton_save);
        QWidget::setTabOrder(pushButton_save, pushButton_delete);
        QWidget::setTabOrder(pushButton_delete, pushButton_cancel);

        retranslateUi(GradeDialog);

        QMetaObject::connectSlotsByName(GradeDialog);
    } // setupUi

    void retranslateUi(QDialog *GradeDialog)
    {
        GradeDialog->setWindowTitle(QCoreApplication::translate("GradeDialog", "Dialog", nullptr));
        pushButton_delete->setText(QCoreApplication::translate("GradeDialog", "\347\241\256\350\256\244\345\210\240\351\231\244", nullptr));
        pushButton_save->setText(QCoreApplication::translate("GradeDialog", "\347\241\256\350\256\244\346\267\273\345\212\240", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("GradeDialog", "\345\217\226\346\266\210", nullptr));
        label->setText(QCoreApplication::translate("GradeDialog", "\345\255\246\347\224\237\345\247\223\345\220\215\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("GradeDialog", "\350\257\276\347\250\213\345\220\215\347\247\260\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("GradeDialog", "\345\275\223\345\211\215\345\255\246\346\234\237\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("GradeDialog", "\350\257\276\347\250\213\346\210\220\347\273\251\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("GradeDialog", "\350\200\203\350\257\225\346\227\245\346\234\237\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("GradeDialog", "\346\267\273\345\212\240\344\277\256\346\224\271\345\210\240\351\231\244\346\210\220\347\273\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GradeDialog: public Ui_GradeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRADEDIALOG_H
