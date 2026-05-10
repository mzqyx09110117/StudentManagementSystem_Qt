#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "databasemanager.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("用户注册"));

    // 检查是否已存在管理员
    bool hasAdmin = DatabaseManager::instance()->adminExists();

    // 设置单选按钮状态
    ui->radioButton_teacher->setChecked(true);  // 默认选中教师

    // 如果已有管理员，禁用管理员选项
    if (hasAdmin) {
        ui->radioButton_admin->setEnabled(false);
        ui->radioButton_admin->setToolTip("系统已存在管理员账号");
    }

    // 设置密码框为密码模式
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirm->setEchoMode(QLineEdit::Password);

    // 设置密保问题输入框的提示文本
    ui->lineEdit_question1->setPlaceholderText("例如：你最喜欢的颜色是什么？");
    ui->lineEdit_question2->setPlaceholderText("例如：你第一只宠物的名字是什么？");

    updateTeacherNameField();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushButton_register_clicked()
{
    if (!validateInput()) {
        return;
    }

    // 收集用户信息
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text();
    QString userType = ui->radioButton_admin->isChecked() ? "admin" : "teacher";
    QString teacherName = ui->lineEdit_teacherName->text().trimmed();

    // 密保问题
    QString question1 = ui->lineEdit_question1->text().trimmed();
    QString answer1 = ui->lineEdit_answer1->text().trimmed();
    QString question2 = ui->lineEdit_question2->text().trimmed();
    QString answer2 = ui->lineEdit_answer2->text().trimmed();

    // 调用数据库管理器注册用户
    bool success = DatabaseManager::instance()->registerUser(
        username, password, userType, teacherName, question1, answer1, question2, answer2);

    if (success) {
        // 如果注册的是管理员，禁用管理员单选按钮
        if (userType == "admin") {
            ui->radioButton_admin->setEnabled(false);
            ui->radioButton_admin->setToolTip("系统已存在管理员账号");
        }

        QMessageBox::information(this, "注册成功",
                                 QString("用户 %1 注册成功！").arg(username));
        this->close();
    } else {
        QMessageBox::warning(this, "注册失败",
                             "注册失败，可能是用户名已存在或数据库错误");
    }
}


void RegisterDialog::on_pushButton_back_clicked()
{
    this->close();
}

void RegisterDialog::on_radioButton_admin_toggled(bool checked)
{
    if (checked) {
        // 如果是管理员，教师姓名禁用并清空
        ui->lineEdit_teacherName->setEnabled(false);
        ui->lineEdit_teacherName->clear();
        ui->lineEdit_teacherName->setPlaceholderText("管理员无需填写教师姓名");
    }
}

void RegisterDialog::on_radioButton_teacher_toggled(bool checked)
{
    if (checked) {
        // 如果是教师，教师姓名必填
        ui->lineEdit_teacherName->setEnabled(true);
        ui->lineEdit_teacherName->setPlaceholderText("请输入教师姓名");
        ui->lineEdit_teacherName->setFocus();
    }
}

void RegisterDialog::updateTeacherNameField()
{
    if (ui->radioButton_admin->isChecked()) {
        ui->lineEdit_teacherName->setEnabled(false);
        ui->lineEdit_teacherName->clear();
        ui->lineEdit_teacherName->setPlaceholderText("管理员无需填写教师姓名");
    } else {
        ui->lineEdit_teacherName->setEnabled(true);
        ui->lineEdit_teacherName->setPlaceholderText("请输入教师姓名");
    }
}

bool RegisterDialog::validateInput()
{
    // 检查是否尝试注册管理员但已存在管理员
    if (ui->radioButton_admin->isChecked()) {
        bool hasAdmin = DatabaseManager::instance()->adminExists();
        if (hasAdmin) {
            QMessageBox::warning(this, "注册失败",
                                 "系统已存在管理员账号，无法重复注册");
            ui->radioButton_teacher->setChecked(true);
            ui->radioButton_admin->setEnabled(false);
            return false;
        }
    }

    // 验证用户名
    QString username = ui->lineEdit_username->text().trimmed();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "账号不能为空");
        ui->lineEdit_username->setFocus();
        return false;
    }

    if (username.length() < 4) {
        QMessageBox::warning(this, "输入错误", "账号长度至少4个字符");
        ui->lineEdit_username->setFocus();
        return false;
    }

    if (username.length() > 20) {
        QMessageBox::warning(this, "输入错误", "账号长度最多20个字符");
        ui->lineEdit_username->setFocus();
        return false;
    }

    // 验证密码
    QString password = ui->lineEdit_password->text();
    QString confirm = ui->lineEdit_confirm->text();

    if (password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "密码不能为空");
        ui->lineEdit_password->setFocus();
        return false;
    }

    if (password.length() < 8) {
        QMessageBox::warning(this, "输入错误", "密码长度至少8个字符");
        ui->lineEdit_password->setFocus();
        return false;
    }

    if (password.length() > 40) {
        QMessageBox::warning(this, "输入错误", "密码长度最多40个字符");
        ui->lineEdit_password->setFocus();
        return false;
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
        return false;
    }

    // 如果是教师，验证教师姓名
    if (ui->radioButton_teacher->isChecked()) {
        QString teacherName = ui->lineEdit_teacherName->text().trimmed();
        if (teacherName.isEmpty()) {
            QMessageBox::warning(this, "输入错误", "教师姓名不能为空");
            ui->lineEdit_teacherName->setFocus();
            return false;
        }
    }

    // 验证密保问题（用户自己输入的）
    QString question1 = ui->lineEdit_question1->text().trimmed();
    QString question2 = ui->lineEdit_question2->text().trimmed();
    QString answer1 = ui->lineEdit_answer1->text().trimmed();
    QString answer2 = ui->lineEdit_answer2->text().trimmed();

    if (question1.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "第一个密保问题不能为空");
        ui->lineEdit_question1->setFocus();
        return false;
    }

    if (answer1.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "第一个密保答案不能为空");
        ui->lineEdit_answer1->setFocus();
        return false;
    }

    if (question2.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "第二个密保问题不能为空");
        ui->lineEdit_question2->setFocus();
        return false;
    }

    if (answer2.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "第二个密保答案不能为空");
        ui->lineEdit_answer2->setFocus();
        return false;
    }

    // 两个密保问题不能相同
    if (question1 == question2) {
        QMessageBox::warning(this, "输入错误", "两个密保问题不能相同");
        ui->lineEdit_question2->clear();
        ui->lineEdit_question2->setFocus();
        return false;
    }

    if (ui->radioButton_admin->isChecked()) {
        ui->lineEdit_teacherName->clear();
    }

    return true;
}
