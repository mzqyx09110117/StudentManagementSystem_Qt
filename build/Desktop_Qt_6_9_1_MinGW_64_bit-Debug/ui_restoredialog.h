/********************************************************************************
** Form generated from reading UI file 'restoredialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTOREDIALOG_H
#define UI_RESTOREDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RestoreDialog
{
public:
    QLabel *label_systemDbInfo_2;
    QPushButton *pushButton_analyzeBackup;
    QLabel *label_restoreStatus;
    QLabel *label_5;
    QPlainTextEdit *plainTextEdit_teachers;
    QLabel *label_teacherDbRestore;
    QLabel *label_7;
    QLabel *label_8;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_startRestore;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;
    QPushButton *pushButton_selectBackup;
    QLabel *label_10;
    QLabel *label_backupTime;
    QLabel *label_systemDbRestore;
    QLabel *label_6;
    QLabel *label_9;
    QLineEdit *lineEdit_backupPath;
    QLabel *label_totalRestore;

    void setupUi(QDialog *RestoreDialog)
    {
        if (RestoreDialog->objectName().isEmpty())
            RestoreDialog->setObjectName("RestoreDialog");
        RestoreDialog->resize(600, 600);
        RestoreDialog->setMinimumSize(QSize(600, 600));
        RestoreDialog->setMaximumSize(QSize(600, 600));
        label_systemDbInfo_2 = new QLabel(RestoreDialog);
        label_systemDbInfo_2->setObjectName("label_systemDbInfo_2");
        label_systemDbInfo_2->setGeometry(QRect(150, 30, 300, 30));
        label_systemDbInfo_2->setMinimumSize(QSize(300, 30));
        label_systemDbInfo_2->setMaximumSize(QSize(300, 30));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(16);
        label_systemDbInfo_2->setFont(font);
        label_systemDbInfo_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        pushButton_analyzeBackup = new QPushButton(RestoreDialog);
        pushButton_analyzeBackup->setObjectName("pushButton_analyzeBackup");
        pushButton_analyzeBackup->setGeometry(QRect(470, 100, 90, 30));
        pushButton_analyzeBackup->setMinimumSize(QSize(90, 30));
        pushButton_analyzeBackup->setMaximumSize(QSize(90, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(12);
        pushButton_analyzeBackup->setFont(font1);
        label_restoreStatus = new QLabel(RestoreDialog);
        label_restoreStatus->setObjectName("label_restoreStatus");
        label_restoreStatus->setGeometry(QRect(210, 470, 360, 30));
        label_restoreStatus->setMinimumSize(QSize(360, 30));
        label_restoreStatus->setMaximumSize(QSize(360, 30));
        label_restoreStatus->setFont(font1);
        label_restoreStatus->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_5 = new QLabel(RestoreDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(50, 190, 120, 30));
        label_5->setMinimumSize(QSize(120, 30));
        label_5->setMaximumSize(QSize(120, 30));
        label_5->setFont(font1);
        plainTextEdit_teachers = new QPlainTextEdit(RestoreDialog);
        plainTextEdit_teachers->setObjectName("plainTextEdit_teachers");
        plainTextEdit_teachers->setGeometry(QRect(210, 240, 360, 90));
        plainTextEdit_teachers->setMinimumSize(QSize(360, 90));
        plainTextEdit_teachers->setMaximumSize(QSize(360, 90));
        plainTextEdit_teachers->setFont(font1);
        label_teacherDbRestore = new QLabel(RestoreDialog);
        label_teacherDbRestore->setObjectName("label_teacherDbRestore");
        label_teacherDbRestore->setGeometry(QRect(210, 370, 360, 30));
        label_teacherDbRestore->setMinimumSize(QSize(360, 30));
        label_teacherDbRestore->setMaximumSize(QSize(360, 30));
        label_teacherDbRestore->setFont(font1);
        label_teacherDbRestore->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_7 = new QLabel(RestoreDialog);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(50, 420, 120, 30));
        label_7->setMinimumSize(QSize(120, 30));
        label_7->setMaximumSize(QSize(120, 30));
        label_7->setFont(font1);
        label_8 = new QLabel(RestoreDialog);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(50, 470, 120, 30));
        label_8->setMinimumSize(QSize(120, 30));
        label_8->setMaximumSize(QSize(120, 30));
        label_8->setFont(font1);
        layoutWidget = new QWidget(RestoreDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(90, 530, 421, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_startRestore = new QPushButton(layoutWidget);
        pushButton_startRestore->setObjectName("pushButton_startRestore");
        pushButton_startRestore->setMinimumSize(QSize(90, 30));
        pushButton_startRestore->setMaximumSize(QSize(90, 30));
        pushButton_startRestore->setFont(font1);

        horizontalLayout->addWidget(pushButton_startRestore);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font1);

        horizontalLayout->addWidget(pushButton_cancel);

        pushButton_selectBackup = new QPushButton(RestoreDialog);
        pushButton_selectBackup->setObjectName("pushButton_selectBackup");
        pushButton_selectBackup->setGeometry(QRect(470, 60, 90, 30));
        pushButton_selectBackup->setMinimumSize(QSize(90, 30));
        pushButton_selectBackup->setMaximumSize(QSize(90, 30));
        pushButton_selectBackup->setFont(font1);
        label_10 = new QLabel(RestoreDialog);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(50, 370, 120, 30));
        label_10->setMinimumSize(QSize(120, 30));
        label_10->setMaximumSize(QSize(120, 30));
        label_10->setFont(font1);
        label_backupTime = new QLabel(RestoreDialog);
        label_backupTime->setObjectName("label_backupTime");
        label_backupTime->setGeometry(QRect(210, 140, 360, 30));
        label_backupTime->setMinimumSize(QSize(360, 30));
        label_backupTime->setMaximumSize(QSize(360, 30));
        label_backupTime->setFont(font1);
        label_backupTime->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_systemDbRestore = new QLabel(RestoreDialog);
        label_systemDbRestore->setObjectName("label_systemDbRestore");
        label_systemDbRestore->setGeometry(QRect(210, 190, 360, 30));
        label_systemDbRestore->setMinimumSize(QSize(360, 30));
        label_systemDbRestore->setMaximumSize(QSize(360, 30));
        label_systemDbRestore->setFont(font1);
        label_systemDbRestore->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_6 = new QLabel(RestoreDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 240, 120, 30));
        label_6->setMinimumSize(QSize(120, 30));
        label_6->setMaximumSize(QSize(120, 30));
        label_6->setFont(font1);
        label_9 = new QLabel(RestoreDialog);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(50, 140, 120, 30));
        label_9->setMinimumSize(QSize(120, 30));
        label_9->setMaximumSize(QSize(120, 30));
        label_9->setFont(font1);
        lineEdit_backupPath = new QLineEdit(RestoreDialog);
        lineEdit_backupPath->setObjectName("lineEdit_backupPath");
        lineEdit_backupPath->setGeometry(QRect(50, 80, 400, 30));
        lineEdit_backupPath->setMinimumSize(QSize(400, 30));
        lineEdit_backupPath->setMaximumSize(QSize(400, 30));
        lineEdit_backupPath->setFont(font1);
        label_totalRestore = new QLabel(RestoreDialog);
        label_totalRestore->setObjectName("label_totalRestore");
        label_totalRestore->setGeometry(QRect(210, 420, 360, 30));
        label_totalRestore->setMinimumSize(QSize(360, 30));
        label_totalRestore->setMaximumSize(QSize(360, 30));
        label_totalRestore->setFont(font1);
        label_totalRestore->setAlignment(Qt::AlignmentFlag::AlignCenter);
        QWidget::setTabOrder(lineEdit_backupPath, pushButton_selectBackup);
        QWidget::setTabOrder(pushButton_selectBackup, pushButton_analyzeBackup);
        QWidget::setTabOrder(pushButton_analyzeBackup, plainTextEdit_teachers);
        QWidget::setTabOrder(plainTextEdit_teachers, pushButton_startRestore);
        QWidget::setTabOrder(pushButton_startRestore, pushButton_cancel);

        retranslateUi(RestoreDialog);

        QMetaObject::connectSlotsByName(RestoreDialog);
    } // setupUi

    void retranslateUi(QDialog *RestoreDialog)
    {
        RestoreDialog->setWindowTitle(QCoreApplication::translate("RestoreDialog", "Dialog", nullptr));
        label_systemDbInfo_2->setText(QCoreApplication::translate("RestoreDialog", "\346\201\242\345\244\215\347\263\273\347\273\237", nullptr));
        pushButton_analyzeBackup->setText(QCoreApplication::translate("RestoreDialog", "\345\210\206\346\236\220\345\244\207\344\273\275", nullptr));
        label_restoreStatus->setText(QCoreApplication::translate("RestoreDialog", "\347\255\211\345\276\205\346\223\215\344\275\234...", nullptr));
        label_5->setText(QCoreApplication::translate("RestoreDialog", "\347\263\273\347\273\237\346\225\260\346\215\256\345\272\223\344\277\241\346\201\257\357\274\232", nullptr));
        plainTextEdit_teachers->setPlainText(QCoreApplication::translate("RestoreDialog", "\346\225\231\345\270\210\346\225\260\346\215\256\345\272\223\345\210\227\350\241\250\346\230\276\347\244\272", nullptr));
        label_teacherDbRestore->setText(QCoreApplication::translate("RestoreDialog", "\346\225\231\345\270\210\346\225\260\346\215\256\345\272\223\347\273\237\350\256\241", nullptr));
        label_7->setText(QCoreApplication::translate("RestoreDialog", "\346\200\273\345\244\207\344\273\275\345\244\247\345\260\217\357\274\232", nullptr));
        label_8->setText(QCoreApplication::translate("RestoreDialog", "\347\212\266\346\200\201\357\274\232", nullptr));
        pushButton_startRestore->setText(QCoreApplication::translate("RestoreDialog", "\345\274\200\345\247\213\346\201\242\345\244\215", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("RestoreDialog", "\345\217\226\346\266\210\346\201\242\345\244\215", nullptr));
        pushButton_selectBackup->setText(QCoreApplication::translate("RestoreDialog", "\346\265\217\350\247\210", nullptr));
        label_10->setText(QCoreApplication::translate("RestoreDialog", "\346\225\231\345\270\210\346\225\260\346\215\256\345\272\223\347\273\237\350\256\241\357\274\232", nullptr));
        label_backupTime->setText(QCoreApplication::translate("RestoreDialog", "\345\244\207\344\273\275\346\227\266\351\227\264\346\240\207\347\255\276", nullptr));
        label_systemDbRestore->setText(QCoreApplication::translate("RestoreDialog", "\347\263\273\347\273\237\346\225\260\346\215\256\345\272\223\344\277\241\346\201\257", nullptr));
        label_6->setText(QCoreApplication::translate("RestoreDialog", "\346\225\231\345\270\210\346\225\260\346\215\256\345\272\223\346\230\276\347\244\272\357\274\232", nullptr));
        label_9->setText(QCoreApplication::translate("RestoreDialog", "\345\244\207\344\273\275\346\227\266\351\227\264\346\240\207\347\255\276", nullptr));
        lineEdit_backupPath->setPlaceholderText(QCoreApplication::translate("RestoreDialog", "\345\244\207\344\273\275\346\226\207\344\273\266\350\257\273\345\217\226\350\267\257\345\276\204", nullptr));
        label_totalRestore->setText(QCoreApplication::translate("RestoreDialog", "\346\200\273\346\201\242\345\244\215\345\244\247\345\260\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RestoreDialog: public Ui_RestoreDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTOREDIALOG_H
