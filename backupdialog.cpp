#include "backupdialog.h"
#include "ui_backupdialog.h"
//#include "databasemanager.h"

BackupDialog::BackupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackupDialog),
    m_tempDir(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("备份系统数据"));

    // 设置默认备份路径
    QString defaultPath = QDir::homePath() + "/教师系统备份_" +
                          QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".zip";
    ui->lineEdit_backupPath->setText(defaultPath);

    // 获取系统数据库信息
    QString systemDbPath = QDir::currentPath() + "/Teacher_System.db";
    QFile systemDb(systemDbPath);
    if (systemDb.exists()) {
        ui->label_systemDbInfo->setText(QString("系统数据库: %1 (大小: %2 KB)")
                                            .arg("Teacher_System.db")
                                            .arg(systemDb.size() / 1024));
    } else {
        ui->label_systemDbInfo->setText("系统数据库: 未找到");
    }

    // 获取教师数据库信息
    QStringList teacherFolders = getAllTeacherFolders();
    ui->label_teacherDbInfo->setText(QString("教师数据库: %1 个教师").arg(teacherFolders.count()));

    // 计算总大小
    qint64 totalSize = systemDb.size();
    foreach (const QString &folder, teacherFolders) {
        QDir dir(folder);
        foreach (const QFileInfo &fileInfo, dir.entryInfoList(QDir::Files)) {
            totalSize += fileInfo.size();
        }
    }
    ui->label_totalSize->setText(QString("总备份大小: %1 MB").arg(totalSize / (1024.0 * 1024.0), 0, 'f', 2));
}

BackupDialog::~BackupDialog()
{
    if (m_tempDir) {
        delete m_tempDir;
    }
    delete ui;
}

QStringList BackupDialog::getAllTeacherFolders() const
{
    QStringList folders;
    QDir currentDir(QDir::currentPath());

    // 查找所有teacher_开头的文件夹
    QStringList filters;
    filters << "teacher_*";
    QStringList dirs = currentDir.entryList(filters, QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QString &dirName, dirs) {
        QString folderPath = currentDir.absoluteFilePath(dirName);
        // 检查是否有data.db文件
        if (QFile::exists(folderPath + "/data.db")) {
            folders.append(folderPath);
        }
    }

    return folders;
}

bool BackupDialog::prepareBackupFiles(const QString &backupDir) const
{
    QDir dir(backupDir);
    if (!dir.exists()) {
        if (!dir.mkpath(backupDir)) {
            qDebug() << "无法创建备份目录:" << backupDir;
            return false;
        }
    }

    // 1. 复制系统数据库
    QString systemDbPath = QDir::currentPath() + "/Teacher_System.db";
    QString destSystemDb = backupDir + "/Teacher_System.db";

    if (QFile::exists(systemDbPath)) {
        if (QFile::exists(destSystemDb)) {
            QFile::remove(destSystemDb);
        }

        if (!QFile::copy(systemDbPath, destSystemDb)) {
            qDebug() << "复制系统数据库失败";
            return false;
        }
    } else {
        qDebug() << "系统数据库不存在:" << systemDbPath;
        return false;
    }

    // 2. 复制教师数据库
    QStringList teacherFolders = getAllTeacherFolders();
    foreach (const QString &folderPath, teacherFolders) {
        QDir folderDir(folderPath);
        QString teacherId = folderDir.dirName();
        QString teacherDbPath = folderPath + "/data.db";

        // 创建教师目录
        QString teacherDestDir = backupDir + "/" + teacherId;
        if (!QDir().mkpath(teacherDestDir)) {
            qDebug() << "无法创建教师目录:" << teacherDestDir;
            continue;
        }

        // 复制数据库文件
        QString destDbPath = teacherDestDir + "/data.db";
        if (QFile::exists(teacherDbPath)) {
            if (QFile::exists(destDbPath)) {
                QFile::remove(destDbPath);
            }

            if (!QFile::copy(teacherDbPath, destDbPath)) {
                qDebug() << "复制教师数据库失败:" << teacherId;
            }
        }
    }

    // 3. 创建备份日志
    createBackupLog(backupDir);

    return true;
}

bool BackupDialog::createZipWithSystemTool(const QString &sourceDir, const QString &zipFilePath) const
{
#ifdef Q_OS_WIN
    // Windows: 使用 PowerShell
    QString program = "powershell";
    QStringList arguments;
    arguments << "-Command" << "Compress-Archive"
              << "-Path" << QString("\"%1\\*\"").arg(sourceDir)
              << "-DestinationPath" << QString("\"%1\"").arg(zipFilePath)
              << "-Force";
#elif defined(Q_OS_MAC)
    // macOS: 使用 zip 命令
    QString program = "zip";
    QStringList arguments;
    arguments << "-r" << "-q" << zipFilePath << ".";
    QProcess::startDetached(program, arguments, sourceDir);
    return true;
#else
    // Linux: 使用 zip 命令
    QString program = "zip";
    QStringList arguments;
    arguments << "-r" << "-q" << zipFilePath << ".";
    QProcess::startDetached(program, arguments, sourceDir);
    return true;
#endif

#ifdef Q_OS_WIN
    QProcess process;
    process.start(program, arguments);

    if (!process.waitForStarted()) {
        qDebug() << "无法启动压缩进程";
        return false;
    }

    // 等待压缩完成（最多30秒）
    if (!process.waitForFinished(30000)) {
        qDebug() << "压缩进程超时";
        process.kill();
        return false;
    }

    if (process.exitCode() != 0) {
        qDebug() << "压缩失败，退出码:" << process.exitCode();
        qDebug() << "错误输出:" << process.readAllStandardError();
        return false;
    }

    return QFile::exists(zipFilePath);
#else
    // macOS/Linux 已经在上面的 startDetached 中处理
    QThread::msleep(2000); // 等待一下
    return QFile::exists(zipFilePath);
#endif
}

void BackupDialog::createBackupLog(const QString &backupDir) const
{
    QString logPath = backupDir + "/backup.log";
    QFile logFile(logPath);

    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法创建备份日志文件";
        return;
    }

    QTextStream out(&logFile);
    // 在 Qt5 中，默认就是 UTF-8 编码

    out << "教师管理系统备份日志\n";
    out << "=====================\n";
    out << "备份时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    out << "备份版本: 1.0\n";

    // 系统数据库信息
    QString systemDbPath = QDir::currentPath() + "/Teacher_System.db";
    QFileInfo systemDbInfo(systemDbPath);
    out << "\n系统数据库:\n";
    out << "  文件: Teacher_System.db\n";
    out << "  大小: " << systemDbInfo.size() << " 字节\n";
    out << "  最后修改: " << systemDbInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss") << "\n";

    // 教师数据库信息
    QStringList teacherFolders = getAllTeacherFolders();
    out << "\n教师数据库 (" << teacherFolders.count() << " 个):\n";
    foreach (const QString &folderPath, teacherFolders) {
        QDir folderDir(folderPath);
        QString teacherId = folderDir.dirName();
        QString dbPath = folderPath + "/data.db";
        QFileInfo dbInfo(dbPath);

        out << "  教师ID: " << teacherId << "\n";
        out << "    文件: data.db\n";
        out << "    大小: " << dbInfo.size() << " 字节\n";
        out << "    最后修改: " << dbInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    }

    out << "\n备份完成状态: 成功\n";
    out.flush();
    logFile.close();
}

bool BackupDialog::createZipFile(const QString &zipFilePath)
{
    // 创建临时目录存放所有要备份的文件
    m_tempDir = new QTemporaryDir();
    if (!m_tempDir->isValid()) {
        qDebug() << "无法创建临时目录";
        delete m_tempDir;
        m_tempDir = nullptr;
        return false;
    }

    QString tempPath = m_tempDir->path();
    QString backupDir = tempPath + "/backup";

    // 准备备份文件
    ui->label_status->setText("正在准备备份文件...");
    QApplication::processEvents();

    if (!prepareBackupFiles(backupDir)) {
        qDebug() << "准备备份文件失败";
        delete m_tempDir;
        m_tempDir = nullptr;
        return false;
    }

    // 创建ZIP文件
    ui->label_status->setText("正在创建压缩文件...");
    QApplication::processEvents();

    bool success = createZipWithSystemTool(backupDir, zipFilePath);

    // 清理临时目录
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }

    return success;
}




void BackupDialog::on_pushButton_selectFolder_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "选择备份文件",
                                                    QDir::homePath() + "/教师系统备份_" +
                                                        QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".zip",
                                                    "ZIP文件 (*.zip);;所有文件 (*.*)");
    if (!filePath.isEmpty()) {
        ui->lineEdit_backupPath->setText(filePath);
    }
}

void BackupDialog::on_pushButton_startBackup_clicked()
{
    QString backupFilePath = ui->lineEdit_backupPath->text().trimmed();

    if (backupFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择备份路径！");
        return;
    }

    // 确保文件以.zip结尾
    if (!backupFilePath.endsWith(".zip", Qt::CaseInsensitive)) {
        backupFilePath += ".zip";
        ui->lineEdit_backupPath->setText(backupFilePath);
    }

    // 检查文件是否已存在
    if (QFile::exists(backupFilePath)) {
        int reply = QMessageBox::question(this, "确认",
                                          "备份文件已存在，是否覆盖？",
                                          QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
        QFile::remove(backupFilePath);
    }

    ui->pushButton_startBackup->setEnabled(false);
    ui->pushButton_cancel->setEnabled(false);
    ui->label_status->setText("正在创建备份...");
    QApplication::processEvents();

    // 创建ZIP压缩文件
    if (!createZipFile(backupFilePath)) {
        QMessageBox::critical(this, "错误",
                              "创建备份文件失败！\n"
                              "请确保：\n"
                              "1. 系统数据库文件存在\n"
                              "2. 有足够的磁盘空间\n"
                              "3. 系统支持压缩命令");

        ui->pushButton_startBackup->setEnabled(true);
        ui->pushButton_cancel->setEnabled(true);
        ui->label_status->setText("备份失败");
        return;
    }

    ui->label_status->setText("备份完成！");
    QMessageBox::information(this, "成功", QString("备份完成！\n备份文件: %1").arg(backupFilePath));

    accept();
}

void BackupDialog::on_pushButton_cancel_clicked()
{
    // 清理临时目录
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }

    reject();
}
