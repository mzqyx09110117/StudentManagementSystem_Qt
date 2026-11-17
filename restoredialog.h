#ifndef RESTOREDIALOG_H
#define RESTOREDIALOG_H

#include <QDialog>

namespace Ui {
class RestoreDialog;
}

/**
 * @brief 单文件数据库恢复：校验备份 → 安全副本 → 替换 → 完整性检查 → 回滚。
 */
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
    bool m_validated = false;
    QString m_backupPath;

    Ui::RestoreDialog *ui;
};

#endif // RESTOREDIALOG_H
