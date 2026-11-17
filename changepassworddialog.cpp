#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"

#include "databasemanager.h"
#include "validation.h"

#include <QCloseEvent>
#include <QMessageBox>

ChangePasswordDialog::ChangePasswordDialog(const QString &userId, bool forced, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePasswordDialog)
    , m_userId(userId)
    , m_forced(forced)
{
    ui->setupUi(this);
    setWindowTitle(forced ? QStringLiteral("首次登录 - 修改密码") : QStringLiteral("修改密码"));

    ui->lineEdit_oldPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);

    if (forced) {
        ui->label->setText(QStringLiteral("首次登录，请修改初始密码"));
        ui->pushButton_cancel->setVisible(false);
    }

    ui->lineEdit_oldPassword->setFocus();
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

void ChangePasswordDialog::reject()
{
    if (m_forced)
        return; // 强制改密时不允许 Esc 退出
    QDialog::reject();
}

void ChangePasswordDialog::closeEvent(QCloseEvent *event)
{
    if (m_forced)
        event->ignore();
    else
        QDialog::closeEvent(event);
}

void ChangePasswordDialog::on_pushButton_confirmPassword_clicked()
{
    const QString oldPassword = ui->lineEdit_oldPassword->text();
    const QString newPassword = ui->lineEdit_password->text();
    const QString confirm = ui->lineEdit_confirm->text();

    if (oldPassword.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("请输入原密码"));
        ui->lineEdit_oldPassword->setFocus();
        return;
    }

    const QString policyError = Validation::passwordError(newPassword);
    if (!policyError.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), policyError);
        ui->lineEdit_password->clear();
        ui->lineEdit_password->setFocus();
        return;
    }

    if (newPassword != confirm) {
        QMessageBox::warning(this, QStringLiteral("输入错误"),
                             QStringLiteral("两次输入的新密码不一致"));
        ui->lineEdit_confirm->clear();
        ui->lineEdit_confirm->setFocus();
        return;
    }

    DatabaseManager *manager = DatabaseManager::instance();
    if (manager->changePassword(m_userId, oldPassword, newPassword)) {
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("密码修改成功，请使用新密码登录。"));
        accept();
    } else {
        QMessageBox::critical(this, QStringLiteral("修改失败"), manager->lastError());
        ui->lineEdit_oldPassword->clear();
        ui->lineEdit_oldPassword->setFocus();
    }
}

void ChangePasswordDialog::on_pushButton_cancel_clicked()
{
    reject();
}
