/********************************************************************************
** Form generated from reading UI file 'teacherwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEACHERWINDOW_H
#define UI_TEACHERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TeacherWindow
{
public:
    QAction *action4;
    QAction *action2;
    QAction *action3;
    QAction *action1;
    QAction *action2_2;
    QAction *action3_2;
    QAction *action1_2;
    QAction *action2_3;
    QAction *action3_3;
    QAction *action1_3;
    QAction *action1_4;
    QAction *action4_2;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QMenu *menu_5;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TeacherWindow)
    {
        if (TeacherWindow->objectName().isEmpty())
            TeacherWindow->setObjectName("TeacherWindow");
        TeacherWindow->resize(600, 400);
        TeacherWindow->setMinimumSize(QSize(600, 400));
        TeacherWindow->setMaximumSize(QSize(600, 400));
        action4 = new QAction(TeacherWindow);
        action4->setObjectName("action4");
        action2 = new QAction(TeacherWindow);
        action2->setObjectName("action2");
        action3 = new QAction(TeacherWindow);
        action3->setObjectName("action3");
        action1 = new QAction(TeacherWindow);
        action1->setObjectName("action1");
        action2_2 = new QAction(TeacherWindow);
        action2_2->setObjectName("action2_2");
        action3_2 = new QAction(TeacherWindow);
        action3_2->setObjectName("action3_2");
        action1_2 = new QAction(TeacherWindow);
        action1_2->setObjectName("action1_2");
        action2_3 = new QAction(TeacherWindow);
        action2_3->setObjectName("action2_3");
        action3_3 = new QAction(TeacherWindow);
        action3_3->setObjectName("action3_3");
        action1_3 = new QAction(TeacherWindow);
        action1_3->setObjectName("action1_3");
        action1_4 = new QAction(TeacherWindow);
        action1_4->setObjectName("action1_4");
        action4_2 = new QAction(TeacherWindow);
        action4_2->setObjectName("action4_2");
        centralwidget = new QWidget(TeacherWindow);
        centralwidget->setObjectName("centralwidget");
        TeacherWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TeacherWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 600, 18));
        QFont font;
        font.setPointSize(9);
        menubar->setFont(font);
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName("menu_4");
        menu_5 = new QMenu(menubar);
        menu_5->setObjectName("menu_5");
        TeacherWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(TeacherWindow);
        statusbar->setObjectName("statusbar");
        TeacherWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu_5->menuAction());
        menu->addAction(action4);
        menu->addAction(action2);
        menu->addAction(action3);
        menu->addSeparator();
        menu->addAction(action4_2);
        menu_2->addAction(action1);
        menu_2->addAction(action2_2);
        menu_2->addAction(action3_2);
        menu_3->addAction(action1_2);
        menu_3->addAction(action2_3);
        menu_3->addAction(action3_3);
        menu_4->addAction(action1_3);
        menu_5->addAction(action1_4);

        retranslateUi(TeacherWindow);

        QMetaObject::connectSlotsByName(TeacherWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TeacherWindow)
    {
        TeacherWindow->setWindowTitle(QCoreApplication::translate("TeacherWindow", "MainWindow", nullptr));
        action4->setText(QCoreApplication::translate("TeacherWindow", "\346\267\273\345\212\240\345\255\246\347\224\237", nullptr));
        action2->setText(QCoreApplication::translate("TeacherWindow", "\345\210\240\351\231\244\345\255\246\347\224\237", nullptr));
        action3->setText(QCoreApplication::translate("TeacherWindow", "\347\274\226\350\276\221\344\277\241\346\201\257", nullptr));
        action1->setText(QCoreApplication::translate("TeacherWindow", "\345\275\225\345\205\245\346\210\220\347\273\251", nullptr));
        action2_2->setText(QCoreApplication::translate("TeacherWindow", "\344\277\256\346\224\271\346\210\220\347\273\251", nullptr));
        action3_2->setText(QCoreApplication::translate("TeacherWindow", "\346\237\245\347\234\213\346\210\220\347\273\251", nullptr));
        action1_2->setText(QCoreApplication::translate("TeacherWindow", "\345\257\274\345\205\245Excel", nullptr));
        action2_3->setText(QCoreApplication::translate("TeacherWindow", "\345\257\274\345\207\272Excel", nullptr));
        action3_3->setText(QCoreApplication::translate("TeacherWindow", "\346\211\223\345\215\260\346\212\245\350\241\250", nullptr));
        action1_3->setText(QCoreApplication::translate("TeacherWindow", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        action1_4->setText(QCoreApplication::translate("TeacherWindow", "\345\205\263\344\272\216", nullptr));
        action4_2->setText(QCoreApplication::translate("TeacherWindow", "\345\210\267\346\226\260\345\210\227\350\241\250", nullptr));
        menu->setTitle(QCoreApplication::translate("TeacherWindow", "\345\255\246\347\224\237\347\256\241\347\220\206", nullptr));
        menu_2->setTitle(QCoreApplication::translate("TeacherWindow", "\346\210\220\347\273\251\347\256\241\347\220\206", nullptr));
        menu_3->setTitle(QCoreApplication::translate("TeacherWindow", "\346\225\260\346\215\256\346\223\215\344\275\234", nullptr));
        menu_4->setTitle(QCoreApplication::translate("TeacherWindow", "\344\270\252\344\272\272\350\256\276\347\275\256", nullptr));
        menu_5->setTitle(QCoreApplication::translate("TeacherWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TeacherWindow: public Ui_TeacherWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEACHERWINDOW_H
