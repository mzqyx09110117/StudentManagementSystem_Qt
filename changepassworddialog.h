#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    // userId 为 users.user_id（不是登录名）
    explicit ChangePasswordDialog(const QString &userId, bool forced = false,
                                  QWidget *parent = nullptr);
    ~ChangePasswordDialog();

private slots:
    void on_pushButton_confirmPassword_clicked();
    void on_pushButton_cancel_clicked();

protected:
    void reject() override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::ChangePasswordDialog *ui;
    QString m_userId;
    bool m_forced;
};

#endif // CHANGEPASSWORDDIALOG_H
