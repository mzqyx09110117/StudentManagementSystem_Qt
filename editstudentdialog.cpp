#include "editstudentdialog.h"
#include "ui_editstudentdialog.h"

EditStudentDialog::EditStudentDialog(const QString &teacherId,
                                     const QString &studentId,
                                     const QString &name,
                                     const QString &gender,
                                     int age,
                                     const QString &className,
                                     const QString &phone,
                                     const QString &address,
                                     QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditStudentDialog)
    , m_teacherId(teacherId)
    , m_originalStudentId(studentId)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("编辑学生"));

    // 设置年龄范围
    ui->spinBox_age->setMinimum(0);
    ui->spinBox_age->setMaximum(100);

    // 班级下拉框设置为不可编辑
    ui->comboBox_class->setEditable(false);

    // 填充现有数据
    ui->lineEdit_studentId->setText(studentId);
    ui->lineEdit_studentId->setReadOnly(true);  // 学号不可编辑
    ui->lineEdit_name->setText(name);

    // 设置性别
    int genderIndex = ui->comboBox_gender->findText(gender);
    if (genderIndex >= 0) {
        ui->comboBox_gender->setCurrentIndex(genderIndex);
    }

    ui->spinBox_age->setValue(age);
    ui->lineEdit_phone->setText(phone);
    ui->lineEdit_address->setText(address);
    ui->comboBox_class->setCurrentText(className);
}

EditStudentDialog::~EditStudentDialog()
{
    delete ui;
}

// 添加：设置班级下拉框选项
void EditStudentDialog::setClassItems(const QStringList &items)
{
    // 保存当前选中的班级
    QString currentClass = ui->comboBox_class->currentText();

    // 清空并添加所有班级选项
    ui->comboBox_class->clear();
    ui->comboBox_class->addItems(items);

    // 恢复当前选中的班级，如果不存在就保留为文本
    if (!currentClass.isEmpty()) {
        int index = ui->comboBox_class->findText(currentClass);
        if (index >= 0) {
            ui->comboBox_class->setCurrentIndex(index);
        } else {
            ui->comboBox_class->setCurrentText(currentClass);
        }
    }
}

QString EditStudentDialog::getStudentId() const
{
    return ui->lineEdit_studentId->text().trimmed();
}

QString EditStudentDialog::getName() const
{
    return ui->lineEdit_name->text().trimmed();
}

QString EditStudentDialog::getGender() const
{
    return ui->comboBox_gender->currentText();
}

int EditStudentDialog::getAge() const
{
    return ui->spinBox_age->value();
}

QString EditStudentDialog::getClassName() const
{
    return ui->comboBox_class->currentText().trimmed();
}

QString EditStudentDialog::getPhone() const
{
    return ui->lineEdit_phone->text().trimmed();
}

QString EditStudentDialog::getAddress() const
{
    return ui->lineEdit_address->text().trimmed();
}

void EditStudentDialog::on_pushButton_confirm_clicked()
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


void EditStudentDialog::on_pushButton_cancel_clicked()
{
        reject();  // 关闭对话框并返回Rejected
}

