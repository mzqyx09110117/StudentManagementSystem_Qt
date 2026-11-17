#ifndef AUDITLOGDIALOG_H
#define AUDITLOGDIALOG_H

#include <QDialog>

class QComboBox;
class QTableWidget;

/**
 * @brief 审计日志查看：展示触发器写入的成绩变更记录。
 */
class AuditLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuditLogDialog(QWidget *parent = nullptr);

private slots:
    void loadData();

private:
    QComboBox *m_operationCombo = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // AUDITLOGDIALOG_H
