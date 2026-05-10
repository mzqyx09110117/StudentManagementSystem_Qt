#include "logindialog.h"
#include "databasemanager.h"
#include "registerdialog.h"
#include "forgotpassworddialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("用户登录"));

    // 设置密码框为密码模式
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}



// 辅助函数：验证输入有效性
bool LoginDialog::validateInput()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名不能为空");
        ui->lineEdit_username->setFocus();
        return false;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "密码不能为空");
        ui->lineEdit_password->setFocus();
        return false;
    }

    return true;
}

void LoginDialog::on_pushButton_register_clicked()
{
    // 打开注册对话框
    RegisterDialog regDialog;
    regDialog.exec();

    // 注册后清空输入框
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}


void LoginDialog::on_pushButton_login_clicked()
{
    if (!validateInput()) {
        return;
    }

    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text();

    QString userType, userId;
    bool success = DatabaseManager::instance()->validateUser(username, password, userType, userId);

    if (success) {
        // 保存用户信息
        m_userType = userType;
        m_userId = userId;

        qDebug() << "登录成功，用户类型：" << userType << "，用户ID：" << userId;
        this->accept();  // 关闭对话框并返回 Accepted
    } else {
        QMessageBox::warning(this, "登录失败", "账号或密码错误");
        ui->lineEdit_password->clear();
        ui->lineEdit_password->setFocus();
    }
}


void LoginDialog::on_pushButton_forgot_clicked()
{
    // 打开忘记密码对话框
    ForgotPasswordDialog forgotDialog;
    forgotDialog.exec();

    // 结束后清空输入框
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}


void LoginDialog::on_radioButton_showPwd_toggled(bool checked)
{
    // 切换密码显示模式
    if (checked) {
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    }
}

