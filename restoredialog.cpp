#include "restoredialog.h"
#include "ui_restoredialog.h"

RestoreDialog::RestoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestoreDialog),
    m_backupPath(""),
    m_tempDir(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("恢复系统数据"));

    // 设置plainTextEdit为只读
    ui->plainTextEdit_teachers->setReadOnly(true);

    // 初始状态
    ui->pushButton_analyzeBackup->setEnabled(false);
    ui->pushButton_startRestore->setEnabled(false);
}

RestoreDialog::~RestoreDialog()
{
    // 清理临时目录
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }
    delete ui;
}

bool RestoreDialog::extractZipFile(const QString &zipPath, const QString &destDir)
{
    // 创建目标目录
    QDir dir(destDir);
    if (!dir.exists()) {
        if (!dir.mkpath(destDir)) {
            qDebug() << "无法创建解压目录:" << destDir;
            return false;
        }
    }

#ifdef Q_OS_WIN
    // Windows: 使用 PowerShell
    QString program = "powershell";
    QStringList arguments;
    arguments << "-Command" << "Expand-Archive"
              << "-Path" << QString("\"%1\"").arg(zipPath)
              << "-DestinationPath" << QString("\"%1\"").arg(destDir)
              << "-Force";
#elif defined(Q_OS_MAC)
    // macOS: 使用 unzip 命令
    QString program = "unzip";
    QStringList arguments;
    arguments << "-q" << "-o" << zipPath << "-d" << destDir;
#else
    // Linux: 使用 unzip 命令
    QString program = "unzip";
    QStringList arguments;
    arguments << "-q" << "-o" << zipPath << "-d" << destDir;
#endif

    QProcess process;
    process.start(program, arguments);

    if (!process.waitForStarted()) {
        qDebug() << "无法启动解压进程";
        // 尝试使用备用方法
        QString errorMsg = QString("无法解压文件。请确保系统已安装解压工具：\n"
                                   "Windows: PowerShell 5.0+\n"
                                   "macOS/Linux: unzip 命令");
        QMessageBox::critical(this, "错误", errorMsg);
        return false;
    }

    // 等待解压完成（最多60秒）
    if (!process.waitForFinished(60000)) {
        qDebug() << "解压进程超时";
        process.kill();
        QMessageBox::critical(this, "错误", "解压超时，请检查文件是否损坏");
        return false;
    }

    if (process.exitCode() != 0) {
        qDebug() << "解压失败，退出码:" << process.exitCode();
        QString errorOutput = process.readAllStandardError();
        qDebug() << "错误输出:" << errorOutput;

        QString errorMsg = QString("解压失败 (错误码: %1)\n%2")
                               .arg(process.exitCode())
                               .arg(errorOutput.left(200)); // 只显示前200个字符
        QMessageBox::critical(this, "错误", errorMsg);
        return false;
    }

    // 验证解压结果
    if (!QDir(destDir).exists()) {
        qDebug() << "解压后目录不存在";
        return false;
    }

    // 检查是否有文件被解压
    QDir destDirObj(destDir);
    if (destDirObj.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).isEmpty()) {
        qDebug() << "解压后目录为空";
        return false;
    }

    return true;
}

bool RestoreDialog::analyzeBackupFolder(const QString &backupPath)
{
    // 如果是ZIP文件，先解压到临时目录
    if (backupPath.endsWith(".zip", Qt::CaseInsensitive)) {
        qDebug() << "检测到ZIP文件，开始解压...";

        // 清理旧的临时目录
        if (m_tempDir) {
            delete m_tempDir;
            m_tempDir = nullptr;
        }

        // 创建临时目录
        m_tempDir = new QTemporaryDir();
        if (!m_tempDir->isValid()) {
            qDebug() << "无法创建临时目录";
            QMessageBox::critical(this, "错误", "无法创建临时目录");
            delete m_tempDir;
            m_tempDir = nullptr;
            return false;
        }

        QString tempPath = m_tempDir->path();
        qDebug() << "临时目录:" << tempPath;

        ui->label_restoreStatus->setText("正在解压文件...");
        QApplication::processEvents();

        // 解压ZIP文件
        if (!extractZipFile(backupPath, tempPath)) {
            qDebug() << "解压ZIP文件失败";
            delete m_tempDir;
            m_tempDir = nullptr;
            return false;
        }

        m_backupPath = tempPath;
    } else {
        // 直接使用目录
        m_backupPath = backupPath;
    }

    QDir backupDir(m_backupPath);
    if (!backupDir.exists()) {
        qDebug() << "备份目录不存在:" << m_backupPath;
        return false;
    }

    // 检查必需的文件
    QString systemDb = m_backupPath + "/Teacher_System.db";
    QString backupLog = m_backupPath + "/backup.log";

    if (!QFile::exists(systemDb)) {
        qDebug() << "系统数据库不存在:" << systemDb;

        // 检查是否在解压后的子目录中
        QStringList systemDbCandidates;
        systemDbCandidates << systemDb;

        // 在临时目录中搜索
        QDirIterator it(m_backupPath, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            QFileInfo fileInfo = it.fileInfo();
            if (fileInfo.isFile() && fileInfo.fileName() == "Teacher_System.db") {
                systemDb = fileInfo.absoluteFilePath();
                qDebug() << "在子目录中找到系统数据库:" << systemDb;
                break;
            }
        }

        if (!QFile::exists(systemDb)) {
            QMessageBox::critical(this, "错误", "备份文件中缺少系统数据库文件");
            return false;
        }
    }

    // 分析系统数据库信息
    QFileInfo systemDbInfo(systemDb);
    ui->label_systemDbRestore->setText(QString("系统数据库: Teacher_System.db (%1 KB)")
                                           .arg(systemDbInfo.size() / 1024));

    // 分析教师数据库
    QStringList teacherFolders;
    QStringList teacherDbPaths;

    // 搜索所有教师数据库
    QDirIterator it(m_backupPath, QStringList() << "data.db", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString dbPath = it.next();
        QFileInfo dbInfo(dbPath);

        // 检查是否为教师数据库（在teacher_*目录下）
        QDir dbDir = dbInfo.dir();
        QString dirName = dbDir.dirName();

        if (dirName.startsWith("teacher_") && dbInfo.fileName() == "data.db") {
            teacherDbPaths.append(dbPath);
            teacherFolders.append(dirName);
        }
    }

    // 去重
    teacherFolders = QSet<QString>(teacherFolders.begin(), teacherFolders.end()).values();

    int teacherCount = teacherFolders.size();
    qint64 totalSize = systemDbInfo.size();

    foreach (const QString &dbPath, teacherDbPaths) {
        QFileInfo dbInfo(dbPath);
        totalSize += dbInfo.size();
    }

    ui->label_teacherDbRestore->setText(QString("教师数据库: %1 个教师").arg(teacherCount));
    ui->label_totalRestore->setText(QString("总恢复大小: %1 MB").arg(totalSize / (1024.0 * 1024.0), 0, 'f', 2));

    // 显示教师列表
    ui->plainTextEdit_teachers->clear();
    if (!teacherFolders.isEmpty()) {
        teacherFolders.sort();
        ui->plainTextEdit_teachers->setPlainText(teacherFolders.join("\n"));
    } else {
        ui->plainTextEdit_teachers->setPlainText("无教师数据库");
    }

    // 如果有备份日志，显示日志
    if (QFile::exists(backupLog)) {
        QFile logFile(backupLog);
        if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&logFile);
            QString line;
            QString backupTime = "未知";
            bool foundTime = false;

            // 查找备份时间
            while (!in.atEnd()) {
                line = in.readLine();
                if (line.startsWith("备份时间:")) {
                    backupTime = line.mid(line.indexOf(":") + 1).trimmed();
                    foundTime = true;
                    break;
                }
            }
            logFile.close();

            if (foundTime) {
                ui->label_backupTime->setText("备份时间: " + backupTime);
            } else {
                ui->label_backupTime->setText("备份时间: 未知");
            }
        }
    } else {
        ui->label_backupTime->setText("备份时间: 未找到日志");
    }

    ui->label_restoreStatus->setText("分析完成");
    return true;
}

bool RestoreDialog::validateBackup() const
{
    if (m_backupPath.isEmpty()) {
        return false;
    }

    // 检查系统数据库是否存在
    QString systemDb = m_backupPath + "/Teacher_System.db";
    if (!QFile::exists(systemDb)) {
        // 在子目录中搜索
        QDirIterator it(m_backupPath, QStringList() << "Teacher_System.db", QDir::Files, QDirIterator::Subdirectories);
        if (!it.hasNext()) {
            return false;
        }
    }

    return true;
}

void RestoreDialog::closeAllDatabaseConnections()
{
    qDebug() << "开始关闭数据库连接...";

    // 移除系统数据库连接
    if (QSqlDatabase::contains("SYSTEM")) {
        QSqlDatabase::removeDatabase("SYSTEM");
        qDebug() << "已移除SYSTEM数据库连接";
    }

    // 查找并移除所有教师数据库连接
    QStringList connectionNames = QSqlDatabase::connectionNames();
    foreach (const QString &connectionName, connectionNames) {
        if (connectionName.startsWith("TEACHER_") || connectionName.startsWith("TEACHER_DB_")) {
            QSqlDatabase::removeDatabase(connectionName);
            qDebug() << "已移除教师数据库连接:" << connectionName;
        }
    }

    // 等待数据库文件释放
    QThread::msleep(500);
    QApplication::processEvents();

    // 强制删除数据库文件锁（Windows下需要）
#ifdef Q_OS_WIN
    QString systemDbPath = QDir::currentPath() + "/Teacher_System.db";
    QString systemDbWal = systemDbPath + "-wal";
    QString systemDbShm = systemDbPath + "-shm";
    QString systemDbJournal = systemDbPath + "-journal";

    // 删除WAL文件
    if (QFile::exists(systemDbWal)) {
        if (QFile::remove(systemDbWal)) {
            qDebug() << "已删除WAL文件";
        }
    }

    // 删除SHM文件
    if (QFile::exists(systemDbShm)) {
        if (QFile::remove(systemDbShm)) {
            qDebug() << "已删除SHM文件";
        }
    }

    // 删除journal文件
    if (QFile::exists(systemDbJournal)) {
        if (QFile::remove(systemDbJournal)) {
            qDebug() << "已删除journal文件";
        }
    }
#endif

    qDebug() << "数据库连接关闭完成";
}

bool RestoreDialog::deleteFileWithRetry(const QString &filePath, int maxRetry, int delayMs)
{
    for (int i = 0; i < maxRetry; i++) {
        if (!QFile::exists(filePath)) {
            return true; // 文件不存在，视为删除成功
        }

        // 设置文件权限
        QFile::setPermissions(filePath, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);

        if (QFile::remove(filePath)) {
            qDebug() << "成功删除文件:" << filePath;
            return true;
        }

        qDebug() << QString("删除文件失败 (尝试 %1/%2): %3")
                        .arg(i + 1).arg(maxRetry).arg(filePath);

        if (i < maxRetry - 1) {
            QThread::msleep(delayMs);
            QApplication::processEvents();
        }
    }

    return false;
}

bool RestoreDialog::restoreSystemDatabase()
{
    // 查找系统数据库文件
    QString sourceDb = m_backupPath + "/Teacher_System.db";
    if (!QFile::exists(sourceDb)) {
        // 在子目录中搜索
        QDirIterator it(m_backupPath, QStringList() << "Teacher_System.db", QDir::Files, QDirIterator::Subdirectories);
        if (it.hasNext()) {
            sourceDb = it.next();
            qDebug() << "在子目录中找到系统数据库:" << sourceDb;
        } else {
            qDebug() << "未找到系统数据库文件";
            return false;
        }
    }

    QString destDb = QDir::currentPath() + "/Teacher_System.db";

    QFile sourceFile(sourceDb);
    if (!sourceFile.exists()) {
        qDebug() << "源系统数据库文件不存在:" << sourceDb;
        return false;
    }

    qDebug() << "开始恢复系统数据库...";
    qDebug() << "源文件:" << sourceDb;
    qDebug() << "目标文件:" << destDb;

    // 关闭所有数据库连接
    closeAllDatabaseConnections();

    // 备份原文件（如果存在）
    if (QFile::exists(destDb)) {
        QString backupName = destDb + ".backup_" +
                             QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

        qDebug() << "备份原文件到:" << backupName;

        if (QFile::copy(destDb, backupName)) {
            qDebug() << "原文件备份成功";
        } else {
            qDebug() << "原文件备份失败，继续尝试恢复";
        }
    }

    // 删除原文件（带重试）
    if (!deleteFileWithRetry(destDb, 10, 200)) {
        qDebug() << "无法删除原系统数据库文件，尝试重命名";

        // 尝试重命名
        QString renamedFile = destDb + ".old_" +
                              QDateTime::currentDateTime().toString("hhmmss");
        if (QFile::rename(destDb, renamedFile)) {
            qDebug() << "重命名原文件成功:" << renamedFile;
        } else {
            qDebug() << "重命名也失败，放弃恢复";
            return false;
        }
    }

    // 复制备份文件
    if (!sourceFile.copy(destDb)) {
        qDebug() << "复制系统数据库失败:" << sourceFile.errorString();

        // 尝试使用QSaveFile
        QFile destFile(destDb);
        if (destFile.open(QIODevice::WriteOnly)) {
            QFile sourceFile2(sourceDb);
            if (sourceFile2.open(QIODevice::ReadOnly)) {
                destFile.write(sourceFile2.readAll());
                destFile.close();
                sourceFile2.close();
                qDebug() << "使用QSaveFile复制成功";
            } else {
                qDebug() << "无法打开源文件读取";
                return false;
            }
        } else {
            qDebug() << "无法打开目标文件写入";
            return false;
        }
    }

    // 设置文件权限
    QFile::setPermissions(destDb, QFile::ReadOwner | QFile::WriteOwner);

    // 验证恢复结果
    if (QFile::exists(destDb)) {
        QFileInfo destInfo(destDb);
        qDebug() << "系统数据库恢复成功，大小:" << destInfo.size() << "字节";
        return true;
    } else {
        qDebug() << "系统数据库恢复失败";
        return false;
    }
}

bool RestoreDialog::restoreTeacherDatabases()
{
    qDebug() << "开始恢复教师数据库...";

    // 搜索所有教师数据库文件
    QStringList teacherDbPaths;
    QDirIterator it(m_backupPath, QStringList() << "data.db", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString dbPath = it.next();
        QFileInfo dbInfo(dbPath);
        QDir dbDir = dbInfo.dir();
        QString dirName = dbDir.dirName();

        if (dirName.startsWith("teacher_")) {
            teacherDbPaths.append(dbPath);
        }
    }

    bool allSuccess = true;
    int successCount = 0;
    int totalCount = teacherDbPaths.size();

    foreach (const QString &sourceDbPath, teacherDbPaths) {
        QFileInfo sourceInfo(sourceDbPath);
        QDir sourceDir = sourceInfo.dir();
        QString teacherId = sourceDir.dirName();

        qDebug() << "恢复教师数据库:" << teacherId;

        // 创建教师文件夹
        QString destDirPath = QDir::currentPath() + "/" + teacherId;
        QDir destDir(destDirPath);
        if (!destDir.exists()) {
            if (!destDir.mkpath(destDirPath)) {
                qDebug() << "无法创建教师目录:" << destDirPath;
                allSuccess = false;
                continue;
            }
        }

        QString destDbPath = destDirPath + "/data.db";

        // 关闭可能的数据库连接
        QString connectionName = "TEACHER_" + teacherId;
        if (QSqlDatabase::contains(connectionName)) {
            QSqlDatabase::removeDatabase(connectionName);
        }

        // 删除原文件（如果存在）
        if (QFile::exists(destDbPath)) {
            if (!deleteFileWithRetry(destDbPath, 5, 100)) {
                qDebug() << "无法删除原教师数据库:" << teacherId;

                // 尝试重命名
                QString renamedFile = destDbPath + ".old";
                if (QFile::rename(destDbPath, renamedFile)) {
                    qDebug() << "重命名原文件成功";
                } else {
                    allSuccess = false;
                    continue;
                }
            }
        }

        // 复制备份文件
        QFile sourceFile(sourceDbPath);
        if (!sourceFile.copy(destDbPath)) {
            qDebug() << "恢复教师数据库失败:" << teacherId;
            allSuccess = false;
        } else {
            // 设置文件权限
            QFile::setPermissions(destDbPath, QFile::ReadOwner | QFile::WriteOwner);
            successCount++;
            qDebug() << "教师数据库恢复成功:" << teacherId;
        }
    }

    qDebug() << QString("教师数据库恢复完成: %1/%2 成功")
                    .arg(successCount).arg(totalCount);

    return allSuccess;
}








void RestoreDialog::on_pushButton_selectBackup_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择备份文件",
                                                    QDir::homePath(),
                                                    "ZIP文件 (*.zip);;所有文件 (*.*)");
    if (!filePath.isEmpty()) {
        ui->lineEdit_backupPath->setText(filePath);
        ui->pushButton_analyzeBackup->setEnabled(true);
        ui->pushButton_startRestore->setEnabled(false);
        ui->label_restoreStatus->setText("请点击'分析备份'按钮");
    }
}

void RestoreDialog::on_pushButton_analyzeBackup_clicked()
{
    QString backupFile = ui->lineEdit_backupPath->text().trimmed();

    if (backupFile.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择备份文件！");
        return;
    }

    if (!QFile::exists(backupFile)) {
        QMessageBox::critical(this, "错误", "备份文件不存在！");
        return;
    }

    ui->pushButton_analyzeBackup->setEnabled(false);
    ui->pushButton_startRestore->setEnabled(false);
    ui->label_restoreStatus->setText("正在分析备份文件...");
    QApplication::processEvents();

    bool success = false;

    if (backupFile.endsWith(".zip", Qt::CaseInsensitive)) {
        success = analyzeBackupFolder(backupFile);
    } else {
        // 直接分析目录
        success = analyzeBackupFolder(backupFile);
    }

    ui->pushButton_analyzeBackup->setEnabled(true);

    if (success) {
        ui->pushButton_startRestore->setEnabled(true);
        ui->label_restoreStatus->setText("分析完成，可以开始恢复");
    } else {
        ui->pushButton_startRestore->setEnabled(false);
        ui->label_restoreStatus->setText("分析失败");
        QMessageBox::critical(this, "错误", "无法分析备份文件，请检查文件格式是否正确");
    }
}

void RestoreDialog::on_pushButton_startRestore_clicked()
{
    if (!validateBackup()) {
        QMessageBox::critical(this, "错误", "备份文件不完整或无效！");
        return;
    }

    int reply = QMessageBox::warning(this, "警告",
                                     "恢复操作将覆盖现有数据！\n"
                                     "建议先备份当前数据。\n\n"
                                     "确定要继续吗？",
                                     QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    ui->pushButton_startRestore->setEnabled(false);
    ui->pushButton_cancel->setEnabled(false);
    ui->pushButton_analyzeBackup->setEnabled(false);

    ui->label_restoreStatus->setText("正在恢复系统数据库...");
    QApplication::processEvents();

    // 1. 恢复系统数据库
    if (!restoreSystemDatabase()) {
        QMessageBox::critical(this, "错误",
                              "恢复系统数据库失败！\n"
                              "可能原因：\n"
                              "1. 数据库文件被其他程序占用\n"
                              "2. 文件权限不足\n"
                              "3. 磁盘空间不足\n\n"
                              "请关闭所有程序窗口后重试。");

        ui->pushButton_startRestore->setEnabled(true);
        ui->pushButton_cancel->setEnabled(true);
        ui->pushButton_analyzeBackup->setEnabled(true);
        return;
    }

    ui->label_restoreStatus->setText("正在恢复教师数据库...");
    QApplication::processEvents();

    // 2. 恢复教师数据库
    bool teacherRestoreSuccess = restoreTeacherDatabases();

    if (!teacherRestoreSuccess) {
        QMessageBox::warning(this, "警告", "部分教师数据库恢复失败，但系统数据库已恢复完成！");
    }

    ui->label_restoreStatus->setText("恢复完成！");

    // 清理临时目录
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }

    QMessageBox::information(this, "成功",
                             "数据恢复完成！");

    // 延迟后重启程序
    QTimer::singleShot(5000, this, []() {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    });

    accept();
}

void RestoreDialog::on_pushButton_cancel_clicked()
{
    // 清理临时目录
    if (m_tempDir) {
        delete m_tempDir;
        m_tempDir = nullptr;
    }

    reject();
}

