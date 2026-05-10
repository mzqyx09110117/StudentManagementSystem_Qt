#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddStudentDialog;
}

class AddStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentDialog(QWidget *parent = nullptr);
    ~AddStudentDialog();

    // 获取用户输入的数据
    QString getStudentId() const;
    QString getName() const;
    QString getGender() const;
    int getAge() const;
    QString getClassName() const;
    QString getPhone() const;
    QString getAddress() const;

    // 添加：设置班级下拉框选项
    void setClassItems(const QStringList &items);

private slots:

    void on_pushButton_confirm_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddStudentDialog *ui;
};

#endif // ADDSTUDENTDIALOG_H
