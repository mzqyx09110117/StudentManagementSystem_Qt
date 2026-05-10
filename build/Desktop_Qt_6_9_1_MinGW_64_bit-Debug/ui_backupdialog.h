/********************************************************************************
** Form generated from reading UI file 'backupdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BACKUPDIALOG_H
#define UI_BACKUPDIALOG_H

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

class Ui_BackupDialog
{
public:
    QLineEdit *lineEdit_backupPath;
    QPushButton *pushButton_selectFolder;
    QLabel *label_systemDbInfo;
    QLabel *label_teacherDbInfo;
    QLabel *label_totalSize;
    QLabel *label_status;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_systemDbInfo_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_startBackup;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *BackupDialog)
    {
        if (BackupDialog->objectName().isEmpty())
            BackupDialog->setObjectName("BackupDialog");
        BackupDialog->resize(600, 400);
        BackupDialog->setMinimumSize(QSize(520, 400));
        BackupDialog->setMaximumSize(QSize(600, 400));
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        BackupDialog->setFont(font);
        lineEdit_backupPath = new QLineEdit(BackupDialog);
        lineEdit_backupPath->setObjectName("lineEdit_backupPath");
        lineEdit_backupPath->setGeometry(QRect(50, 70, 400, 30));
        lineEdit_backupPath->setMinimumSize(QSize(400, 30));
        lineEdit_backupPath->setMaximumSize(QSize(400, 30));
        lineEdit_backupPath->setFont(font);
        pushButton_selectFolder = new QPushButton(BackupDialog);
        pushButton_selectFolder->setObjectName("pushButton_selectFolder");
        pushButton_selectFolder->setGeometry(QRect(460, 70, 90, 30));
        pushButton_selectFolder->setMinimumSize(QSize(90, 30));
        pushButton_selectFolder->setMaximumSize(QSize(90, 30));
        pushButton_selectFolder->setFont(font);
        label_systemDbInfo = new QLabel(BackupDialog);
        label_systemDbInfo->setObjectName("label_systemDbInfo");
        label_systemDbInfo->setGeometry(QRect(190, 130, 360, 30));
        label_systemDbInfo->setMinimumSize(QSize(360, 30));
        label_systemDbInfo->setMaximumSize(QSize(360, 30));
        label_systemDbInfo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_teacherDbInfo = new QLabel(BackupDialog);
        label_teacherDbInfo->setObjectName("label_teacherDbInfo");
        label_teacherDbInfo->setGeometry(QRect(190, 180, 360, 30));
        label_teacherDbInfo->setMinimumSize(QSize(360, 30));
        label_teacherDbInfo->setMaximumSize(QSize(360, 30));
        label_teacherDbInfo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_totalSize = new QLabel(BackupDialog);
        label_totalSize->setObjectName("label_totalSize");
        label_totalSize->setGeometry(QRect(190, 230, 360, 30));
        label_totalSize->setMinimumSize(QSize(360, 30));
        label_totalSize->setMaximumSize(QSize(360, 30));
        label_totalSize->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_status = new QLabel(BackupDialog);
        label_status->setObjectName("label_status");
        label_status->setGeometry(QRect(190, 280, 360, 30));
        label_status->setMinimumSize(QSize(360, 30));
        label_status->setMaximumSize(QSize(360, 30));
        label_status->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_5 = new QLabel(BackupDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(50, 130, 120, 30));
        label_5->setMinimumSize(QSize(120, 30));
        label_5->setMaximumSize(QSize(120, 30));
        label_6 = new QLabel(BackupDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 180, 120, 30));
        label_6->setMinimumSize(QSize(120, 30));
        label_6->setMaximumSize(QSize(120, 30));
        label_7 = new QLabel(BackupDialog);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(50, 230, 120, 30));
        label_7->setMinimumSize(QSize(120, 30));
        label_7->setMaximumSize(QSize(120, 30));
        label_8 = new QLabel(BackupDialog);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(50, 280, 120, 30));
        label_8->setMinimumSize(QSize(120, 30));
        label_8->setMaximumSize(QSize(120, 30));
        label_systemDbInfo_2 = new QLabel(BackupDialog);
        label_systemDbInfo_2->setObjectName("label_systemDbInfo_2");
        label_systemDbInfo_2->setGeometry(QRect(150, 20, 300, 30));
        label_systemDbInfo_2->setMinimumSize(QSize(300, 30));
        label_systemDbInfo_2->setMaximumSize(QSize(300, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        label_systemDbInfo_2->setFont(font1);
        label_systemDbInfo_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        layoutWidget = new QWidget(BackupDialog);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(90, 340, 421, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_startBackup = new QPushButton(layoutWidget);
        pushButton_startBackup->setObjectName("pushButton_startBackup");
        pushButton_startBackup->setMinimumSize(QSize(90, 30));
        pushButton_startBackup->setMaximumSize(QSize(90, 30));
        pushButton_startBackup->setFont(font);

        horizontalLayout->addWidget(pushButton_startBackup);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_cancel = new QPushButton(layoutWidget);
        pushButton_cancel->setObjectName("pushButton_cancel");
        pushButton_cancel->setMinimumSize(QSize(90, 30));
        pushButton_cancel->setMaximumSize(QSize(90, 30));
        pushButton_cancel->setFont(font);

        horizontalLayout->addWidget(pushButton_cancel);


        retranslateUi(BackupDialog);

        QMetaObject::connectSlotsByName(BackupDialog);
    } // setupUi

    void retranslateUi(QDialog *BackupDialog)
    {
        BackupDialog->setWindowTitle(QCoreApplication::translate("BackupDialog", "Dialog", nullptr));
        lineEdit_backupPath->setPlaceholderText(QCoreApplication::translate("BackupDialog", "\345\244\207\344\273\275\350\267\257\345\276\204", nullptr));
        pushButton_selectFolder->setText(QCoreApplication::translate("BackupDialog", "\346\265\217\350\247\210", nullptr));
        label_systemDbInfo->setText(QCoreApplication::translate("BackupDialog", "\347\263\273\347\273\237\346\225\260\346\215\256\345\272\223\344\277\241\346\201\257", nullptr));
        label_teacherDbInfo->setText(QCoreApplication::translate("BackupDialog", "\346\225\231\345\270\210\346\225\260\346\215\256\345\272\223\347\273\237\350\256\241", nullptr));
        label_totalSize->setText(QCoreApplication::translate("BackupDialog", "\346\200\273\345\244\207\344\273\275\345\244\247\345\260\217", nullptr));
        label_status->setText(QCoreApplication::translate("BackupDialog", "\347\255\211\345\276\205\346\223\215\344\275\234...", nullptr));
        label_5->setText(QCoreApplication::translate("BackupDialog", "\347\263\273\347\273\237\346\225\260\346\215\256\345\272\223\344\277\241\346\201\257\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("BackupDialog", "\346\225\231\345\270\210\346\225\260\346\215\256\345\272\223\347\273\237\350\256\241\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("BackupDialog", "\346\200\273\345\244\207\344\273\275\345\244\247\345\260\217\357\274\232", nullptr));
        label_8->setText(QCoreApplication::translate("BackupDialog", "\347\212\266\346\200\201\357\274\232", nullptr));
        label_systemDbInfo_2->setText(QCoreApplication::translate("BackupDialog", "\345\244\207\344\273\275\347\263\273\347\273\237", nullptr));
        pushButton_startBackup->setText(QCoreApplication::translate("BackupDialog", "\345\274\200\345\247\213\345\244\207\344\273\275", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("BackupDialog", "\345\217\226\346\266\210\345\244\207\344\273\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BackupDialog: public Ui_BackupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BACKUPDIALOG_H
