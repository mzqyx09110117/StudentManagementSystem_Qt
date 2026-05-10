#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(const QString &username, QWidget *parent = nullptr);
    ~ChangePasswordDialog();

private slots:
    void on_pushButton_confirmPassword_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::ChangePasswordDialog *ui;
    QString m_username;
};

#endif // CHANGEPASSWORDDIALOG_H
