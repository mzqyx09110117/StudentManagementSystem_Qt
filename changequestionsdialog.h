#ifndef CHANGEQUESTIONSDIALOG_H
#define CHANGEQUESTIONSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>

namespace Ui {
class ChangeQuestionsDialog;
}

class ChangeQuestionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeQuestionsDialog(const QString &username, QWidget *parent = nullptr);
    ~ChangeQuestionsDialog();

private slots:
    void on_pushButton_confirmquestions_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::ChangeQuestionsDialog *ui;
    QString m_username;
};

#endif // CHANGEQUESTIONSDIALOG_H
