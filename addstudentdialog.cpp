#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"

AddStudentDialog::AddStudentDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddStudentDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("添加学生"));

    // 设置年龄初始值
    ui->spinBox_age->setValue(0);
    ui->spinBox_age->setMinimum(0);
    ui->spinBox_age->setMaximum(100);

    // 班级下拉框设置为不可编辑
    ui->comboBox_class->setEditable(false);
}

AddStudentDialog::~AddStudentDialog()
{
    delete ui;
}

// 添加：设置班级下拉框选项
void AddStudentDialog::setClassItems(const QStringList &items)
{
    ui->comboBox_class->clear();
    ui->comboBox_class->addItems(items);
}

QString AddStudentDialog::getStudentId() const
{
    return ui->lineEdit_studentId->text().trimmed();
}

QString AddStudentDialog::getName() const
{
    return ui->lineEdit_name->text().trimmed();
}

QString AddStudentDialog::getGender() const
{
    return ui->comboBox_gender->currentText();
}

int AddStudentDialog::getAge() const
{
    return ui->spinBox_age->value();
}

QString AddStudentDialog::getClassName() const
{
    return ui->comboBox_class->currentText().trimmed();
}

QString AddStudentDialog::getPhone() const
{
    return ui->lineEdit_phone->text().trimmed();
}

QString AddStudentDialog::getAddress() const
{
    return ui->lineEdit_address->text().trimmed();
}

void AddStudentDialog::on_pushButton_confirm_clicked()
{
    // 验证必填字段
    if (getStudentId().isEmpty()) {
        QMessageBox::warning(this, "警告", "学号不能为空！");
        return;
    }
    if (getName().isEmpty()) {
        QMessageBox::warning(this, "警告", "姓名不能为空！");
        return;
    }

    QString phone = getPhone().trimmed();  // 去除前后空格

    if (!phone.isEmpty()) {
        // 检查长度是否为11位
        if (phone.length() != 11) {
            QMessageBox::warning(this, "警告", "手机号必须是11位！");
            return;
        }

        // 检查开头是否为1
        if (!phone.startsWith('1')) {
            QMessageBox::warning(this, "警告", "手机号必须以1开头！");
            return;
        }

        // 检查是否全为数字
        static QRegularExpression re("^\\d+$");
        if (!re.match(phone).hasMatch()) {
            QMessageBox::warning(this, "警告", "手机号必须全部由数字组成！");
            return;
        }
    }
    accept();  // 关闭对话框并返回Accepted
}


void AddStudentDialog::on_pushButton_2_clicked()
{
    reject();  // 关闭对话框并返回Rejected
}

