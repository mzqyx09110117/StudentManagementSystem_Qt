#ifndef SUBJECTDIALOG_H
#define SUBJECTDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class SubjectDialog;
}

class SubjectDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        AddMode,
        EditMode,
        DeleteMode
    };

    explicit SubjectDialog(const QString &teacherId, Mode mode, QWidget *parent = nullptr);
    explicit SubjectDialog(const QString &teacherId, const QString &subjectId,
                           QWidget *parent = nullptr);
    ~SubjectDialog();

    QString getSubjectId() const;
    QString getSubjectName() const;
    double getCredit() const;

private slots:
    void onSaveButtonClicked();
    void onDeleteButtonClicked();

private:
    Ui::SubjectDialog *ui;
    QString m_teacherId;
    QString m_subjectId;
    Mode m_mode;
    QSqlDatabase m_db;

    void initUI();
    void loadSubjectData();
    bool validateInput();
    void setupDatabase();
};

#endif // SUBJECTDIALOG_H
