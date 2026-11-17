#include "selftest.h"

#include "databasemanager.h"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>

namespace {

int passed = 0;
int failed = 0;
QTextStream *stream = nullptr;

void check(const QString &name, bool ok, const QString &detail = QString())
{
    if (ok) {
        ++passed;
        *stream << "[PASS] " << name << '\n';
    } else {
        ++failed;
        *stream << "[FAIL] " << name;
        if (!detail.isEmpty())
            *stream << "  (" << detail << ")";
        *stream << '\n';
    }
    stream->flush();
}

int scalar(const QString &sql)
{
    QSqlQuery query(DatabaseManager::instance()->database());
    if (query.exec(sql) && query.next())
        return query.value(0).toInt();
    return -1;
}

bool integrityOk()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    return query.exec(QStringLiteral("PRAGMA integrity_check")) && query.next()
           && query.value(0).toString() == QLatin1String("ok");
}

bool foreignKeysOk()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    return query.exec(QStringLiteral("PRAGMA foreign_key_check")) && !query.next();
}

} // namespace

int runSelfTest(const QString &logPath)
{
    QFile logFile(logPath);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return 2;

    QTextStream out(&logFile);
    out.setEncoding(QStringConverter::Utf8);
    stream = &out;

    DatabaseManager *manager = DatabaseManager::instance();
    QString userType;
    QString userId;

    check(QStringLiteral("数据库已打开"), manager->database().isOpen(), manager->lastError());
    check(QStringLiteral("数据库文件存在"), QFileInfo::exists(manager->databasePath()),
          manager->databasePath());

    check(QStringLiteral("管理员登录"),
          manager->validateUser(QStringLiteral("admin"), QStringLiteral("admin123"), userType, userId)
              && userType == QStringLiteral("admin"), manager->lastError());
    check(QStringLiteral("教师登录"),
          manager->validateUser(QStringLiteral("teacher01"), QStringLiteral("123456"), userType, userId)
              && userType == QStringLiteral("teacher"), manager->lastError());
    check(QStringLiteral("学生登录"),
          manager->validateUser(QStringLiteral("2024001"), QStringLiteral("123456"), userType, userId)
              && userType == QStringLiteral("student"), manager->lastError());
    check(QStringLiteral("错误密码被拒绝"),
          !manager->validateUser(QStringLiteral("admin"), QStringLiteral("wrong-password"),
                                 userType, userId));
    check(QStringLiteral("空密码被拒绝"),
          !manager->validateUser(QStringLiteral("admin"), QString(), userType, userId));

    check(QStringLiteral("演示学生数量"),
          scalar(QStringLiteral("SELECT COUNT(*) FROM students")) == 30);
    check(QStringLiteral("演示成绩数量"),
          scalar(QStringLiteral("SELECT COUNT(*) FROM grades")) == 480);
    check(QStringLiteral("视图 GPA 可查询"),
          scalar(QStringLiteral("SELECT COUNT(*) FROM v_student_overview WHERE gpa > 0")) > 0);

    // ---------- 学生账号生命周期 ----------
    {
        QString username;
        QString initialPassword;
        check(QStringLiteral("开通学生账号"),
              manager->createStudentAccount(QStringLiteral("2024002"), username, initialPassword));
        const QString studentUserId = QStringLiteral("stu_") + username;
        check(QStringLiteral("初始密码需强制修改"), manager->mustChangePassword(studentUserId));
        check(QStringLiteral("学生初始密码可登录"),
              manager->validateUser(username, initialPassword, userType, userId)
                  && userType == QStringLiteral("student"));
        check(QStringLiteral("学生修改密码"),
              manager->changePassword(studentUserId, initialPassword, QStringLiteral("Abc12345")));
        check(QStringLiteral("学生新密码可登录"),
              manager->validateUser(username, QStringLiteral("Abc12345"), userType, userId));
        check(QStringLiteral("注销学生账号"),
              manager->deleteStudentAccount(QStringLiteral("2024002")));
    }

    // ---------- 旧版哈希自动升级 ----------
    {
        const QString legacyHash = QString::fromLatin1(
            QCryptographicHash::hash("oldpass123", QCryptographicHash::Sha256).toHex());
        QSqlQuery insertLegacy(DatabaseManager::instance()->database());
        insertLegacy.prepare(QStringLiteral(
            "INSERT INTO users (user_id, username, password_hash, password_salt, user_type) "
            "VALUES ('legacy_001', 'legacy01', ?, '', 'teacher')"));
        insertLegacy.addBindValue(legacyHash);
        check(QStringLiteral("写入旧版账号"), insertLegacy.exec(), insertLegacy.lastError().text());
        check(QStringLiteral("旧版哈希可登录"),
              manager->validateUser(QStringLiteral("legacy01"), QStringLiteral("oldpass123"),
                                    userType, userId));

        QSqlQuery upgraded(DatabaseManager::instance()->database());
        upgraded.prepare(QStringLiteral(
            "SELECT password_salt, password_hash FROM users WHERE user_id = 'legacy_001'"));
        bool isUpgraded = false;
        if (upgraded.exec() && upgraded.next()) {
            isUpgraded = !upgraded.value(0).toString().isEmpty()
                         && upgraded.value(1).toString().startsWith(QStringLiteral("sha256i$"));
        }
        check(QStringLiteral("旧版哈希已升级"), isUpgraded);

        QSqlQuery deleteLegacy(DatabaseManager::instance()->database());
        deleteLegacy.exec(QStringLiteral("DELETE FROM users WHERE user_id = 'legacy_001'"));
    }

    // ---------- 密保 ----------
    check(QStringLiteral("修改密保问题"),
          manager->changeSecurityQuestions(QStringLiteral("admin_001"), QStringLiteral("admin123"),
                                           QStringLiteral("问题A"), QStringLiteral("答案A"),
                                           QStringLiteral("问题B"), QStringLiteral("答案B")));
    check(QStringLiteral("密保答案校验"),
          manager->validateSecurityAnswer(QStringLiteral("admin"), 1, QStringLiteral("答案A")));
    check(QStringLiteral("错误密保答案被拒绝"),
          !manager->validateSecurityAnswer(QStringLiteral("admin"), 1, QStringLiteral("错误")));

    // ---------- 教师注册与删除 ----------
    QString newTeacherId;
    check(QStringLiteral("注册教师账号"),
          manager->registerUser(QStringLiteral("newteacher"), QStringLiteral("NewPass123"),
                                QStringLiteral("teacher"), QStringLiteral("新教师"),
                                QStringLiteral("问题A"), QStringLiteral("答案A"),
                                QStringLiteral("问题B"), QStringLiteral("答案B")),
          manager->lastError());
    {
        QSqlQuery query(DatabaseManager::instance()->database());
        query.prepare(QStringLiteral(
            "SELECT user_id FROM users WHERE username = 'newteacher'"));
        if (query.exec() && query.next())
            newTeacherId = query.value(0).toString();
    }
    check(QStringLiteral("教师档案已创建"), !newTeacherId.isEmpty());
    check(QStringLiteral("重复管理员注册被拒绝"),
          !manager->registerUser(QStringLiteral("admin2"), QStringLiteral("Admin1234"),
                                 QStringLiteral("admin"), QString(),
                                 QStringLiteral("问题A"), QStringLiteral("答案A"),
                                 QStringLiteral("问题B"), QStringLiteral("答案B")));
    check(QStringLiteral("删除教师账号"), manager->deleteTeacher(newTeacherId));
    check(QStringLiteral("教师账号已删除"),
          scalar(QStringLiteral("SELECT COUNT(*) FROM users WHERE user_id = '%1'")
                     .arg(newTeacherId)) == 0);

    // ---------- 约束与级联（所有 QSqlQuery 在本作用域结束时销毁） ----------
    {
        QSqlQuery query(DatabaseManager::instance()->database());
        query.prepare(QStringLiteral(
            "INSERT INTO students (student_id, name, gender, age, class_name) "
            "VALUES ('TEST0001', '自检学生', '男', 20, '人工智能2401')"));
        check(QStringLiteral("插入测试学生"), query.exec(), query.lastError().text());

        QSqlQuery badScore(DatabaseManager::instance()->database());
        badScore.prepare(QStringLiteral(
            "INSERT INTO grades (student_id, subject_id, semester, score) "
            "VALUES ('TEST0001', 'S001', '2099-2100-1', 101)"));
        check(QStringLiteral("成绩 101 被拒绝"), !badScore.exec());

        QSqlQuery badSemester(DatabaseManager::instance()->database());
        badSemester.prepare(QStringLiteral(
            "INSERT INTO grades (student_id, subject_id, semester, score) "
            "VALUES ('TEST0001', 'S001', '2099-1', 80)"));
        check(QStringLiteral("非法学期被拒绝"), !badSemester.exec());

        manager->setCurrentUser(QStringLiteral("teacher_001"));
        QSqlQuery gradeInsert(DatabaseManager::instance()->database());
        gradeInsert.prepare(QStringLiteral(
            "INSERT INTO grades (student_id, subject_id, semester, score, entered_by) "
            "VALUES ('TEST0001', 'S001', '2099-2100-1', 88, 'teacher_001')"));
        check(QStringLiteral("插入测试成绩"), gradeInsert.exec(), gradeInsert.lastError().text());
        check(QStringLiteral("触发器写入审计日志"),
              scalar(QStringLiteral("SELECT COUNT(*) FROM audit_log WHERE record_key LIKE 'TEST0001/%'")) == 1);

        QSqlQuery duplicate(DatabaseManager::instance()->database());
        duplicate.prepare(QStringLiteral(
            "INSERT INTO grades (student_id, subject_id, semester, score) "
            "VALUES ('TEST0001', 'S001', '2099-2100-1', 90)"));
        check(QStringLiteral("重复成绩被拒绝"), !duplicate.exec());

        QSqlQuery deleteStudent(DatabaseManager::instance()->database());
        deleteStudent.prepare(QStringLiteral(
            "DELETE FROM students WHERE student_id = 'TEST0001'"));
        check(QStringLiteral("删除测试学生"), deleteStudent.exec());
    }

    check(QStringLiteral("删除学生级联删除成绩"),
          scalar(QStringLiteral("SELECT COUNT(*) FROM grades WHERE student_id = 'TEST0001'")) == 0);

    // ---------- 备份 ----------
    const QString backupPath = QFileInfo(logPath).absoluteDir().filePath(
        QStringLiteral("selftest_backup.db"));
    QFile::remove(backupPath);

    QString error;
    check(QStringLiteral("数据库备份"), manager->backupTo(backupPath, &error), error);
    check(QStringLiteral("备份文件校验"), manager->verifyBackup(backupPath, &error), error);

    {
        QSqlQuery marker(DatabaseManager::instance()->database());
        marker.prepare(QStringLiteral(
            "INSERT INTO students (student_id, name, gender, age, class_name) "
            "VALUES ('TEST0002', '恢复前标记', '女', 20, '人工智能2401')"));
        check(QStringLiteral("写入恢复前标记"), marker.exec(), marker.lastError().text());
    }

    // ---------- 恢复 ----------
    check(QStringLiteral("从备份恢复"), manager->restoreFrom(backupPath, &error), error);
    check(QStringLiteral("恢复后标记消失"),
          scalar(QStringLiteral("SELECT COUNT(*) FROM students WHERE student_id = 'TEST0002'")) == 0);
    check(QStringLiteral("恢复后仍可登录"),
          manager->validateUser(QStringLiteral("admin"), QStringLiteral("admin123"),
                                userType, userId));
    check(QStringLiteral("恢复后完整性"), integrityOk());
    check(QStringLiteral("恢复后外键无冲突"), foreignKeysOk());

    *stream << "\n结果：" << passed << " 通过，" << failed << " 失败\n";
    stream->flush();
    logFile.close();
    stream = nullptr;
    return failed == 0 ? 0 : 1;
}
