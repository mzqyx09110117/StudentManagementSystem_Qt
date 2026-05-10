#include "subjectdialog.h"
#include "ui_subjectdialog.h"
#include "databasemanager.h"

SubjectDialog::SubjectDialog(const QString &teacherId, Mode mode, QWidget *parent)
    : QDialog(parent), ui(new Ui::SubjectDialog),
    m_teacherId(teacherId), m_mode(mode)
{
    ui->setupUi(this);
    initUI();
    setupDatabase();
}

SubjectDialog::SubjectDialog(const QString &teacherId, const QString &subjectId, QWidget *parent)
    : QDialog(parent), ui(new Ui::SubjectDialog),
    m_teacherId(teacherId), m_subjectId(subjectId), m_mode(EditMode)
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
    // 获取数据库连接
    DatabaseManager* dbManager = DatabaseManager::instance();
    m_db = dbManager->getTeacherDatabase(m_teacherId);

    if (!m_db.isOpen()) {
        QMessageBox::critical(this, "错误", "无法连接数据库！");
        reject();  // 关闭对话框
    }
}

void SubjectDialog::initUI()
{
    // 根据模式设置窗口标题和按钮
    QString windowTitle;
    QString saveButtonText;
    QString mainTitle;

    switch(m_mode) {
    case AddMode:
        windowTitle = "添加学科";
        saveButtonText = "确认添加";
        mainTitle = "添加课程";
        ui->pushButton_delete->setVisible(false);
        ui->pushButton_save->setVisible(true);
        ui->pushButton_save->setText("确认添加");
        break;
    case EditMode:
        windowTitle = "编辑学科";
        saveButtonText = "保存修改";
        mainTitle = "修改课程";
        ui->pushButton_delete->setVisible(false);
        ui->pushButton_save->setVisible(true);
        ui->pushButton_save->setText("保存修改");
        break;
    case DeleteMode:
        windowTitle = "删除学科";
        saveButtonText = ""; // 删除模式没有保存按钮
        mainTitle = "删除课程";
        ui->pushButton_save->setVisible(false);
        ui->pushButton_delete->setVisible(true);
        ui->pushButton_delete->setText("确认删除");
        break;
    }

    setWindowTitle(windowTitle);

    // 正确设置标题标签的文本
    if (ui->label_6) {
        ui->label_6->setText(mainTitle);
    }

    // 在编辑模式下禁用学科ID输入
    if (m_mode == EditMode) {
        ui->lineEdit_subjectId->setReadOnly(true);
    } else {
        ui->lineEdit_subjectId->setReadOnly(false);
    }

    // 连接信号槽
    connect(ui->pushButton_save, &QPushButton::clicked,
            this, &SubjectDialog::onSaveButtonClicked);
    connect(ui->pushButton_delete, &QPushButton::clicked,
            this, &SubjectDialog::onDeleteButtonClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked,
            this, &SubjectDialog::reject);
}


void SubjectDialog::loadSubjectData()
{
    if (m_subjectId.isEmpty()) return;

    if (!m_db.isOpen()) {
        setupDatabase();
        if (!m_db.isOpen()) return;
    }

    // 查询学科信息
    QSqlQuery query(m_db);
    query.prepare("SELECT subject_id, subject_name, credit FROM subjects WHERE subject_id = ?");
    query.addBindValue(m_subjectId);

    if (query.exec() && query.next()) {
        ui->lineEdit_subjectId->setText(query.value(0).toString());
        ui->lineEdit_subjectName->setText(query.value(1).toString());
        ui->doubleSpinBox_credit->setValue(query.value(2).toDouble());
    } else {
        QMessageBox::warning(this, "警告", "未找到学科信息！");
        reject();
    }
}

bool SubjectDialog::validateInput()
{
    if (ui->lineEdit_subjectId->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "学科编号不能为空！");
        ui->lineEdit_subjectId->setFocus();
        return false;
    }

    if (ui->lineEdit_subjectName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "学科名称不能为空！");
        ui->lineEdit_subjectName->setFocus();
        return false;
    }

    if (ui->doubleSpinBox_credit->value() <= 0) {
        QMessageBox::warning(this, "警告", "学分必须大于0！");
        ui->doubleSpinBox_credit->setFocus();
        return false;
    }

    return true;
}

void SubjectDialog::onSaveButtonClicked()
{
    if (!validateInput()) return;

    if (!m_db.isOpen()) {
        QMessageBox::critical(this, "错误", "数据库连接失败！");
        return;
    }

    QString subjectId = ui->lineEdit_subjectId->text().trimmed();
    QString subjectName = ui->lineEdit_subjectName->text().trimmed();
    double credit = ui->doubleSpinBox_credit->value();

    QSqlQuery query(m_db);

    if (m_mode == AddMode) {
        // 检查学科ID是否已存在
        query.prepare("SELECT COUNT(*) FROM subjects WHERE subject_id = ?");
        query.addBindValue(subjectId);

        if (query.exec() && query.next() && query.value(0).toInt() > 0) {
            QMessageBox::warning(this, "警告", "学科编号已存在！");
            return;
        }

        // 插入新学科 - 只插入3个参数
        query.prepare("INSERT INTO subjects (subject_id, subject_name, credit) "
                      "VALUES (?, ?, ?)");  // 只有3个问号，对应3个参数
        query.addBindValue(subjectId);
        query.addBindValue(subjectName);
        query.addBindValue(credit);
    } else if (m_mode == EditMode) {
        // 更新学科信息 - 只更新2个字段
        query.prepare("UPDATE subjects SET subject_name = ?, "
                      "credit = ? WHERE subject_id = ?");
        query.addBindValue(subjectName);
        query.addBindValue(credit);
        query.addBindValue(m_subjectId);  // 使用原始ID
    }

    if (query.exec()) {
        QMessageBox::information(this, "成功",
                                 m_mode == AddMode ? "学科添加成功！" : "学科更新成功！");
        accept();
    } else {
        QMessageBox::critical(this, "错误", "操作失败：" + query.lastError().text());
        qDebug() << "SQL Error:" << query.lastError().text();
        qDebug() << "SQL Query:" << query.lastQuery();
    }
}

void SubjectDialog::onDeleteButtonClicked()
{
    if (m_subjectId.isEmpty()) {
        QMessageBox::warning(this, "警告", "没有可删除的学科！");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  "确定要删除该学科吗？\n（注意：删除学科会同时删除该学科的所有成绩记录）",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (!m_db.isOpen()) {
            QMessageBox::critical(this, "错误", "数据库连接失败！");
            return;
        }

        QSqlQuery query(m_db);

        // 首先删除相关成绩记录（因为有外键约束）
        query.prepare("DELETE FROM grades WHERE subject_id = ?");
        query.addBindValue(m_subjectId);
        if (!query.exec()) {
            QMessageBox::critical(this, "错误", "删除相关成绩失败：" + query.lastError().text());
            return;
        }

        // 然后删除学科
        query.prepare("DELETE FROM subjects WHERE subject_id = ?");
        query.addBindValue(m_subjectId);

        if (query.exec()) {
            QMessageBox::information(this, "成功", "学科删除成功！");
            accept();
        } else {
            QMessageBox::critical(this, "错误", "删除失败：" + query.lastError().text());
        }
    }
}

// Getter 方法
QString SubjectDialog::getSubjectId() const { return ui->lineEdit_subjectId->text(); }
QString SubjectDialog::getSubjectName() const { return ui->lineEdit_subjectName->text(); }
double SubjectDialog::getCredit() const { return ui->doubleSpinBox_credit->value(); }
