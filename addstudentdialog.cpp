#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"

#include "validation.h"

AddStudentDialog::AddStudentDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddStudentDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("添加学生"));

    ui->spinBox_age->setMinimum(5);
    ui->spinBox_age->setMaximum(100);
    ui->spinBox_age->setValue(18);

    ui->comboBox_class->setEditable(false);
    ui->comboBox_gender->setCurrentIndex(0);
}

AddStudentDialog::~AddStudentDialog()
{
    delete ui;
}

void AddStudentDialog::setClassItems(const QStringList &items)
{
    ui->comboBox_class->clear();
    for (const QString &className : items)
        ui->comboBox_class->addItem(className, className);
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
    return ui->comboBox_class->currentData().toString();
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
    const QString idError = Validation::accountIdError(getStudentId());
    if (!idError.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), idError);
        ui->lineEdit_studentId->setFocus();
        return;
    }

    const QString nameError = Validation::nameError(getName());
    if (!nameError.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), nameError);
        ui->lineEdit_name->setFocus();
        return;
    }

    if (getClassName().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("请选择班级；若没有班级，请先在主界面添加班级"));
        ui->comboBox_class->setFocus();
        return;
    }

    const QString phoneError = Validation::phoneError(getPhone());
    if (!phoneError.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), phoneError);
        ui->lineEdit_phone->setFocus();
        return;
    }

    accept();
}

void AddStudentDialog::on_pushButton_2_clicked()
{
    reject();
}
