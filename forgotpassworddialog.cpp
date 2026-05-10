#include "forgotpassworddialog.h"
#include "ui_forgotpassworddialog.h"
#include "databasemanager.h"

ForgotPasswordDialog::ForgotPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ForgotPasswordDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("忘记密码"));

    // 设置密码框为密码模式
    ui->lineEdit_newPassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);

    // 初始状态：只启用用户名输入和确认用户按钮
    resetInputFields();
}

ForgotPasswordDialog::~ForgotPasswordDialog()
{
    delete ui;
}

void ForgotPasswordDialog::resetInputFields()
{
    // 重置所有输入状态
    m_selectedQuestion = 0;
    m_currentUsername.clear();
    m_question1.clear();
    m_question2.clear();

    // 清空所有输入框
    ui->lineEdit_answer->clear();
    ui->lineEdit_newPassword->clear();
    ui->lineEdit_confirm->clear();

    // 重置密保问题显示
    ui->radioButton_question1->setText("请先确认用户名");
    ui->radioButton_question2->setText("请先确认用户名");

    // 设置控件状态
    ui->lineEdit_username->setEnabled(true);
    ui->pushButton_confirmUser->setEnabled(true);

    // 禁用密保问题和答案部分
    ui->radioButton_question1->setEnabled(false);
    ui->radioButton_question2->setEnabled(false);
    ui->lineEdit_answer->setEnabled(false);
    ui->lineEdit_newPassword->setEnabled(false);
    ui->lineEdit_confirm->setEnabled(false);
    ui->pushButton_confirmPassword->setEnabled(false);

    // 清空占位符
    ui->lineEdit_answer->setPlaceholderText("");
    ui->lineEdit_newPassword->setPlaceholderText("");
    ui->lineEdit_confirm->setPlaceholderText("");
}

void ForgotPasswordDialog::on_pushButton_confirmUser_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入用户名");
        ui->lineEdit_username->setFocus();
        return;
    }

    // 检查用户是否存在
    QString userType, userId;
    bool userExists = DatabaseManager::instance()->validateUser(username, "", userType, userId);

    if (!userExists) {
        QMessageBox::warning(this, "错误", "用户名不存在");
        ui->lineEdit_username->selectAll();
        ui->lineEdit_username->setFocus();
        return;
    }

    // 检查用户是否设置了密保问题
    QPair<QString, QString> questions = DatabaseManager::instance()->getSecurityQuestions(username);

    if (questions.first.isEmpty() || questions.second.isEmpty()) {
        QMessageBox::warning(this, "错误", "该用户未设置密保问题，无法找回密码");
        resetInputFields();
        return;
    }

    // 保存当前用户名
    m_currentUsername = username;

    // 加载密保问题
    loadSecurityQuestions(username);
}

void ForgotPasswordDialog::loadSecurityQuestions(const QString &username)
{
    // 从数据库获取密保问题
    QPair<QString, QString> questions = DatabaseManager::instance()->getSecurityQuestions(username);

    // 存储问题
    m_question1 = questions.first;
    m_question2 = questions.second;

    // 显示密保问题
    ui->radioButton_question1->setText("问题1: " + m_question1);
    ui->radioButton_question2->setText("问题2: " + m_question2);

    // 启用密保问题选择
    ui->radioButton_question1->setEnabled(true);
    ui->radioButton_question2->setEnabled(true);

    // 禁用用户名输入和确认用户按钮（防止中途修改）
    ui->lineEdit_username->setEnabled(false);
    ui->pushButton_confirmUser->setEnabled(false);

    // 清空其他输入框
    ui->lineEdit_answer->clear();
    ui->lineEdit_newPassword->clear();
    ui->lineEdit_confirm->clear();

    // 设置单选按钮互斥
    ui->radioButton_question1->setAutoExclusive(true);
    ui->radioButton_question2->setAutoExclusive(true);
    ui->radioButton_question1->setChecked(false);
    ui->radioButton_question2->setChecked(false);

    // 设置占位符提示
    ui->lineEdit_answer->setPlaceholderText("请先选择一个密保问题");
}

void ForgotPasswordDialog::on_pushButton_confirmPassword_clicked()
{
    // 验证用户名是否已确认
    if (m_currentUsername.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先确认用户名");
        return;
    }

    // 验证是否选择了密保问题
    if (m_selectedQuestion == 0) {
        QMessageBox::warning(this, "错误", "请选择一个密保问题进行回答");
        return;
    }

    QString answer = ui->lineEdit_answer->text().trimmed();
    QString newPassword = ui->lineEdit_newPassword->text();
    QString confirmPassword = ui->lineEdit_confirm->text();

    // 验证输入
    if (answer.isEmpty()) {
        QMessageBox::warning(this, "错误", "请输入密保答案");
        ui->lineEdit_answer->setFocus();
        return;
    }

    if (newPassword.length() < 8) {
        QMessageBox::warning(this, "错误", "密码长度至少8个字符");
        ui->lineEdit_newPassword->clear();
        ui->lineEdit_newPassword->setFocus();
        return;
    }

    if (newPassword.length() > 40) {
        QMessageBox::warning(this, "错误", "密码长度最多40个字符");
        ui->lineEdit_newPassword->clear();
        ui->lineEdit_newPassword->setFocus();
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致");
        ui->lineEdit_confirm->clear();
        ui->lineEdit_confirm->setFocus();
        return;
    }

    // 检查是否包含至少两种字符类型
    bool hasLetter = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (int i = 0; i < newPassword.length(); ++i) {
        const QChar &ch = newPassword.at(i);
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
        QMessageBox::warning(this, "错误",
                             "密码必须包含至少两种字符类型：\n"
                             "字母、数字、特殊字符（如@#$%等）");
        ui->lineEdit_newPassword->clear();
        ui->lineEdit_newPassword->setFocus();
        return;
    }

    // 验证密保答案
    bool answerCorrect = DatabaseManager::instance()->validateSecurityAnswer(
        m_currentUsername, m_selectedQuestion, answer);

    if (!answerCorrect) {
        QMessageBox::warning(this, "错误", "密保答案不正确");
        ui->lineEdit_answer->clear();
        ui->lineEdit_answer->setFocus();
        return;
    }

    // 更新密码
    bool success = DatabaseManager::instance()->updatePassword(m_currentUsername, newPassword);

    if (success) {
        QMessageBox::information(this, "成功",
                                 QString("用户 %1 的密码修改成功！\n请使用新密码登录。").arg(m_currentUsername));
        this->close();
    } else {
        QMessageBox::warning(this, "错误", "密码修改失败，请重试");
    }
}


void ForgotPasswordDialog::on_pushButton_back_clicked()
{
    this->close();
}


void ForgotPasswordDialog::on_radioButton_question1_toggled(bool checked)
{
    if (checked) {
        m_selectedQuestion = 1;
        ui->lineEdit_answer->setEnabled(true);
        ui->lineEdit_newPassword->setEnabled(true);
        ui->lineEdit_confirm->setEnabled(true);
        ui->pushButton_confirmPassword->setEnabled(true);
        ui->lineEdit_answer->setPlaceholderText("请输入问题1的答案");
        ui->lineEdit_answer->setFocus();
    }
}


void ForgotPasswordDialog::on_radioButton_question2_toggled(bool checked)
{
    if (checked) {
        m_selectedQuestion = 2;
        ui->lineEdit_answer->setEnabled(true);
        ui->lineEdit_newPassword->setEnabled(true);
        ui->lineEdit_confirm->setEnabled(true);
        ui->pushButton_confirmPassword->setEnabled(true);
        ui->lineEdit_answer->setPlaceholderText("请输入问题2的答案");
        ui->lineEdit_answer->setFocus();
    }
}

