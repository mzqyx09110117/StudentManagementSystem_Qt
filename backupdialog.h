#ifndef BACKUPDIALOG_H
#define BACKUPDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QProcess>
#include <QDebug>
#include <QTextStream>
#include <QTemporaryDir>
#include <QThread>

namespace Ui {
class BackupDialog;
}

class BackupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BackupDialog(QWidget *parent = nullptr);
    ~BackupDialog();

private slots:

    void on_pushButton_selectFolder_clicked();

    void on_pushButton_startBackup_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::BackupDialog *ui;
    QTemporaryDir *m_tempDir;  // 添加临时目录指针

    // 获取所有教师文件夹路径
    QStringList getAllTeacherFolders() const;
    // 创建ZIP压缩文件
    bool createZipFile(const QString &zipFilePath);
    // 准备备份文件到临时目录
    bool prepareBackupFiles(const QString &backupDir) const;
    // 使用系统工具创建ZIP
    bool createZipWithSystemTool(const QString &sourceDir, const QString &zipFilePath) const;
    // 创建备份日志
    void createBackupLog(const QString &backupDir) const;
};

#endif // BACKUPDIALOG_H
