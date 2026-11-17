#include "logindialog.h"
#include "databasemanager.h"
#include "registerdialog.h"
#include "forgotpassworddialog.h"
#include "ui_logindialog.h"

#include <QLabel>
#include <QTimer>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("学生信息管理系统 - 登录"));

    // 设置密码框为密码模式
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

    // 演示账号提示（课程演示版）
    auto *demoLabel = new QLabel(
        QStringLiteral("演示账号：admin / admin123　·　teacher01 / 123456　·　2024001 / 123456"),
        this);
    demoLabel->setGeometry(10, 270, 500, 20);
    demoLabel->setAlignment(Qt::AlignCenter);
    demoLabel->setStyleSheet(QStringLiteral("color:#6b7280;font-size:8pt;"));
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
    DatabaseManager *manager = DatabaseManager::instance();
    bool success = manager->validateUser(username, password, userType, userId);

    if (success) {
        // 保存用户信息
        m_userType = userType;
        m_userId = userId;
        m_failedAttempts = 0;
        qDebug() << "登录成功，用户类型：" << userType << "，用户ID：" << userId;
        this->accept();  // 关闭对话框并返回 Accepted
    } else {
        ++m_failedAttempts;
        QMessageBox::warning(this, QStringLiteral("登录失败"),
                             manager->lastError().isEmpty()
                                 ? QStringLiteral("账号或密码错误")
                                 : manager->lastError());
        ui->lineEdit_password->clear();
        ui->lineEdit_password->setFocus();

        // 简单防爆破：连续 5 次失败后锁定 30 秒
        if (m_failedAttempts >= 5) {
            ui->pushButton_login->setEnabled(false);
            ui->pushButton_login->setText(QStringLiteral("已锁定 30 秒"));
            QTimer::singleShot(30000, this, [this]() {
                m_failedAttempts = 0;
                ui->pushButton_login->setEnabled(true);
                ui->pushButton_login->setText(QStringLiteral("登录"));
            });
        }
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

