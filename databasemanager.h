#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

/**
 * @brief 统一数据库管理器（SQLite 单库）。
 *
 * 负责：数据库路径解析、连接与 PRAGMA、建库脚本执行、演示数据、
 * 账号认证、密码/密保哈希、事务、备份恢复。
 */
class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager *instance();
    static void destroy();

    // ---------- 初始化与连接 ----------
    bool initializeDatabases();
    QSqlDatabase database();
    // 兼容旧代码：统一库之后二者都返回同一个连接
    QSqlDatabase getTeacherDatabase(const QString &teacherId = QString());
    QSqlDatabase getSystemDatabase();
    void closeTeacherDatabase(const QString &teacherId = QString());
    void closeDatabase();

    QString databasePath() const { return m_dbPath; }
    QString dataDirectory() const { return m_dataDir; }
    QString lastError() const { return m_lastError; }

    // ---------- SQL 脚本 ----------
    bool executeScriptResource(const QString &resourcePath, QString *error = nullptr);
    bool executeScriptText(const QString &script, QString *error = nullptr);

    // ---------- 账号与认证 ----------
    bool adminExists();
    bool userExists(const QString &username);
    bool registerUser(const QString &username, const QString &password,
                      const QString &userType, const QString &teacherName = QString(),
                      const QString &question1 = QString(), const QString &answer1 = QString(),
                      const QString &question2 = QString(), const QString &answer2 = QString());
    bool validateUser(const QString &username, const QString &password,
                      QString &userType, QString &userId);
    bool verifyPassword(const QString &userId, const QString &password);
    bool changePassword(const QString &userId, const QString &oldPassword,
                        const QString &newPassword);
    bool resetPassword(const QString &username, const QString &newPassword);
    // 兼容旧接口：忘记密码流程使用
    bool updatePassword(const QString &username, const QString &newPassword)
    {
        return resetPassword(username, newPassword);
    }
    bool changeSecurityQuestions(const QString &userId, const QString &password,
                                 const QString &question1, const QString &answer1,
                                 const QString &question2, const QString &answer2);
    QPair<QString, QString> getSecurityQuestions(const QString &username);
    bool validateSecurityAnswer(const QString &username, int questionNum,
                                const QString &answer);
    bool mustChangePassword(const QString &userId);
    QString getUsernameByUserId(const QString &userId);
    QString getUserTypeByUserId(const QString &userId);
    QString getDisplayName(const QString &userId);

    // 学生自助账号
    bool createStudentAccount(const QString &studentId, QString &username,
                              QString &initialPassword);
    bool deleteStudentAccount(const QString &studentId);
    bool deleteTeacher(const QString &teacherId);

    // ---------- 会话（审计触发器使用） ----------
    QString currentUser();
    void setCurrentUser(const QString &userId);

    // ---------- 备份与恢复 ----------
    bool backupTo(const QString &filePath, QString *error = nullptr);
    bool verifyBackup(const QString &filePath, QString *error = nullptr);
    bool restoreFrom(const QString &filePath, QString *error = nullptr);

    // ---------- 演示数据 ----------
    bool seedDemoData(QString *error = nullptr);

    // ---------- 哈希工具（公开便于测试/迁移工具） ----------
    static QString makeSalt();
    static QString hashSecret(const QString &secret, const QString &salt);
    static bool verifySecret(const QString &secret, const QString &hash,
                             const QString &salt);

private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager() override;

    QString resolveDatabasePath();
    bool openDatabase(QString *error = nullptr);
    void applyPragmas();
    QString readResource(const QString &resourcePath, QString *error);

    bool insertUser(const QString &userId, const QString &username,
                    const QString &password, const QString &userType,
                    const QString &question1, const QString &answer1,
                    const QString &question2, const QString &answer2,
                    bool mustChangePassword, QString *error);
    bool isLegacyHash(const QString &hash, const QString &salt) const;
    static bool constantTimeEquals(const QString &a, const QString &b);
    QString nextTeacherId();

    QString m_dbPath;
    QString m_dataDir;
    QString m_lastError;
    bool m_initialized = false;

    static DatabaseManager *m_instance;
};

#endif // DATABASEMANAGER_H
