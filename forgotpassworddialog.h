#ifndef FORGOTPASSWORDDIALOG_H
#define FORGOTPASSWORDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class ForgotPasswordDialog;
}

class ForgotPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotPasswordDialog(QWidget *parent = nullptr);
    ~ForgotPasswordDialog();

private slots:
    void on_pushButton_confirmUser_clicked();

    void on_pushButton_confirmPassword_clicked();

    void on_pushButton_back_clicked();

    void on_radioButton_question1_toggled(bool checked);

    void on_radioButton_question2_toggled(bool checked);

private:
    Ui::ForgotPasswordDialog *ui;

    // 加载用户的密保问题
    void loadSecurityQuestions(const QString &username);

    // 重置所有输入框
    void resetInputFields();

    // 当前选中的密保问题编号（1或2）
    int m_selectedQuestion;

    // 当前验证的用户名
    QString m_currentUsername;

    // 存储从数据库获取的密保问题
    QString m_question1;
    QString m_question2;
};

#endif // FORGOTPASSWORDDIALOG_H
