#include "screenshottool.h"

#include "addstudentdialog.h"
#include "adminmainwindow.h"
#include "auditlogdialog.h"
#include "backupdialog.h"
#include "changepassworddialog.h"
#include "changequestionsdialog.h"
#include "databasemanager.h"
#include "editstudentdialog.h"
#include "forgotpassworddialog.h"
#include "gradedialog.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "restoredialog.h"
#include "statsdialog.h"
#include "studentmainwindow.h"
#include "subjectdialog.h"
#include "teachermainwindow.h"

#include <QApplication>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDoubleSpinBox>
#include <QEventLoop>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSqlQuery>
#include <QThread>

namespace {

bool showAndGrab(QWidget *widget, const QString &filePath)
{
    if (!widget)
        return false;

    widget->show();
    QApplication::processEvents(QEventLoop::AllEvents, 300);
    QThread::msleep(180);
    QApplication::processEvents(QEventLoop::AllEvents, 300);

    const QPixmap pixmap = widget->grab();
    const bool ok = pixmap.save(filePath, "PNG");
    widget->hide();
    QApplication::processEvents(QEventLoop::AllEvents, 100);
    return ok;
}

QStringList classNames()
{
    QStringList names;
    QSqlQuery query(DatabaseManager::instance()->database());
    if (query.exec(QStringLiteral("SELECT class_name FROM classes ORDER BY class_name"))) {
        while (query.next())
            names << query.value(0).toString();
    }
    return names;
}

void insertAuditSamples()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.exec(QStringLiteral(
        "UPDATE app_context SET current_user = 'teacher_001' WHERE id = 1"));
    query.exec(QStringLiteral(
        "UPDATE grades SET score = score + 1 "
        "WHERE student_id = '2024001' AND subject_id = 'S001' AND semester = '2024-2025-1'"));
    query.exec(QStringLiteral(
        "INSERT INTO grades (student_id, subject_id, semester, score, exam_date, entered_by) "
        "VALUES ('2024002', 'S008', '2025-2026-1', 88, '2025-12-20', 'teacher_002') "
        "ON CONFLICT(student_id, subject_id, semester) DO UPDATE SET score = excluded.score"));
    query.exec(QStringLiteral(
        "DELETE FROM grades WHERE student_id = '2024003' AND subject_id = 'S008' "
        "AND semester = '2024-2025-2'"));
    query.exec(QStringLiteral(
        "UPDATE app_context SET current_user = 'admin_001' WHERE id = 1"));
}

} // namespace

bool ScreenshotTool::run(const QString &outputDir)
{
    QDir dir(outputDir);
    if (!dir.exists() && !QDir().mkpath(outputDir)) {
        qWarning() << "无法创建截图目录：" << outputDir;
        return false;
    }

    const QString base = dir.absolutePath();
    int saved = 0;

    // 1. 登录界面
    {
        LoginDialog dialog;
        if (auto *username = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_username")))
            username->setText(QStringLiteral("teacher01"));
        if (auto *password = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_password")))
            password->setText(QStringLiteral("123456"));
        saved += showAndGrab(&dialog, base + QStringLiteral("/01_login.png")) ? 1 : 0;
    }

    // 2. 注册界面
    {
        RegisterDialog dialog;
        if (auto *username = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_username")))
            username->setText(QStringLiteral("teacher03"));
        saved += showAndGrab(&dialog, base + QStringLiteral("/02_register.png")) ? 1 : 0;
    }

    // 3. 忘记密码
    {
        ForgotPasswordDialog dialog;
        if (auto *username = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_username")))
            username->setText(QStringLiteral("teacher01"));
        saved += showAndGrab(&dialog, base + QStringLiteral("/03_forgot.png")) ? 1 : 0;
    }

    // 4. 教师主界面
    {
        TeacherMainWindow window(QStringLiteral("teacher_001"));
        saved += showAndGrab(&window, base + QStringLiteral("/04_teacher_main.png")) ? 1 : 0;
    }

    // 5. 添加学生
    {
        AddStudentDialog dialog;
        dialog.setClassItems(classNames());
        if (auto *id = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_studentId")))
            id->setText(QStringLiteral("2024031"));
        if (auto *name = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_name")))
            name->setText(QStringLiteral("林晓雨"));
        if (auto *phone = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_phone")))
            phone->setText(QStringLiteral("13800001234"));
        if (auto *address = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_address")))
            address->setText(QStringLiteral("示例省示例市大学路 99 号"));
        saved += showAndGrab(&dialog, base + QStringLiteral("/05_add_student.png")) ? 1 : 0;
    }

    // 6. 编辑学生
    {
        EditStudentDialog dialog(QStringLiteral("teacher_001"), QStringLiteral("2024001"),
                                 QStringLiteral("王小明"), QStringLiteral("男"), 19,
                                 QStringLiteral("人工智能2401"), QStringLiteral("13810000000"),
                                 QStringLiteral("示例省示例市大学路 1 号"));
        dialog.setClassItems(classNames());
        saved += showAndGrab(&dialog, base + QStringLiteral("/06_edit_student.png")) ? 1 : 0;
    }

    // 7. 成绩录入
    {
        GradeDialog dialog(QStringLiteral("teacher_001"));
        if (auto *score = dialog.findChild<QDoubleSpinBox *>(QStringLiteral("doubleSpinBox_score")))
            score->setValue(92.5);
        saved += showAndGrab(&dialog, base + QStringLiteral("/07_grade_input.png")) ? 1 : 0;
    }

    // 8. 课程管理
    {
        SubjectDialog dialog(QStringLiteral("teacher_001"), SubjectDialog::AddMode);
        if (auto *id = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_subjectId")))
            id->setText(QStringLiteral("S009"));
        if (auto *name = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_subjectName")))
            name->setText(QStringLiteral("机器学习"));
        saved += showAndGrab(&dialog, base + QStringLiteral("/08_subject.png")) ? 1 : 0;
    }

    // 9. 学生端
    {
        auto *window = new StudentMainWindow(QStringLiteral("stu_2024001"));
        saved += showAndGrab(window, base + QStringLiteral("/09_student.png")) ? 1 : 0;
        delete window;
    }

    // 10. 管理员端
    {
        auto *window = new AdminMainWindow(QStringLiteral("admin_001"));
        saved += showAndGrab(window, base + QStringLiteral("/10_admin_main.png")) ? 1 : 0;
        delete window;
    }

    /// 审计样本必须在管理员截图后、审计日志截图前生成
    insertAuditSamples();

    // 11. 统计看板（同时触发一次刷新，验证图表刷新逻辑）
    {
        StatsDialog dialog;
        for (QPushButton *button : dialog.findChildren<QPushButton *>()) {
            if (button->text() == QStringLiteral("刷新"))
                button->click();
        }
        saved += showAndGrab(&dialog, base + QStringLiteral("/11_dashboard.png")) ? 1 : 0;
    }

    // 12. 审计日志
    {
        AuditLogDialog dialog;
        saved += showAndGrab(&dialog, base + QStringLiteral("/12_audit_log.png")) ? 1 : 0;
    }

    // 13. 备份
    {
        BackupDialog dialog;
        saved += showAndGrab(&dialog, base + QStringLiteral("/13_backup.png")) ? 1 : 0;
    }

    // 14. 恢复
    {
        RestoreDialog dialog;
        saved += showAndGrab(&dialog, base + QStringLiteral("/14_restore.png")) ? 1 : 0;
    }

    // 15. 修改密码
    {
        ChangePasswordDialog dialog(QStringLiteral("teacher_001"), false);
        if (auto *oldPassword = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_oldPassword")))
            oldPassword->setText(QStringLiteral("123456"));
        if (auto *newPassword = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_password")))
            newPassword->setText(QStringLiteral("Teacher@2026"));
        if (auto *confirm = dialog.findChild<QLineEdit *>(QStringLiteral("lineEdit_confirm")))
            confirm->setText(QStringLiteral("Teacher@2026"));
        saved += showAndGrab(&dialog, base + QStringLiteral("/15_change_password.png")) ? 1 : 0;
    }

    // 16. 修改密保
    {
        ChangeQuestionsDialog dialog(QStringLiteral("teacher_001"));
        saved += showAndGrab(&dialog, base + QStringLiteral("/16_change_questions.png")) ? 1 : 0;
    }

    qInfo() << "截图完成：" << saved << " 张 ->" << base;
    return saved >= 12;
}
