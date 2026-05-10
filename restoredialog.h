#ifndef RESTOREDIALOG_H
#define RESTOREDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include <QThread>
#include <QRegularExpression>
#include <QTextStream>
#include <QTemporaryDir>
#include <QTimer>
#include <QProcess>
#include <QSqlDatabase>
#include <QDirIterator>
#include <QCoreApplication>

namespace Ui {
class RestoreDialog;
}

class RestoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RestoreDialog(QWidget *parent = nullptr);
    ~RestoreDialog();

private slots:
    void on_pushButton_selectBackup_clicked();

    void on_pushButton_analyzeBackup_clicked();

    void on_pushButton_startRestore_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::RestoreDialog *ui;
    QString m_backupPath;
    QTemporaryDir *m_tempDir;

    // 分析备份文件
    bool analyzeBackupFolder(const QString &backupPath);
    // 解压ZIP文件
    bool extractZipFile(const QString &zipPath, const QString &destDir);
    // 恢复系统数据库
    bool restoreSystemDatabase();
    // 恢复教师数据库
    bool restoreTeacherDatabases();
    // 验证备份完整性
    bool validateBackup() const;
    // 关闭所有数据库连接
    void closeAllDatabaseConnections();
    // 删除文件（带重试）
    bool deleteFileWithRetry(const QString &filePath, int maxRetry = 10, int delayMs = 100);
};

#endif // RESTOREDIALOG_H
