#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPair>
#include <QCryptographicHash>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    // 单例模式，确保只有一个数据库管理器
    static DatabaseManager* instance();

    // 初始化所有数据库
    bool initializeDatabases();

    // 判断管理员是否存在
    bool adminExists();
    // 用户相关操作
    bool registerUser(const QString &username, const QString &password,
                      const QString &userType, const QString &teacherName = "",
                      const QString &question1 = "", const QString &answer1 = "",
                      const QString &question2 = "", const QString &answer2 = "");

    bool validateUser(const QString &username, const QString &password,
                      QString &userType, QString &userId);

    // 密保问题相关操作
    QPair<QString, QString> getSecurityQuestions(const QString &username);
    bool validateSecurityAnswer(const QString &username, int questionNum,
                                const QString &answer);
    bool updatePassword(const QString &username, const QString &newPassword);

    // 获取数据库连接
    QSqlDatabase getSystemDatabase();
    QSqlDatabase getTeacherDatabase(const QString &teacherId);

    // 销毁单例实例
    static void destroy();

    // 关闭教师数据库连接
    void closeTeacherDatabase(const QString &teacherId);

    // 用于修改密码和密保问题
    QString getUsernameByUserId(const QString &userId);


private:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // 初始化系统数据库（用户管理）
    bool initSystemDatabase();

    // 为教师创建个人数据库
    bool createTeacherDatabase(const QString &teacherId);

    QString m_workbenchPath;  // 工作台目录路径

    static DatabaseManager* m_instance;

    // 密码哈希函数
    QString hashPassword(const QString &password);
};

#endif // DATABASEMANAGER_H
