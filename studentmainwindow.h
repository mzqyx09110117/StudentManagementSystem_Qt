#ifndef STUDENTMAINWINDOW_H
#define STUDENTMAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class QLabel;
class QTableWidget;

/**
 * @brief 学生端主窗口：只读查看个人信息、GPA/排名和成绩明细。
 */
class StudentMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentMainWindow(const QString &userId, QWidget *parent = nullptr);
    ~StudentMainWindow();

private slots:
    void refreshGrades();
    void onChangePassword();
    void onChangeQuestions();
    void onExit();

private:
    void buildUi();
    bool loadStudent();
    void loadSummary();
    void loadGrades();

    QString m_userId;
    QString m_studentId;
    QString m_name;
    QString m_className;
    QString m_status;

    QLabel *m_nameLabel = nullptr;
    QLabel *m_metaLabel = nullptr;
    QLabel *m_gpaValue = nullptr;
    QLabel *m_avgValue = nullptr;
    QLabel *m_creditValue = nullptr;
    QLabel *m_rankValue = nullptr;
    QComboBox *m_semesterCombo = nullptr;
    QTableWidget *m_table = nullptr;
};

#endif // STUDENTMAINWINDOW_H
