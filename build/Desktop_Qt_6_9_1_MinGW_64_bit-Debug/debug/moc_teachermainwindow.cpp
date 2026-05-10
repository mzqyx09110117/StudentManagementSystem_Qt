/****************************************************************************
** Meta object code from reading C++ file 'teachermainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../teachermainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'teachermainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17TeacherMainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto TeacherMainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN17TeacherMainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TeacherMainWindow",
        "on_pushButton_prev_clicked",
        "",
        "on_pushButton_page1_clicked",
        "on_pushButton_page2_clicked",
        "on_pushButton_page3_clicked",
        "on_pushButton_page4_clicked",
        "on_pushButton_page5_clicked",
        "on_pushButton_ellipsis_clicked",
        "on_pushButton_next_clicked",
        "on_lineEdit_search_returnPressed",
        "on_pushButton_search_clicked",
        "on_listWidget_classes_itemClicked",
        "QListWidgetItem*",
        "item",
        "on_tableWidget_students_cellDoubleClicked",
        "row",
        "column",
        "on_action_addStudent_triggered",
        "on_action_deleteStudent_triggered",
        "on_action_editStudent_triggered",
        "on_action_refreshList_triggered",
        "on_action_inputGrade_triggered",
        "on_action_modifyGrade_triggered",
        "on_action_viewGrade_triggered",
        "on_action_changePassword_triggered",
        "on_action_changequestions_triggered",
        "on_action_add_triggered",
        "on_action_edit_triggered",
        "on_action_delete_triggered",
        "on_action_refresh_triggered",
        "on_action_exit_triggered",
        "on_listWidget_classes_customContextMenuRequested",
        "pos",
        "on_action_add_class_triggered",
        "on_action_edit_class_triggered",
        "on_action_delete_class_triggered",
        "on_action_exit_2_triggered",
        "on_action_addSubject_triggered",
        "on_action_editSubject_triggered",
        "on_action_deleteSubject_triggered",
        "on_action_manageSubjects_triggered",
        "on_action_classGPA_triggered",
        "on_action_allStudentsGPA_triggered",
        "on_action_subjectClass_triggered",
        "on_action_subjectAll_triggered",
        "on_action_addSubject_2_triggered",
        "on_action_editSubject_2_triggered",
        "on_action_deleteSubject_2_triggered",
        "on_action_deleteGrade_triggered"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_pushButton_prev_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_page1_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_page2_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_page3_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_page4_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_page5_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_ellipsis_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_next_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_lineEdit_search_returnPressed'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_pushButton_search_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_listWidget_classes_itemClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 13, 14 },
        }}),
        // Slot 'on_tableWidget_students_cellDoubleClicked'
        QtMocHelpers::SlotData<void(int, int)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 16 }, { QMetaType::Int, 17 },
        }}),
        // Slot 'on_action_addStudent_triggered'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_deleteStudent_triggered'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_editStudent_triggered'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_refreshList_triggered'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_inputGrade_triggered'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_modifyGrade_triggered'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_viewGrade_triggered'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_changePassword_triggered'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_changequestions_triggered'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_add_triggered'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_edit_triggered'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_delete_triggered'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_refresh_triggered'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_exit_triggered'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_listWidget_classes_customContextMenuRequested'
        QtMocHelpers::SlotData<void(const QPoint &)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 33 },
        }}),
        // Slot 'on_action_add_class_triggered'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_edit_class_triggered'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_delete_class_triggered'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_exit_2_triggered'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_addSubject_triggered'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_editSubject_triggered'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_deleteSubject_triggered'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_manageSubjects_triggered'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_classGPA_triggered'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_allStudentsGPA_triggered'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_subjectClass_triggered'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_subjectAll_triggered'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_addSubject_2_triggered'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_editSubject_2_triggered'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_deleteSubject_2_triggered'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_action_deleteGrade_triggered'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TeacherMainWindow, qt_meta_tag_ZN17TeacherMainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TeacherMainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17TeacherMainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17TeacherMainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17TeacherMainWindowE_t>.metaTypes,
    nullptr
} };

void TeacherMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TeacherMainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_pushButton_prev_clicked(); break;
        case 1: _t->on_pushButton_page1_clicked(); break;
        case 2: _t->on_pushButton_page2_clicked(); break;
        case 3: _t->on_pushButton_page3_clicked(); break;
        case 4: _t->on_pushButton_page4_clicked(); break;
        case 5: _t->on_pushButton_page5_clicked(); break;
        case 6: _t->on_pushButton_ellipsis_clicked(); break;
        case 7: _t->on_pushButton_next_clicked(); break;
        case 8: _t->on_lineEdit_search_returnPressed(); break;
        case 9: _t->on_pushButton_search_clicked(); break;
        case 10: _t->on_listWidget_classes_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 11: _t->on_tableWidget_students_cellDoubleClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 12: _t->on_action_addStudent_triggered(); break;
        case 13: _t->on_action_deleteStudent_triggered(); break;
        case 14: _t->on_action_editStudent_triggered(); break;
        case 15: _t->on_action_refreshList_triggered(); break;
        case 16: _t->on_action_inputGrade_triggered(); break;
        case 17: _t->on_action_modifyGrade_triggered(); break;
        case 18: _t->on_action_viewGrade_triggered(); break;
        case 19: _t->on_action_changePassword_triggered(); break;
        case 20: _t->on_action_changequestions_triggered(); break;
        case 21: _t->on_action_add_triggered(); break;
        case 22: _t->on_action_edit_triggered(); break;
        case 23: _t->on_action_delete_triggered(); break;
        case 24: _t->on_action_refresh_triggered(); break;
        case 25: _t->on_action_exit_triggered(); break;
        case 26: _t->on_listWidget_classes_customContextMenuRequested((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 27: _t->on_action_add_class_triggered(); break;
        case 28: _t->on_action_edit_class_triggered(); break;
        case 29: _t->on_action_delete_class_triggered(); break;
        case 30: _t->on_action_exit_2_triggered(); break;
        case 31: _t->on_action_addSubject_triggered(); break;
        case 32: _t->on_action_editSubject_triggered(); break;
        case 33: _t->on_action_deleteSubject_triggered(); break;
        case 34: _t->on_action_manageSubjects_triggered(); break;
        case 35: _t->on_action_classGPA_triggered(); break;
        case 36: _t->on_action_allStudentsGPA_triggered(); break;
        case 37: _t->on_action_subjectClass_triggered(); break;
        case 38: _t->on_action_subjectAll_triggered(); break;
        case 39: _t->on_action_addSubject_2_triggered(); break;
        case 40: _t->on_action_editSubject_2_triggered(); break;
        case 41: _t->on_action_deleteSubject_2_triggered(); break;
        case 42: _t->on_action_deleteGrade_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject *TeacherMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TeacherMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17TeacherMainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int TeacherMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 43)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 43;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 43)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 43;
    }
    return _id;
}
QT_WARNING_POP
