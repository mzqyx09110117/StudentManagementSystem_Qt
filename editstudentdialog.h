#ifndef EDITSTUDENTDIALOG_H
#define EDITSTUDENTDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class EditStudentDialog;
}

class EditStudentDialog : public QDialog
{
    Q_OBJECT

public:
    // 构造函数：传入当前学生的所有信息
    explicit EditStudentDialog(const QString &teacherId,
                               const QString &studentId,
                               const QString &name,
                               const QString &gender,
                               int age,
                               const QString &className,
                               const QString &phone,
                               const QString &address,
                               QWidget *parent = nullptr);

    ~EditStudentDialog();

    // 获取用户修改后的数据
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

    void on_pushButton_cancel_clicked();

private:
    Ui::EditStudentDialog *ui;
    QString m_teacherId;
    QString m_originalStudentId;
    QString m_className;
};

#endif // EDITSTUDENTDIALOG_H
