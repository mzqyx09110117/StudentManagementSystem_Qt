#include "gradedialog.h"
#include "ui_gradedialog.h"
#include "databasemanager.h"

GradeDialog::GradeDialog(const QString &teacherId, QWidget *parent)
    : QDialog(parent), ui(new Ui::GradeDialog), m_teacherId(teacherId)
{
    ui->setupUi(this);
    initUI();
}

GradeDialog::GradeDialog(const QString &teacherId, const QString &studentId,
                         const QString &subjectId, const QString &semester, QWidget *parent)
    : QDialog(parent), ui(new Ui::GradeDialog),
    m_teacherId(teacherId), m_studentId(studentId),
    m_subjectId(subjectId), m_semester(semester)
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
    // 根据是否有学生ID判断是添加还是编辑模式
    bool isEditMode = !m_studentId.isEmpty() && !m_subjectId.isEmpty() && !m_semester.isEmpty();

    if (isEditMode) {
        setWindowTitle("编辑成绩");
        // 编辑模式：显示"保存修改"按钮，隐藏删除按钮
        ui->pushButton_save->setText("保存修改");
        ui->pushButton_delete->setVisible(false);
        ui->label_6->setText("修改成绩");
    } else {
        setWindowTitle("添加成绩");
        // 添加模式：显示"确认添加"按钮，隐藏删除按钮
        ui->pushButton_save->setText("确认添加");
        ui->pushButton_delete->setVisible(false);
        ui->label_6->setText("添加成绩");
    }

    // 设置日期编辑框
    ui->dateEdit_examDate->setDate(QDate::currentDate());
    ui->dateEdit_examDate->setCalendarPopup(true);

    // 加载学生和学科列表
    loadStudents();
    loadSubjects();

    // 设置学期默认值（当前学年学期）
    QDate currentDate = QDate::currentDate();
    int year = currentDate.year();
    int month = currentDate.month();
    QString semester = QString("%1-%2").arg(year).arg(month <= 6 ? 1 : 2);
    ui->lineEdit_semester->setText(semester);

    // 连接信号槽
    connect(ui->pushButton_save, &QPushButton::clicked,
            this, &GradeDialog::onSaveButtonClicked);
    connect(ui->pushButton_delete, &QPushButton::clicked,
            this, &GradeDialog::onDeleteButtonClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked,
            this, &GradeDialog::reject);
}

void GradeDialog::loadStudents()
{
    ui->comboBox_student->clear();

    // 获取数据库连接
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("SELECT student_id, name FROM students ORDER BY student_id");

    if (query.exec()) {
        while (query.next()) {
            QString studentInfo = QString("%1 - %2")
            .arg(query.value(0).toString())
                .arg(query.value(1).toString());
            ui->comboBox_student->addItem(studentInfo, query.value(0).toString());
        }
    }
}

void GradeDialog::loadSubjects()
{
    ui->comboBox_subject->clear();

    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("SELECT subject_id, subject_name FROM subjects ORDER BY subject_id");

    if (query.exec()) {
        while (query.next()) {
            QString subjectInfo = QString("%1 - %2")
            .arg(query.value(0).toString())
                .arg(query.value(1).toString());
            ui->comboBox_subject->addItem(subjectInfo, query.value(0).toString());
        }
    }
}

void GradeDialog::loadGradeData()
{
    if (m_studentId.isEmpty() || m_subjectId.isEmpty() || m_semester.isEmpty())
        return;

    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("SELECT score, exam_date, remark FROM grades "
                  "WHERE student_id = ? AND subject_id = ? AND semester = ?");
    query.addBindValue(m_studentId);
    query.addBindValue(m_subjectId);
    query.addBindValue(m_semester);

    if (query.exec() && query.next()) {
        ui->doubleSpinBox_score->setValue(query.value(0).toDouble());
        ui->dateEdit_examDate->setDate(query.value(1).toDate());
        ui->lineEdit_semester->setText(m_semester);

        // 设置选中的学生和学科
        for (int i = 0; i < ui->comboBox_student->count(); i++) {
            if (ui->comboBox_student->itemData(i).toString() == m_studentId) {
                ui->comboBox_student->setCurrentIndex(i);
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_subject->count(); i++) {
            if (ui->comboBox_subject->itemData(i).toString() == m_subjectId) {
                ui->comboBox_subject->setCurrentIndex(i);
                break;
            }
        }

        // 禁用学生、学科和学期的选择（因为是编辑模式）
        ui->comboBox_student->setEnabled(false);
        ui->comboBox_subject->setEnabled(false);
        ui->lineEdit_semester->setEnabled(false);
        ui->pushButton_delete->setVisible(true);
    }
}

bool GradeDialog::validateInput()
{
    if (ui->comboBox_student->currentIndex() < 0) {
        QMessageBox::warning(this, "警告", "请选择学生！");
        return false;
    }

    if (ui->comboBox_subject->currentIndex() < 0) {
        QMessageBox::warning(this, "警告", "请选择学科！");
        return false;
    }

    if (ui->lineEdit_semester->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入学期！");
        return false;
    }

    double score = ui->doubleSpinBox_score->value();
    if (score < 0 || score > 100) {
        QMessageBox::warning(this, "警告", "成绩必须在0-100之间！");
        return false;
    }

    return true;
}

void GradeDialog::onSaveButtonClicked()
{
    if (!validateInput()) return;

    QString studentId = ui->comboBox_student->currentData().toString();
    QString subjectId = ui->comboBox_subject->currentData().toString();
    QString semester = ui->lineEdit_semester->text().trimmed();
    double score = ui->doubleSpinBox_score->value();
    QDate examDate = ui->dateEdit_examDate->date();

    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) {
        QMessageBox::critical(this, "错误", "数据库连接失败！");
        return;
    }

    QSqlQuery query(db);

    if (m_studentId.isEmpty()) {  // 添加模式
        query.prepare("INSERT INTO grades (student_id, subject_id, score, "
                      "exam_date, semester) "
                      "VALUES (?, ?, ?, ?, ?)");  // 只有5个参数，对应5个字段
        query.addBindValue(studentId);
        query.addBindValue(subjectId);
        query.addBindValue(score);
        query.addBindValue(examDate);
        query.addBindValue(semester);
    } else {  // 编辑模式
        query.prepare("UPDATE grades SET score = ?, exam_date = ? "
                      "WHERE student_id = ? AND subject_id = ? AND semester = ?");
        query.addBindValue(score);
        query.addBindValue(examDate);
        query.addBindValue(m_studentId);
        query.addBindValue(m_subjectId);
        query.addBindValue(m_semester);
    }

    if (query.exec()) {
        QMessageBox::information(this, "成功",
                                 m_studentId.isEmpty() ? "成绩添加成功！" : "成绩更新成功！");
        accept();
    } else {
        QMessageBox::critical(this, "错误", "操作失败：" + query.lastError().text());
        qDebug() << "SQL Error:" << query.lastError().text();
        qDebug() << "SQL Query:" << query.lastQuery();
    }
}

void GradeDialog::onDeleteButtonClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  "确定要删除该成绩记录吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        DatabaseManager* dbManager = DatabaseManager::instance();
        QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

        QSqlQuery query(db);
        query.prepare("DELETE FROM grades WHERE student_id = ? AND "
                      "subject_id = ? AND semester = ?");
        query.addBindValue(m_studentId);
        query.addBindValue(m_subjectId);
        query.addBindValue(m_semester);

        if (query.exec()) {
            QMessageBox::information(this, "成功", "成绩删除成功！");
            accept();
        } else {
            QMessageBox::critical(this, "错误", "删除失败：" + query.lastError().text());
        }
    }
}

// Getter 方法
QString GradeDialog::getStudentId() const {
    return ui->comboBox_student->currentData().toString();
}
QString GradeDialog::getSubjectId() const {
    return ui->comboBox_subject->currentData().toString();
}
double GradeDialog::getScore() const { return ui->doubleSpinBox_score->value(); }
QString GradeDialog::getExamDate() const { return ui->dateEdit_examDate->date().toString("yyyy-MM-dd"); }
QString GradeDialog::getSemester() const { return ui->lineEdit_semester->text(); }
