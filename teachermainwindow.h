#ifndef TEACHERMAINWINDOW_H
#define TEACHERMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QStringList>
#include <QTableWidget>
#include <QTimer>


namespace Ui {
class TeacherMainWindow;
}

class TeacherMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeacherMainWindow(const QString &teacherId, QWidget *parent = nullptr);
    ~TeacherMainWindow();

private slots:
    void on_pushButton_prev_clicked();

    void on_pushButton_page1_clicked();

    void on_pushButton_page2_clicked();

    void on_pushButton_page3_clicked();

    void on_pushButton_page4_clicked();

    void on_pushButton_page5_clicked();

    void on_pushButton_ellipsis_clicked();

    void on_pushButton_next_clicked();

    void on_lineEdit_search_returnPressed();

    void on_pushButton_search_clicked();

    void on_listWidget_classes_itemClicked(QListWidgetItem *item);

    void on_tableWidget_students_cellDoubleClicked(int row, int column);

    void on_action_addStudent_triggered();

    void on_action_deleteStudent_triggered();

    void on_action_editStudent_triggered();

    void on_action_refreshList_triggered();

    void on_action_inputGrade_triggered();

    void on_action_modifyGrade_triggered();

    void on_action_viewGrade_triggered();

    void on_action_changePassword_triggered();

    void on_action_changequestions_triggered();

    void on_action_add_triggered();

    void on_action_edit_triggered();

    void on_action_delete_triggered();

    void on_action_refresh_triggered();

    void on_action_exit_triggered();

    void on_listWidget_classes_customContextMenuRequested(const QPoint &pos);

    void on_action_add_class_triggered();

    void on_action_edit_class_triggered();

    void on_action_delete_class_triggered();

    void on_action_exit_2_triggered();

    void on_action_addSubject_triggered();

    void on_action_editSubject_triggered();

    void on_action_deleteSubject_triggered();

    void on_action_manageSubjects_triggered();

    void on_action_classGPA_triggered();

    void on_action_allStudentsGPA_triggered();

    void on_action_subjectClass_triggered();

    void on_action_subjectAll_triggered();

    void on_action_addSubject_2_triggered();

    void on_action_editSubject_2_triggered();

    void on_action_deleteSubject_2_triggered();

    void on_action_deleteGrade_triggered();

private:
    Ui::TeacherMainWindow *ui;
    QString m_teacherId;  // 教师ID

    void updatePagination(int currentPage, int totalPages);
    void loadPage(int page);

    int m_currentPage;
    int m_totalPages;
    QList<QPushButton*> m_pageButtons;  // 存储5个页码按钮

    // 学生管理相关
    void initStudentTable();  //初始化学生
    void refreshStudentList();  // 刷新学生列表（从数据库加载）
    bool addStudent(const QString &studentId, const QString &name,
                    const QString &gender, int age, const QString &className,
                    const QString &phone, const QString &address);
    bool updateStudent(const QString &oldStudentId, const QString &studentId,
                       const QString &name, const QString &gender, int age,
                       const QString &className, const QString &phone,
                       const QString &address);
    bool deleteStudent(const QString &studentId);

    // 班级管理相关
    void refreshClassList();
    void loadClassStudents(const QString &className);

    // 学科管理
    void manageSubjects();
    void addSubject();
    void editSubject();
    void deleteSubject();

    // 成绩管理
    void inputGrade();
    void modifyGrade();
    void viewGrade();

    // 报表管理
    void exportClassGPAReport();      // 班级绩点排名
    void exportAllStudentsGPAReport(); // 所有学生绩点排名
    void exportSubjectClassReport();   // 单科班级成绩排名
    void exportSubjectAllReport();     // 单科所有学生成绩排名

    // 辅助方法
    void loadDataToTable(const QList<QMap<QString, QString>> &studentList);
    int getTotalStudentCount();
    QList<QMap<QString, QString>> getStudentsByPage(int page, int pageSize = 15);
    double calculateGPA(double score);
    QString getCurrentSemester();
    QString currentClassFilter() const;
    QStringList classNames() const;
    QString selectedStudentId() const;
    bool addClassInteractive();
    void renameClass(const QString &oldName);
    void deleteClass(const QString &className);
    void createStudentAccountForSelected();

    // 报表导出/打印
    void exportTableToCsv(QTableWidget *table, const QString &defaultName);
    void printTable(QTableWidget *table, const QString &title);
    void showReportDialog(const QString &title, const QStringList &headers,
                          const QList<QStringList> &rows, const QString &csvName,
                          const QString &summary = QString());

    QString m_searchKeyword;
};

#endif // TEACHERMAINWINDOW_H
