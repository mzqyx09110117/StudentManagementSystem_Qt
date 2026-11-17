#ifndef BACKUPDIALOG_H
#define BACKUPDIALOG_H

#include <QDialog>

namespace Ui {
class BackupDialog;
}

/**
 * @brief 单文件数据库备份（VACUUM INTO 一致性快照）。
 */
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
    void updateInfo();

    Ui::BackupDialog *ui;
};

#endif // BACKUPDIALOG_H
