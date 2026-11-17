#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    // 获取登录成功的用户信息
    QString getUserType() const { return m_userType; }
    QString getUserId() const { return m_userId; }

private slots:

    void on_pushButton_register_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_forgot_clicked();

    void on_radioButton_showPwd_toggled(bool checked);

private:
    Ui::LoginDialog *ui;

    bool validateInput();
    QString m_userType;  // admin / teacher / student
    QString m_userId;    // users.user_id
    int m_failedAttempts = 0;
};

#endif // LOGINDIALOG_H
