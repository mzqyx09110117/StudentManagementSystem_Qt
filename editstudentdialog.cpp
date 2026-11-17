#include "editstudentdialog.h"
#include "ui_editstudentdialog.h"

#include "validation.h"

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
    , m_className(className)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("编辑学生"));

    ui->spinBox_age->setMinimum(5);
    ui->spinBox_age->setMaximum(100);
    ui->comboBox_class->setEditable(false);

    ui->lineEdit_studentId->setText(studentId);
    ui->lineEdit_studentId->setReadOnly(true);
    ui->lineEdit_name->setText(name);

    const int genderIndex = ui->comboBox_gender->findText(gender);
    if (genderIndex >= 0)
        ui->comboBox_gender->setCurrentIndex(genderIndex);

    ui->spinBox_age->setValue(qMax(5, age));
    ui->lineEdit_phone->setText(phone);
    ui->lineEdit_address->setText(address);
}

EditStudentDialog::~EditStudentDialog()
{
    delete ui;
}

void EditStudentDialog::setClassItems(const QStringList &items)
{
    ui->comboBox_class->clear();
    ui->comboBox_class->addItem(QStringLiteral("未分班"), QString());
    for (const QString &className : items)
        ui->comboBox_class->addItem(className, className);

    const int index = ui->comboBox_class->findData(m_className);
    ui->comboBox_class->setCurrentIndex(index >= 0 ? index : 0);
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
    return ui->comboBox_class->currentData().toString();
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
    const QString nameError = Validation::nameError(getName());
    if (!nameError.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), nameError);
        ui->lineEdit_name->setFocus();
        return;
    }

    const QString phoneError = Validation::phoneError(getPhone());
    if (!phoneError.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), phoneError);
        ui->lineEdit_phone->setFocus();
        return;
    }

    if (getAge() < 5 || getAge() > 100) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("年龄必须在 5-100 之间"));
        ui->spinBox_age->setFocus();
        return;
    }

    accept();
}

void EditStudentDialog::on_pushButton_cancel_clicked()
{
    reject();
}
