/********************************************************************************
** Form generated from reading UI file 'adminmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINMAINWINDOW_H
#define UI_ADMINMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminMainWindow
{
public:
    QAction *action_backup;
    QAction *action_restore;
    QAction *action_exit;
    QAction *actionNew;
    QAction *actionDelete;
    QAction *actionRefresh;
    QAction *actionBackup;
    QAction *actionRestore;
    QAction *actionExit;
    QAction *action_new;
    QAction *action_delete;
    QAction *action_changepassword;
    QAction *action_changequestoions;
    QAction *action_refresh;
    QWidget *centralwidget;
    QTableWidget *tableWidget;
    QComboBox *comboBox;
    QPushButton *pushButton_search;
    QLineEdit *lineEdit_search;
    QPushButton *pushButton_page4;
    QPushButton *pushButton_page1;
    QPushButton *pushButton_page5;
    QToolButton *pushButton_ellipsis;
    QLabel *label_pageInfo;
    QPushButton *pushButton_page3;
    QPushButton *pushButton_next;
    QPushButton *pushButton_prev;
    QPushButton *pushButton_page2;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_4;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *AdminMainWindow)
    {
        if (AdminMainWindow->objectName().isEmpty())
            AdminMainWindow->setObjectName("AdminMainWindow");
        AdminMainWindow->resize(1200, 900);
        AdminMainWindow->setMinimumSize(QSize(1200, 900));
        AdminMainWindow->setMaximumSize(QSize(1200, 900));
        action_backup = new QAction(AdminMainWindow);
        action_backup->setObjectName("action_backup");
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        action_backup->setFont(font);
        action_restore = new QAction(AdminMainWindow);
        action_restore->setObjectName("action_restore");
        action_restore->setFont(font);
        action_exit = new QAction(AdminMainWindow);
        action_exit->setObjectName("action_exit");
        action_exit->setFont(font);
        actionNew = new QAction(AdminMainWindow);
        actionNew->setObjectName("actionNew");
        actionNew->setFont(font);
        actionNew->setMenuRole(QAction::MenuRole::NoRole);
        actionDelete = new QAction(AdminMainWindow);
        actionDelete->setObjectName("actionDelete");
        actionDelete->setFont(font);
        actionDelete->setMenuRole(QAction::MenuRole::NoRole);
        actionRefresh = new QAction(AdminMainWindow);
        actionRefresh->setObjectName("actionRefresh");
        actionRefresh->setFont(font);
        actionRefresh->setMenuRole(QAction::MenuRole::NoRole);
        actionBackup = new QAction(AdminMainWindow);
        actionBackup->setObjectName("actionBackup");
        actionBackup->setFont(font);
        actionBackup->setMenuRole(QAction::MenuRole::NoRole);
        actionRestore = new QAction(AdminMainWindow);
        actionRestore->setObjectName("actionRestore");
        actionRestore->setFont(font);
        actionRestore->setMenuRole(QAction::MenuRole::NoRole);
        actionExit = new QAction(AdminMainWindow);
        actionExit->setObjectName("actionExit");
        actionExit->setFont(font);
        actionExit->setMenuRole(QAction::MenuRole::NoRole);
        action_new = new QAction(AdminMainWindow);
        action_new->setObjectName("action_new");
        action_new->setFont(font);
        action_delete = new QAction(AdminMainWindow);
        action_delete->setObjectName("action_delete");
        action_delete->setFont(font);
        action_changepassword = new QAction(AdminMainWindow);
        action_changepassword->setObjectName("action_changepassword");
        action_changepassword->setFont(font);
        action_changequestoions = new QAction(AdminMainWindow);
        action_changequestoions->setObjectName("action_changequestoions");
        action_changequestoions->setFont(font);
        action_refresh = new QAction(AdminMainWindow);
        action_refresh->setObjectName("action_refresh");
        action_refresh->setFont(font);
        centralwidget = new QWidget(AdminMainWindow);
        centralwidget->setObjectName("centralwidget");
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font1);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font1);
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font1);
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font1);
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(20, 80, 1080, 700));
        tableWidget->setMinimumSize(QSize(1080, 700));
        tableWidget->setMaximumSize(QSize(1080, 700));
        tableWidget->setFont(font1);
        tableWidget->horizontalHeader()->setDefaultSectionSize(184);
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setDefaultSectionSize(40);
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(530, 30, 120, 30));
        comboBox->setMinimumSize(QSize(120, 30));
        comboBox->setMaximumSize(QSize(120, 30));
        comboBox->setFont(font);
        pushButton_search = new QPushButton(centralwidget);
        pushButton_search->setObjectName("pushButton_search");
        pushButton_search->setGeometry(QRect(680, 30, 100, 30));
        pushButton_search->setMinimumSize(QSize(100, 30));
        pushButton_search->setMaximumSize(QSize(100, 30));
        pushButton_search->setFont(font);
        lineEdit_search = new QLineEdit(centralwidget);
        lineEdit_search->setObjectName("lineEdit_search");
        lineEdit_search->setGeometry(QRect(200, 30, 300, 30));
        lineEdit_search->setMinimumSize(QSize(300, 30));
        lineEdit_search->setMaximumSize(QSize(300, 30));
        lineEdit_search->setFont(font);
        pushButton_page4 = new QPushButton(centralwidget);
        pushButton_page4->setObjectName("pushButton_page4");
        pushButton_page4->setGeometry(QRect(620, 810, 30, 30));
        pushButton_page4->setMinimumSize(QSize(30, 30));
        pushButton_page4->setMaximumSize(QSize(30, 30));
        pushButton_page4->setFont(font);
        pushButton_page1 = new QPushButton(centralwidget);
        pushButton_page1->setObjectName("pushButton_page1");
        pushButton_page1->setGeometry(QRect(470, 810, 30, 30));
        pushButton_page1->setMinimumSize(QSize(30, 30));
        pushButton_page1->setMaximumSize(QSize(30, 30));
        pushButton_page1->setFont(font);
        pushButton_page5 = new QPushButton(centralwidget);
        pushButton_page5->setObjectName("pushButton_page5");
        pushButton_page5->setGeometry(QRect(670, 810, 30, 30));
        pushButton_page5->setMinimumSize(QSize(30, 30));
        pushButton_page5->setMaximumSize(QSize(30, 30));
        pushButton_page5->setFont(font);
        pushButton_ellipsis = new QToolButton(centralwidget);
        pushButton_ellipsis->setObjectName("pushButton_ellipsis");
        pushButton_ellipsis->setGeometry(QRect(720, 810, 30, 30));
        pushButton_ellipsis->setMinimumSize(QSize(30, 30));
        pushButton_ellipsis->setMaximumSize(QSize(30, 30));
        pushButton_ellipsis->setFont(font);
        label_pageInfo = new QLabel(centralwidget);
        label_pageInfo->setObjectName("label_pageInfo");
        label_pageInfo->setGeometry(QRect(210, 810, 120, 30));
        label_pageInfo->setMinimumSize(QSize(120, 30));
        label_pageInfo->setMaximumSize(QSize(120, 30));
        label_pageInfo->setFont(font);
        label_pageInfo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        pushButton_page3 = new QPushButton(centralwidget);
        pushButton_page3->setObjectName("pushButton_page3");
        pushButton_page3->setGeometry(QRect(570, 810, 30, 30));
        pushButton_page3->setMinimumSize(QSize(30, 30));
        pushButton_page3->setMaximumSize(QSize(30, 30));
        pushButton_page3->setFont(font);
        pushButton_next = new QPushButton(centralwidget);
        pushButton_next->setObjectName("pushButton_next");
        pushButton_next->setGeometry(QRect(770, 810, 100, 30));
        pushButton_next->setMinimumSize(QSize(100, 30));
        pushButton_next->setMaximumSize(QSize(100, 30));
        pushButton_next->setFont(font);
        pushButton_prev = new QPushButton(centralwidget);
        pushButton_prev->setObjectName("pushButton_prev");
        pushButton_prev->setGeometry(QRect(350, 810, 100, 30));
        pushButton_prev->setMinimumSize(QSize(100, 30));
        pushButton_prev->setMaximumSize(QSize(100, 30));
        pushButton_prev->setFont(font);
        pushButton_page2 = new QPushButton(centralwidget);
        pushButton_page2->setObjectName("pushButton_page2");
        pushButton_page2->setGeometry(QRect(520, 810, 30, 30));
        pushButton_page2->setMinimumSize(QSize(30, 30));
        pushButton_page2->setMaximumSize(QSize(30, 30));
        pushButton_page2->setFont(font);
        AdminMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(AdminMainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 18));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu->setFont(font);
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_2->setFont(font);
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName("menu_4");
        menu_4->setFont(font);
        AdminMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(AdminMainWindow);
        statusbar->setObjectName("statusbar");
        statusbar->setFont(font);
        AdminMainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(AdminMainWindow);
        toolBar->setObjectName("toolBar");
        AdminMainWindow->addToolBar(Qt::ToolBarArea::LeftToolBarArea, toolBar);
        QWidget::setTabOrder(lineEdit_search, comboBox);
        QWidget::setTabOrder(comboBox, pushButton_search);
        QWidget::setTabOrder(pushButton_search, tableWidget);
        QWidget::setTabOrder(tableWidget, pushButton_prev);
        QWidget::setTabOrder(pushButton_prev, pushButton_page1);
        QWidget::setTabOrder(pushButton_page1, pushButton_page2);
        QWidget::setTabOrder(pushButton_page2, pushButton_page3);
        QWidget::setTabOrder(pushButton_page3, pushButton_page4);
        QWidget::setTabOrder(pushButton_page4, pushButton_page5);
        QWidget::setTabOrder(pushButton_page5, pushButton_ellipsis);
        QWidget::setTabOrder(pushButton_ellipsis, pushButton_next);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_4->menuAction());
        menu->addAction(action_new);
        menu->addAction(action_delete);
        menu->addSeparator();
        menu->addAction(action_refresh);
        menu_2->addAction(action_backup);
        menu_2->addAction(action_restore);
        menu_4->addAction(action_changepassword);
        menu_4->addAction(action_changequestoions);
        menu_4->addSeparator();
        menu_4->addAction(action_exit);
        toolBar->addAction(actionNew);
        toolBar->addSeparator();
        toolBar->addAction(actionDelete);
        toolBar->addSeparator();
        toolBar->addAction(actionRefresh);
        toolBar->addSeparator();
        toolBar->addAction(actionBackup);
        toolBar->addSeparator();
        toolBar->addAction(actionRestore);
        toolBar->addSeparator();
        toolBar->addAction(actionExit);

        retranslateUi(AdminMainWindow);

        QMetaObject::connectSlotsByName(AdminMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *AdminMainWindow)
    {
        AdminMainWindow->setWindowTitle(QCoreApplication::translate("AdminMainWindow", "MainWindow", nullptr));
        action_backup->setText(QCoreApplication::translate("AdminMainWindow", "\345\244\207\344\273\275\346\225\260\346\215\256", nullptr));
        action_restore->setText(QCoreApplication::translate("AdminMainWindow", "\346\201\242\345\244\215\346\225\260\346\215\256", nullptr));
        action_exit->setText(QCoreApplication::translate("AdminMainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
        actionNew->setText(QCoreApplication::translate("AdminMainWindow", "\342\236\225 \346\226\260\345\273\272\346\225\231\345\270\210", nullptr));
#if QT_CONFIG(tooltip)
        actionNew->setToolTip(QCoreApplication::translate("AdminMainWindow", "\346\226\260\345\273\272\346\225\231\345\270\210", nullptr));
#endif // QT_CONFIG(tooltip)
        actionDelete->setText(QCoreApplication::translate("AdminMainWindow", "\342\235\214 \345\210\240\351\231\244\346\225\231\345\270\210", nullptr));
#if QT_CONFIG(tooltip)
        actionDelete->setToolTip(QCoreApplication::translate("AdminMainWindow", "\345\210\240\351\231\244\346\225\231\345\270\210", nullptr));
#endif // QT_CONFIG(tooltip)
        actionRefresh->setText(QCoreApplication::translate("AdminMainWindow", "\360\237\224\204 \345\210\267\346\226\260\347\225\214\351\235\242", nullptr));
#if QT_CONFIG(tooltip)
        actionRefresh->setToolTip(QCoreApplication::translate("AdminMainWindow", "\345\210\267\346\226\260\347\225\214\351\235\242", nullptr));
#endif // QT_CONFIG(tooltip)
        actionBackup->setText(QCoreApplication::translate("AdminMainWindow", "\360\237\223\202 \345\244\207\344\273\275\346\225\260\346\215\256", nullptr));
#if QT_CONFIG(tooltip)
        actionBackup->setToolTip(QCoreApplication::translate("AdminMainWindow", "\345\244\207\344\273\275\346\225\260\346\215\256", nullptr));
#endif // QT_CONFIG(tooltip)
        actionRestore->setText(QCoreApplication::translate("AdminMainWindow", "\342\217\252 \346\201\242\345\244\215\346\225\260\346\215\256", nullptr));
#if QT_CONFIG(tooltip)
        actionRestore->setToolTip(QCoreApplication::translate("AdminMainWindow", "\346\201\242\345\244\215\346\225\260\346\215\256", nullptr));
#endif // QT_CONFIG(tooltip)
        actionExit->setText(QCoreApplication::translate("AdminMainWindow", "\360\237\232\252 \351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
#if QT_CONFIG(tooltip)
        actionExit->setToolTip(QCoreApplication::translate("AdminMainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
#endif // QT_CONFIG(tooltip)
        action_new->setText(QCoreApplication::translate("AdminMainWindow", "\346\226\260\345\273\272\346\225\231\345\270\210", nullptr));
        action_delete->setText(QCoreApplication::translate("AdminMainWindow", "\345\210\240\351\231\244\346\225\231\345\270\210", nullptr));
        action_changepassword->setText(QCoreApplication::translate("AdminMainWindow", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
#if QT_CONFIG(tooltip)
        action_changepassword->setToolTip(QCoreApplication::translate("AdminMainWindow", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
#endif // QT_CONFIG(tooltip)
        action_changequestoions->setText(QCoreApplication::translate("AdminMainWindow", "\344\277\256\346\224\271\345\257\206\344\277\235\351\227\256\351\242\230", nullptr));
#if QT_CONFIG(tooltip)
        action_changequestoions->setToolTip(QCoreApplication::translate("AdminMainWindow", "\344\277\256\346\224\271\345\257\206\344\277\235\351\227\256\351\242\230", nullptr));
#endif // QT_CONFIG(tooltip)
        action_refresh->setText(QCoreApplication::translate("AdminMainWindow", "\345\210\267\346\226\260\347\225\214\351\235\242", nullptr));
#if QT_CONFIG(tooltip)
        action_refresh->setToolTip(QCoreApplication::translate("AdminMainWindow", "\345\210\267\346\226\260\347\225\214\351\235\242", nullptr));
#endif // QT_CONFIG(tooltip)
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("AdminMainWindow", "\346\225\231\345\270\210ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("AdminMainWindow", "\345\247\223\345\220\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("AdminMainWindow", "\350\264\246\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("AdminMainWindow", "\346\263\250\345\206\214\346\227\266\351\227\264", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("AdminMainWindow", "\346\225\231\345\270\210ID", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("AdminMainWindow", "\345\247\223\345\220\215", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("AdminMainWindow", "\350\264\246\345\217\267", nullptr));

        pushButton_search->setText(QCoreApplication::translate("AdminMainWindow", "\346\220\234\347\264\242", nullptr));
        lineEdit_search->setPlaceholderText(QCoreApplication::translate("AdminMainWindow", "\346\220\234\347\264\242\350\200\201\345\270\210", nullptr));
        pushButton_page4->setText(QCoreApplication::translate("AdminMainWindow", "4", nullptr));
        pushButton_page1->setText(QCoreApplication::translate("AdminMainWindow", "1", nullptr));
        pushButton_page5->setText(QCoreApplication::translate("AdminMainWindow", "5", nullptr));
        pushButton_ellipsis->setText(QCoreApplication::translate("AdminMainWindow", "...", nullptr));
        label_pageInfo->setText(QCoreApplication::translate("AdminMainWindow", "\347\254\2541\351\241\265/\345\205\2615\351\241\265", nullptr));
        pushButton_page3->setText(QCoreApplication::translate("AdminMainWindow", "3", nullptr));
        pushButton_next->setText(QCoreApplication::translate("AdminMainWindow", "\344\270\213\344\270\200\351\241\265", nullptr));
        pushButton_prev->setText(QCoreApplication::translate("AdminMainWindow", "\344\270\212\344\270\200\351\241\265", nullptr));
        pushButton_page2->setText(QCoreApplication::translate("AdminMainWindow", "2", nullptr));
        menu->setTitle(QCoreApplication::translate("AdminMainWindow", "\346\225\231\345\270\210\347\256\241\347\220\206", nullptr));
        menu_2->setTitle(QCoreApplication::translate("AdminMainWindow", "\347\263\273\347\273\237\347\256\241\347\220\206", nullptr));
        menu_4->setTitle(QCoreApplication::translate("AdminMainWindow", "\350\264\246\345\217\267\347\256\241\347\220\206", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("AdminMainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminMainWindow: public Ui_AdminMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINMAINWINDOW_H
