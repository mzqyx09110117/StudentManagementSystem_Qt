/********************************************************************************
** Form generated from reading UI file 'teachermainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEACHERMAINWINDOW_H
#define UI_TEACHERMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
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

class Ui_TeacherMainWindow
{
public:
    QAction *action_addStudent;
    QAction *action_deleteStudent;
    QAction *action_editStudent;
    QAction *action_refreshList;
    QAction *action_inputGrade;
    QAction *action_modifyGrade;
    QAction *action_viewGrade;
    QAction *action_changePassword;
    QAction *action_add;
    QAction *action_edit;
    QAction *action_delete;
    QAction *action_refresh;
    QAction *action_exit;
    QAction *action_add_class;
    QAction *action_edit_class;
    QAction *action_delete_class;
    QAction *action_changequestions;
    QAction *action_exit_2;
    QAction *action_addSubject;
    QAction *action_editSubject;
    QAction *action_deleteSubject;
    QAction *action_addSubject_2;
    QAction *action_editSubject_2;
    QAction *action_deleteSubject_2;
    QAction *action_manageSubjects;
    QAction *action_classGPA;
    QAction *action_allStudentsGPA;
    QAction *action_subjectClass;
    QAction *action_subjectAll;
    QAction *action_deleteGrade;
    QWidget *centralwidget;
    QListWidget *listWidget_classes;
    QTableWidget *tableWidget_students;
    QLineEdit *lineEdit_search;
    QPushButton *pushButton_search;
    QPushButton *pushButton_prev;
    QLabel *label_pageInfo;
    QPushButton *pushButton_next;
    QPushButton *pushButton_page1;
    QPushButton *pushButton_page2;
    QPushButton *pushButton_page3;
    QPushButton *pushButton_page4;
    QPushButton *pushButton_page5;
    QToolButton *pushButton_ellipsis;
    QComboBox *comboBox;
    QMenuBar *menubar_main;
    QMenu *menu_student;
    QMenu *menu_score;
    QMenu *menu_data;
    QMenu *menu_personal;
    QMenu *menu;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TeacherMainWindow)
    {
        if (TeacherMainWindow->objectName().isEmpty())
            TeacherMainWindow->setObjectName("TeacherMainWindow");
        TeacherMainWindow->resize(1600, 900);
        TeacherMainWindow->setMinimumSize(QSize(1600, 900));
        TeacherMainWindow->setMaximumSize(QSize(1600, 900));
        action_addStudent = new QAction(TeacherMainWindow);
        action_addStudent->setObjectName("action_addStudent");
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font.setPointSize(12);
        action_addStudent->setFont(font);
        action_deleteStudent = new QAction(TeacherMainWindow);
        action_deleteStudent->setObjectName("action_deleteStudent");
        action_deleteStudent->setFont(font);
        action_editStudent = new QAction(TeacherMainWindow);
        action_editStudent->setObjectName("action_editStudent");
        action_editStudent->setFont(font);
        action_refreshList = new QAction(TeacherMainWindow);
        action_refreshList->setObjectName("action_refreshList");
        action_refreshList->setFont(font);
        action_inputGrade = new QAction(TeacherMainWindow);
        action_inputGrade->setObjectName("action_inputGrade");
        action_inputGrade->setFont(font);
        action_modifyGrade = new QAction(TeacherMainWindow);
        action_modifyGrade->setObjectName("action_modifyGrade");
        action_modifyGrade->setFont(font);
        action_viewGrade = new QAction(TeacherMainWindow);
        action_viewGrade->setObjectName("action_viewGrade");
        action_viewGrade->setFont(font);
        action_changePassword = new QAction(TeacherMainWindow);
        action_changePassword->setObjectName("action_changePassword");
        action_changePassword->setFont(font);
        action_add = new QAction(TeacherMainWindow);
        action_add->setObjectName("action_add");
        action_add->setFont(font);
        action_add->setMenuRole(QAction::MenuRole::NoRole);
        action_edit = new QAction(TeacherMainWindow);
        action_edit->setObjectName("action_edit");
        action_edit->setFont(font);
        action_edit->setMenuRole(QAction::MenuRole::NoRole);
        action_delete = new QAction(TeacherMainWindow);
        action_delete->setObjectName("action_delete");
        action_delete->setFont(font);
        action_delete->setMenuRole(QAction::MenuRole::NoRole);
        action_refresh = new QAction(TeacherMainWindow);
        action_refresh->setObjectName("action_refresh");
        action_refresh->setFont(font);
        action_refresh->setMenuRole(QAction::MenuRole::NoRole);
        action_exit = new QAction(TeacherMainWindow);
        action_exit->setObjectName("action_exit");
        action_exit->setFont(font);
        action_exit->setMenuRole(QAction::MenuRole::NoRole);
        action_add_class = new QAction(TeacherMainWindow);
        action_add_class->setObjectName("action_add_class");
        action_add_class->setFont(font);
        action_add_class->setMenuRole(QAction::MenuRole::NoRole);
        action_edit_class = new QAction(TeacherMainWindow);
        action_edit_class->setObjectName("action_edit_class");
        action_edit_class->setFont(font);
        action_edit_class->setMenuRole(QAction::MenuRole::NoRole);
        action_delete_class = new QAction(TeacherMainWindow);
        action_delete_class->setObjectName("action_delete_class");
        action_delete_class->setFont(font);
        action_delete_class->setMenuRole(QAction::MenuRole::NoRole);
        action_changequestions = new QAction(TeacherMainWindow);
        action_changequestions->setObjectName("action_changequestions");
        action_changequestions->setFont(font);
        action_exit_2 = new QAction(TeacherMainWindow);
        action_exit_2->setObjectName("action_exit_2");
        action_exit_2->setFont(font);
        action_addSubject = new QAction(TeacherMainWindow);
        action_addSubject->setObjectName("action_addSubject");
        action_addSubject->setFont(font);
        action_editSubject = new QAction(TeacherMainWindow);
        action_editSubject->setObjectName("action_editSubject");
        action_editSubject->setFont(font);
        action_deleteSubject = new QAction(TeacherMainWindow);
        action_deleteSubject->setObjectName("action_deleteSubject");
        action_deleteSubject->setFont(font);
        action_addSubject_2 = new QAction(TeacherMainWindow);
        action_addSubject_2->setObjectName("action_addSubject_2");
        action_addSubject_2->setFont(font);
        action_addSubject_2->setMenuRole(QAction::MenuRole::NoRole);
        action_editSubject_2 = new QAction(TeacherMainWindow);
        action_editSubject_2->setObjectName("action_editSubject_2");
        action_editSubject_2->setFont(font);
        action_editSubject_2->setMenuRole(QAction::MenuRole::NoRole);
        action_deleteSubject_2 = new QAction(TeacherMainWindow);
        action_deleteSubject_2->setObjectName("action_deleteSubject_2");
        action_deleteSubject_2->setFont(font);
        action_deleteSubject_2->setMenuRole(QAction::MenuRole::NoRole);
        action_manageSubjects = new QAction(TeacherMainWindow);
        action_manageSubjects->setObjectName("action_manageSubjects");
        action_classGPA = new QAction(TeacherMainWindow);
        action_classGPA->setObjectName("action_classGPA");
        action_allStudentsGPA = new QAction(TeacherMainWindow);
        action_allStudentsGPA->setObjectName("action_allStudentsGPA");
        action_subjectClass = new QAction(TeacherMainWindow);
        action_subjectClass->setObjectName("action_subjectClass");
        action_subjectAll = new QAction(TeacherMainWindow);
        action_subjectAll->setObjectName("action_subjectAll");
        action_deleteGrade = new QAction(TeacherMainWindow);
        action_deleteGrade->setObjectName("action_deleteGrade");
        centralwidget = new QWidget(TeacherMainWindow);
        centralwidget->setObjectName("centralwidget");
        listWidget_classes = new QListWidget(centralwidget);
        listWidget_classes->setObjectName("listWidget_classes");
        listWidget_classes->setGeometry(QRect(20, 20, 300, 800));
        listWidget_classes->setMinimumSize(QSize(300, 800));
        listWidget_classes->setMaximumSize(QSize(300, 800));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\345\256\213\344\275\223")});
        font1.setPointSize(16);
        listWidget_classes->setFont(font1);
        listWidget_classes->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        listWidget_classes->setProperty("isWrapping", QVariant(true));
        listWidget_classes->setSpacing(6);
        tableWidget_students = new QTableWidget(centralwidget);
        if (tableWidget_students->columnCount() < 8)
            tableWidget_students->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFont(font1);
        tableWidget_students->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableWidget_students->setObjectName("tableWidget_students");
        tableWidget_students->setGeometry(QRect(330, 60, 1120, 680));
        tableWidget_students->setMinimumSize(QSize(1120, 680));
        tableWidget_students->setMaximumSize(QSize(1120, 680));
        tableWidget_students->setFont(font1);
        tableWidget_students->horizontalHeader()->setDefaultSectionSize(138);
        tableWidget_students->horizontalHeader()->setStretchLastSection(true);
        tableWidget_students->verticalHeader()->setMinimumSectionSize(30);
        tableWidget_students->verticalHeader()->setDefaultSectionSize(40);
        lineEdit_search = new QLineEdit(centralwidget);
        lineEdit_search->setObjectName("lineEdit_search");
        lineEdit_search->setGeometry(QRect(520, 10, 300, 30));
        lineEdit_search->setMinimumSize(QSize(300, 30));
        lineEdit_search->setMaximumSize(QSize(300, 30));
        lineEdit_search->setFont(font);
        pushButton_search = new QPushButton(centralwidget);
        pushButton_search->setObjectName("pushButton_search");
        pushButton_search->setGeometry(QRect(1000, 10, 100, 30));
        pushButton_search->setMinimumSize(QSize(100, 30));
        pushButton_search->setMaximumSize(QSize(100, 30));
        pushButton_search->setFont(font);
        pushButton_prev = new QPushButton(centralwidget);
        pushButton_prev->setObjectName("pushButton_prev");
        pushButton_prev->setGeometry(QRect(700, 790, 100, 30));
        pushButton_prev->setMinimumSize(QSize(100, 30));
        pushButton_prev->setMaximumSize(QSize(100, 30));
        pushButton_prev->setFont(font);
        label_pageInfo = new QLabel(centralwidget);
        label_pageInfo->setObjectName("label_pageInfo");
        label_pageInfo->setGeometry(QRect(560, 790, 120, 30));
        label_pageInfo->setMinimumSize(QSize(120, 30));
        label_pageInfo->setMaximumSize(QSize(120, 30));
        label_pageInfo->setFont(font);
        label_pageInfo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        pushButton_next = new QPushButton(centralwidget);
        pushButton_next->setObjectName("pushButton_next");
        pushButton_next->setGeometry(QRect(1120, 790, 100, 30));
        pushButton_next->setMinimumSize(QSize(100, 30));
        pushButton_next->setMaximumSize(QSize(100, 30));
        pushButton_next->setFont(font);
        pushButton_page1 = new QPushButton(centralwidget);
        pushButton_page1->setObjectName("pushButton_page1");
        pushButton_page1->setGeometry(QRect(820, 790, 30, 30));
        pushButton_page1->setMinimumSize(QSize(30, 30));
        pushButton_page1->setMaximumSize(QSize(30, 30));
        pushButton_page1->setFont(font);
        pushButton_page2 = new QPushButton(centralwidget);
        pushButton_page2->setObjectName("pushButton_page2");
        pushButton_page2->setGeometry(QRect(870, 790, 30, 30));
        pushButton_page2->setMinimumSize(QSize(30, 30));
        pushButton_page2->setMaximumSize(QSize(30, 30));
        pushButton_page2->setFont(font);
        pushButton_page3 = new QPushButton(centralwidget);
        pushButton_page3->setObjectName("pushButton_page3");
        pushButton_page3->setGeometry(QRect(920, 790, 30, 30));
        pushButton_page3->setMinimumSize(QSize(30, 30));
        pushButton_page3->setMaximumSize(QSize(30, 30));
        pushButton_page3->setFont(font);
        pushButton_page4 = new QPushButton(centralwidget);
        pushButton_page4->setObjectName("pushButton_page4");
        pushButton_page4->setGeometry(QRect(970, 790, 30, 30));
        pushButton_page4->setMinimumSize(QSize(30, 30));
        pushButton_page4->setMaximumSize(QSize(30, 30));
        pushButton_page4->setFont(font);
        pushButton_page5 = new QPushButton(centralwidget);
        pushButton_page5->setObjectName("pushButton_page5");
        pushButton_page5->setGeometry(QRect(1020, 790, 30, 30));
        pushButton_page5->setMinimumSize(QSize(30, 30));
        pushButton_page5->setMaximumSize(QSize(30, 30));
        pushButton_page5->setFont(font);
        pushButton_ellipsis = new QToolButton(centralwidget);
        pushButton_ellipsis->setObjectName("pushButton_ellipsis");
        pushButton_ellipsis->setGeometry(QRect(1070, 790, 30, 30));
        pushButton_ellipsis->setMinimumSize(QSize(30, 30));
        pushButton_ellipsis->setMaximumSize(QSize(30, 30));
        pushButton_ellipsis->setFont(font);
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(850, 10, 120, 30));
        comboBox->setMinimumSize(QSize(120, 30));
        comboBox->setMaximumSize(QSize(120, 30));
        comboBox->setFont(font);
        TeacherMainWindow->setCentralWidget(centralwidget);
        menubar_main = new QMenuBar(TeacherMainWindow);
        menubar_main->setObjectName("menubar_main");
        menubar_main->setGeometry(QRect(0, 0, 1600, 18));
        menubar_main->setFont(font);
        menu_student = new QMenu(menubar_main);
        menu_student->setObjectName("menu_student");
        menu_student->setFont(font);
        menu_score = new QMenu(menubar_main);
        menu_score->setObjectName("menu_score");
        menu_score->setFont(font);
        menu_data = new QMenu(menubar_main);
        menu_data->setObjectName("menu_data");
        menu_data->setFont(font);
        menu_personal = new QMenu(menubar_main);
        menu_personal->setObjectName("menu_personal");
        menu_personal->setFont(font);
        menu = new QMenu(menubar_main);
        menu->setObjectName("menu");
        menu->setFont(font);
        TeacherMainWindow->setMenuBar(menubar_main);
        toolBar = new QToolBar(TeacherMainWindow);
        toolBar->setObjectName("toolBar");
        toolBar->setFont(font);
        TeacherMainWindow->addToolBar(Qt::ToolBarArea::LeftToolBarArea, toolBar);
        statusBar = new QStatusBar(TeacherMainWindow);
        statusBar->setObjectName("statusBar");
        TeacherMainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(lineEdit_search, comboBox);
        QWidget::setTabOrder(comboBox, pushButton_search);
        QWidget::setTabOrder(pushButton_search, listWidget_classes);
        QWidget::setTabOrder(listWidget_classes, tableWidget_students);
        QWidget::setTabOrder(tableWidget_students, pushButton_prev);
        QWidget::setTabOrder(pushButton_prev, pushButton_page1);
        QWidget::setTabOrder(pushButton_page1, pushButton_page2);
        QWidget::setTabOrder(pushButton_page2, pushButton_page3);
        QWidget::setTabOrder(pushButton_page3, pushButton_page4);
        QWidget::setTabOrder(pushButton_page4, pushButton_page5);
        QWidget::setTabOrder(pushButton_page5, pushButton_ellipsis);
        QWidget::setTabOrder(pushButton_ellipsis, pushButton_next);

        menubar_main->addAction(menu_student->menuAction());
        menubar_main->addAction(menu->menuAction());
        menubar_main->addAction(menu_score->menuAction());
        menubar_main->addAction(menu_data->menuAction());
        menubar_main->addAction(menu_personal->menuAction());
        menu_student->addAction(action_addStudent);
        menu_student->addAction(action_deleteStudent);
        menu_student->addAction(action_editStudent);
        menu_score->addAction(action_inputGrade);
        menu_score->addAction(action_deleteGrade);
        menu_score->addAction(action_modifyGrade);
        menu_score->addAction(action_viewGrade);
        menu_data->addAction(action_classGPA);
        menu_data->addAction(action_allStudentsGPA);
        menu_data->addSeparator();
        menu_data->addAction(action_subjectClass);
        menu_data->addAction(action_subjectAll);
        menu_data->addSeparator();
        menu_data->addAction(action_refreshList);
        menu_personal->addAction(action_changePassword);
        menu_personal->addAction(action_changequestions);
        menu_personal->addSeparator();
        menu_personal->addAction(action_exit_2);
        menu->addAction(action_addSubject);
        menu->addAction(action_editSubject);
        menu->addAction(action_deleteSubject);
        menu->addAction(action_manageSubjects);
        toolBar->addAction(action_add);
        toolBar->addAction(action_add_class);
        toolBar->addAction(action_addSubject_2);
        toolBar->addSeparator();
        toolBar->addAction(action_edit);
        toolBar->addAction(action_edit_class);
        toolBar->addAction(action_editSubject_2);
        toolBar->addSeparator();
        toolBar->addAction(action_delete);
        toolBar->addAction(action_delete_class);
        toolBar->addAction(action_deleteSubject_2);
        toolBar->addSeparator();
        toolBar->addAction(action_refresh);
        toolBar->addAction(action_exit);
        toolBar->addSeparator();

        retranslateUi(TeacherMainWindow);

        QMetaObject::connectSlotsByName(TeacherMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TeacherMainWindow)
    {
        TeacherMainWindow->setWindowTitle(QCoreApplication::translate("TeacherMainWindow", "MainWindow", nullptr));
        action_addStudent->setText(QCoreApplication::translate("TeacherMainWindow", "\346\267\273\345\212\240\345\255\246\347\224\237", nullptr));
        action_deleteStudent->setText(QCoreApplication::translate("TeacherMainWindow", "\345\210\240\351\231\244\345\255\246\347\224\237", nullptr));
        action_editStudent->setText(QCoreApplication::translate("TeacherMainWindow", "\347\274\226\350\276\221\344\277\241\346\201\257", nullptr));
        action_refreshList->setText(QCoreApplication::translate("TeacherMainWindow", "\345\210\267\346\226\260\345\210\227\350\241\250", nullptr));
        action_inputGrade->setText(QCoreApplication::translate("TeacherMainWindow", "\345\275\225\345\205\245\346\210\220\347\273\251", nullptr));
        action_modifyGrade->setText(QCoreApplication::translate("TeacherMainWindow", "\344\277\256\346\224\271\346\210\220\347\273\251", nullptr));
        action_viewGrade->setText(QCoreApplication::translate("TeacherMainWindow", "\346\237\245\347\234\213\346\210\220\347\273\251", nullptr));
        action_changePassword->setText(QCoreApplication::translate("TeacherMainWindow", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        action_add->setText(QCoreApplication::translate("TeacherMainWindow", "\342\236\225 \346\267\273\345\212\240\345\255\246\347\224\237", nullptr));
#if QT_CONFIG(tooltip)
        action_add->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\346\267\273\345\212\240\346\226\260\345\255\246\347\224\237", nullptr));
#endif // QT_CONFIG(tooltip)
        action_edit->setText(QCoreApplication::translate("TeacherMainWindow", "\342\234\217\357\270\217 \347\274\226\350\276\221\345\255\246\347\224\237", nullptr));
#if QT_CONFIG(tooltip)
        action_edit->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\347\274\226\350\276\221\351\200\211\344\270\255\345\255\246\347\224\237", nullptr));
#endif // QT_CONFIG(tooltip)
        action_delete->setText(QCoreApplication::translate("TeacherMainWindow", "\342\235\214 \345\210\240\351\231\244\345\255\246\347\224\237", nullptr));
#if QT_CONFIG(tooltip)
        action_delete->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\345\210\240\351\231\244\351\200\211\344\270\255\345\255\246\347\224\237", nullptr));
#endif // QT_CONFIG(tooltip)
        action_refresh->setText(QCoreApplication::translate("TeacherMainWindow", "\360\237\224\204 \345\210\267\346\226\260\345\210\227\350\241\250", nullptr));
#if QT_CONFIG(tooltip)
        action_refresh->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\345\210\267\346\226\260\345\210\227\350\241\250", nullptr));
#endif // QT_CONFIG(tooltip)
        action_exit->setText(QCoreApplication::translate("TeacherMainWindow", "\360\237\232\252 \351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
#if QT_CONFIG(tooltip)
        action_exit->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
#endif // QT_CONFIG(tooltip)
        action_add_class->setText(QCoreApplication::translate("TeacherMainWindow", "\342\236\225 \346\267\273\345\212\240\347\217\255\347\272\247", nullptr));
#if QT_CONFIG(tooltip)
        action_add_class->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\346\267\273\345\212\240\345\255\246\347\224\237\347\217\255\347\272\247", nullptr));
#endif // QT_CONFIG(tooltip)
        action_edit_class->setText(QCoreApplication::translate("TeacherMainWindow", "\342\234\217\357\270\217 \347\274\226\350\276\221\347\217\255\347\272\247", nullptr));
#if QT_CONFIG(tooltip)
        action_edit_class->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\347\274\226\350\276\221\345\255\246\347\224\237\347\217\255\347\272\247", nullptr));
#endif // QT_CONFIG(tooltip)
        action_delete_class->setText(QCoreApplication::translate("TeacherMainWindow", "\342\235\214 \345\210\240\351\231\244\347\217\255\347\272\247", nullptr));
#if QT_CONFIG(tooltip)
        action_delete_class->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\345\210\240\351\231\244\345\255\246\347\224\237\347\217\255\347\272\247", nullptr));
#endif // QT_CONFIG(tooltip)
        action_changequestions->setText(QCoreApplication::translate("TeacherMainWindow", "\344\277\256\346\224\271\345\257\206\344\277\235\351\227\256\351\242\230", nullptr));
#if QT_CONFIG(tooltip)
        action_changequestions->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\344\277\256\346\224\271\345\257\206\344\277\235\351\227\256\351\242\230", nullptr));
#endif // QT_CONFIG(tooltip)
        action_exit_2->setText(QCoreApplication::translate("TeacherMainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
#if QT_CONFIG(tooltip)
        action_exit_2->setToolTip(QCoreApplication::translate("TeacherMainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
#endif // QT_CONFIG(tooltip)
        action_addSubject->setText(QCoreApplication::translate("TeacherMainWindow", "\346\267\273\345\212\240\350\257\276\347\250\213", nullptr));
        action_editSubject->setText(QCoreApplication::translate("TeacherMainWindow", "\344\277\256\346\224\271\350\257\276\347\250\213", nullptr));
        action_deleteSubject->setText(QCoreApplication::translate("TeacherMainWindow", "\345\210\240\351\231\244\350\257\276\347\250\213", nullptr));
        action_addSubject_2->setText(QCoreApplication::translate("TeacherMainWindow", "\342\236\225 \346\267\273\345\212\240\350\257\276\347\250\213", nullptr));
        action_editSubject_2->setText(QCoreApplication::translate("TeacherMainWindow", "\342\234\217\357\270\217 \347\274\226\350\276\221\350\257\276\347\250\213", nullptr));
        action_deleteSubject_2->setText(QCoreApplication::translate("TeacherMainWindow", "\342\235\214 \345\210\240\351\231\244\350\257\276\347\250\213", nullptr));
        action_manageSubjects->setText(QCoreApplication::translate("TeacherMainWindow", "\347\256\241\347\220\206\350\257\276\347\250\213", nullptr));
        action_classGPA->setText(QCoreApplication::translate("TeacherMainWindow", "\347\217\255\347\272\247\347\273\251\347\202\271\346\216\222\345\220\215", nullptr));
        action_allStudentsGPA->setText(QCoreApplication::translate("TeacherMainWindow", "\346\211\200\346\234\211\345\255\246\347\224\237\347\273\251\347\202\271\346\216\222\345\220\215", nullptr));
        action_subjectClass->setText(QCoreApplication::translate("TeacherMainWindow", "\345\215\225\347\247\221\347\217\255\347\272\247\346\210\220\347\273\251\346\216\222\345\220\215", nullptr));
        action_subjectAll->setText(QCoreApplication::translate("TeacherMainWindow", "\345\215\225\347\247\221\346\211\200\346\234\211\345\255\246\347\224\237\346\210\220\347\273\251\346\216\222\345\220\215", nullptr));
        action_deleteGrade->setText(QCoreApplication::translate("TeacherMainWindow", "\345\210\240\351\231\244\346\210\220\347\273\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_students->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TeacherMainWindow", "\345\255\246\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_students->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TeacherMainWindow", "\345\247\223\345\220\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_students->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TeacherMainWindow", "\346\200\247\345\210\253", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_students->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TeacherMainWindow", "\345\271\264\351\276\204", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_students->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("TeacherMainWindow", "\347\217\255\347\272\247", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_students->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("TeacherMainWindow", "\347\224\265\350\257\235", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_students->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("TeacherMainWindow", "\345\234\260\345\235\200", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_students->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("TeacherMainWindow", "\345\210\233\345\273\272\346\227\266\351\227\264", nullptr));
        lineEdit_search->setPlaceholderText(QCoreApplication::translate("TeacherMainWindow", "\346\220\234\347\264\242\345\255\246\347\224\237", nullptr));
        pushButton_search->setText(QCoreApplication::translate("TeacherMainWindow", "\346\220\234\347\264\242", nullptr));
        pushButton_prev->setText(QCoreApplication::translate("TeacherMainWindow", "\344\270\212\344\270\200\351\241\265", nullptr));
        label_pageInfo->setText(QCoreApplication::translate("TeacherMainWindow", "\347\254\2541\351\241\265/\345\205\2615\351\241\265", nullptr));
        pushButton_next->setText(QCoreApplication::translate("TeacherMainWindow", "\344\270\213\344\270\200\351\241\265", nullptr));
        pushButton_page1->setText(QCoreApplication::translate("TeacherMainWindow", "1", nullptr));
        pushButton_page2->setText(QCoreApplication::translate("TeacherMainWindow", "2", nullptr));
        pushButton_page3->setText(QCoreApplication::translate("TeacherMainWindow", "3", nullptr));
        pushButton_page4->setText(QCoreApplication::translate("TeacherMainWindow", "4", nullptr));
        pushButton_page5->setText(QCoreApplication::translate("TeacherMainWindow", "5", nullptr));
        pushButton_ellipsis->setText(QCoreApplication::translate("TeacherMainWindow", "...", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("TeacherMainWindow", "\345\255\246\345\217\267", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("TeacherMainWindow", "\345\247\223\345\220\215", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("TeacherMainWindow", "\346\200\247\345\210\253", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("TeacherMainWindow", "\345\271\264\351\276\204", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("TeacherMainWindow", "\347\217\255\347\272\247", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("TeacherMainWindow", "\347\224\265\350\257\235", nullptr));
        comboBox->setItemText(6, QCoreApplication::translate("TeacherMainWindow", "\345\234\260\345\235\200", nullptr));

        menu_student->setTitle(QCoreApplication::translate("TeacherMainWindow", "\345\255\246\347\224\237\347\256\241\347\220\206", nullptr));
        menu_score->setTitle(QCoreApplication::translate("TeacherMainWindow", "\346\210\220\347\273\251\347\256\241\347\220\206", nullptr));
        menu_data->setTitle(QCoreApplication::translate("TeacherMainWindow", "\346\225\260\346\215\256\346\223\215\344\275\234", nullptr));
        menu_personal->setTitle(QCoreApplication::translate("TeacherMainWindow", "\350\264\246\345\217\267\350\256\276\347\275\256", nullptr));
        menu->setTitle(QCoreApplication::translate("TeacherMainWindow", "\350\257\276\347\250\213\347\256\241\347\220\206", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("TeacherMainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TeacherMainWindow: public Ui_TeacherMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEACHERMAINWINDOW_H
