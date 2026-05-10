#ifndef CONFIRMDELETEDIALOG_H
#define CONFIRMDELETEDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmDeleteDialog;
}

class ConfirmDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDeleteDialog(const QString &studentId,
                                 const QString &studentName,
                                 QWidget *parent = nullptr);
    ~ConfirmDeleteDialog();

    // 获取学生信息（虽然这里可能不需要）
    QString getStudentId() const;
    QString getStudentName() const;


private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::ConfirmDeleteDialog *ui;

    // 存储学生信息
    QString m_studentId;
    QString m_studentName;
};

#endif // CONFIRMDELETEDIALOG_H
