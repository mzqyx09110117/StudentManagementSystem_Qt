#include "gradedialog.h"
#include "ui_gradedialog.h"

#include "databasemanager.h"
#include "validation.h"

#include <QCompleter>
#include <QDate>
#include <QSqlError>
#include <QStringList>
#include <QTimer>

GradeDialog::GradeDialog(const QString &teacherId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GradeDialog)
    , m_teacherId(teacherId)
{
    ui->setupUi(this);
    initUI();
}

GradeDialog::GradeDialog(const QString &teacherId, const QString &studentId,
                         const QString &subjectId, const QString &semester, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GradeDialog)
    , m_teacherId(teacherId)
    , m_studentId(studentId)
    , m_subjectId(subjectId)
    , m_semester(semester)
{
    ui->setupUi(this);
    initUI();
    loadGradeData();
}

GradeDialog::~GradeDialog()
{
    delete ui;
}

void GradeDialog::initUI()
{
    const bool isEditMode = !m_studentId.isEmpty() && !m_subjectId.isEmpty()
                            && !m_semester.isEmpty();

    if (isEditMode) {
        setWindowTitle(QStringLiteral("编辑成绩"));
        ui->pushButton_save->setText(QStringLiteral("保存修改"));
        ui->label_6->setText(QStringLiteral("修改成绩"));
    } else {
        setWindowTitle(QStringLiteral("录入成绩"));
        ui->pushButton_save->setText(QStringLiteral("确认添加"));
        ui->label_6->setText(QStringLiteral("录入成绩"));
    }
    ui->pushButton_delete->setVisible(false);

    // 分数范围：修复原 UI 默认上限 99.99 的问题
    ui->doubleSpinBox_score->setMinimum(0.0);
    ui->doubleSpinBox_score->setMaximum(100.0);
    ui->doubleSpinBox_score->setDecimals(1);

    ui->dateEdit_examDate->setDate(QDate::currentDate());
    ui->dateEdit_examDate->setCalendarPopup(true);

    // 学期：规范为 2024-2025-1，并提供历史学期补全
    ui->lineEdit_semester->setPlaceholderText(QStringLiteral("例如：2024-2025-1"));
    auto *completer = new QCompleter(Validation::recentSemesters(8), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_semester->setCompleter(completer);
    ui->lineEdit_semester->setText(Validation::semesterForDate());

    loadStudents();
    loadSubjects();

    connect(ui->pushButton_save, &QPushButton::clicked,
            this, &GradeDialog::onSaveButtonClicked);
    connect(ui->pushButton_delete, &QPushButton::clicked,
            this, &GradeDialog::onDeleteButtonClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::reject);
}

void GradeDialog::loadStudents()
{
    ui->comboBox_student->clear();

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    if (!query.exec(QStringLiteral(
            "SELECT student_id, name, COALESCE(class_name,'未分班') "
            "FROM students ORDER BY class_name IS NULL, class_name, student_id"))) {
        QMessageBox::warning(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    while (query.next()) {
        const QString text = QStringLiteral("%1 - %2（%3）")
                                 .arg(query.value(0).toString(),
                                      query.value(1).toString(),
                                      query.value(2).toString());
        ui->comboBox_student->addItem(text, query.value(0).toString());
    }
}

void GradeDialog::loadSubjects()
{
    ui->comboBox_subject->clear();

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(QStringLiteral(
        "SELECT subject_id, subject_name FROM subjects "
        "WHERE teacher_id = ? ORDER BY subject_id"));
    query.addBindValue(m_teacherId);
    if (!query.exec()) {
        QMessageBox::warning(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    while (query.next()) {
        const QString text = QStringLiteral("%1 - %2")
                                 .arg(query.value(0).toString(), query.value(1).toString());
        ui->comboBox_subject->addItem(text, query.value(0).toString());
    }

    if (ui->comboBox_subject->count() == 0) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("当前教师还没有课程，请先在课程管理中添加课程。"));
    }
}

void GradeDialog::loadGradeData()
{
    if (m_studentId.isEmpty() || m_subjectId.isEmpty() || m_semester.isEmpty())
        return;

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(QStringLiteral(
        "SELECT g.score, g.exam_date, g.remark FROM grades g "
        "JOIN subjects sub ON sub.subject_id = g.subject_id "
        "WHERE g.student_id = ? AND g.subject_id = ? AND g.semester = ? "
        "AND sub.teacher_id = ?"));
    query.addBindValue(m_studentId);
    query.addBindValue(m_subjectId);
    query.addBindValue(m_semester);
    query.addBindValue(m_teacherId);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("未找到该成绩记录，可能已被删除。"));
        QTimer::singleShot(0, this, &QDialog::reject);
        return;
    }

    ui->doubleSpinBox_score->setValue(query.value(0).toDouble());
    const QDate examDate = query.value(1).toDate();
    if (examDate.isValid())
        ui->dateEdit_examDate->setDate(examDate);
    ui->lineEdit_semester->setText(m_semester);
    ui->lineEdit_semester->setReadOnly(true);

    for (int i = 0; i < ui->comboBox_student->count(); ++i) {
        if (ui->comboBox_student->itemData(i).toString() == m_studentId) {
            ui->comboBox_student->setCurrentIndex(i);
            break;
        }
    }
    for (int i = 0; i < ui->comboBox_subject->count(); ++i) {
        if (ui->comboBox_subject->itemData(i).toString() == m_subjectId) {
            ui->comboBox_subject->setCurrentIndex(i);
            break;
        }
    }

    ui->comboBox_student->setEnabled(false);
    ui->comboBox_subject->setEnabled(false);
    ui->pushButton_delete->setVisible(true);
}

bool GradeDialog::validateInput()
{
    if (ui->comboBox_student->currentIndex() < 0) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("请选择学生"));
        return false;
    }
    if (ui->comboBox_subject->currentIndex() < 0) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("请选择课程"));
        return false;
    }
    if (!Validation::isValidSemester(ui->lineEdit_semester->text())) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("学期格式应为 2024-2025-1 或 2024-2025-2"));
        ui->lineEdit_semester->setFocus();
        return false;
    }
    const double score = ui->doubleSpinBox_score->value();
    if (score < 0 || score > 100) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("成绩必须在 0 到 100 之间"));
        return false;
    }
    return true;
}

void GradeDialog::onSaveButtonClicked()
{
    if (!validateInput())
        return;

    const QString studentId = ui->comboBox_student->currentData().toString();
    const QString subjectId = ui->comboBox_subject->currentData().toString();
    const QString semester = ui->lineEdit_semester->text().trimmed();
    const double score = ui->doubleSpinBox_score->value();
    const QString examDate = ui->dateEdit_examDate->date().toString(QStringLiteral("yyyy-MM-dd"));

    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
                              QStringLiteral("数据库连接失败"));
        return;
    }

    const bool addMode = m_studentId.isEmpty();
    if (addMode) {
        QSqlQuery subjectOwn(db);
        subjectOwn.prepare(QStringLiteral(
            "SELECT 1 FROM subjects WHERE subject_id = ? AND teacher_id = ?"));
        subjectOwn.addBindValue(subjectId);
        subjectOwn.addBindValue(m_teacherId);
        if (!subjectOwn.exec() || !subjectOwn.next()) {
            QMessageBox::warning(this, QStringLiteral("警告"),
                                 QStringLiteral("只能录入本人负责课程的成绩"));
            return;
        }

        QSqlQuery exists(db);
        exists.prepare(QStringLiteral(
            "SELECT score FROM grades WHERE student_id = ? AND subject_id = ? AND semester = ?"));
        exists.addBindValue(studentId);
        exists.addBindValue(subjectId);
        exists.addBindValue(semester);
        if (exists.exec() && exists.next()) {
            const QString detail = QStringLiteral(
                "该学生本学期已有此课程成绩（%1 分），是否覆盖？")
                                       .arg(exists.value(0).toDouble());
            if (QMessageBox::question(this, QStringLiteral("成绩已存在"), detail,
                                      QMessageBox::Yes | QMessageBox::No)
                != QMessageBox::Yes) {
                return;
            }
        }
    }

    QSqlQuery query(db);
    if (addMode) {
        query.prepare(QStringLiteral(
            "INSERT INTO grades (student_id, subject_id, semester, score, exam_date, entered_by) "
            "VALUES (?, ?, ?, ?, ?, ?) "
            "ON CONFLICT(student_id, subject_id, semester) DO UPDATE SET "
            "score = excluded.score, exam_date = excluded.exam_date, "
            "entered_by = excluded.entered_by, updated_at = datetime('now','localtime')"));
    } else {
        query.prepare(QStringLiteral(
            "UPDATE grades SET score = ?, exam_date = ?, entered_by = ?, "
            "updated_at = datetime('now','localtime') "
            "WHERE student_id = ? AND subject_id = ? AND semester = ? "
            "AND subject_id IN (SELECT subject_id FROM subjects WHERE teacher_id = ?)"));
    }

    if (addMode) {
        query.addBindValue(studentId);
        query.addBindValue(subjectId);
        query.addBindValue(semester);
        query.addBindValue(score);
        query.addBindValue(examDate);
        query.addBindValue(m_teacherId);
    } else {
        query.addBindValue(score);
        query.addBindValue(examDate);
        query.addBindValue(m_teacherId);
        query.addBindValue(m_studentId);
        query.addBindValue(m_subjectId);
        query.addBindValue(m_semester);
        query.addBindValue(m_teacherId);
    }

    if (!query.exec() || (!addMode && query.numRowsAffected() <= 0)) {
        QMessageBox::critical(this, QStringLiteral("保存失败"), query.lastError().text());
        return;
    }

    QMessageBox::information(this, QStringLiteral("成功"),
                             addMode ? QStringLiteral("成绩录入成功")
                                     : QStringLiteral("成绩修改成功"));
    accept();
}

void GradeDialog::onDeleteButtonClicked()
{
    if (m_studentId.isEmpty() || m_subjectId.isEmpty() || m_semester.isEmpty())
        return;

    if (QMessageBox::question(this, QStringLiteral("确认删除"),
                              QStringLiteral("确定要删除这条成绩记录吗？"),
                              QMessageBox::Yes | QMessageBox::No)
        != QMessageBox::Yes) {
        return;
    }

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(QStringLiteral(
        "DELETE FROM grades WHERE student_id = ? AND subject_id = ? AND semester = ? "
        "AND subject_id IN (SELECT subject_id FROM subjects WHERE teacher_id = ?)"));
    query.addBindValue(m_studentId);
    query.addBindValue(m_subjectId);
    query.addBindValue(m_semester);
    query.addBindValue(m_teacherId);

    if (!query.exec() || query.numRowsAffected() <= 0) {
        QMessageBox::critical(this, QStringLiteral("删除失败"), query.lastError().text());
        return;
    }

    QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("成绩删除成功"));
    accept();
}

QString GradeDialog::getStudentId() const { return ui->comboBox_student->currentData().toString(); }
QString GradeDialog::getSubjectId() const { return ui->comboBox_subject->currentData().toString(); }
double GradeDialog::getScore() const { return ui->doubleSpinBox_score->value(); }
QString GradeDialog::getExamDate() const { return ui->dateEdit_examDate->date().toString(QStringLiteral("yyyy-MM-dd")); }
QString GradeDialog::getSemester() const { return ui->lineEdit_semester->text().trimmed(); }
