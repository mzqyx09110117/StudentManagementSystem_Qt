#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"
#include "databasemanager.h"

ChangePasswordDialog::ChangePasswordDialog(const QString &username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordDialog)
    , m_username(username)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("修改密码"));

    // 设置密码框为密码模式
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

void ChangePasswordDialog::on_pushButton_confirmPassword_clicked()
{
    // 验证密码
    QString password = ui->lineEdit_password->text();
    QString confirm = ui->lineEdit_confirm->text();

    if (password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "密码不能为空");
        ui->lineEdit_password->setFocus();
        return;
    }

    if (password.length() < 8) {
        QMessageBox::warning(this, "输入错误", "密码长度至少8个字符");
        ui->lineEdit_password->setFocus();
        return;
    }

    if (password.length() > 40) {
        QMessageBox::warning(this, "输入错误", "密码长度最多40个字符");
        ui->lineEdit_password->setFocus();
        return;
    }

    // 检查是否包含至少两种字符类型
    bool hasLetter = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (int i = 0; i < password.length(); ++i) {
        const QChar &ch = password.at(i);
        if (ch.isLetter()) {
            hasLetter = true;
        } else if (ch.isDigit()) {
            hasDigit = true;
        } else if (!ch.isSpace()) {
            hasSpecial = true;
        }
    }

    // 统计包含的类型数量
    int typeCount = (hasLetter ? 1 : 0) + (hasDigit ? 1 : 0) + (hasSpecial ? 1 : 0);

    if (typeCount < 2) {
        QMessageBox::warning(this, "输入错误",
                             "密码必须包含至少两种字符类型：\n"
                             "字母、数字、特殊字符（如@#$%等）");
        ui->lineEdit_password->setFocus();
        return;
    }

    DatabaseManager* dbManager = DatabaseManager::instance();

    // 更新密码
    if (dbManager->updatePassword(m_username, password)) {
        QMessageBox::information(this, "成功", "密码修改成功！");
        accept();
    } else {
        QMessageBox::critical(this, "错误", "密码修改失败！");
    }
}


void ChangePasswordDialog::on_pushButton_cancel_clicked()
{
    reject();
}

