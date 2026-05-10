#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_back_clicked();

    void on_radioButton_admin_toggled(bool checked);

    void on_radioButton_teacher_toggled(bool checked);

private:
    Ui::RegisterDialog *ui;

    // 验证输入
    bool validateInput();
    void updateTeacherNameField();
};

#endif // REGISTERDIALOG_H
