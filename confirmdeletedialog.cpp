#include "confirmdeletedialog.h"
#include "ui_confirmdeletedialog.h"

ConfirmDeleteDialog::ConfirmDeleteDialog(const QString &studentId,
                                         const QString &studentName,
                                         QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ConfirmDeleteDialog),
    m_studentId(studentId),
    m_studentName(studentName)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("删除学生"));

    // 在对话框中显示学生信息
    ui->label_studentId->setText(studentId);
    ui->label_studentName->setText(studentName);
}

ConfirmDeleteDialog::~ConfirmDeleteDialog()
{
    delete ui;
}

QString ConfirmDeleteDialog::getStudentId() const
{
    return m_studentId;
}

QString ConfirmDeleteDialog::getStudentName() const
{
    return m_studentName;
}

void ConfirmDeleteDialog::on_pushButton_confirm_clicked()
{
    accept();
}


void ConfirmDeleteDialog::on_pushButton_cancel_clicked()
{
    reject();
}

