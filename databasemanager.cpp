#include "databasemanager.h"

#include "sqlscript.h"
#include "validation.h"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

namespace {

const QString kConnectionName = QStringLiteral("SMS_MAIN");
const QString kHashPrefix = QStringLiteral("sha256i$10000$");
const int kHashIterations = 10000;

} // namespace

DatabaseManager *DatabaseManager::m_instance = nullptr;

DatabaseManager *DatabaseManager::instance()
{
    if (!m_instance)
        m_instance = new DatabaseManager();
    return m_instance;
}

void DatabaseManager::destroy()
{
    if (!m_instance)
        return;
    m_instance->closeDatabase();
    delete m_instance;
    m_instance = nullptr;
}

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    m_dbPath = resolveDatabasePath();
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

QString DatabaseManager::resolveDatabasePath()
{
    // 1) 测试/多实例：环境变量优先
    const QString envPath = qEnvironmentVariable("SMS_DB_PATH").trimmed();
    if (!envPath.isEmpty()) {
        const QFileInfo info(envPath);
        m_dataDir = info.absolutePath();
        QDir().mkpath(m_dataDir);
        return info.absoluteFilePath();
    }

    // 2) 程序目录/data；不可写时回退到用户 AppData
    const QString appDir = QCoreApplication::applicationDirPath();
    QString dataDir = QDir(appDir).filePath(QStringLiteral("data"));
    const bool appDirWritable = QDir().mkpath(dataDir) && QFileInfo(dataDir).isWritable();
    if (!appDirWritable) {
        dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (dataDir.isEmpty())
            dataDir = QDir::homePath() + QStringLiteral("/.student_management_system");
        QDir().mkpath(dataDir);
    }
    m_dataDir = dataDir;
    return QDir(dataDir).filePath(QStringLiteral("StudentSystem.db"));
}

bool DatabaseManager::openDatabase(QString *error)
{
    if (!QSqlDatabase::isDriverAvailable(QStringLiteral("QSQLITE"))) {
        const QString msg = QStringLiteral("Qt SQLite 驱动不可用（qsqlite.dll 未找到）");
        m_lastError = msg;
        if (error) *error = msg;
        return false;
    }

    QSqlDatabase db;
    if (QSqlDatabase::contains(kConnectionName)) {
        db = QSqlDatabase::database(kConnectionName, false);
    } else {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), kConnectionName);
    }

    if (db.databaseName().isEmpty())
        db.setDatabaseName(m_dbPath);
    db.setConnectOptions(QStringLiteral("QSQLITE_BUSY_TIMEOUT=5000"));

    if (!db.isOpen() && !db.open()) {
        const QString msg = QStringLiteral("无法打开数据库：%1\n%2")
                                .arg(m_dbPath, db.lastError().text());
        m_lastError = msg;
        if (error) *error = msg;
        return false;
    }

    applyPragmas();
    return true;
}

void DatabaseManager::applyPragmas()
{
    QSqlDatabase db = QSqlDatabase::database(kConnectionName, false);
    if (!db.isOpen())
        return;

    QSqlQuery q(db);
    q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
    q.exec(QStringLiteral("PRAGMA busy_timeout = 5000"));
    q.exec(QStringLiteral("PRAGMA journal_mode = WAL"));
    q.exec(QStringLiteral("PRAGMA synchronous = NORMAL"));

    // 再次确认外键真的开启，否则所有引用完整性都是摆设
    if (q.exec(QStringLiteral("PRAGMA foreign_keys")) && q.next()) {
        if (q.value(0).toInt() != 1)
            qWarning() << "DatabaseManager: foreign_keys 未开启";
    }
}

QSqlDatabase DatabaseManager::database()
{
    if (!QSqlDatabase::contains(kConnectionName) || !QSqlDatabase::database(kConnectionName, false).isOpen())
        openDatabase();
    return QSqlDatabase::database(kConnectionName, false);
}

QSqlDatabase DatabaseManager::getTeacherDatabase(const QString &teacherId)
{
    Q_UNUSED(teacherId);
    return database();
}

QSqlDatabase DatabaseManager::getSystemDatabase()
{
    return database();
}

void DatabaseManager::closeTeacherDatabase(const QString &teacherId)
{
    Q_UNUSED(teacherId);
}

void DatabaseManager::closeDatabase()
{
    if (!QSqlDatabase::contains(kConnectionName))
        return;
    {
        QSqlDatabase db = QSqlDatabase::database(kConnectionName, false);
        if (db.isOpen())
            db.close();
    }
    QSqlDatabase::removeDatabase(kConnectionName);
    m_initialized = false;
}

bool DatabaseManager::initializeDatabases()
{
    if (m_initialized && database().isOpen())
        return true;

    m_lastError.clear();
    QString error;
    if (!openDatabase(&error)) {
        m_lastError = error;
        return false;
    }

    if (!executeScriptResource(QStringLiteral(":/sql/schema.sql"), &error)) {
        m_lastError = error;
        return false;
    }

    QSqlQuery q(database());
    if (!q.exec(QStringLiteral("SELECT version FROM schema_meta WHERE id = 1")) || !q.next()) {
        m_lastError = QStringLiteral("数据库缺少 schema_meta 元数据：%1").arg(q.lastError().text());
        return false;
    }
    const int version = q.value(0).toInt();
    if (version > 1) {
        m_lastError = QStringLiteral("数据库版本 %1 高于程序支持的版本 1，请升级程序").arg(version);
        return false;
    }

    // 每次启动把审计操作者重置为 system，登录后再写入真实用户
    q.exec(QStringLiteral("UPDATE app_context SET current_user = 'system' WHERE id = 1"));

    if (!q.exec(QStringLiteral("SELECT COUNT(*) FROM users")) || !q.next()) {
        m_lastError = QStringLiteral("无法读取用户表：%1").arg(q.lastError().text());
        return false;
    }
    const int userCount = q.value(0).toInt();
    if (userCount == 0) {
        if (!seedDemoData(&error)) {
            m_lastError = error;
            return false;
        }
    }

    m_initialized = true;
    return true;
}

QString DatabaseManager::readResource(const QString &resourcePath, QString *error)
{
    auto readFile = [](const QString &path, QString *content) -> bool {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;
        *content = QString::fromUtf8(file.readAll());
        return true;
    };

    QString content;
    if (readFile(resourcePath, &content))
        return content;

    // 开发/测试回退：允许从 exe 目录、SMS_SQL_DIR 或当前目录读取
    const QString baseName = QFileInfo(resourcePath).fileName();
    QStringList candidates;
    candidates << QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("sql/") + baseName);
    const QString sqlDir = qEnvironmentVariable("SMS_SQL_DIR").trimmed();
    if (!sqlDir.isEmpty())
        candidates << QDir(sqlDir).filePath(baseName);
    candidates << QDir(QDir::currentPath()).filePath(QStringLiteral("sql/") + baseName);

    for (const QString &candidate : candidates) {
        if (readFile(candidate, &content))
            return content;
    }

    const QString msg = QStringLiteral("无法读取 SQL 脚本：%1").arg(resourcePath);
    if (error) *error = msg;
    m_lastError = msg;
    return QString();
}

bool DatabaseManager::executeScriptResource(const QString &resourcePath, QString *error)
{
    QString readError;
    const QString script = readResource(resourcePath, &readError);
    if (script.isEmpty()) {
        if (error) *error = readError;
        return false;
    }
    return executeScriptText(script, error);
}

bool DatabaseManager::executeScriptText(const QString &script, QString *error)
{
    const QStringList statements = SqlScript::split(script);
    if (statements.isEmpty()) {
        const QString msg = QStringLiteral("SQL 脚本为空或无法解析");
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    QSqlDatabase db = database();
    if (!db.isOpen()) {
        const QString msg = QStringLiteral("数据库未打开，无法执行 SQL 脚本");
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    for (int i = 0; i < statements.size(); ++i) {
        QSqlQuery query(db);
        if (!query.exec(statements.at(i))) {
            const QString msg = QStringLiteral("第 %1 条 SQL 执行失败：%2\n%3")
                                    .arg(i + 1)
                                    .arg(query.lastError().text(),
                                         statements.at(i).left(300));
            if (error) *error = msg;
            m_lastError = msg;
            return false;
        }
    }
    return true;
}

// ============================================================
// 哈希
// ============================================================

QString DatabaseManager::makeSalt()
{
    QByteArray bytes(16, 0);
    for (int i = 0; i < bytes.size(); ++i)
        bytes[i] = static_cast<char>(QRandomGenerator::system()->bounded(256));
    return QString::fromLatin1(bytes.toHex());
}

QString DatabaseManager::hashSecret(const QString &secret, const QString &salt)
{
    const QByteArray saltBytes = salt.toUtf8();
    QByteArray digest = QCryptographicHash::hash(saltBytes + secret.toUtf8(),
                                                 QCryptographicHash::Sha256);
    for (int i = 1; i < kHashIterations; ++i)
        digest = QCryptographicHash::hash(digest + saltBytes, QCryptographicHash::Sha256);
    return kHashPrefix + QString::fromLatin1(digest.toHex());
}

bool DatabaseManager::constantTimeEquals(const QString &a, const QString &b)
{
    if (a.size() != b.size())
        return false;
    int diff = 0;
    for (int i = 0; i < a.size(); ++i)
        diff |= a.at(i).unicode() ^ b.at(i).unicode();
    return diff == 0;
}

bool DatabaseManager::isLegacyHash(const QString &hash, const QString &salt) const
{
    if (!salt.isEmpty() || hash.size() != 64)
        return false;
    static const QRegularExpression re(QStringLiteral("^[0-9a-fA-F]{64}$"));
    return re.match(hash).hasMatch();
}

bool DatabaseManager::verifySecret(const QString &secret, const QString &hash,
                                   const QString &salt)
{
    if (hash.isEmpty())
        return false;
    if (salt.isEmpty() && hash.size() == 64) {
        static const QRegularExpression re(QStringLiteral("^[0-9a-fA-F]{64}$"));
        if (re.match(hash).hasMatch()) {
            const QString legacy = QString::fromLatin1(
                QCryptographicHash::hash(secret.toUtf8(), QCryptographicHash::Sha256).toHex());
            return constantTimeEquals(legacy, hash);
        }
    }
    return constantTimeEquals(hashSecret(secret, salt), hash);
}

// ============================================================
// 账号
// ============================================================

bool DatabaseManager::adminExists()
{
    QSqlQuery q(database());
    if (!q.exec(QStringLiteral("SELECT COUNT(*) FROM users WHERE user_type = 'admin'")) || !q.next()) {
        m_lastError = q.lastError().text();
        return false;
    }
    return q.value(0).toInt() > 0;
}

bool DatabaseManager::userExists(const QString &username)
{
    QSqlQuery q(database());
    q.prepare(QStringLiteral("SELECT 1 FROM users WHERE username = ?"));
    q.addBindValue(username.trimmed());
    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    return q.next();
}

bool DatabaseManager::insertUser(const QString &userId, const QString &username,
                                 const QString &password, const QString &userType,
                                 const QString &question1, const QString &answer1,
                                 const QString &question2, const QString &answer2,
                                 bool mustChangePassword, QString *error)
{
    const QString salt = makeSalt();
    const QString passwordHash = hashSecret(password, salt);

    QString answer1Salt;
    QString answer1Hash;
    if (!answer1.isEmpty()) {
        answer1Salt = makeSalt();
        answer1Hash = hashSecret(Validation::normalizeAnswer(answer1), answer1Salt);
    }
    QString answer2Salt;
    QString answer2Hash;
    if (!answer2.isEmpty()) {
        answer2Salt = makeSalt();
        answer2Hash = hashSecret(Validation::normalizeAnswer(answer2), answer2Salt);
    }

    QSqlQuery q(database());
    q.prepare(QStringLiteral(
        "INSERT INTO users (user_id, username, password_hash, password_salt, user_type, "
        "question1, answer1_hash, answer1_salt, question2, answer2_hash, answer2_salt, "
        "must_change_pw) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
    q.addBindValue(userId);
    q.addBindValue(username.trimmed());
    q.addBindValue(passwordHash);
    q.addBindValue(salt);
    q.addBindValue(userType);
    q.addBindValue(question1.trimmed());
    q.addBindValue(answer1Hash);
    q.addBindValue(answer1Salt);
    q.addBindValue(question2.trimmed());
    q.addBindValue(answer2Hash);
    q.addBindValue(answer2Salt);
    q.addBindValue(mustChangePassword ? 1 : 0);

    if (!q.exec()) {
        const QString msg = q.lastError().text();
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }
    return true;
}

QString DatabaseManager::nextTeacherId()
{
    QSqlQuery q(database());
    const QString sql = QStringLiteral(
        "SELECT COALESCE(MAX(CAST(SUBSTR(user_id, 9) AS INTEGER)), 0) + 1 "
        "FROM users WHERE user_id GLOB 'teacher_[0-9][0-9][0-9]'");
    if (q.exec(sql) && q.next())
        return QStringLiteral("teacher_%1").arg(q.value(0).toInt(), 3, 10, QLatin1Char('0'));
    return QStringLiteral("teacher_001");
}

bool DatabaseManager::registerUser(const QString &username, const QString &password,
                                   const QString &userType, const QString &teacherName,
                                   const QString &question1, const QString &answer1,
                                   const QString &question2, const QString &answer2)
{
    m_lastError.clear();
    const QString name = username.trimmed();
    if (name.length() < 3 || name.length() > 50) {
        m_lastError = QStringLiteral("用户名长度应为 3-50 个字符");
        return false;
    }
    const QString passwordError = Validation::passwordError(password);
    if (!passwordError.isEmpty()) {
        m_lastError = passwordError;
        return false;
    }
    if (userType != QLatin1String("admin") && userType != QLatin1String("teacher")) {
        m_lastError = QStringLiteral("不支持的账号类型");
        return false;
    }
    if (userType == QLatin1String("admin") && adminExists()) {
        m_lastError = QStringLiteral("系统已存在管理员账号");
        return false;
    }
    if (userType == QLatin1String("teacher") && teacherName.trimmed().isEmpty()) {
        m_lastError = QStringLiteral("教师姓名不能为空");
        return false;
    }

    QSqlDatabase db = database();
    if (!db.transaction()) {
        m_lastError = QStringLiteral("无法开始事务：%1").arg(db.lastError().text());
        return false;
    }

    const QString userId = (userType == QLatin1String("admin"))
                               ? QStringLiteral("admin_001")
                               : nextTeacherId();
    QString error;
    if (!insertUser(userId, name, password, userType,
                    question1, answer1, question2, answer2, false, &error)) {
        db.rollback();
        if (error.contains(QStringLiteral("UNIQUE"), Qt::CaseInsensitive))
            m_lastError = QStringLiteral("用户名或账号编号已存在");
        else
            m_lastError = error;
        return false;
    }

    if (userType == QLatin1String("teacher")) {
        QSqlQuery q(db);
        q.prepare(QStringLiteral(
            "INSERT INTO teachers (teacher_id, name, department) VALUES (?, ?, ?)"));
        q.addBindValue(userId);
        q.addBindValue(teacherName.trimmed());
        q.addBindValue(QStringLiteral("人工智能学院"));
        if (!q.exec()) {
            const QString msg = q.lastError().text();
            db.rollback();
            m_lastError = msg;
            return false;
        }
    }

    if (!db.commit()) {
        const QString msg = db.lastError().text();
        db.rollback();
        m_lastError = msg;
        return false;
    }
    return true;
}

bool DatabaseManager::validateUser(const QString &username, const QString &password,
                                   QString &userType, QString &userId)
{
    m_lastError.clear();
    const QString name = username.trimmed();
    if (name.isEmpty() || password.isEmpty()) {
        m_lastError = QStringLiteral("账号和密码不能为空");
        return false;
    }

    QSqlQuery q(database());
    q.prepare(QStringLiteral(
        "SELECT user_id, password_hash, password_salt, user_type, is_active "
        "FROM users WHERE username = ?"));
    q.addBindValue(name);
    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        m_lastError = QStringLiteral("账号或密码错误");
        return false;
    }

    const QString uid = q.value(0).toString();
    const QString storedHash = q.value(1).toString();
    const QString storedSalt = q.value(2).toString();
    const QString type = q.value(3).toString();
    const int active = q.value(4).toInt();

    if (active != 1) {
        m_lastError = QStringLiteral("该账号已被停用");
        return false;
    }
    if (!verifySecret(password, storedHash, storedSalt)) {
        m_lastError = QStringLiteral("账号或密码错误");
        return false;
    }

    // 旧版无盐 SHA-256 在登录成功后自动升级
    if (isLegacyHash(storedHash, storedSalt)) {
        const QString newSalt = makeSalt();
        QSqlQuery upgrade(database());
        upgrade.prepare(QStringLiteral(
            "UPDATE users SET password_hash = ?, password_salt = ? WHERE user_id = ?"));
        upgrade.addBindValue(hashSecret(password, newSalt));
        upgrade.addBindValue(newSalt);
        upgrade.addBindValue(uid);
        upgrade.exec();
    }

    QSqlQuery touch(database());
    touch.prepare(QStringLiteral(
        "UPDATE users SET last_login_at = datetime('now','localtime') WHERE user_id = ?"));
    touch.addBindValue(uid);
    touch.exec();

    setCurrentUser(uid);
    userType = type;
    userId = uid;
    return true;
}

bool DatabaseManager::verifyPassword(const QString &userId, const QString &password)
{
    QSqlQuery q(database());
    q.prepare(QStringLiteral(
        "SELECT password_hash, password_salt FROM users WHERE user_id = ?"));
    q.addBindValue(userId);
    if (!q.exec() || !q.next())
        return false;
    return verifySecret(password, q.value(0).toString(), q.value(1).toString());
}

bool DatabaseManager::changePassword(const QString &userId, const QString &oldPassword,
                                     const QString &newPassword)
{
    m_lastError.clear();
    if (oldPassword.isEmpty()) {
        m_lastError = QStringLiteral("请输入原密码");
        return false;
    }
    if (oldPassword == newPassword) {
        m_lastError = QStringLiteral("新密码不能与原密码相同");
        return false;
    }
    const QString passwordError = Validation::passwordError(newPassword);
    if (!passwordError.isEmpty()) {
        m_lastError = passwordError;
        return false;
    }
    if (!verifyPassword(userId, oldPassword)) {
        m_lastError = QStringLiteral("原密码不正确");
        return false;
    }

    const QString salt = makeSalt();
    QSqlQuery q(database());
    q.prepare(QStringLiteral(
        "UPDATE users SET password_hash = ?, password_salt = ?, must_change_pw = 0 "
        "WHERE user_id = ?"));
    q.addBindValue(hashSecret(newPassword, salt));
    q.addBindValue(salt);
    q.addBindValue(userId);
    if (!q.exec() || q.numRowsAffected() <= 0) {
        m_lastError = q.lastError().text().isEmpty()
                          ? QStringLiteral("密码修改失败")
                          : q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::resetPassword(const QString &username, const QString &newPassword)
{
    m_lastError.clear();
    const QString passwordError = Validation::passwordError(newPassword);
    if (!passwordError.isEmpty()) {
        m_lastError = passwordError;
        return false;
    }

    const QString salt = makeSalt();
    QSqlQuery q(database());
    q.prepare(QStringLiteral(
        "UPDATE users SET password_hash = ?, password_salt = ?, must_change_pw = 0 "
        "WHERE username = ?"));
    q.addBindValue(hashSecret(newPassword, salt));
    q.addBindValue(salt);
    q.addBindValue(username.trimmed());
    if (!q.exec() || q.numRowsAffected() <= 0) {
        m_lastError = QStringLiteral("账号不存在或密码重置失败");
        return false;
    }
    return true;
}

bool DatabaseManager::changeSecurityQuestions(const QString &userId, const QString &password,
                                              const QString &question1, const QString &answer1,
                                              const QString &question2, const QString &answer2)
{
    m_lastError.clear();
    if (!verifyPassword(userId, password)) {
        m_lastError = QStringLiteral("当前密码不正确");
        return false;
    }
    if (question1.trimmed().isEmpty() || question2.trimmed().isEmpty()
        || answer1.trimmed().isEmpty() || answer2.trimmed().isEmpty()) {
        m_lastError = QStringLiteral("密保问题和答案不能为空");
        return false;
    }
    if (question1.trimmed() == question2.trimmed()) {
        m_lastError = QStringLiteral("两个密保问题不能相同");
        return false;
    }

    const QString salt1 = makeSalt();
    const QString salt2 = makeSalt();
    QSqlQuery q(database());
    q.prepare(QStringLiteral(
        "UPDATE users SET question1 = ?, answer1_hash = ?, answer1_salt = ?, "
        "question2 = ?, answer2_hash = ?, answer2_salt = ? WHERE user_id = ?"));
    q.addBindValue(question1.trimmed());
    q.addBindValue(hashSecret(Validation::normalizeAnswer(answer1), salt1));
    q.addBindValue(salt1);
    q.addBindValue(question2.trimmed());
    q.addBindValue(hashSecret(Validation::normalizeAnswer(answer2), salt2));
    q.addBindValue(salt2);
    q.addBindValue(userId);
    if (!q.exec() || q.numRowsAffected() <= 0) {
        m_lastError = QStringLiteral("密保问题修改失败");
        return false;
    }
    return true;
}

QPair<QString, QString> DatabaseManager::getSecurityQuestions(const QString &username)
{
    QPair<QString, QString> result;
    QSqlQuery q(database());
    q.prepare(QStringLiteral(
        "SELECT question1, question2 FROM users WHERE username = ?"));
    q.addBindValue(username.trimmed());
    if (q.exec() && q.next()) {
        result.first = q.value(0).toString();
        result.second = q.value(1).toString();
    } else {
        m_lastError = q.lastError().text();
    }
    return result;
}

bool DatabaseManager::validateSecurityAnswer(const QString &username, int questionNum,
                                             const QString &answer)
{
    if (questionNum != 1 && questionNum != 2)
        return false;

    const QString hashColumn = (questionNum == 1)
                                   ? QStringLiteral("answer1_hash")
                                   : QStringLiteral("answer2_hash");
    const QString saltColumn = (questionNum == 1)
                                   ? QStringLiteral("answer1_salt")
                                   : QStringLiteral("answer2_salt");

    QSqlQuery q(database());
    q.prepare(QStringLiteral("SELECT %1, %2 FROM users WHERE username = ?")
                  .arg(hashColumn, saltColumn));
    q.addBindValue(username.trimmed());
    if (!q.exec() || !q.next())
        return false;

    const QString storedHash = q.value(0).toString();
    const QString storedSalt = q.value(1).toString();
    const QString normalized = Validation::normalizeAnswer(answer);

    // 旧版明文答案：兼容并升级
    if (storedHash.startsWith(QStringLiteral("plain$"))) {
        if (normalized != Validation::normalizeAnswer(storedHash.mid(6)))
            return false;
        const QString newSalt = makeSalt();
        QSqlQuery upgrade(database());
        upgrade.prepare(QStringLiteral(
            "UPDATE users SET %1 = ?, %2 = ? WHERE username = ?")
                            .arg(hashColumn, saltColumn));
        upgrade.addBindValue(hashSecret(normalized, newSalt));
        upgrade.addBindValue(newSalt);
        upgrade.addBindValue(username.trimmed());
        upgrade.exec();
        return true;
    }

    return verifySecret(normalized, storedHash, storedSalt);
}

bool DatabaseManager::mustChangePassword(const QString &userId)
{
    QSqlQuery q(database());
    q.prepare(QStringLiteral("SELECT must_change_pw FROM users WHERE user_id = ?"));
    q.addBindValue(userId);
    if (q.exec() && q.next())
        return q.value(0).toInt() == 1;
    return false;
}

QString DatabaseManager::getUsernameByUserId(const QString &userId)
{
    QSqlQuery q(database());
    q.prepare(QStringLiteral("SELECT username FROM users WHERE user_id = ?"));
    q.addBindValue(userId);
    if (q.exec() && q.next())
        return q.value(0).toString();
    return QString();
}

QString DatabaseManager::getUserTypeByUserId(const QString &userId)
{
    QSqlQuery q(database());
    q.prepare(QStringLiteral("SELECT user_type FROM users WHERE user_id = ?"));
    q.addBindValue(userId);
    if (q.exec() && q.next())
        return q.value(0).toString();
    return QString();
}

QString DatabaseManager::getDisplayName(const QString &userId)
{
    QSqlQuery q(database());
    q.prepare(QStringLiteral("SELECT user_type, username FROM users WHERE user_id = ?"));
    q.addBindValue(userId);
    if (!q.exec() || !q.next())
        return QString();

    const QString type = q.value(0).toString();
    const QString username = q.value(1).toString();

    if (type == QLatin1String("teacher")) {
        QSqlQuery t(database());
        t.prepare(QStringLiteral("SELECT name FROM teachers WHERE teacher_id = ?"));
        t.addBindValue(userId);
        if (t.exec() && t.next())
            return t.value(0).toString();
    } else if (type == QLatin1String("student")) {
        QSqlQuery s(database());
        s.prepare(QStringLiteral("SELECT name FROM students WHERE user_id = ?"));
        s.addBindValue(userId);
        if (s.exec() && s.next())
            return s.value(0).toString();
    }
    return username;
}

bool DatabaseManager::createStudentAccount(const QString &studentId, QString &username,
                                           QString &initialPassword)
{
    m_lastError.clear();
    QSqlQuery s(database());
    s.prepare(QStringLiteral("SELECT name, user_id FROM students WHERE student_id = ?"));
    s.addBindValue(studentId.trimmed());
    if (!s.exec() || !s.next()) {
        m_lastError = QStringLiteral("学生不存在");
        return false;
    }
    const QString studentName = s.value(0).toString();
    const QString existingUserId = s.value(1).toString();
    if (!existingUserId.isEmpty()) {
        m_lastError = QStringLiteral("该学生已经开通过账号");
        return false;
    }

    const QString loginName = studentId.trimmed();
    QSqlQuery check(database());
    check.prepare(QStringLiteral("SELECT 1 FROM users WHERE username = ?"));
    check.addBindValue(loginName);
    if (check.exec() && check.next()) {
        m_lastError = QStringLiteral("登录名 %1 已被占用").arg(loginName);
        return false;
    }

    const QString userId = QStringLiteral("stu_") + loginName;
    const QString password = loginName; // 初始密码 = 学号，首次登录强制修改
    const QString q1 = QStringLiteral("你的学号是？");
    const QString q2 = QStringLiteral("你的姓名是？");

    QSqlDatabase db = database();
    if (!db.transaction()) {
        m_lastError = db.lastError().text();
        return false;
    }

    QString error;
    if (!insertUser(userId, loginName, password, QStringLiteral("student"),
                    q1, loginName, q2, studentName, true, &error)) {
        db.rollback();
        m_lastError = error;
        return false;
    }

    QSqlQuery update(db);
    update.prepare(QStringLiteral("UPDATE students SET user_id = ? WHERE student_id = ?"));
    update.addBindValue(userId);
    update.addBindValue(loginName);
    if (!update.exec() || update.numRowsAffected() <= 0) {
        db.rollback();
        m_lastError = QStringLiteral("绑定学生账号失败");
        return false;
    }

    if (!db.commit()) {
        m_lastError = db.lastError().text();
        db.rollback();
        return false;
    }

    username = loginName;
    initialPassword = password;
    return true;
}

bool DatabaseManager::deleteStudentAccount(const QString &studentId)
{
    QSqlQuery s(database());
    s.prepare(QStringLiteral("SELECT user_id FROM students WHERE student_id = ?"));
    s.addBindValue(studentId.trimmed());
    if (!s.exec() || !s.next())
        return true; // 学生不存在或无账号

    const QString userId = s.value(0).toString();
    if (userId.isEmpty())
        return true;

    QSqlQuery q(database());
    q.prepare(QStringLiteral("DELETE FROM users WHERE user_id = ? AND user_type = 'student'"));
    q.addBindValue(userId);
    if (!q.exec()) {
        m_lastError = q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteTeacher(const QString &teacherId)
{
    m_lastError.clear();
    QSqlQuery q(database());
    q.prepare(QStringLiteral("DELETE FROM users WHERE user_id = ? AND user_type = 'teacher'"));
    q.addBindValue(teacherId);
    if (!q.exec() || q.numRowsAffected() <= 0) {
        m_lastError = q.lastError().text().isEmpty()
                          ? QStringLiteral("教师不存在")
                          : q.lastError().text();
        return false;
    }
    return true;
}

// ============================================================
// 会话
// ============================================================

QString DatabaseManager::currentUser()
{
    QSqlQuery q(database());
    if (q.exec(QStringLiteral("SELECT current_user FROM app_context WHERE id = 1")) && q.next())
        return q.value(0).toString();
    return QStringLiteral("system");
}

void DatabaseManager::setCurrentUser(const QString &userId)
{
    QSqlQuery q(database());
    q.prepare(QStringLiteral("UPDATE app_context SET current_user = ? WHERE id = 1"));
    q.addBindValue(userId.isEmpty() ? QStringLiteral("system") : userId);
    q.exec();
}

// ============================================================
// 备份 / 恢复
// ============================================================

bool DatabaseManager::backupTo(const QString &filePath, QString *error)
{
    m_lastError.clear();
    if (filePath.trimmed().isEmpty()) {
        const QString msg = QStringLiteral("备份路径不能为空");
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    const QString absoluteTarget = QFileInfo(filePath).absoluteFilePath();
    if (absoluteTarget == QFileInfo(m_dbPath).absoluteFilePath()) {
        const QString msg = QStringLiteral("备份文件不能覆盖正在使用的数据库");
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    QDir().mkpath(QFileInfo(absoluteTarget).absolutePath());
    if (QFile::exists(absoluteTarget))
        QFile::remove(absoluteTarget);

    QString escaped = absoluteTarget;
    escaped.replace(QLatin1Char('\''), QStringLiteral("''"));

    QSqlQuery q(database());
    if (!q.exec(QStringLiteral("VACUUM INTO '%1'").arg(escaped))) {
        const QString msg = QStringLiteral("备份失败：%1").arg(q.lastError().text());
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }
    return verifyBackup(absoluteTarget, error);
}

bool DatabaseManager::verifyBackup(const QString &filePath, QString *error)
{
    const QFileInfo info(filePath);
    if (!info.exists() || !info.isFile() || info.size() == 0) {
        const QString msg = QStringLiteral("备份文件不存在或为空");
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    static int verifyCounter = 0;
    const QString connection = QStringLiteral("SMS_VERIFY_%1").arg(++verifyCounter);
    bool ok = false;
    QString detail;

    {
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connection);
        db.setDatabaseName(info.absoluteFilePath());
        db.setConnectOptions(QStringLiteral("QSQLITE_OPEN_READONLY"));
        if (!db.open()) {
            detail = db.lastError().text();
        } else {
            QSqlQuery q(db);
            if (!q.exec(QStringLiteral("PRAGMA integrity_check")) || !q.next()
                || q.value(0).toString() != QLatin1String("ok")) {
                detail = QStringLiteral("integrity_check 未通过");
            } else if (q.exec(QStringLiteral(
                           "SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name IN "
                           "('users','teachers','students','subjects','grades','schema_meta')"))
                       && q.next() && q.value(0).toInt() == 6) {
                if (q.exec(QStringLiteral("SELECT version FROM schema_meta WHERE id = 1"))
                    && q.next() && q.value(0).toInt() == 1) {
                    ok = true;
                } else {
                    detail = QStringLiteral("schema_meta 版本不匹配");
                }
            } else {
                detail = QStringLiteral("缺少核心数据表");
            }
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(connection);

    if (!ok) {
        const QString msg = QStringLiteral("不是有效的系统备份文件：%1").arg(detail);
        if (error) *error = msg;
        m_lastError = msg;
    }
    return ok;
}

bool DatabaseManager::restoreFrom(const QString &filePath, QString *error)
{
    m_lastError.clear();
    if (!verifyBackup(filePath, error))
        return false;

    const QString source = QFileInfo(filePath).absoluteFilePath();
    if (source == QFileInfo(m_dbPath).absoluteFilePath()) {
        const QString msg = QStringLiteral("不能从当前正在使用的数据库恢复");
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    closeDatabase();

    const QString safety = m_dbPath + QStringLiteral(".bak_")
                           + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss"));
    const bool hadCurrent = QFileInfo::exists(m_dbPath);
    if (hadCurrent && !QFile::copy(m_dbPath, safety)) {
        openDatabase();
        const QString msg = QStringLiteral("无法创建恢复前安全副本，已取消恢复");
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    auto rollback = [&](const QString &reason) {
        closeDatabase();
        QFile::remove(m_dbPath + QStringLiteral("-wal"));
        QFile::remove(m_dbPath + QStringLiteral("-shm"));
        QFile::remove(m_dbPath);
        if (hadCurrent)
            QFile::copy(safety, m_dbPath);
        openDatabase();
        const QString msg = QStringLiteral("恢复失败：%1").arg(reason);
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    };

    QFile::remove(m_dbPath + QStringLiteral("-wal"));
    QFile::remove(m_dbPath + QStringLiteral("-shm"));
    if (QFile::exists(m_dbPath) && !QFile::remove(m_dbPath))
        return rollback(QStringLiteral("无法删除当前数据库文件"));
    if (!QFile::copy(source, m_dbPath))
        return rollback(QStringLiteral("无法复制备份文件"));

    QString openError;
    if (!openDatabase(&openError))
        return rollback(openError);

    QSqlQuery q(database());
    if (!q.exec(QStringLiteral("PRAGMA integrity_check")) || !q.next()
        || q.value(0).toString() != QLatin1String("ok")) {
        return rollback(QStringLiteral("恢复后的数据库完整性检查失败"));
    }
    if (q.exec(QStringLiteral("PRAGMA foreign_key_check")) && q.next())
        return rollback(QStringLiteral("恢复后的数据库存在外键冲突"));

    QSqlQuery reset(database());
    reset.exec(QStringLiteral("UPDATE app_context SET current_user = 'system' WHERE id = 1"));
    m_initialized = true;
    return true;
}

// ============================================================
// 演示数据
// ============================================================

bool DatabaseManager::seedDemoData(QString *error)
{
    QSqlQuery countQuery(database());
    if (!countQuery.exec(QStringLiteral("SELECT COUNT(*) FROM users")) || !countQuery.next()) {
        const QString msg = countQuery.lastError().text();
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }
    if (countQuery.value(0).toInt() > 0)
        return true; // 已有账号，不重复写入

    QSqlDatabase db = database();
    if (!db.transaction()) {
        const QString msg = QStringLiteral("无法开始初始化事务：%1").arg(db.lastError().text());
        if (error) *error = msg;
        m_lastError = msg;
        return false;
    }

    auto fail = [&](const QString &reason) {
        db.rollback();
        if (error) *error = reason;
        m_lastError = reason;
        return false;
    };

    QString err;
    if (!insertUser(QStringLiteral("admin_001"), QStringLiteral("admin"),
                    QStringLiteral("admin123"), QStringLiteral("admin"),
                    QStringLiteral("管理员编号是？"), QStringLiteral("admin_001"),
                    QStringLiteral("系统名称是？"), QStringLiteral("学生信息管理系统"),
                    false, &err)) {
        return fail(QStringLiteral("创建管理员失败：%1").arg(err));
    }

    struct DemoTeacher {
        const char *id;
        const char *username;
        const char *name;
    };
    const DemoTeacher demoTeachers[] = {
        {"teacher_001", "teacher01", "张伟"},
        {"teacher_002", "teacher02", "李娜"},
    };

    for (const DemoTeacher &t : demoTeachers) {
        const QString teacherId = QString::fromLatin1(t.id);
        if (!insertUser(teacherId, QString::fromLatin1(t.username),
                        QStringLiteral("123456"), QStringLiteral("teacher"),
                        QStringLiteral("你的工号是？"), teacherId,
                        QStringLiteral("你的姓名是？"), QString::fromUtf8(t.name),
                        false, &err)) {
            return fail(QStringLiteral("创建教师账号失败：%1").arg(err));
        }
        QSqlQuery q(db);
        q.prepare(QStringLiteral(
            "INSERT INTO teachers (teacher_id, name, department) VALUES (?, ?, ?)"));
        q.addBindValue(teacherId);
        q.addBindValue(QString::fromUtf8(t.name));
        q.addBindValue(QStringLiteral("人工智能学院"));
        if (!q.exec())
            return fail(QStringLiteral("创建教师档案失败：%1").arg(q.lastError().text()));
    }

    if (!executeScriptResource(QStringLiteral(":/sql/seed.sql"), &err))
        return fail(QStringLiteral("写入演示数据失败：%1").arg(err));

    // 给第一名学生开通账号，用于演示学生角色
    QSqlQuery studentQuery(db);
    studentQuery.prepare(QStringLiteral(
        "SELECT name FROM students WHERE student_id = ?"));
    studentQuery.addBindValue(QStringLiteral("2024001"));
    QString studentName;
    if (studentQuery.exec() && studentQuery.next())
        studentName = studentQuery.value(0).toString();

    const QString studentUserId = QStringLiteral("stu_2024001");
    if (!insertUser(studentUserId, QStringLiteral("2024001"),
                    QStringLiteral("123456"), QStringLiteral("student"),
                    QStringLiteral("你的学号是？"), QStringLiteral("2024001"),
                    QStringLiteral("你的姓名是？"), studentName,
                    false, &err)) {
        return fail(QStringLiteral("创建学生账号失败：%1").arg(err));
    }
    QSqlQuery link(db);
    link.prepare(QStringLiteral("UPDATE students SET user_id = ? WHERE student_id = ?"));
    link.addBindValue(studentUserId);
    link.addBindValue(QStringLiteral("2024001"));
    if (!link.exec())
        return fail(QStringLiteral("绑定学生账号失败：%1").arg(link.lastError().text()));

    if (!db.commit())
        return fail(QStringLiteral("提交演示数据失败：%1").arg(db.lastError().text()));

    QSqlQuery cleanup(database());
    // 先调整演示数据的时间线，再清空种子阶段由触发器产生的审计日志；
    // 顺序不能反，否则 UPDATE grades 会再次写入 480 条审计记录。
    cleanup.exec(QStringLiteral(
        "UPDATE users SET created_time = '2024-08-01 09:00:00' "
        "WHERE user_id IN ('admin_001','teacher_001','teacher_002','stu_2024001')"));
    cleanup.exec(QStringLiteral(
        "UPDATE teachers SET created_time = '2024-08-01 09:00:00'"));
    cleanup.exec(QStringLiteral(
        "UPDATE grades SET created_time = COALESCE(exam_date || ' 12:00:00', created_time)"));
    cleanup.exec(QStringLiteral("DELETE FROM audit_log"));
    cleanup.exec(QStringLiteral("UPDATE schema_meta SET seed_version = 1 WHERE id = 1"));
    cleanup.exec(QStringLiteral("UPDATE app_context SET current_user = 'system' WHERE id = 1"));
    return true;
}
