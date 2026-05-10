#include "databasemanager.h"

DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager* DatabaseManager::instance()
{
    if (!m_instance) {
        m_instance = new DatabaseManager();
    }
    return m_instance;
}

void DatabaseManager::destroy()
{
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
        qDebug() << "DatabaseManager 实例已销毁";
    }
}

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // 获取工作台目录（程序所在目录）
    m_workbenchPath = QDir::currentPath();
    qDebug() << "工作台目录：" << m_workbenchPath;
}

DatabaseManager::~DatabaseManager()
{
    // 关闭所有数据库连接
    if (QSqlDatabase::contains("SYSTEM")) {
        QSqlDatabase::removeDatabase("SYSTEM");
    }

    // 注意：这里只移除了SYSTEM连接，教师数据库连接会在程序结束时自动清理
    qDebug() << "DatabaseManager 析构函数调用";
}

bool DatabaseManager::initializeDatabases()
{
    // 1. 初始化系统数据库
    if (!initSystemDatabase()) {
        QMessageBox::critical(nullptr, "错误", "初始化系统数据库失败！");
        return false;
    }

    qDebug() << "数据库初始化完成";
    return true;
}

bool DatabaseManager::initSystemDatabase()
{
    // 系统数据库路径
    QString systemDbPath = m_workbenchPath + "/Teacher_System.db";
    qDebug() << "系统数据库路径：" << systemDbPath;

    // 如果已有连接，先移除
    if (QSqlDatabase::contains("SYSTEM")) {
        QSqlDatabase::removeDatabase("SYSTEM");
    }

    // 创建系统数据库连接
    QSqlDatabase systemDb = QSqlDatabase::addDatabase("QSQLITE", "SYSTEM");
    systemDb.setDatabaseName(systemDbPath);

    // 打开数据库（如果不存在会自动创建）
    if (!systemDb.open()) {
        qDebug() << "无法打开系统数据库：" << systemDb.lastError().text();
        return false;
    }

    // 创建用户表（包含密保问题）
    QSqlQuery query(systemDb);
    QString sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id VARCHAR(20) UNIQUE NOT NULL, "
        "username VARCHAR(50) NOT NULL, "
        "password VARCHAR(50) NOT NULL, "
        "user_type VARCHAR(10) NOT NULL, "  // admin 或 teacher
        "teacher_name VARCHAR(50), "
        "question1 VARCHAR(200) NOT NULL, "
        "answer1 VARCHAR(200) NOT NULL, "
        "question2 VARCHAR(200) NOT NULL, "
        "answer2 VARCHAR(200) NOT NULL, "
        "created_time DATETIME DEFAULT (datetime('now', 'localtime'))"
        ")";

    if (!query.exec(sql)) {
        qDebug() << "创建用户表失败：" << query.lastError().text();
        systemDb.close();
        return false;
    }

    // 检查是否有用户，如果没有则提示先注册
    if (!query.exec("SELECT COUNT(*) FROM users")) {
        qDebug() << "查询用户数量失败：" << query.lastError().text();
    } else if (query.next() && query.value(0).toInt() == 0) {
        qDebug() << "系统中暂无用户，请先注册管理员或教师";
    }

    qDebug() << "系统数据库初始化完成";
    return true;
}

bool DatabaseManager::adminExists()
{
    QSqlDatabase db = getSystemDatabase();
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE user_type = 'admin'");

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

bool DatabaseManager::registerUser(const QString &username, const QString &password,
                                   const QString &userType, const QString &teacherName,
                                   const QString &question1, const QString &answer1,
                                   const QString &question2, const QString &answer2)
{
    QSqlDatabase db = getSystemDatabase();
    if (!db.isOpen()) {
        qDebug() << "数据库未打开，尝试重新打开...";
        if (!db.open()) {
            qDebug() << "无法打开数据库：" << db.lastError().text();
            return false;
        }
    }

    qDebug() << "开始注册用户：" << username << "，类型：" << userType;

    // 检查用户名是否已存在
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    checkQuery.addBindValue(username);

    if (!checkQuery.exec()) {
        qDebug() << "检查用户名失败：" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            qDebug() << "注册失败：用户名已存在：" << username;
            return false;
        }
    }

    // 生成用户ID
    QString userId;
    if (userType == "teacher") {
        // 查询当前有多少教师
        QSqlQuery countQuery(db);
        if (countQuery.exec("SELECT COUNT(*) FROM users WHERE user_type = 'teacher'") && countQuery.next()) {
            int teacherCount = countQuery.value(0).toInt() + 1;
            userId = QString("teacher_%1").arg(teacherCount, 3, 10, QChar('0'));
        } else {
            userId = "teacher_001";
        }
    } else {
        userId = "admin_VIP";
    }

    qDebug() << "生成的用户ID：" << userId;

    // 插入用户数据
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (user_id, username, password, user_type, teacher_name, "
                  "question1, answer1, question2, answer2) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(username);
    // 对密码进行哈希
    QString hashedPassword = hashPassword(password);
    query.addBindValue(hashedPassword);
    query.addBindValue(userType);
    query.addBindValue(teacherName);
    query.addBindValue(question1);
    query.addBindValue(answer1);
    query.addBindValue(question2);
    query.addBindValue(answer2);

    if (!query.exec()) {
        qDebug() << "插入用户数据失败：" << query.lastError().text();
        qDebug() << "SQL错误：" << query.lastQuery();
        return false;
    }

    qDebug() << "用户注册成功：" << username;

    // 如果是教师，创建个人数据库
    if (userType == "teacher") {
        createTeacherDatabase(userId);
    }

    return true;
}

bool DatabaseManager::validateUser(const QString &username, const QString &password,
                                   QString &userType, QString &userId)
{
    QSqlDatabase db = getSystemDatabase();
    if (!db.isOpen()) {
        qDebug() << "验证用户时数据库未打开";
        userType = "";
        userId = "";
        return false;
    }

    QSqlQuery query(db);

    // 如果没有提供密码，只检查用户名是否存在
    if (password.isEmpty()) {
        query.prepare("SELECT user_id, user_type FROM users WHERE username = ?");
        query.addBindValue(username);
    } else {
        // 对密码进行哈希后比较
        QString hashedPassword = hashPassword(password);
        query.prepare("SELECT user_id, user_type FROM users WHERE username = ? AND password = ?");
        query.addBindValue(username);
        query.addBindValue(hashedPassword);
    }

    if (!query.exec()) {
        qDebug() << "验证用户失败：" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        userId = query.value(0).toString();
        userType = query.value(1).toString();
        qDebug() << "用户验证成功：" << username << "，类型：" << userType;
        return true;
    } else {
        qDebug() << "用户验证失败：" << username;
        return false;
    }
}

QPair<QString, QString> DatabaseManager::getSecurityQuestions(const QString &username)
{
    QPair<QString, QString> questions;

    QSqlDatabase db = getSystemDatabase();
    if (!db.isOpen()) {
        qDebug() << "获取密保问题时数据库未打开";
        return questions;
    }

    QSqlQuery query(db);
    query.prepare("SELECT question1, question2 FROM users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        questions.first = query.value(0).toString();
        questions.second = query.value(1).toString();
    } else {
        qDebug() << "获取密保问题失败：" << query.lastError().text();
    }

    return questions;
}

bool DatabaseManager::validateSecurityAnswer(const QString &username, int questionNum,
                                             const QString &answer)
{
    if (questionNum != 1 && questionNum != 2) {
        return false;
    }

    QSqlDatabase db = getSystemDatabase();
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    QString fieldName = (questionNum == 1) ? "answer1" : "answer2";
    query.prepare(QString("SELECT %1 FROM users WHERE username = ?").arg(fieldName));
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        QString correctAnswer = query.value(0).toString();
        return (answer == correctAnswer);
    }

    return false;
}

bool DatabaseManager::updatePassword(const QString &username, const QString &newPassword)
{
    QSqlDatabase db = getSystemDatabase();
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    // 对新密码进行哈希
    QString hashedPassword = hashPassword(newPassword);
    query.prepare("UPDATE users SET password = ? WHERE username = ?");
    query.addBindValue(hashedPassword);
    query.addBindValue(username);

    bool success = query.exec();
    if (success) {
        qDebug() << "密码更新成功：" << username;
    } else {
        qDebug() << "密码更新失败：" << query.lastError().text();
    }

    return success;
}

bool DatabaseManager::createTeacherDatabase(const QString &teacherId)
{
    // 创建教师文件夹
    QString teacherDir = m_workbenchPath + "/" + teacherId;
    QDir dir(teacherDir);
    if (!dir.exists()) {
        if (!dir.mkpath(teacherDir)) {
            qDebug() << "创建教师文件夹失败：" << teacherDir;
            return false;
        }
    }

    // 创建教师个人数据库
    QString dbPath = teacherDir + "/data.db";
    QString connectionName = "TEACHER_" + teacherId;

    QSqlDatabase teacherDb = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    teacherDb.setDatabaseName(dbPath);

    if (!teacherDb.open()) {
        qDebug() << "无法创建教师数据库：" << teacherDb.lastError().text();
        return false;
    }

    // 创建学生表
    QSqlQuery query(teacherDb);
    QString sql =
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "student_id VARCHAR(20) UNIQUE NOT NULL, "
        "name VARCHAR(50) NOT NULL, "
        "gender VARCHAR(10), "
        "age INTEGER, "
        "class_name VARCHAR(50), "
        "phone VARCHAR(20), "
        "address VARCHAR(200), "
        "created_time DATETIME DEFAULT (datetime('now', 'localtime'))"
        ")";

    if (!query.exec(sql)) {
        qDebug() << "创建学生表失败：" << query.lastError().text();
        teacherDb.close();
        return false;
    }

    // 新增：创建学科表
    sql = "CREATE TABLE IF NOT EXISTS subjects ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "subject_id VARCHAR(20) UNIQUE NOT NULL, "
          "subject_name VARCHAR(50) NOT NULL, "
          "credit DECIMAL(3,1) DEFAULT 1.0, "  // 学分
          "description TEXT, "
          "created_time DATETIME DEFAULT (datetime('now', 'localtime'))"
          ")";

    if (!query.exec(sql)) {
        qDebug() << "创建学科表失败：" << query.lastError().text();
        teacherDb.close();
        return false;
    }

    // 新增：创建成绩表
    sql = "CREATE TABLE IF NOT EXISTS grades ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "student_id VARCHAR(20) NOT NULL, "
          "subject_id VARCHAR(20) NOT NULL, "
          "score DECIMAL(5,2) DEFAULT 0.0, "  // 成绩
          "exam_date DATE, "
          "semester VARCHAR(20), "  // 学期，如：2023-2024-1
          "remark TEXT, "
          "created_time DATETIME DEFAULT (datetime('now', 'localtime')), "
          "FOREIGN KEY (student_id) REFERENCES students(student_id), "
          "FOREIGN KEY (subject_id) REFERENCES subjects(subject_id), "
          "UNIQUE(student_id, subject_id, semester)"  // 同一学生同一学科同一学期只能有一条记录
          ")";

    if (!query.exec(sql)) {
        qDebug() << "创建成绩表失败：" << query.lastError().text();
        teacherDb.close();
        return false;
    }

    qDebug() << "教师数据库创建成功：" << dbPath;
    return true;
}

QSqlDatabase DatabaseManager::getSystemDatabase()
{
    // 检查是否已有打开的连接
    if (QSqlDatabase::contains("SYSTEM")) {
        QSqlDatabase db = QSqlDatabase::database("SYSTEM");
        if (db.isOpen()) {
            return db;
        }
    }

    // 创建新的连接
    QString dbPath = m_workbenchPath + "/Teacher_System.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "SYSTEM");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "无法打开系统数据库：" << db.lastError().text();
    }

    return db;
}

QSqlDatabase DatabaseManager::getTeacherDatabase(const QString &teacherId)
{
    QString dbPath = m_workbenchPath + "/" + teacherId + "/data.db";
    QString connectionName = "TEACHER_DB_" + teacherId;

    // 检查是否已存在连接
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        if (db.isOpen()) {
            return db;
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "无法打开教师数据库：" << db.lastError().text();
        // 返回一个无效的数据库连接
        return QSqlDatabase();
    }

    return db;
}

QString DatabaseManager::hashPassword(const QString &password)
{
    // 使用 SHA256 哈希
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
        );
    return QString(hash.toHex());
}

void DatabaseManager::closeTeacherDatabase(const QString &teacherId)
{
    QString connectionName = "TEACHER_DB_" + teacherId;

    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
        qDebug() << "已关闭教师数据库连接：" << connectionName;
    }
}

QString DatabaseManager::getUsernameByUserId(const QString &userId)
{
    QSqlDatabase db = getSystemDatabase();
    if (!db.isOpen()) {
        return "";
    }

    QSqlQuery query(db);
    query.prepare("SELECT username FROM users WHERE user_id = ?");
    query.addBindValue(userId);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}
