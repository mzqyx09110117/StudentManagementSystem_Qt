#include "teachermainwindow.h"
#include "ui_teachermainwindow.h"
#include "databasemanager.h"
#include "addstudentdialog.h"
#include "confirmdeletedialog.h"
#include "editstudentdialog.h"
#include "changepassworddialog.h"
#include "changequestionsdialog.h"
#include "subjectdialog.h"
#include "gradedialog.h"
#include "validation.h"

#include <QAction>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QRegularExpression>
#include <QStatusBar>
#include <QTextDocument>
#include <QTextStream>
#include <QVariant>

TeacherMainWindow::TeacherMainWindow(const QString &teacherId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeacherMainWindow),
    m_teacherId(teacherId)
{
    ui->setupUi(this);
    const QString displayName = DatabaseManager::instance()->getDisplayName(teacherId);
    setWindowTitle(QStringLiteral("学生信息管理系统 - 教师端（%1）")
                       .arg(displayName.isEmpty() ? teacherId : displayName));

    // 初始化分页控件
    m_currentPage = 1;
    m_totalPages = 1;

    // 收集5个页码按钮指针
    m_pageButtons.clear();
    m_pageButtons.append(ui->pushButton_page1);
    m_pageButtons.append(ui->pushButton_page2);
    m_pageButtons.append(ui->pushButton_page3);
    m_pageButtons.append(ui->pushButton_page4);
    m_pageButtons.append(ui->pushButton_page5);

    // 搜索字段选择：全部/学号/姓名/...
    if (ui->comboBox->count() == 0 || ui->comboBox->itemText(0) != QStringLiteral("全部"))
        ui->comboBox->insertItem(0, QStringLiteral("全部"));
    ui->comboBox->setCurrentIndex(0);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) {
                if (!m_searchKeyword.isEmpty()) {
                    m_currentPage = 1;
                    refreshStudentList();
                }
            });

    // 初始化表格列
    initStudentTable();

    // 在学生管理菜单中加入“开通学生账号”
    for (QMenu *menu : menuBar()->findChildren<QMenu *>()) {
        if (menu->title().contains(QStringLiteral("学生"))) {
            QAction *accountAction = menu->addAction(QStringLiteral("开通学生账号..."));
            connect(accountAction, &QAction::triggered,
                    this, &TeacherMainWindow::createStudentAccountForSelected);
            break;
        }
    }

    // 初始化班级列表
    refreshClassList();

    // 初始加载数据
    refreshStudentList();
}

TeacherMainWindow::~TeacherMainWindow()
{
    delete ui;
}

void TeacherMainWindow::initStudentTable()
{
    // 设置表格属性
    ui->tableWidget_students->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_students->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// 刷新班级列表：数据源为 classes 表，空班级在重启后依然保留
void TeacherMainWindow::refreshClassList()
{
    const QString selected = currentClassFilter();
    const QStringList names = classNames();

    bool hasUnassigned = false;
    QSqlQuery unassignedQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    if (unassignedQuery.exec(QStringLiteral(
            "SELECT COUNT(*) FROM students WHERE class_name IS NULL"))
        && unassignedQuery.next()) {
        hasUnassigned = unassignedQuery.value(0).toInt() > 0;
    }

    ui->listWidget_classes->blockSignals(true);
    ui->listWidget_classes->clear();
    ui->listWidget_classes->addItem(QStringLiteral("全部学生"));
    for (const QString &name : names)
        ui->listWidget_classes->addItem(name);
    if (hasUnassigned)
        ui->listWidget_classes->addItem(QStringLiteral("未分班"));
    ui->listWidget_classes->addItem(QStringLiteral("+ 添加班级"));

    int targetRow = 0;
    if (!selected.isEmpty()) {
        for (int i = 0; i < ui->listWidget_classes->count(); ++i) {
            if (ui->listWidget_classes->item(i)->text() == selected) {
                targetRow = i;
                break;
            }
        }
    }
    ui->listWidget_classes->setCurrentRow(targetRow);
    ui->listWidget_classes->blockSignals(false);
}

QString TeacherMainWindow::currentClassFilter() const
{
    QListWidgetItem *item = ui->listWidget_classes->currentItem();
    if (!item)
        return QString();
    const QString text = item->text();
    if (text == QStringLiteral("全部学生") || text == QStringLiteral("+ 添加班级"))
        return QString();
    return text;
}

QStringList TeacherMainWindow::classNames() const
{
    QStringList names;
    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    if (query.exec(QStringLiteral("SELECT class_name FROM classes ORDER BY class_name"))) {
        while (query.next())
            names << query.value(0).toString();
    }
    return names;
}

QString TeacherMainWindow::selectedStudentId() const
{
    const int row = ui->tableWidget_students->currentRow();
    if (row < 0)
        return QString();
    QTableWidgetItem *item = ui->tableWidget_students->item(row, 0);
    return item ? item->text().trimmed() : QString();
}

bool TeacherMainWindow::addClassInteractive()
{
    bool ok = false;
    const QString className = QInputDialog::getText(
        this, QStringLiteral("添加班级"),
        QStringLiteral("请输入班级名称（例如：人工智能2404）："),
        QLineEdit::Normal, QString(), &ok).trimmed();
    if (!ok || className.isEmpty())
        return false;

    if (className == QStringLiteral("全部学生") || className == QStringLiteral("未分班")
        || className == QStringLiteral("+ 添加班级")) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("该名称为系统保留名称"));
        return false;
    }
    if (className.length() > 50) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("班级名称不能超过 50 个字符"));
        return false;
    }
    if (classNames().contains(className)) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("该班级已存在"));
        return false;
    }

    int gradeYear = QDate::currentDate().year();
    static const QRegularExpression yearRe(QStringLiteral("(20\\d{2})"));
    const QRegularExpressionMatch match = yearRe.match(className);
    if (match.hasMatch())
        gradeYear = match.captured(1).toInt();

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(QStringLiteral(
        "INSERT INTO classes (class_name, grade_year) VALUES (?, ?)"));
    query.addBindValue(className);
    query.addBindValue(gradeYear);
    if (!query.exec()) {
        QMessageBox::critical(this, QStringLiteral("添加失败"), query.lastError().text());
        return false;
    }

    refreshClassList();
    loadClassStudents(className);
    statusBar()->showMessage(QStringLiteral("班级 %1 添加成功").arg(className));
    return true;
}

void TeacherMainWindow::renameClass(const QString &oldName)
{
    if (oldName.isEmpty() || oldName == QStringLiteral("全部学生")
        || oldName == QStringLiteral("未分班") || oldName == QStringLiteral("+ 添加班级")) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("该条目不可重命名"));
        return;
    }

    bool ok = false;
    const QString newName = QInputDialog::getText(
        this, QStringLiteral("重命名班级"),
        QStringLiteral("请输入新的班级名称："),
        QLineEdit::Normal, oldName, &ok).trimmed();
    if (!ok || newName.isEmpty() || newName == oldName)
        return;

    if (newName == QStringLiteral("全部学生") || newName == QStringLiteral("未分班")
        || newName == QStringLiteral("+ 添加班级")) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("该名称为系统保留名称"));
        return;
    }
    if (classNames().contains(newName)) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("该班级名称已存在"));
        return;
    }

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(QStringLiteral("UPDATE classes SET class_name = ? WHERE class_name = ?"));
    query.addBindValue(newName);
    query.addBindValue(oldName);
    if (!query.exec() || query.numRowsAffected() <= 0) {
        QMessageBox::critical(this, QStringLiteral("重命名失败"), query.lastError().text());
        return;
    }

    refreshClassList();
    loadClassStudents(newName);
    statusBar()->showMessage(QStringLiteral("班级已重命名为 %1").arg(newName));
}

void TeacherMainWindow::createStudentAccountForSelected()
{
    const QString studentId = selectedStudentId();
    if (studentId.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("请先在学生列表中选择一名学生"));
        return;
    }

    QString username;
    QString initialPassword;
    DatabaseManager *manager = DatabaseManager::instance();
    if (!manager->createStudentAccount(studentId, username, initialPassword)) {
        QMessageBox::warning(this, QStringLiteral("开通失败"), manager->lastError());
        return;
    }

    QMessageBox::information(
        this, QStringLiteral("账号已开通"),
        QStringLiteral("学生 %1 的登录账号已创建：\n\n登录名：%2\n初始密码：%3\n\n"
                       "首次登录时系统会强制要求修改密码。")
            .arg(studentId, username, initialPassword));
}

void TeacherMainWindow::deleteClass(const QString &className)
{
    if (className.isEmpty() || className == QStringLiteral("全部学生")
        || className == QStringLiteral("未分班") || className == QStringLiteral("+ 添加班级")) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("该条目不可删除"));
        return;
    }

    int studentCount = 0;
    QSqlQuery countQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    countQuery.prepare(QStringLiteral(
        "SELECT COUNT(*) FROM students WHERE class_name = ?"));
    countQuery.addBindValue(className);
    if (countQuery.exec() && countQuery.next())
        studentCount = countQuery.value(0).toInt();

    QMessageBox box(QMessageBox::Question, QStringLiteral("确认删除"),
                    QStringLiteral("确定要删除班级【%1】吗？").arg(className),
                    QMessageBox::Yes | QMessageBox::No, this);
    box.setInformativeText(QStringLiteral(
        "班级中的 %1 名学生不会被删除，将变为“未分班”。").arg(studentCount));
    if (box.exec() != QMessageBox::Yes)
        return;

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(QStringLiteral("DELETE FROM classes WHERE class_name = ?"));
    query.addBindValue(className);
    if (!query.exec()) {
        QMessageBox::critical(this, QStringLiteral("删除失败"), query.lastError().text());
        return;
    }

    refreshClassList();
    m_currentPage = 1;
    refreshStudentList();
    statusBar()->showMessage(QStringLiteral("班级 %1 已删除").arg(className));
}

// 核心：更新分页显示
void TeacherMainWindow::updatePagination(int currentPage, int totalPages)
{
    m_currentPage = currentPage;
    m_totalPages = totalPages;

    // 1. 更新页码标签
    ui->label_pageInfo->setText(QString("第%1页/共%2页").arg(currentPage).arg(totalPages));

    // 2. 控制上下页按钮状态
    ui->pushButton_prev->setEnabled(currentPage > 1);
    ui->pushButton_next->setEnabled(currentPage < totalPages);

    // 3. 动态设置页码按钮
    if (totalPages <= 5) {
        // 总页数≤5：显示所有页码
        for (int i = 0; i < 5; i++) {
            if (i < totalPages) {
                m_pageButtons[i]->setText(QString::number(i + 1));
                m_pageButtons[i]->setVisible(true);
                m_pageButtons[i]->setEnabled((i + 1) != currentPage);
            } else {
                m_pageButtons[i]->setVisible(false);
            }
        }
        ui->pushButton_ellipsis->setVisible(false);
    } else {
        // 总页数>5：智能显示
        int startPage = 1;
        int endPage = 5;

        if (currentPage > 3) {
            startPage = currentPage - 2;
            endPage = currentPage + 2;

            if (endPage > totalPages) {
                endPage = totalPages;
                startPage = totalPages - 4;
            }
        }

        // 设置显示的页码
        for (int i = 0; i < 5; i++) {
            int pageNum = startPage + i;
            m_pageButtons[i]->setText(QString::number(pageNum));
            m_pageButtons[i]->setVisible(true);
            m_pageButtons[i]->setEnabled(pageNum != currentPage);
        }

        // 显示/隐藏"..."按钮
        ui->pushButton_ellipsis->setVisible(endPage < totalPages);
    }
}

// 加载页面数据
void TeacherMainWindow::loadPage(int page)
{
    if (page < 1 || page > m_totalPages) {
        return;
    }

    m_currentPage = page;
    refreshStudentList();

    qDebug() << "加载第" << page << "页数据";
}

namespace {

// 构造学生列表的统一筛选条件（班级 + 关键词），供计数与分页查询复用
QString buildStudentWhere(const QString &className, const QString &keyword,
                          const QString &field, QVariantList &params)
{
    QStringList conditions;
    if (!className.isEmpty()) {
        if (className == QStringLiteral("未分班")) {
            conditions << QStringLiteral("class_name IS NULL");
        } else {
            conditions << QStringLiteral("class_name = ?");
            params << className;
        }
    }

    const QString kw = keyword.trimmed();
    if (!kw.isEmpty()) {
        const QString like = QStringLiteral("%") + kw + QStringLiteral("%");
        QString column;
        if (field == QStringLiteral("学号"))
            column = QStringLiteral("student_id");
        else if (field == QStringLiteral("姓名"))
            column = QStringLiteral("name");
        else if (field == QStringLiteral("性别"))
            column = QStringLiteral("gender");
        else if (field == QStringLiteral("年龄"))
            column = QStringLiteral("CAST(age AS TEXT)");
        else if (field == QStringLiteral("班级"))
            column = QStringLiteral("COALESCE(class_name,'')");
        else if (field == QStringLiteral("电话"))
            column = QStringLiteral("COALESCE(phone,'')");
        else if (field == QStringLiteral("地址"))
            column = QStringLiteral("COALESCE(address,'')");

        if (!column.isEmpty()) {
            conditions << QStringLiteral("(%1 LIKE ?)").arg(column);
            params << like;
        } else {
            conditions << QStringLiteral(
                "(student_id LIKE ? OR name LIKE ? OR COALESCE(class_name,'') LIKE ? "
                "OR COALESCE(phone,'') LIKE ? OR COALESCE(address,'') LIKE ?)");
            for (int i = 0; i < 5; ++i)
                params << like;
        }
    }

    return conditions.isEmpty() ? QString()
                                : QStringLiteral(" WHERE ") + conditions.join(QStringLiteral(" AND "));
}

} // namespace

// 刷新学生列表
void TeacherMainWindow::refreshStudentList()
{
    int totalCount = getTotalStudentCount();
    const int pageSize = 15;
    m_totalPages = (totalCount + pageSize - 1) / pageSize;

    if (m_currentPage > m_totalPages && m_totalPages > 0)
        m_currentPage = m_totalPages;
    else if (m_totalPages == 0)
        m_currentPage = 1;

    const QList<QMap<QString, QString>> students = getStudentsByPage(m_currentPage, pageSize);
    loadDataToTable(students);
    updatePagination(m_currentPage, m_totalPages);

    const QString filter = currentClassFilter();
    QString message = QStringLiteral("共 %1 名学生").arg(totalCount);
    if (!filter.isEmpty())
        message += QStringLiteral("（班级：%1）").arg(filter);
    if (!m_searchKeyword.isEmpty())
        message += QStringLiteral("，搜索：%1").arg(m_searchKeyword);
    statusBar()->showMessage(message);
}

// 获取学生总数（含班级与关键词筛选）
int TeacherMainWindow::getTotalStudentCount()
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen())
        return 0;

    QVariantList params;
    const QString where = buildStudentWhere(currentClassFilter(), m_searchKeyword,
                                            ui->comboBox->currentText(), params);

    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT COUNT(*) FROM students") + where);
    for (const QVariant &param : params)
        query.addBindValue(param);

    if (query.exec() && query.next())
        return query.value(0).toInt();

    qDebug() << "查询学生总数失败：" << query.lastError().text();
    return 0;
}

// 分页获取学生数据（含班级与关键词筛选）
QList<QMap<QString, QString>> TeacherMainWindow::getStudentsByPage(int page, int pageSize)
{
    QList<QMap<QString, QString>> studentList;
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen())
        return studentList;

    QVariantList params;
    const QString where = buildStudentWhere(currentClassFilter(), m_searchKeyword,
                                            ui->comboBox->currentText(), params);
    const int offset = (page - 1) * pageSize;

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
                      "SELECT student_id, name, gender, age, class_name, phone, address, created_time "
                      "FROM students")
                  + where + QStringLiteral(" ORDER BY student_id LIMIT ? OFFSET ?"));
    for (const QVariant &param : params)
        query.addBindValue(param);
    query.addBindValue(pageSize);
    query.addBindValue(offset);

    if (query.exec()) {
        while (query.next()) {
            QMap<QString, QString> student;
            student["学号"] = query.value("student_id").toString();
            student["姓名"] = query.value("name").toString();
            student["性别"] = query.value("gender").toString();
            student["年龄"] = query.value("age").toString();
            student["班级"] = query.value("class_name").toString();
            student["电话"] = query.value("phone").toString();
            student["地址"] = query.value("address").toString();
            student["创建时间"] = query.value("created_time").toString();
            studentList.append(student);
        }
    } else {
        qDebug() << "分页查询学生失败：" << query.lastError().text();
    }

    return studentList;
}

// 添加学生到数据库（重复学号由 UNIQUE 约束兜底）
bool TeacherMainWindow::addStudent(const QString &studentId, const QString &name,
                                   const QString &gender, int age, const QString &className,
                                   const QString &phone, const QString &address)
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("数据库连接失败"));
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "INSERT INTO students (student_id, name, gender, age, class_name, phone, address) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)"));
    query.addBindValue(studentId.trimmed());
    query.addBindValue(name.trimmed());
    query.addBindValue(gender);
    query.addBindValue(age);
    query.addBindValue(className.isEmpty() ? QVariant() : QVariant(className));
    query.addBindValue(phone);
    query.addBindValue(address);

    if (!query.exec()) {
        const QString error = query.lastError().text();
        QMessageBox::warning(this, QStringLiteral("添加失败"),
                             error.contains(QStringLiteral("UNIQUE"), Qt::CaseInsensitive)
                                 ? QStringLiteral("学号 %1 已存在").arg(studentId)
                                 : error);
        return false;
    }
    return true;
}

// 更新学生信息（学号由界面锁定，不在这里修改）
bool TeacherMainWindow::updateStudent(const QString &oldStudentId, const QString &studentId,
                                      const QString &name, const QString &gender, int age,
                                      const QString &className, const QString &phone,
                                      const QString &address)
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("数据库连接失败"));
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "UPDATE students SET name = ?, gender = ?, age = ?, class_name = ?, "
        "phone = ?, address = ? WHERE student_id = ?"));
    query.addBindValue(name.trimmed());
    query.addBindValue(gender);
    query.addBindValue(age);
    query.addBindValue(className.isEmpty() ? QVariant() : QVariant(className));
    query.addBindValue(phone);
    query.addBindValue(address);
    query.addBindValue(oldStudentId);

    if (!query.exec() || query.numRowsAffected() <= 0) {
        QMessageBox::warning(this, QStringLiteral("更新失败"), query.lastError().text());
        return false;
    }

    // 学号是成绩表的外键，界面中保持只读；如未来允许修改，
    // 数据库的 ON UPDATE CASCADE 会自动同步 grades.student_id。
    Q_UNUSED(studentId);
    return true;
}

bool TeacherMainWindow::deleteStudent(const QString &studentId)
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen())
        return false;

    if (!db.transaction()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
                              QStringLiteral("无法开始事务：%1").arg(db.lastError().text()));
        return false;
    }

    // 先注销学生账号（若有），再由外键级联删除成绩
    QSqlQuery accountQuery(db);
    accountQuery.prepare(QStringLiteral(
        "DELETE FROM users WHERE user_id = "
        "(SELECT user_id FROM students WHERE student_id = ?) AND user_type = 'student'"));
    accountQuery.addBindValue(studentId);
    if (!accountQuery.exec()) {
        db.rollback();
        QMessageBox::critical(this, QStringLiteral("删除失败"), accountQuery.lastError().text());
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM students WHERE student_id = ?"));
    query.addBindValue(studentId);
    if (!query.exec() || query.numRowsAffected() <= 0) {
        db.rollback();
        QMessageBox::critical(this, QStringLiteral("删除失败"), query.lastError().text());
        return false;
    }

    if (!db.commit()) {
        const QString error = db.lastError().text();
        db.rollback();
        QMessageBox::critical(this, QStringLiteral("删除失败"), error);
        return false;
    }
    return true;
}

// 加载数据到表格
void TeacherMainWindow::loadDataToTable(const QList<QMap<QString, QString>> &studentList)
{
    ui->tableWidget_students->clearContents();
    ui->tableWidget_students->setRowCount(studentList.size());

    for (int row = 0; row < studentList.size(); row++) {
        const QMap<QString, QString> &student = studentList[row];

        ui->tableWidget_students->setItem(row, 0, new QTableWidgetItem(student["学号"]));
        ui->tableWidget_students->setItem(row, 1, new QTableWidgetItem(student["姓名"]));
        ui->tableWidget_students->setItem(row, 2, new QTableWidgetItem(student["性别"]));
        ui->tableWidget_students->setItem(row, 3, new QTableWidgetItem(student["年龄"]));
        ui->tableWidget_students->setItem(row, 4, new QTableWidgetItem(student["班级"]));
        ui->tableWidget_students->setItem(row, 5, new QTableWidgetItem(student["电话"]));
        ui->tableWidget_students->setItem(row, 6, new QTableWidgetItem(student["地址"]));
        ui->tableWidget_students->setItem(row, 7, new QTableWidgetItem(student["创建时间"]));
    }

    // 调整列宽
    ui->tableWidget_students->resizeColumnsToContents();
}

// 选中指定班级并刷新（班级视图与全部学生共用分页逻辑）
void TeacherMainWindow::loadClassStudents(const QString &className)
{
    for (int i = 0; i < ui->listWidget_classes->count(); ++i) {
        if (ui->listWidget_classes->item(i)->text() == className) {
            ui->listWidget_classes->setCurrentRow(i);
            break;
        }
    }
    m_currentPage = 1;
    refreshStudentList();
}


void TeacherMainWindow::manageSubjects()
{
    QDialog dialog(this);
    dialog.setWindowTitle(QStringLiteral("课程管理"));
    dialog.resize(680, 430);

    auto *layout = new QVBoxLayout(&dialog);
    auto *hint = new QLabel(QStringLiteral("仅显示当前教师负责的课程"), &dialog);
    hint->setStyleSheet(QStringLiteral("color:#6b7280;"));
    layout->addWidget(hint);

    auto *table = new QTableWidget(&dialog);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QStringList()
        << QStringLiteral("课程编号") << QStringLiteral("课程名称")
        << QStringLiteral("学分") << QStringLiteral("创建时间"));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);

    auto load = [&]() {
        table->setRowCount(0);
        QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
        query.prepare(QStringLiteral(
            "SELECT subject_id, subject_name, credit, created_time "
            "FROM subjects WHERE teacher_id = ? ORDER BY subject_id"));
        query.addBindValue(m_teacherId);
        if (!query.exec()) {
            QMessageBox::critical(&dialog, QStringLiteral("查询失败"),
                                  query.lastError().text());
            return;
        }
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            table->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            table->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            table->setItem(row, 2, new QTableWidgetItem(
                QString::number(query.value(2).toDouble(), 'f', 1)));
            table->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            ++row;
        }
    };

    auto *buttonLayout = new QHBoxLayout();
    auto *addButton = new QPushButton(QStringLiteral("添加课程"), &dialog);
    auto *editButton = new QPushButton(QStringLiteral("编辑课程"), &dialog);
    auto *deleteButton = new QPushButton(QStringLiteral("删除课程"), &dialog);
    auto *closeButton = new QPushButton(QStringLiteral("关闭"), &dialog);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, [&]() {
        SubjectDialog subjectDialog(m_teacherId, SubjectDialog::AddMode, &dialog);
        if (subjectDialog.exec() == QDialog::Accepted)
            load();
    });

    connect(editButton, &QPushButton::clicked, [&]() {
        const int row = table->currentRow();
        if (row < 0) {
            QMessageBox::warning(&dialog, QStringLiteral("警告"),
                                 QStringLiteral("请先选择一门课程"));
            return;
        }
        const QString subjectId = table->item(row, 0)->text();
        SubjectDialog subjectDialog(m_teacherId, subjectId, &dialog);
        if (subjectDialog.exec() == QDialog::Accepted)
            load();
    });

    connect(deleteButton, &QPushButton::clicked, [&]() {
        const int row = table->currentRow();
        if (row < 0) {
            QMessageBox::warning(&dialog, QStringLiteral("警告"),
                                 QStringLiteral("请先选择一门课程"));
            return;
        }
        const QString subjectId = table->item(row, 0)->text();
        const QString subjectName = table->item(row, 1)->text();

        QSqlQuery countQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
        countQuery.prepare(QStringLiteral("SELECT COUNT(*) FROM grades WHERE subject_id = ?"));
        countQuery.addBindValue(subjectId);
        int gradeCount = 0;
        if (countQuery.exec() && countQuery.next())
            gradeCount = countQuery.value(0).toInt();

        QMessageBox box(QMessageBox::Question, QStringLiteral("确认删除"),
                        QStringLiteral("确定要删除课程【%1 - %2】吗？").arg(subjectId, subjectName),
                        QMessageBox::Yes | QMessageBox::No, &dialog);
        box.setInformativeText(QStringLiteral(
            "该课程下已有 %1 条成绩记录，删除课程会一并删除这些成绩，且不可撤销。")
                                   .arg(gradeCount));
        if (box.exec() != QMessageBox::Yes)
            return;

        QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
        if (!db.transaction()) {
            QMessageBox::critical(&dialog, QStringLiteral("错误"),
                                  QStringLiteral("无法开始事务"));
            return;
        }
        QSqlQuery deleteGrades(db);
        deleteGrades.prepare(QStringLiteral("DELETE FROM grades WHERE subject_id = ?"));
        deleteGrades.addBindValue(subjectId);
        if (!deleteGrades.exec()) {
            db.rollback();
            QMessageBox::critical(&dialog, QStringLiteral("删除失败"),
                                  deleteGrades.lastError().text());
            return;
        }
        QSqlQuery deleteSubject(db);
        deleteSubject.prepare(QStringLiteral(
            "DELETE FROM subjects WHERE subject_id = ? AND teacher_id = ?"));
        deleteSubject.addBindValue(subjectId);
        deleteSubject.addBindValue(m_teacherId);
        if (!deleteSubject.exec() || deleteSubject.numRowsAffected() <= 0) {
            db.rollback();
            QMessageBox::critical(&dialog, QStringLiteral("删除失败"),
                                  deleteSubject.lastError().text());
            return;
        }
        if (!db.commit()) {
            db.rollback();
            QMessageBox::critical(&dialog, QStringLiteral("删除失败"), db.lastError().text());
            return;
        }
        load();
        QMessageBox::information(&dialog, QStringLiteral("成功"),
                                 QStringLiteral("课程删除成功"));
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    load();
    dialog.exec();
}

void TeacherMainWindow::addSubject()
{
    SubjectDialog dialog(m_teacherId, SubjectDialog::AddMode, this);
    if (dialog.exec() == QDialog::Accepted)
        statusBar()->showMessage(QStringLiteral("课程添加成功"));
}

void TeacherMainWindow::editSubject()
{
    manageSubjects();
}

void TeacherMainWindow::deleteSubject()
{
    manageSubjects();
}

void TeacherMainWindow::inputGrade()
{
    GradeDialog dialog(m_teacherId, this);
    dialog.exec();
}

void TeacherMainWindow::modifyGrade()
{
    QDialog dialog(this);
    dialog.setWindowTitle(QStringLiteral("成绩管理"));
    dialog.resize(940, 580);

    auto *layout = new QVBoxLayout(&dialog);
    auto *filterLayout = new QHBoxLayout();
    auto *classCombo = new QComboBox(&dialog);
    auto *subjectCombo = new QComboBox(&dialog);
    auto *semesterCombo = new QComboBox(&dialog);
    auto *filterButton = new QPushButton(QStringLiteral("筛选"), &dialog);

    classCombo->addItem(QStringLiteral("全部班级"), QString());
    subjectCombo->addItem(QStringLiteral("全部课程"), QString());
    semesterCombo->addItem(QStringLiteral("全部学期"), QString());

    QSqlQuery classQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    if (classQuery.exec(QStringLiteral("SELECT class_name FROM classes ORDER BY class_name"))) {
        while (classQuery.next())
            classCombo->addItem(classQuery.value(0).toString(), classQuery.value(0).toString());
    }

    QSqlQuery subjectQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    subjectQuery.prepare(QStringLiteral(
        "SELECT subject_id, subject_name FROM subjects WHERE teacher_id = ? ORDER BY subject_id"));
    subjectQuery.addBindValue(m_teacherId);
    if (subjectQuery.exec()) {
        while (subjectQuery.next()) {
            subjectCombo->addItem(subjectQuery.value(1).toString(),
                                  subjectQuery.value(0).toString());
        }
    }

    QSqlQuery semesterQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    semesterQuery.prepare(QStringLiteral(
        "SELECT DISTINCT g.semester FROM grades g "
        "JOIN subjects sub ON sub.subject_id = g.subject_id "
        "WHERE sub.teacher_id = ? ORDER BY g.semester DESC"));
    semesterQuery.addBindValue(m_teacherId);
    if (semesterQuery.exec()) {
        while (semesterQuery.next())
            semesterCombo->addItem(semesterQuery.value(0).toString(),
                                   semesterQuery.value(0).toString());
    }

    filterLayout->addWidget(new QLabel(QStringLiteral("班级："), &dialog));
    filterLayout->addWidget(classCombo);
    filterLayout->addWidget(new QLabel(QStringLiteral("课程："), &dialog));
    filterLayout->addWidget(subjectCombo);
    filterLayout->addWidget(new QLabel(QStringLiteral("学期："), &dialog));
    filterLayout->addWidget(semesterCombo);
    filterLayout->addWidget(filterButton);
    filterLayout->addStretch();
    layout->addLayout(filterLayout);

    auto *table = new QTableWidget(&dialog);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(QStringList()
        << QStringLiteral("学号") << QStringLiteral("姓名") << QStringLiteral("班级")
        << QStringLiteral("课程") << QStringLiteral("学期") << QStringLiteral("成绩")
        << QStringLiteral("考试日期"));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);

    auto loadGrades = [&]() {
        table->setRowCount(0);
        QString sql = QStringLiteral(
            "SELECT g.student_id, s.name, COALESCE(s.class_name,'未分班'), "
            "sub.subject_name, g.semester, g.score, COALESCE(g.exam_date,''), g.subject_id "
            "FROM grades g "
            "JOIN students s ON s.student_id = g.student_id "
            "JOIN subjects sub ON sub.subject_id = g.subject_id "
            "WHERE sub.teacher_id = ?");
        QVariantList params;
        params << m_teacherId;

        if (!classCombo->currentData().toString().isEmpty()) {
            sql += QStringLiteral(" AND s.class_name = ?");
            params << classCombo->currentData().toString();
        }
        if (!subjectCombo->currentData().toString().isEmpty()) {
            sql += QStringLiteral(" AND g.subject_id = ?");
            params << subjectCombo->currentData().toString();
        }
        if (!semesterCombo->currentData().toString().isEmpty()) {
            sql += QStringLiteral(" AND g.semester = ?");
            params << semesterCombo->currentData().toString();
        }
        sql += QStringLiteral(" ORDER BY g.semester DESC, s.student_id, g.subject_id LIMIT 1000");

        QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
        query.prepare(sql);
        for (const QVariant &param : params)
            query.addBindValue(param);
        if (!query.exec()) {
            QMessageBox::critical(&dialog, QStringLiteral("查询失败"), query.lastError().text());
            return;
        }

        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            for (int col = 0; col < 7; ++col) {
                QString text = query.value(col).toString();
                if (col == 5)
                    text = QString::number(query.value(5).toDouble(), 'f', 1);
                auto *item = new QTableWidgetItem(text);
                if (col == 5) {
                    const double score = query.value(5).toDouble();
                    if (score < 60)
                        item->setForeground(QBrush(QColor(QStringLiteral("#dc2626"))));
                    item->setTextAlignment(Qt::AlignCenter);
                }
                table->setItem(row, col, item);
            }
            // 隐藏列数据：课程 ID，便于精确编辑；学期已在第 4 列
            table->item(row, 3)->setData(Qt::UserRole, query.value(7).toString());
            ++row;
        }
    };

    auto *buttonLayout = new QHBoxLayout();
    auto *editButton = new QPushButton(QStringLiteral("编辑选中成绩"), &dialog);
    auto *deleteButton = new QPushButton(QStringLiteral("删除选中成绩"), &dialog);
    auto *closeButton = new QPushButton(QStringLiteral("关闭"), &dialog);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);

    connect(filterButton, &QPushButton::clicked, &dialog, loadGrades);
    connect(classCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), &dialog, [&](int) { loadGrades(); });
    connect(subjectCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), &dialog, [&](int) { loadGrades(); });
    connect(semesterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), &dialog, [&](int) { loadGrades(); });

    connect(editButton, &QPushButton::clicked, &dialog, [&]() {
        const int row = table->currentRow();
        if (row < 0) {
            QMessageBox::warning(&dialog, QStringLiteral("警告"),
                                 QStringLiteral("请先选择一条成绩记录"));
            return;
        }
        const QString studentId = table->item(row, 0)->text();
        const QString subjectId = table->item(row, 3)->data(Qt::UserRole).toString();
        const QString semester = table->item(row, 4)->text();
        GradeDialog gradeDialog(m_teacherId, studentId, subjectId, semester, &dialog);
        if (gradeDialog.exec() == QDialog::Accepted)
            loadGrades();
    });

    connect(deleteButton, &QPushButton::clicked, &dialog, [&]() {
        const int row = table->currentRow();
        if (row < 0) {
            QMessageBox::warning(&dialog, QStringLiteral("警告"),
                                 QStringLiteral("请先选择一条成绩记录"));
            return;
        }
        const QString studentId = table->item(row, 0)->text();
        const QString studentName = table->item(row, 1)->text();
        const QString subjectId = table->item(row, 3)->data(Qt::UserRole).toString();
        const QString semester = table->item(row, 4)->text();

        if (QMessageBox::question(&dialog, QStringLiteral("确认删除"),
                                  QStringLiteral("确定删除【%1 - %2】的该条成绩吗？")
                                      .arg(studentId, studentName),
                                  QMessageBox::Yes | QMessageBox::No)
            != QMessageBox::Yes) {
            return;
        }

        QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
        query.prepare(QStringLiteral(
            "DELETE FROM grades WHERE student_id = ? AND subject_id = ? AND semester = ?"));
        query.addBindValue(studentId);
        query.addBindValue(subjectId);
        query.addBindValue(semester);
        if (!query.exec() || query.numRowsAffected() <= 0) {
            QMessageBox::critical(&dialog, QStringLiteral("删除失败"), query.lastError().text());
            return;
        }
        loadGrades();
        QMessageBox::information(&dialog, QStringLiteral("成功"), QStringLiteral("成绩删除成功"));
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    loadGrades();
    dialog.exec();
}

void TeacherMainWindow::viewGrade()
{
    QDialog dialog(this);
    dialog.setWindowTitle(QStringLiteral("成绩查询"));
    dialog.resize(980, 580);

    auto *layout = new QVBoxLayout(&dialog);
    auto *filterLayout = new QHBoxLayout();
    auto *semesterCombo = new QComboBox(&dialog);
    semesterCombo->addItem(QStringLiteral("全部学期"), QString());
    QSqlQuery semesterQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    semesterQuery.prepare(QStringLiteral(
        "SELECT DISTINCT g.semester FROM grades g "
        "JOIN subjects sub ON sub.subject_id = g.subject_id "
        "WHERE sub.teacher_id = ? ORDER BY g.semester DESC"));
    semesterQuery.addBindValue(m_teacherId);
    if (semesterQuery.exec()) {
        while (semesterQuery.next())
            semesterCombo->addItem(semesterQuery.value(0).toString(),
                                   semesterQuery.value(0).toString());
    }
    filterLayout->addWidget(new QLabel(QStringLiteral("学期："), &dialog));
    filterLayout->addWidget(semesterCombo);
    filterLayout->addStretch();
    layout->addLayout(filterLayout);

    auto *table = new QTableWidget(&dialog);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels(QStringList()
        << QStringLiteral("学期") << QStringLiteral("学号") << QStringLiteral("姓名")
        << QStringLiteral("班级") << QStringLiteral("课程") << QStringLiteral("成绩")
        << QStringLiteral("等级") << QStringLiteral("任课教师"));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);

    auto *hint = new QLabel(QStringLiteral("最多显示 1000 条记录，可按学期筛选"), &dialog);
    hint->setStyleSheet(QStringLiteral("color:#6b7280;"));
    layout->addWidget(hint);

    auto load = [&]() {
        table->setRowCount(0);
        QString sql = QStringLiteral(
            "SELECT semester, student_id, student_name, COALESCE(class_name,'未分班'), "
            "subject_name, score, grade_level, teacher_name "
            "FROM v_student_grades WHERE subject_id IN "
            "(SELECT subject_id FROM subjects WHERE teacher_id = ?)");
        QVariantList params;
        params << m_teacherId;
        if (!semesterCombo->currentData().toString().isEmpty()) {
            sql += QStringLiteral(" AND semester = ?");
            params << semesterCombo->currentData().toString();
        }
        sql += QStringLiteral(" ORDER BY semester DESC, class_name, student_id, subject_id LIMIT 1000");

        QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
        query.prepare(sql);
        for (const QVariant &param : params)
            query.addBindValue(param);
        if (!query.exec()) {
            QMessageBox::critical(&dialog, QStringLiteral("查询失败"), query.lastError().text());
            return;
        }
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            for (int col = 0; col < 8; ++col) {
                QString text = query.value(col).toString();
                if (col == 5)
                    text = QString::number(query.value(5).toDouble(), 'f', 1);
                auto *item = new QTableWidgetItem(text);
                if (col == 5) {
                    const double score = query.value(5).toDouble();
                    if (score < 60)
                        item->setForeground(QBrush(QColor(QStringLiteral("#dc2626"))));
                    item->setTextAlignment(Qt::AlignCenter);
                }
                table->setItem(row, col, item);
            }
            ++row;
        }
    };

    auto *buttonLayout = new QHBoxLayout();
    auto *exportButton = new QPushButton(QStringLiteral("导出 CSV"), &dialog);
    auto *closeButton = new QPushButton(QStringLiteral("关闭"), &dialog);
    buttonLayout->addWidget(exportButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);

    connect(semesterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), &dialog, [&](int) { load(); });
    connect(exportButton, &QPushButton::clicked, &dialog, [&]() {
        exportTableToCsv(table, QStringLiteral("成绩查询"));
    });
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    load();
    dialog.exec();
}

void TeacherMainWindow::exportTableToCsv(QTableWidget *table, const QString &defaultName)
{
    if (!table || table->rowCount() == 0) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("没有可导出的数据"));
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this, QStringLiteral("导出 CSV"),
        QDir::homePath() + QStringLiteral("/") + defaultName + QStringLiteral(".csv"),
        QStringLiteral("CSV 文件 (*.csv)"));
    if (filePath.isEmpty())
        return;
    if (!filePath.endsWith(QStringLiteral(".csv"), Qt::CaseInsensitive))
        filePath += QStringLiteral(".csv");

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QStringLiteral("导出失败"), file.errorString());
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << QChar(0xFEFF); // UTF-8 BOM，Excel 打开不乱码

    auto csvField = [](QString value) {
        value.replace(QLatin1Char('"'), QStringLiteral("\"\""));
        return QStringLiteral("\"") + value + QStringLiteral("\"");
    };

    QStringList headers;
    for (int col = 0; col < table->columnCount(); ++col) {
        QTableWidgetItem *item = table->horizontalHeaderItem(col);
        headers << csvField(item ? item->text() : QString::number(col + 1));
    }
    out << headers.join(QLatin1Char(',')) << '\n';

    for (int row = 0; row < table->rowCount(); ++row) {
        QStringList fields;
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            fields << csvField(item ? item->text() : QString());
        }
        out << fields.join(QLatin1Char(',')) << '\n';
    }
    out.flush();
    file.close();

    QMessageBox::information(this, QStringLiteral("导出成功"),
                             QStringLiteral("已导出到：\n%1").arg(filePath));
}

void TeacherMainWindow::printTable(QTableWidget *table, const QString &title)
{
    if (!table || table->rowCount() == 0) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("没有可打印的数据"));
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(QStringLiteral("打印报表"));
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString html = QStringLiteral("<h2>%1</h2>").arg(Validation::escapeHtml(title));
    html += QStringLiteral("<table border='1' cellspacing='0' cellpadding='4' width='100%'><tr>");
    for (int col = 0; col < table->columnCount(); ++col) {
        QTableWidgetItem *item = table->horizontalHeaderItem(col);
        html += QStringLiteral("<th>%1</th>")
                    .arg(Validation::escapeHtml(item ? item->text() : QString()));
    }
    html += QStringLiteral("</tr>");
    for (int row = 0; row < table->rowCount(); ++row) {
        html += QStringLiteral("<tr>");
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            html += QStringLiteral("<td>%1</td>")
                        .arg(Validation::escapeHtml(item ? item->text() : QString()));
        }
        html += QStringLiteral("</tr>");
    }
    html += QStringLiteral("</table>");

    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
}

void TeacherMainWindow::showReportDialog(const QString &title, const QStringList &headers,
                                         const QList<QStringList> &rows, const QString &csvName,
                                         const QString &summary)
{
    QDialog dialog(this);
    dialog.setWindowTitle(title);
    dialog.resize(900, 580);

    auto *layout = new QVBoxLayout(&dialog);
    auto *titleLabel = new QLabel(QStringLiteral("<h2>%1</h2>").arg(Validation::escapeHtml(title)),
                                  &dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    if (!summary.isEmpty()) {
        auto *summaryLabel = new QLabel(summary, &dialog);
        summaryLabel->setAlignment(Qt::AlignCenter);
        summaryLabel->setStyleSheet(QStringLiteral("color:#475569;"));
        layout->addWidget(summaryLabel);
    }

    auto *table = new QTableWidget(&dialog);
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setRowCount(rows.size());
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);

    QList<int> rankColumns;
    QList<int> scoreColumns;
    for (int i = 0; i < headers.size(); ++i) {
        if (headers.at(i).contains(QStringLiteral("排名")))
            rankColumns << i;
        if (headers.at(i).contains(QStringLiteral("成绩")) || headers.at(i).contains(QStringLiteral("平均分")))
            scoreColumns << i;
    }

    for (int row = 0; row < rows.size(); ++row) {
        const QStringList &values = rows.at(row);
        for (int col = 0; col < headers.size() && col < values.size(); ++col) {
            auto *item = new QTableWidgetItem(values.at(col));
            item->setTextAlignment(Qt::AlignCenter);
            if (rankColumns.contains(col)) {
                const int rank = values.at(col).toInt();
                if (rank <= 3)
                    item->setForeground(QBrush(QColor(QStringLiteral("#dc2626"))));
                QFont font = item->font();
                font.setBold(rank <= 3);
                item->setFont(font);
            }
            if (scoreColumns.contains(col)) {
                const double score = values.at(col).toDouble();
                if (score < 60)
                    item->setForeground(QBrush(QColor(QStringLiteral("#dc2626"))));
                else if (score >= 90)
                    item->setForeground(QBrush(QColor(QStringLiteral("#15803d"))));
            }
            table->setItem(row, col, item);
        }
    }
    table->resizeColumnsToContents();
    layout->addWidget(table);

    auto *buttonLayout = new QHBoxLayout();
    auto *exportButton = new QPushButton(QStringLiteral("导出 CSV"), &dialog);
    auto *printButton = new QPushButton(QStringLiteral("打印"), &dialog);
    auto *closeButton = new QPushButton(QStringLiteral("关闭"), &dialog);
    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(printButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);

    connect(exportButton, &QPushButton::clicked, &dialog, [&]() {
        exportTableToCsv(table, csvName);
    });
    connect(printButton, &QPushButton::clicked, &dialog, [&]() {
        printTable(table, title);
    });
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void TeacherMainWindow::exportClassGPAReport()
{
    const QStringList classes = classNames();
    if (classes.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("没有班级数据"));
        return;
    }

    bool ok = false;
    const QString className = QInputDialog::getItem(
        this, QStringLiteral("选择班级"), QStringLiteral("请选择班级："),
        classes, 0, false, &ok);
    if (!ok || className.isEmpty())
        return;

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(QStringLiteral(
        "SELECT class_rank, student_id, name, weighted_avg, total_credit, gpa "
        "FROM v_student_overview WHERE class_name = ? AND subject_count > 0 "
        "ORDER BY gpa DESC, student_id"));
    query.addBindValue(className);
    if (!query.exec()) {
        QMessageBox::critical(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    QList<QStringList> rows;
    double gpaSum = 0.0;
    while (query.next()) {
        rows << QStringList{
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            QString::number(query.value(3).toDouble(), 'f', 2),
            QString::number(query.value(4).toDouble(), 'f', 1),
            QString::number(query.value(5).toDouble(), 'f', 3),
        };
        gpaSum += query.value(5).toDouble();
    }

    if (rows.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("该班级暂无成绩数据"));
        return;
    }

    const QString summary = QStringLiteral("班级：%1　人数：%2　平均 GPA：%3")
                                .arg(className)
                                .arg(rows.size())
                                .arg(gpaSum / rows.size(), 0, 'f', 3);
    showReportDialog(QStringLiteral("班级绩点排名 - %1").arg(className),
                     QStringList() << QStringLiteral("班级排名") << QStringLiteral("学号")
                                   << QStringLiteral("姓名") << QStringLiteral("加权平均分")
                                   << QStringLiteral("总学分") << QStringLiteral("GPA"),
                     rows, className + QStringLiteral("_绩点排名"), summary);
}

void TeacherMainWindow::exportAllStudentsGPAReport()
{
    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    if (!query.exec(QStringLiteral(
            "SELECT grade_rank, class_rank, student_id, name, "
            "COALESCE(class_name,'未分班'), weighted_avg, total_credit, gpa "
            "FROM v_student_overview WHERE subject_count > 0 "
            "ORDER BY gpa DESC, student_id"))) {
        QMessageBox::critical(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    QList<QStringList> rows;
    double gpaSum = 0.0;
    while (query.next()) {
        rows << QStringList{
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            QString::number(query.value(5).toDouble(), 'f', 2),
            QString::number(query.value(6).toDouble(), 'f', 1),
            QString::number(query.value(7).toDouble(), 'f', 3),
        };
        gpaSum += query.value(7).toDouble();
    }

    if (rows.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("暂无成绩数据"));
        return;
    }

    const QString summary = QStringLiteral("学生数：%1　平均 GPA：%2")
                                .arg(rows.size())
                                .arg(gpaSum / rows.size(), 0, 'f', 3);
    showReportDialog(QStringLiteral("所有学生绩点排名"),
                     QStringList() << QStringLiteral("年级排名") << QStringLiteral("班级排名")
                                   << QStringLiteral("学号") << QStringLiteral("姓名")
                                   << QStringLiteral("班级") << QStringLiteral("加权平均分")
                                   << QStringLiteral("总学分") << QStringLiteral("GPA"),
                     rows, QStringLiteral("所有学生绩点排名"), summary);
}

void TeacherMainWindow::exportSubjectClassReport()
{
    QSqlQuery subjectQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    subjectQuery.prepare(QStringLiteral(
        "SELECT subject_id, subject_name FROM subjects WHERE teacher_id = ? ORDER BY subject_id"));
    subjectQuery.addBindValue(m_teacherId);
    QStringList subjectItems;
    QMap<QString, QString> subjectMap;
    if (subjectQuery.exec()) {
        while (subjectQuery.next()) {
            const QString text = QStringLiteral("%1 - %2")
                                     .arg(subjectQuery.value(0).toString(),
                                          subjectQuery.value(1).toString());
            subjectItems << text;
            subjectMap[text] = subjectQuery.value(0).toString();
        }
    }
    if (subjectItems.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("当前教师还没有课程"));
        return;
    }

    bool ok = false;
    const QString subjectText = QInputDialog::getItem(
        this, QStringLiteral("选择课程"), QStringLiteral("请选择课程："),
        subjectItems, 0, false, &ok);
    if (!ok || subjectText.isEmpty())
        return;
    const QString subjectId = subjectMap.value(subjectText);

    const QStringList classes = classNames();
    if (classes.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("没有班级数据"));
        return;
    }
    const QString className = QInputDialog::getItem(
        this, QStringLiteral("选择班级"), QStringLiteral("请选择班级："),
        classes, 0, false, &ok);
    if (!ok || className.isEmpty())
        return;

    QStringList semesters;
    semesters << QStringLiteral("全部学期");
    QSqlQuery semesterQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    semesterQuery.prepare(QStringLiteral(
        "SELECT DISTINCT semester FROM grades WHERE subject_id = ? ORDER BY semester DESC"));
    semesterQuery.addBindValue(subjectId);
    if (semesterQuery.exec()) {
        while (semesterQuery.next())
            semesters << semesterQuery.value(0).toString();
    }
    const QString semesterChoice = QInputDialog::getItem(
        this, QStringLiteral("选择学期"), QStringLiteral("请选择学期："),
        semesters, 0, false, &ok);
    if (!ok)
        return;
    const QString semester = (semesterChoice == QStringLiteral("全部学期"))
                                 ? QString()
                                 : semesterChoice;

    QString sql = QStringLiteral(
        "SELECT RANK() OVER (ORDER BY score DESC) AS ranking, "
        "student_id, student_name, score, grade_level "
        "FROM v_student_grades WHERE subject_id = ? AND class_name = ?");
    QVariantList params;
    params << subjectId << className;
    if (!semester.isEmpty()) {
        sql += QStringLiteral(" AND semester = ?");
        params << semester;
    }
    sql += QStringLiteral(" ORDER BY score DESC, student_id");

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(sql);
    for (const QVariant &param : params)
        query.addBindValue(param);
    if (!query.exec()) {
        QMessageBox::critical(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    QList<QStringList> rows;
    double sum = 0.0;
    double maxScore = 0.0;
    double minScore = 100.0;
    int passCount = 0;
    while (query.next()) {
        const double score = query.value(3).toDouble();
        rows << QStringList{query.value(0).toString(), query.value(1).toString(),
                            query.value(2).toString(),
                            QString::number(score, 'f', 1), query.value(4).toString()};
        sum += score;
        maxScore = qMax(maxScore, score);
        minScore = qMin(minScore, score);
        if (score >= 60)
            ++passCount;
    }

    if (rows.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("该条件下没有成绩记录"));
        return;
    }

    const double avg = sum / rows.size();
    const QString summary = QStringLiteral(
        "课程：%1　班级：%2　学期：%3　人数：%4　平均分：%5　最高：%6　最低：%7　及格率：%8%")
                                .arg(subjectText, className,
                                     semester.isEmpty() ? QStringLiteral("全部") : semester)
                                .arg(rows.size())
                                .arg(avg, 0, 'f', 2)
                                .arg(maxScore, 0, 'f', 1)
                                .arg(minScore, 0, 'f', 1)
                                .arg(100.0 * passCount / rows.size(), 0, 'f', 1);

    showReportDialog(QStringLiteral("%1 - %2 成绩排名").arg(className, subjectText),
                     QStringList() << QStringLiteral("排名") << QStringLiteral("学号")
                                   << QStringLiteral("姓名") << QStringLiteral("成绩")
                                   << QStringLiteral("等级"),
                     rows, QStringLiteral("%1_%2_成绩排名").arg(className, subjectText), summary);
}

void TeacherMainWindow::exportSubjectAllReport()
{
    QSqlQuery subjectQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    subjectQuery.prepare(QStringLiteral(
        "SELECT subject_id, subject_name FROM subjects WHERE teacher_id = ? ORDER BY subject_id"));
    subjectQuery.addBindValue(m_teacherId);
    QStringList subjectItems;
    QMap<QString, QString> subjectMap;
    if (subjectQuery.exec()) {
        while (subjectQuery.next()) {
            const QString text = QStringLiteral("%1 - %2")
                                     .arg(subjectQuery.value(0).toString(),
                                          subjectQuery.value(1).toString());
            subjectItems << text;
            subjectMap[text] = subjectQuery.value(0).toString();
        }
    }
    if (subjectItems.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("当前教师还没有课程"));
        return;
    }

    bool ok = false;
    const QString subjectText = QInputDialog::getItem(
        this, QStringLiteral("选择课程"), QStringLiteral("请选择课程："),
        subjectItems, 0, false, &ok);
    if (!ok || subjectText.isEmpty())
        return;
    const QString subjectId = subjectMap.value(subjectText);

    QStringList semesters;
    semesters << QStringLiteral("全部学期");
    QSqlQuery semesterQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    semesterQuery.prepare(QStringLiteral(
        "SELECT DISTINCT semester FROM grades WHERE subject_id = ? ORDER BY semester DESC"));
    semesterQuery.addBindValue(subjectId);
    if (semesterQuery.exec()) {
        while (semesterQuery.next())
            semesters << semesterQuery.value(0).toString();
    }
    const QString semesterChoice = QInputDialog::getItem(
        this, QStringLiteral("选择学期"), QStringLiteral("请选择学期："),
        semesters, 0, false, &ok);
    if (!ok)
        return;
    const QString semester = (semesterChoice == QStringLiteral("全部学期"))
                                 ? QString()
                                 : semesterChoice;

    QString sql = QStringLiteral(
        "SELECT RANK() OVER (ORDER BY score DESC) AS ranking, "
        "student_id, student_name, COALESCE(class_name,'未分班'), score, grade_level "
        "FROM v_student_grades WHERE subject_id = ?");
    QVariantList params;
    params << subjectId;
    if (!semester.isEmpty()) {
        sql += QStringLiteral(" AND semester = ?");
        params << semester;
    }
    sql += QStringLiteral(" ORDER BY score DESC, class_name, student_id");

    QSqlQuery query(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    query.prepare(sql);
    for (const QVariant &param : params)
        query.addBindValue(param);
    if (!query.exec()) {
        QMessageBox::critical(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    QList<QStringList> rows;
    double sum = 0.0;
    while (query.next()) {
        const double score = query.value(4).toDouble();
        rows << QStringList{query.value(0).toString(), query.value(1).toString(),
                            query.value(2).toString(), query.value(3).toString(),
                            QString::number(score, 'f', 1), query.value(5).toString()};
        sum += score;
    }

    if (rows.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("该课程暂无成绩记录"));
        return;
    }

    const QString summary = QStringLiteral("课程：%1　学期：%2　人数：%3　平均分：%4")
                                .arg(subjectText,
                                     semester.isEmpty() ? QStringLiteral("全部") : semester)
                                .arg(rows.size())
                                .arg(sum / rows.size(), 0, 'f', 2);
    showReportDialog(QStringLiteral("%1 所有学生成绩排名").arg(subjectText),
                     QStringList() << QStringLiteral("排名") << QStringLiteral("学号")
                                   << QStringLiteral("姓名") << QStringLiteral("班级")
                                   << QStringLiteral("成绩") << QStringLiteral("等级"),
                     rows, subjectText + QStringLiteral("_所有学生成绩排名"), summary);
}

double TeacherMainWindow::calculateGPA(double score)
{
    // 与 v_student_overview 中的绩点映射保持一致
    if (score >= 90) return 4.0;
    if (score >= 85) return 3.7;
    if (score >= 82) return 3.3;
    if (score >= 78) return 3.0;
    if (score >= 75) return 2.7;
    if (score >= 72) return 2.3;
    if (score >= 68) return 2.0;
    if (score >= 64) return 1.5;
    if (score >= 60) return 1.0;
    return 0.0;
}

QString TeacherMainWindow::getCurrentSemester()
{
    return Validation::semesterForDate();
}











void TeacherMainWindow::on_pushButton_prev_clicked()
{
    if (m_currentPage > 1) {
        loadPage(m_currentPage - 1);
    }
}


void TeacherMainWindow::on_pushButton_page1_clicked()
{
    int page = ui->pushButton_page1->text().toInt();
    loadPage(page);
}


void TeacherMainWindow::on_pushButton_page2_clicked()
{
    int page = ui->pushButton_page2->text().toInt();
    loadPage(page);
}


void TeacherMainWindow::on_pushButton_page3_clicked()
{
    int page = ui->pushButton_page3->text().toInt();
    loadPage(page);
}


void TeacherMainWindow::on_pushButton_page4_clicked()
{
    int page = ui->pushButton_page4->text().toInt();
    loadPage(page);
}


void TeacherMainWindow::on_pushButton_page5_clicked()
{
    int page = ui->pushButton_page5->text().toInt();
    loadPage(page);
}


void TeacherMainWindow::on_pushButton_ellipsis_clicked()
{
    if (m_currentPage + 3 <= m_totalPages) {
        loadPage(m_currentPage + 3);
    } else {
        loadPage(m_totalPages);
    }
}


void TeacherMainWindow::on_pushButton_next_clicked()
{
    if (m_currentPage < m_totalPages) {
        loadPage(m_currentPage + 1);
    }
}




void TeacherMainWindow::on_lineEdit_search_returnPressed()
{
    m_searchKeyword = ui->lineEdit_search->text().trimmed();
    m_currentPage = 1;
    refreshStudentList();
}


void TeacherMainWindow::on_pushButton_search_clicked()
{
    on_lineEdit_search_returnPressed();
}


void TeacherMainWindow::on_listWidget_classes_itemClicked(QListWidgetItem *item)
{
    if (!item)
        return;
    const QString text = item->text();

    if (text == QStringLiteral("+ 添加班级")) {
        addClassInteractive();
    } else {
        m_currentPage = 1;
        refreshStudentList();
    }
}


void TeacherMainWindow::on_tableWidget_students_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row < 0 || !ui->tableWidget_students->item(row, 0))
        return;

    auto cellText = [this, row](int col) -> QString {
        QTableWidgetItem *item = ui->tableWidget_students->item(row, col);
        return item ? item->text() : QString();
    };

    const QString studentId = cellText(0);
    const QString name = cellText(1);
    const QString gender = cellText(2);
    const int age = cellText(3).toInt();
    const QString className = cellText(4);
    const QString phone = cellText(5);
    const QString address = cellText(6);

    EditStudentDialog dialog(m_teacherId, studentId, name, gender, age, className, phone, address, this);
    dialog.setClassItems(classNames());

    if (dialog.exec() == QDialog::Accepted) {
        if (updateStudent(studentId, dialog.getStudentId(), dialog.getName(),
                          dialog.getGender(), dialog.getAge(), dialog.getClassName(),
                          dialog.getPhone(), dialog.getAddress())) {
            refreshClassList();
            refreshStudentList();
        }
    }
}




void TeacherMainWindow::on_action_addStudent_triggered()
{
    AddStudentDialog dialog(this);

    // 班级数据来自 classes 表（包含空班级）
    const QStringList classList = classNames();
    if (classList.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("暂无班级，请先在左侧添加班级"));
        return;
    }
    dialog.setClassItems(classList);

    if (dialog.exec() == QDialog::Accepted) {
        // 获取用户输入的数据
        QString studentId = dialog.getStudentId();
        QString name = dialog.getName();
        QString gender = dialog.getGender();
        int age = dialog.getAge();
        QString className = dialog.getClassName();
        QString phone = dialog.getPhone();
        QString address = dialog.getAddress();

        // 添加到数据库
        if (addStudent(studentId, name, gender, age, className, phone, address)) {
            QMessageBox::information(this, "成功", "学生添加成功！");
            refreshClassList();
            refreshStudentList();
        } else {
            QMessageBox::critical(this, "错误", "添加失败！");
        }
    }
}


void TeacherMainWindow::on_action_deleteStudent_triggered()
{
    const int row = ui->tableWidget_students->currentRow();
    const QString studentId = selectedStudentId();
    if (row < 0 || studentId.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("请先选择要删除的学生！"));
        return;
    }

    QTableWidgetItem *nameItem = ui->tableWidget_students->item(row, 1);
    const QString studentName = nameItem ? nameItem->text() : QString();

    int gradeCount = 0;
    QSqlQuery countQuery(DatabaseManager::instance()->getTeacherDatabase(m_teacherId));
    countQuery.prepare(QStringLiteral("SELECT COUNT(*) FROM grades WHERE student_id = ?"));
    countQuery.addBindValue(studentId);
    if (countQuery.exec() && countQuery.next())
        gradeCount = countQuery.value(0).toInt();

    QMessageBox box(QMessageBox::Question, QStringLiteral("确认删除"),
                    QStringLiteral("确定要删除学生【%1 - %2】吗？").arg(studentId, studentName),
                    QMessageBox::Yes | QMessageBox::No, this);
    box.setInformativeText(QStringLiteral(
        "该操作会同时删除该学生的 %1 条成绩记录，并注销其学生账号，且不可撤销。")
                               .arg(gradeCount));
    if (box.exec() == QMessageBox::Yes) {
        if (deleteStudent(studentId)) {
            QMessageBox::information(this, QStringLiteral("成功"),
                                     QStringLiteral("学生删除成功"));
            refreshClassList();
            refreshStudentList();
        }
    }
}


void TeacherMainWindow::on_action_editStudent_triggered()
{
    const int row = ui->tableWidget_students->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("请先选择要编辑的学生！"));
        return;
    }

    auto cellText = [this, row](int column) -> QString {
        QTableWidgetItem *item = ui->tableWidget_students->item(row, column);
        return item ? item->text() : QString();
    };

    const QString studentId = cellText(0);
    const QString name = cellText(1);
    const QString gender = cellText(2);
    const int age = cellText(3).toInt();
    const QString className = cellText(4);
    const QString phone = cellText(5);
    const QString address = cellText(6);

    EditStudentDialog dialog(m_teacherId, studentId, name, gender, age, className, phone, address, this);

    // 班级数据来自 classes 表（包含空班级）
    const QStringList classList = classNames();
    if (classList.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("暂无班级，请先在左侧添加班级"));
        return;
    }
    dialog.setClassItems(classList);

    if (dialog.exec() == QDialog::Accepted) {
        // 获取修改后的数据
        QString newStudentId = dialog.getStudentId();
        QString newName = dialog.getName();
        QString newGender = dialog.getGender();
        int newAge = dialog.getAge();
        QString newClassName = dialog.getClassName();
        QString newPhone = dialog.getPhone();
        QString newAddress = dialog.getAddress();

        // 更新到数据库
        if (updateStudent(studentId, newStudentId, newName, newGender, newAge,
                          newClassName, newPhone, newAddress)) {
            QMessageBox::information(this, QStringLiteral("成功"),
                                     QStringLiteral("学生信息更新成功"));
            refreshClassList();
            refreshStudentList();
        }
    }
}


void TeacherMainWindow::on_action_refreshList_triggered()
{
    refreshStudentList();
}


void TeacherMainWindow::on_action_inputGrade_triggered()
{
    inputGrade();
}


void TeacherMainWindow::on_action_deleteGrade_triggered()
{
    modifyGrade();
}


void TeacherMainWindow::on_action_modifyGrade_triggered()
{
    modifyGrade();
}


void TeacherMainWindow::on_action_viewGrade_triggered()
{
    viewGrade();
}

void TeacherMainWindow::on_action_classGPA_triggered()
{
    exportClassGPAReport();
}

void TeacherMainWindow::on_action_allStudentsGPA_triggered()
{
    exportAllStudentsGPAReport();
}

void TeacherMainWindow::on_action_subjectClass_triggered()
{
    exportSubjectClassReport();
}

void TeacherMainWindow::on_action_subjectAll_triggered()
{
    exportSubjectAllReport();
}


void TeacherMainWindow::on_action_changePassword_triggered()
{
    ChangePasswordDialog dialog(m_teacherId, false, this);
    dialog.exec();
}

void TeacherMainWindow::on_action_changequestions_triggered()
{
    ChangeQuestionsDialog dialog(m_teacherId, this);
    dialog.exec();
}


void TeacherMainWindow::on_action_add_triggered()
{
    TeacherMainWindow::on_action_addStudent_triggered();
}


void TeacherMainWindow::on_action_edit_triggered()
{
    TeacherMainWindow::on_action_editStudent_triggered();
}


void TeacherMainWindow::on_action_delete_triggered()
{
    TeacherMainWindow::on_action_deleteStudent_triggered();
}


void TeacherMainWindow::on_action_refresh_triggered()
{
    TeacherMainWindow::refreshStudentList();
}


void TeacherMainWindow::on_action_exit_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认退出",
                                  "确定要退出系统吗？",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}


void TeacherMainWindow::on_listWidget_classes_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget_classes->itemAt(pos);
    if (!item) return;

    QString text = item->text();
    if (text == "全部学生" || text == "+ 添加班级") {
        return; // 这两个项目不需要右键菜单
    }

    QMenu menu(this);
    QAction *renameAction = menu.addAction("重命名班级");
    QAction *deleteAction = menu.addAction("删除班级");
    QAction *selectedAction = menu.exec(ui->listWidget_classes->mapToGlobal(pos));

    if (selectedAction == renameAction) {
        renameClass(text);
    } else if (selectedAction == deleteAction) {
        deleteClass(text);
    }
}


void TeacherMainWindow::on_action_add_class_triggered()
{
    addClassInteractive();
}


void TeacherMainWindow::on_action_edit_class_triggered()
{
    const QString className = currentClassFilter();
    if (className.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请先选择一个班级"));
        return;
    }
    renameClass(className);
}


void TeacherMainWindow::on_action_delete_class_triggered()
{
    const QString className = currentClassFilter();
    if (className.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请先选择一个班级"));
        return;
    }
    deleteClass(className);
}

void TeacherMainWindow::on_action_exit_2_triggered()
{
    on_action_exit_triggered();
}


void TeacherMainWindow::on_action_manageSubjects_triggered()
{
    manageSubjects();
}


void TeacherMainWindow::on_action_addSubject_triggered()
{
    addSubject();
}


void TeacherMainWindow::on_action_editSubject_triggered()
{
    editSubject();
}


void TeacherMainWindow::on_action_deleteSubject_triggered()
{
    deleteSubject();
}


void TeacherMainWindow::on_action_addSubject_2_triggered()
{
    addSubject();
}


void TeacherMainWindow::on_action_editSubject_2_triggered()
{
    editSubject();
}


void TeacherMainWindow::on_action_deleteSubject_2_triggered()
{
    deleteSubject();
}



