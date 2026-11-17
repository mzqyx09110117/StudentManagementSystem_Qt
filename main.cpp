#include "adminmainwindow.h"
#include "changepassworddialog.h"
#include "databasemanager.h"
#include "logindialog.h"
#include "screenshottool.h"
#include "selftest.h"
#include "studentmainwindow.h"
#include "teachermainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QStyleFactory>

namespace {

void applyStyleSheet(QApplication &app)
{
    QFile styleFile(QStringLiteral(":/style.qss"));
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
}

} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("学生信息管理系统"));
    app.setOrganizationName(QStringLiteral("人工智能学院"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("学生信息管理系统"));
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("db"),
                                        QStringLiteral("指定数据库文件路径"),
                                        QStringLiteral("path")));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("capture"),
                                        QStringLiteral("自动生成运行截图到指定目录"),
                                        QStringLiteral("dir")));
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("self-test"),
                                        QStringLiteral("运行数据库自检并写入 selftest.log")));
    parser.process(app);

    QString captureDir;
    if (parser.isSet(QStringLiteral("capture"))) {
        captureDir = parser.value(QStringLiteral("capture"));
        QDir().mkpath(captureDir);
        const QString demoDb = QDir(captureDir).filePath(QStringLiteral("_capture.db"));
        QFile::remove(demoDb);
        QFile::remove(demoDb + QStringLiteral("-wal"));
        QFile::remove(demoDb + QStringLiteral("-shm"));
        qputenv("SMS_DB_PATH", demoDb.toUtf8());
    } else if (parser.isSet(QStringLiteral("db"))) {
        qputenv("SMS_DB_PATH", parser.value(QStringLiteral("db")).toUtf8());
    }

    if (QStyleFactory::keys().contains(QStringLiteral("Fusion")))
        QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    applyStyleSheet(app);

    if (!DatabaseManager::instance()->initializeDatabases()) {
        QMessageBox::critical(nullptr, QStringLiteral("系统错误"),
                              QStringLiteral("数据库初始化失败！\n%1")
                                  .arg(DatabaseManager::instance()->lastError()));
        return -1;
    }

    if (!captureDir.isEmpty()) {
        const bool ok = ScreenshotTool::run(captureDir);
        DatabaseManager::destroy();
        return ok ? 0 : 1;
    }

    if (parser.isSet(QStringLiteral("self-test"))) {
        const QString logPath = QDir(QDir::currentPath()).filePath(QStringLiteral("selftest.log"));
        const int result = runSelfTest(logPath);
        DatabaseManager::destroy();
        return result;
    }

    LoginDialog loginDialog;
    if (loginDialog.exec() != QDialog::Accepted) {
        DatabaseManager::destroy();
        return 0;
    }

    const QString userType = loginDialog.getUserType();
    const QString userId = loginDialog.getUserId();

    // 首次登录（如学生初始账号）强制修改密码
    if (DatabaseManager::instance()->mustChangePassword(userId)) {
        ChangePasswordDialog forceDialog(userId, true);
        if (forceDialog.exec() != QDialog::Accepted) {
            DatabaseManager::destroy();
            return 0;
        }
    }

    QMainWindow *mainWindow = nullptr;
    if (userType == QLatin1String("admin"))
        mainWindow = new AdminMainWindow(userId);
    else if (userType == QLatin1String("teacher"))
        mainWindow = new TeacherMainWindow(userId);
    else if (userType == QLatin1String("student"))
        mainWindow = new StudentMainWindow(userId);
    else {
        QMessageBox::critical(nullptr, QStringLiteral("系统错误"),
                              QStringLiteral("未知的用户类型：%1").arg(userType));
        DatabaseManager::destroy();
        return -1;
    }

    mainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mainWindow->show();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        DatabaseManager::instance()->closeDatabase();
    });

    const int result = app.exec();
    DatabaseManager::destroy();
    return result;
}
