#ifndef GRADEDIALOG_H
#define GRADEDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDate>
#include <QGroupBox>

namespace Ui {
class GradeDialog;
}

class GradeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradeDialog(const QString &teacherId, QWidget *parent = nullptr);
    explicit GradeDialog(const QString &teacherId, const QString &studentId,
                         const QString &subjectId, const QString &semester, QWidget *parent = nullptr);
    ~GradeDialog();

    QString getStudentId() const;
    QString getSubjectId() const;
    double getScore() const;
    QString getExamDate() const;
    QString getSemester() const;

private slots:
    void onSaveButtonClicked();
    void onDeleteButtonClicked();
    void loadStudents();
    void loadSubjects();
    void loadGradeData();

private:
    Ui::GradeDialog *ui;
    QString m_teacherId;
    QString m_studentId;
    QString m_subjectId;
    QString m_semester;
    QSqlDatabase m_db;

    void initUI();
    bool validateInput();
};

#endif // GRADEDIALOG_H
