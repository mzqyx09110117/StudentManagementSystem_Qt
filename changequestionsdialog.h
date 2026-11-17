#ifndef CHANGEQUESTIONS_H
#define CHANGEQUESTIONS_H

#include <QDialog>

namespace Ui {
class ChangeQuestionsDialog;
}

class ChangeQuestionsDialog : public QDialog
{
    Q_OBJECT

public:
    // userId 为 users.user_id
    explicit ChangeQuestionsDialog(const QString &userId, QWidget *parent = nullptr);
    ~ChangeQuestionsDialog();

private slots:
    void on_pushButton_confirmquestions_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::ChangeQuestionsDialog *ui;
    QString m_userId;
};

#endif // CHANGEQUESTIONS_H
