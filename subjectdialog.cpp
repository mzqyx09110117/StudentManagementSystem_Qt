#include "subjectdialog.h"
#include "ui_subjectdialog.h"

#include "databasemanager.h"

#include <QSqlError>

SubjectDialog::SubjectDialog(const QString &teacherId, Mode mode, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SubjectDialog)
    , m_teacherId(teacherId)
    , m_mode(mode)
{
    ui->setupUi(this);
    initUI();
    setupDatabase();
}

SubjectDialog::SubjectDialog(const QString &teacherId, const QString &subjectId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SubjectDialog)
    , m_teacherId(teacherId)
    , m_subjectId(subjectId)
    , m_mode(EditMode)
{
    ui->setupUi(this);
    initUI();
    setupDatabase();
    loadSubjectData();
}

SubjectDialog::~SubjectDialog()
{
    delete ui;
}

void SubjectDialog::setupDatabase()
{
    m_db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!m_db.isOpen()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
                              QStringLiteral("无法连接数据库！"));
        reject();
    }
}

void SubjectDialog::initUI()
{
    QString windowTitle;
    QString saveButtonText;
    QString mainTitle;

    switch (m_mode) {
    case AddMode:
        windowTitle = QStringLiteral("添加课程");
        saveButtonText = QStringLiteral("确认添加");
        mainTitle = QStringLiteral("添加课程");
        ui->pushButton_delete->setVisible(false);
        break;
    case EditMode:
        windowTitle = QStringLiteral("编辑课程");
        saveButtonText = QStringLiteral("保存修改");
        mainTitle = QStringLiteral("修改课程");
        ui->pushButton_delete->setVisible(true);
        ui->pushButton_delete->setText(QStringLiteral("删除课程"));
        break;
    case DeleteMode:
        windowTitle = QStringLiteral("删除课程");
        mainTitle = QStringLiteral("删除课程");
        ui->pushButton_save->setVisible(false);
        ui->pushButton_delete->setVisible(true);
        break;
    }

    setWindowTitle(windowTitle);
    if (ui->label_6)
        ui->label_6->setText(mainTitle);
    ui->pushButton_save->setText(saveButtonText);
    ui->lineEdit_subjectId->setReadOnly(m_mode == EditMode);

    ui->doubleSpinBox_credit->setMinimum(0.5);
    ui->doubleSpinBox_credit->setMaximum(20.0);
    if (m_mode == AddMode)
        ui->doubleSpinBox_credit->setValue(3.0);

    connect(ui->pushButton_save, &QPushButton::clicked,
            this, &SubjectDialog::onSaveButtonClicked);
    connect(ui->pushButton_delete, &QPushButton::clicked,
            this, &SubjectDialog::onDeleteButtonClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::reject);
}

void SubjectDialog::loadSubjectData()
{
    if (m_subjectId.isEmpty())
        return;

    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT subject_id, subject_name, credit FROM subjects "
        "WHERE subject_id = ? AND teacher_id = ?"));
    query.addBindValue(m_subjectId);
    query.addBindValue(m_teacherId);

    if (query.exec() && query.next()) {
        ui->lineEdit_subjectId->setText(query.value(0).toString());
        ui->lineEdit_subjectName->setText(query.value(1).toString());
        ui->doubleSpinBox_credit->setValue(query.value(2).toDouble());
    } else {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("未找到该课程，或该课程不属于当前教师"));
        reject();
    }
}

bool SubjectDialog::validateInput()
{
    if (ui->lineEdit_subjectId->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("课程编号不能为空"));
        ui->lineEdit_subjectId->setFocus();
        return false;
    }
    if (ui->lineEdit_subjectName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("课程名称不能为空"));
        ui->lineEdit_subjectName->setFocus();
        return false;
    }
    if (ui->lineEdit_subjectName->text().trimmed().length() > 50) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("课程名称不能超过 50 个字符"));
        ui->lineEdit_subjectName->setFocus();
        return false;
    }
    if (ui->doubleSpinBox_credit->value() <= 0 || ui->doubleSpinBox_credit->value() > 20) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("学分必须大于 0 且不超过 20"));
        ui->doubleSpinBox_credit->setFocus();
        return false;
    }
    return true;
}

void SubjectDialog::onSaveButtonClicked()
{
    if (!validateInput())
        return;
    if (!m_db.isOpen()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
                              QStringLiteral("数据库连接失败"));
        return;
    }

    const QString subjectId = ui->lineEdit_subjectId->text().trimmed();
    const QString subjectName = ui->lineEdit_subjectName->text().trimmed();
    const double credit = ui->doubleSpinBox_credit->value();

    QSqlQuery query(m_db);
    if (m_mode == AddMode) {
        query.prepare(QStringLiteral(
            "INSERT INTO subjects (subject_id, subject_name, credit, teacher_id) "
            "VALUES (?, ?, ?, ?)"));
        query.addBindValue(subjectId);
        query.addBindValue(subjectName);
        query.addBindValue(credit);
        query.addBindValue(m_teacherId);
    } else {
        query.prepare(QStringLiteral(
            "UPDATE subjects SET subject_name = ?, credit = ? "
            "WHERE subject_id = ? AND teacher_id = ?"));
        query.addBindValue(subjectName);
        query.addBindValue(credit);
        query.addBindValue(m_subjectId);
        query.addBindValue(m_teacherId);
    }

    if (!query.exec() || (m_mode == EditMode && query.numRowsAffected() <= 0)) {
        const QString error = query.lastError().text();
        if (error.contains(QStringLiteral("UNIQUE"), Qt::CaseInsensitive)) {
            QMessageBox::warning(this, QStringLiteral("保存失败"),
                                 QStringLiteral("课程编号或课程名称已存在"));
        } else {
            QMessageBox::critical(this, QStringLiteral("保存失败"), error);
        }
        return;
    }

    QMessageBox::information(this, QStringLiteral("成功"),
                             m_mode == AddMode ? QStringLiteral("课程添加成功")
                                               : QStringLiteral("课程修改成功"));
    accept();
}

void SubjectDialog::onDeleteButtonClicked()
{
    if (m_subjectId.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("没有可删除的课程"));
        return;
    }
    if (!m_db.isOpen()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
                              QStringLiteral("数据库连接失败"));
        return;
    }

    QSqlQuery countQuery(m_db);
    countQuery.prepare(QStringLiteral("SELECT COUNT(*) FROM grades WHERE subject_id = ?"));
    countQuery.addBindValue(m_subjectId);
    int gradeCount = 0;
    if (countQuery.exec() && countQuery.next())
        gradeCount = countQuery.value(0).toInt();

    QMessageBox box(QMessageBox::Question, QStringLiteral("确认删除"),
                    QStringLiteral("确定要删除课程【%1】吗？").arg(m_subjectId),
                    QMessageBox::Yes | QMessageBox::No, this);
    box.setInformativeText(QStringLiteral(
        "该课程下有 %1 条成绩记录，删除课程会一并删除这些成绩。").arg(gradeCount));
    if (box.exec() != QMessageBox::Yes)
        return;

    if (!m_db.transaction()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
                              QStringLiteral("无法开始事务"));
        return;
    }

    QSqlQuery deleteGrades(m_db);
    deleteGrades.prepare(QStringLiteral("DELETE FROM grades WHERE subject_id = ?"));
    deleteGrades.addBindValue(m_subjectId);
    if (!deleteGrades.exec()) {
        m_db.rollback();
        QMessageBox::critical(this, QStringLiteral("删除失败"),
                              deleteGrades.lastError().text());
        return;
    }

    QSqlQuery deleteSubject(m_db);
    deleteSubject.prepare(QStringLiteral(
        "DELETE FROM subjects WHERE subject_id = ? AND teacher_id = ?"));
    deleteSubject.addBindValue(m_subjectId);
    deleteSubject.addBindValue(m_teacherId);
    if (!deleteSubject.exec() || deleteSubject.numRowsAffected() <= 0) {
        m_db.rollback();
        QMessageBox::critical(this, QStringLiteral("删除失败"),
                              deleteSubject.lastError().text());
        return;
    }

    if (!m_db.commit()) {
        m_db.rollback();
        QMessageBox::critical(this, QStringLiteral("删除失败"), m_db.lastError().text());
        return;
    }

    QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("课程删除成功"));
    accept();
}

QString SubjectDialog::getSubjectId() const { return ui->lineEdit_subjectId->text().trimmed(); }
QString SubjectDialog::getSubjectName() const { return ui->lineEdit_subjectName->text().trimmed(); }
double SubjectDialog::getCredit() const { return ui->doubleSpinBox_credit->value(); }
