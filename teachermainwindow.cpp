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

TeacherMainWindow::TeacherMainWindow(const QString &teacherId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeacherMainWindow),
    m_teacherId(teacherId)
{
    ui->setupUi(this);
    setWindowTitle(QString("教师管理系统 - 教师模式 (%1)").arg(teacherId));

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

    // 初始化表格列
    initStudentTable();

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

// 刷新班级列表 - 保证不丢失空班级
void TeacherMainWindow::refreshClassList()
{
    // 1. 先保存当前的班级列表（包括空班级）
    QStringList currentClasses;
    for (int i = 1; i < ui->listWidget_classes->count() - 1; i++) {
        QListWidgetItem* item = ui->listWidget_classes->item(i);
        if (item && item->text() != "全部学生" && item->text() != "+ 添加班级") {
            currentClasses.append(item->text());
        }
    }

    // 保存当前选中的班级
    QString selectedClass = "";
    if (ui->listWidget_classes->currentItem()) {
        selectedClass = ui->listWidget_classes->currentItem()->text();
    }

    // 2. 清空列表
    ui->listWidget_classes->clear();

    // 3. 重新添加"全部学生"
    ui->listWidget_classes->addItem("全部学生");

    // 4. 从数据库获取有学生的班级
    QStringList dbClasses;
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (db.isOpen()) {
        QSqlQuery query(db);
        if (query.exec("SELECT DISTINCT class_name FROM students WHERE class_name IS NOT NULL AND class_name != '' ORDER BY class_name")) {
            while (query.next()) {
                QString className = query.value(0).toString();
                if (!className.isEmpty()) {
                    dbClasses.append(className);
                }
            }
        }
    }

    // 5. 合并：数据库中的班级 + 之前保存的班级（包括空班级）
    QStringList allClasses = dbClasses; // 先添加有学生的班级

    // 再添加之前存在但现在没学生的班级（去重）
    foreach (QString className, currentClasses) {
        if (!allClasses.contains(className)) {
            allClasses.append(className);
        }
    }

    // 6. 排序并添加到列表
    allClasses.sort();
    foreach (QString className, allClasses) {
        ui->listWidget_classes->addItem(className);
    }

    // 7. 添加"+ 添加班级"
    ui->listWidget_classes->addItem("+ 添加班级");

    // 8. 恢复选中状态
    bool found = false;
    if (!selectedClass.isEmpty() && selectedClass != "+ 添加班级") {
        for (int i = 0; i < ui->listWidget_classes->count(); i++) {
            if (ui->listWidget_classes->item(i)->text() == selectedClass) {
                ui->listWidget_classes->setCurrentRow(i);
                found = true;
                break;
            }
        }
    }

    if (!found) {
        ui->listWidget_classes->setCurrentRow(0); // 选中"全部学生"
    }
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

// 刷新学生列表
void TeacherMainWindow::refreshStudentList()
{
    int totalCount = getTotalStudentCount();
    int pageSize = 15;  // 每页显示15条
    m_totalPages = (totalCount + pageSize - 1) / pageSize;  // 计算总页数

    // 如果当前页超过总页数，重置为最后一页
    if (m_currentPage > m_totalPages && m_totalPages > 0) {
        m_currentPage = m_totalPages;
    } else if (m_totalPages == 0) {
        m_currentPage = 1;
    }

    // 获取当前页数据
    QList<QMap<QString, QString>> students = getStudentsByPage(m_currentPage, pageSize);

    // 加载到表格
    loadDataToTable(students);

    // 更新分页显示
    updatePagination(m_currentPage, m_totalPages);

    // 在函数的最后添加
    refreshClassList();

    // 如果当前选中的是班级，重新加载该班级学生
    QListWidgetItem *currentItem = ui->listWidget_classes->currentItem();
    if (currentItem && currentItem->text() != "全部学生" && currentItem->text() != "+ 添加班级") {
        loadClassStudents(currentItem->text());
    }
}

// 获取学生总数
int TeacherMainWindow::getTotalStudentCount()
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return 0;
    }

    QSqlQuery query(db);
    if (query.exec("SELECT COUNT(*) FROM students")) {
        if (query.next()) {
            return query.value(0).toInt();
        }
    } else {
        qDebug() << "查询学生总数失败：" << query.lastError().text();
    }

    return 0;
}

// 分页获取学生数据
QList<QMap<QString, QString>> TeacherMainWindow::getStudentsByPage(int page, int pageSize)
{
    QList<QMap<QString, QString>> studentList;

    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        return studentList;
    }

    int offset = (page - 1) * pageSize;

    QSqlQuery query(db);
    query.prepare("SELECT student_id, name, gender, age, class_name, phone, address, created_time "
                  "FROM students ORDER BY created_time DESC LIMIT ? OFFSET ?");
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

// 添加学生到数据库
bool TeacherMainWindow::addStudent(const QString &studentId, const QString &name,
                                   const QString &gender, int age, const QString &className,
                                   const QString &phone, const QString &address)
{
    // 验证学号是否已存在
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM students WHERE student_id = ?");
    checkQuery.addBindValue(studentId);

    if (checkQuery.exec() && checkQuery.next()) {
        if (checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "警告", "学号已存在！");
            return false;
        }
    }

    // 插入新学生
    QSqlQuery query(db);
    query.prepare("INSERT INTO students (student_id, name, gender, age, class_name, phone, address) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(studentId);
    query.addBindValue(name);
    query.addBindValue(gender);
    query.addBindValue(age);
    query.addBindValue(className);
    query.addBindValue(phone);
    query.addBindValue(address);

    return query.exec();
}

// 更新学生信息
bool TeacherMainWindow::updateStudent(const QString &oldStudentId, const QString &studentId,
                                      const QString &name, const QString &gender, int age,
                                      const QString &className, const QString &phone,
                                      const QString &address)
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        return false;
    }

    // 如果学号改变，检查新学号是否已存在
    if (oldStudentId != studentId) {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM students WHERE student_id = ?");
        checkQuery.addBindValue(studentId);

        if (checkQuery.exec() && checkQuery.next()) {
            if (checkQuery.value(0).toInt() > 0) {
                QMessageBox::warning(this, "警告", "学号已存在！");
                return false;
            }
        }
    }

    QSqlQuery query(db);
    query.prepare("UPDATE students SET student_id = ?, name = ?, gender = ?, age = ?, "
                  "class_name = ?, phone = ?, address = ? WHERE student_id = ?");
    query.addBindValue(studentId);
    query.addBindValue(name);
    query.addBindValue(gender);
    query.addBindValue(age);
    query.addBindValue(className);
    query.addBindValue(phone);
    query.addBindValue(address);
    query.addBindValue(oldStudentId);

    return query.exec();
}

bool TeacherMainWindow::deleteStudent(const QString &studentId)
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM students WHERE student_id = ?");
    query.addBindValue(studentId);

    bool success = query.exec();

    if (!success) {
        qDebug() << "删除学生失败：" << query.lastError().text();
    }

    return success;
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

// 加载指定班级的学生
void TeacherMainWindow::loadClassStudents(const QString &className)
{
    QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
    if (!db.isOpen()) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT student_id, name, gender, age, class_name, phone, address, created_time "
                  "FROM students WHERE class_name = ? ORDER BY created_time DESC");
    query.addBindValue(className);

    QList<QMap<QString, QString>> studentList;

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
    }

    // 加载到表格
    loadDataToTable(studentList);

    // 更新分页显示（班级视图不分页或按需实现）
    ui->label_pageInfo->setText(QString("班级：%1（共%2人）").arg(className).arg(studentList.size()));
    ui->pushButton_prev->setEnabled(false);
    ui->pushButton_next->setEnabled(false);

    // 隐藏页码按钮（班级视图不分页）
    for (QPushButton *btn : std::as_const(m_pageButtons)) {
        btn->setVisible(false);
    }

    ui->pushButton_ellipsis->setVisible(false);
}


void TeacherMainWindow::manageSubjects()
{
    // 创建一个对话框显示所有学科
    QDialog dialog(this);
    dialog.setWindowTitle("学科管理");
    dialog.resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 创建表格显示学科
    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(4); // 修改为4列
    table->setHorizontalHeaderLabels(QStringList() << "学科编号" << "学科名称"
                                                   << "学分" << "创建时间"); // 移除"描述"列

    // 加载学科数据
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (db.isOpen()) {
        QSqlQuery query(db);
        if (query.exec("SELECT subject_id, subject_name, credit, "
                       "created_time FROM subjects ORDER BY subject_id")) { // 移除description字段
            int row = 0;
            while (query.next()) {
                table->insertRow(row);
                for (int col = 0; col < 4; col++) { // 修改为4列
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                    table->setItem(row, col, item);
                }
                row++;
            }
        }
    }

    layout->addWidget(table);

    // 添加按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("添加学科", &dialog);
    QPushButton *editButton = new QPushButton("编辑学科", &dialog);
    QPushButton *deleteButton = new QPushButton("删除学科", &dialog);
    QPushButton *closeButton = new QPushButton("关闭", &dialog);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    // 连接信号槽
    connect(addButton, &QPushButton::clicked, [&]() {
        dialog.done(QDialog::Accepted);
        addSubject();
    });

    connect(editButton, &QPushButton::clicked, [&]() {
        int row = table->currentRow();
        if (row >= 0) {
            QString subjectId = table->item(row, 0)->text();
            dialog.done(QDialog::Accepted);

            // 编辑学科 - 使用正确的构造函数
            SubjectDialog editDialog(m_teacherId, subjectId, this);
            if (editDialog.exec() == QDialog::Accepted) {
                manageSubjects(); // 刷新显示
            }
        } else {
            QMessageBox::warning(&dialog, "警告", "请先选择一个学科！");
        }
    });

    connect(deleteButton, &QPushButton::clicked, [&]() {
        int row = table->currentRow();
        if (row >= 0) {
            QString subjectId = table->item(row, 0)->text();
            QString subjectName = table->item(row, 1)->text();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(&dialog, "确认删除",
                                          QString("确定要删除学科【%1 - %2】吗？\n"
                                                  "（同时会删除所有相关成绩记录）")
                                              .arg(subjectId).arg(subjectName),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                DatabaseManager* dbManager = DatabaseManager::instance();
                QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

                if (db.isOpen()) {
                    QSqlQuery query(db);

                    // 先删除相关成绩
                    query.prepare("DELETE FROM grades WHERE subject_id = ?");
                    query.addBindValue(subjectId);
                    query.exec();

                    // 再删除学科
                    query.prepare("DELETE FROM subjects WHERE subject_id = ?");
                    query.addBindValue(subjectId);

                    if (query.exec()) {
                        table->removeRow(row);
                        QMessageBox::information(&dialog, "成功", "学科删除成功！");
                    }
                }
            }
        } else {
            QMessageBox::warning(&dialog, "警告", "请先选择一个学科！");
        }
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void TeacherMainWindow::addSubject()
{
    SubjectDialog dialog(m_teacherId, SubjectDialog::AddMode, this);
    if (dialog.exec() == QDialog::Accepted) {
        // 刷新可能需要的地方
    }
}

void TeacherMainWindow::editSubject()
{
    // 让用户选择要编辑的学科
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery query(db);
    if (query.exec("SELECT subject_id, subject_name FROM subjects ORDER BY subject_id")) {
        QStringList items;
        QMap<QString, QString> subjectMap;

        while (query.next()) {
            QString id = query.value(0).toString();
            QString name = query.value(1).toString();
            items << QString("%1 - %2").arg(id).arg(name);
            subjectMap[items.last()] = id;
        }

        if (items.isEmpty()) {
            QMessageBox::information(this, "提示", "没有学科可编辑！");
            return;
        }

        bool ok;
        QString selected = QInputDialog::getItem(this, "选择学科",
                                                 "请选择要编辑的学科:",
                                                 items, 0, false, &ok);

        if (ok && !selected.isEmpty()) {
            QString subjectId = subjectMap[selected];
            SubjectDialog dialog(m_teacherId, subjectId, this);
            dialog.exec();
        }
    }
}

void TeacherMainWindow::deleteSubject()
{
    // 类似 editSubject，让用户选择然后删除
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery query(db);
    if (query.exec("SELECT subject_id, subject_name FROM subjects ORDER BY subject_id")) {
        QStringList items;
        QMap<QString, QString> subjectMap;

        while (query.next()) {
            QString id = query.value(0).toString();
            QString name = query.value(1).toString();
            items << QString("%1 - %2").arg(id).arg(name);
            subjectMap[items.last()] = id;
        }

        if (items.isEmpty()) {
            QMessageBox::information(this, "提示", "没有学科可删除！");
            return;
        }

        bool ok;
        QString selected = QInputDialog::getItem(this, "删除学科",
                                                 "请选择要删除的学科:",
                                                 items, 0, false, &ok);

        if (ok && !selected.isEmpty()) {
            QString subjectId = subjectMap[selected];
            QString subjectName = selected.split(" - ")[1];

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "确认删除",
                                          QString("确定要删除学科【%1】吗？\n"
                                                  "（同时会删除所有相关成绩记录）")
                                              .arg(selected),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                QSqlQuery deleteQuery(db);

                // 先删除相关成绩
                deleteQuery.prepare("DELETE FROM grades WHERE subject_id = ?");
                deleteQuery.addBindValue(subjectId);
                deleteQuery.exec();

                // 再删除学科
                deleteQuery.prepare("DELETE FROM subjects WHERE subject_id = ?");
                deleteQuery.addBindValue(subjectId);

                if (deleteQuery.exec()) {
                    QMessageBox::information(this, "成功", "学科删除成功！");
                }
            }
        }
    }
}

void TeacherMainWindow::inputGrade()
{
    GradeDialog dialog(m_teacherId, this);
    dialog.exec();
}

void TeacherMainWindow::modifyGrade()
{
    // 创建对话框让用户选择要修改的成绩
    QDialog dialog(this);
    dialog.setWindowTitle("选择要修改的成绩");
    dialog.resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 添加过滤器
    QHBoxLayout *filterLayout = new QHBoxLayout();
    QComboBox *classCombo = new QComboBox(&dialog);
    QComboBox *subjectCombo = new QComboBox(&dialog);
    QComboBox *semesterCombo = new QComboBox(&dialog);
    QPushButton *filterButton = new QPushButton("筛选", &dialog);

    classCombo->addItem("全部班级", "");
    subjectCombo->addItem("全部学科", "");
    semesterCombo->addItem("全部学期", "");

    filterLayout->addWidget(new QLabel("班级:", &dialog));
    filterLayout->addWidget(classCombo);
    filterLayout->addWidget(new QLabel("学科:", &dialog));
    filterLayout->addWidget(subjectCombo);
    filterLayout->addWidget(new QLabel("学期:", &dialog));
    filterLayout->addWidget(semesterCombo);
    filterLayout->addWidget(filterButton);
    filterLayout->addStretch();

    layout->addLayout(filterLayout);

    // 创建表格
    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(QStringList() << "学生学号" << "学生姓名"
                                                   << "学科" << "成绩" << "学分" << "学期" << "考试日期");

    layout->addWidget(table);

    // 加载数据
    auto loadGrades = [&]() {
        table->setRowCount(0);

        DatabaseManager* dbManager = DatabaseManager::instance();
        QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

        if (!db.isOpen()) return;

        QString sql = "SELECT s.student_id, s.name, sub.subject_name, "
                      "g.score, sub.credit, g.semester, g.exam_date "
                      "FROM grades g "
                      "JOIN students s ON g.student_id = s.student_id "
                      "JOIN subjects sub ON g.subject_id = sub.subject_id "
                      "WHERE 1=1";

        QStringList conditions;
        QVariantList params;

        QString selectedClass = classCombo->currentData().toString();
        if (!selectedClass.isEmpty()) {
            sql += " AND s.class_name = ?";
            params << selectedClass;
        }

        QString selectedSubject = subjectCombo->currentData().toString();
        if (!selectedSubject.isEmpty()) {
            sql += " AND g.subject_id = ?";
            params << selectedSubject;
        }

        QString selectedSemester = semesterCombo->currentData().toString();
        if (!selectedSemester.isEmpty()) {
            sql += " AND g.semester = ?";
            params << selectedSemester;
        }

        sql += " ORDER BY g.semester DESC, g.subject_id, s.student_id";

        QSqlQuery query(db);
        query.prepare(sql);

        for (int i = 0; i < params.size(); i++) {
            query.addBindValue(params[i]);
        }

        if (query.exec()) {
            int row = 0;
            while (query.next()) {
                table->insertRow(row);
                for (int col = 0; col < 7; col++) {
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                    table->setItem(row, col, item);
                }
                row++;
            }
        }
    };

    // 加载筛选条件
    auto loadFilters = [&]() {
        DatabaseManager* dbManager = DatabaseManager::instance();
        QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

        if (!db.isOpen()) return;

        // 加载班级
        QSqlQuery classQuery(db);
        if (classQuery.exec("SELECT DISTINCT class_name FROM students "
                            "WHERE class_name IS NOT NULL AND class_name != ''")) {
            while (classQuery.next()) {
                QString className = classQuery.value(0).toString();
                classCombo->addItem(className, className);
            }
        }

        // 加载学科
        QSqlQuery subjectQuery(db);
        if (subjectQuery.exec("SELECT subject_id, subject_name FROM subjects")) {
            while (subjectQuery.next()) {
                QString subjectId = subjectQuery.value(0).toString();
                QString subjectName = subjectQuery.value(1).toString();
                subjectCombo->addItem(subjectName, subjectId);
            }
        }

        // 加载学期
        QSqlQuery semesterQuery(db);
        if (semesterQuery.exec("SELECT DISTINCT semester FROM grades ORDER BY semester DESC")) {
            while (semesterQuery.next()) {
                QString semester = semesterQuery.value(0).toString();
                semesterCombo->addItem(semester, semester);
            }
        }
    };

    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *editButton = new QPushButton("编辑选中成绩", &dialog);
    QPushButton *deleteButton = new QPushButton("删除选中成绩", &dialog);
    QPushButton *closeButton = new QPushButton("关闭", &dialog);

    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    // 连接信号槽
    connect(filterButton, &QPushButton::clicked, loadGrades);
    connect(editButton, &QPushButton::clicked, [&]() {
        int row = table->currentRow();
        if (row >= 0) {
            QString studentId = table->item(row, 0)->text();
            QString subjectName = table->item(row, 2)->text();
            QString semester = table->item(row, 5)->text();

            // 获取学科ID
            DatabaseManager* dbManager = DatabaseManager::instance();
            QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

            QSqlQuery query(db);
            query.prepare("SELECT subject_id FROM subjects WHERE subject_name = ?");
            query.addBindValue(subjectName);

            if (query.exec() && query.next()) {
                QString subjectId = query.value(0).toString();
                dialog.done(QDialog::Accepted);

                GradeDialog gradeDialog(m_teacherId, studentId, subjectId, semester, this);
                if (gradeDialog.exec() == QDialog::Accepted) {
                    // 刷新表格
                    loadGrades();
                }
            }
        } else {
            QMessageBox::warning(&dialog, "警告", "请先选择一条成绩记录！");
        }
    });

    connect(deleteButton, &QPushButton::clicked, [&]() {
        int row = table->currentRow();
        if (row >= 0) {
            QString studentId = table->item(row, 0)->text();
            QString studentName = table->item(row, 1)->text();
            QString subjectName = table->item(row, 2)->text();
            QString semester = table->item(row, 5)->text();

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(&dialog, "确认删除",
                                          QString("确定要删除【%1 - %2】的【%3】成绩吗？")
                                              .arg(studentId).arg(studentName).arg(subjectName),
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                DatabaseManager* dbManager = DatabaseManager::instance();
                QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

                // 获取学科ID
                QSqlQuery query(db);
                query.prepare("SELECT subject_id FROM subjects WHERE subject_name = ?");
                query.addBindValue(subjectName);

                if (query.exec() && query.next()) {
                    QString subjectId = query.value(0).toString();

                    QSqlQuery deleteQuery(db);
                    deleteQuery.prepare("DELETE FROM grades WHERE student_id = ? AND "
                                        "subject_id = ? AND semester = ?");
                    deleteQuery.addBindValue(studentId);
                    deleteQuery.addBindValue(subjectId);
                    deleteQuery.addBindValue(semester);

                    if (deleteQuery.exec()) {
                        table->removeRow(row);
                        QMessageBox::information(&dialog, "成功", "成绩删除成功！");
                    }
                }
            }
        } else {
            QMessageBox::warning(&dialog, "警告", "请先选择一条成绩记录！");
        }
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    // 加载数据
    loadFilters();
    loadGrades();

    dialog.exec();
}

void TeacherMainWindow::viewGrade()
{
    // 极简版本，只显示数据
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) {
        QMessageBox::warning(this, "错误", "数据库未连接");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("成绩查看");
    dialog.resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 简单查询
    QSqlQuery query(db);
    bool success = query.exec("SELECT s.student_id, s.name, s.class_name, "
                              "sub.subject_name, g.score, g.exam_date "
                              "FROM grades g "
                              "JOIN students s ON g.student_id = s.student_id "
                              "JOIN subjects sub ON g.subject_id = sub.subject_id "
                              "LIMIT 100");  // 限制100条，避免数据太多

    if (!success) {
        QMessageBox::warning(&dialog, "错误", "查询失败：" + query.lastError().text());
        return;
    }

    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels(QStringList() << "学号" << "姓名" << "班级" << "学科" << "成绩" << "考试日期");

    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col = 0; col < 6; col++) {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    layout->addWidget(table);

    QPushButton *closeBtn = new QPushButton("关闭", &dialog);
    layout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}


void TeacherMainWindow::exportClassGPAReport()
{
    // 选择班级
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.exec("SELECT DISTINCT class_name FROM students "
               "WHERE class_name IS NOT NULL AND class_name != '' "
               "ORDER BY class_name");

    QStringList classList;
    while (query.next()) {
        classList << query.value(0).toString();
    }

    if (classList.isEmpty()) {
        QMessageBox::information(this, "提示", "没有班级数据！");
        return;
    }

    bool ok;
    QString className = QInputDialog::getItem(this, "选择班级",
                                              "请选择班级:", classList, 0, false, &ok);

    if (!ok || className.isEmpty()) return;

    // 查询班级绩点排名
    query.prepare(
        "SELECT s.student_id, s.name, "
        "AVG(g.score) as avg_score, "
        "SUM(sub.credit) as total_credit, "
        "AVG(CASE "
        "WHEN g.score >= 90 THEN 4.0 "
        "WHEN g.score >= 80 THEN 3.0 "
        "WHEN g.score >= 70 THEN 2.0 "
        "WHEN g.score >= 60 THEN 1.0 "
        "ELSE 0.0 END) as gpa, "
        "RANK() OVER (ORDER BY AVG(CASE "
        "WHEN g.score >= 90 THEN 4.0 "
        "WHEN g.score >= 80 THEN 3.0 "
        "WHEN g.score >= 70 THEN 2.0 "
        "WHEN g.score >= 60 THEN 1.0 "
        "ELSE 0.0 END) DESC) as ranking "
        "FROM grades g "
        "JOIN students s ON g.student_id = s.student_id "
        "JOIN subjects sub ON g.subject_id = sub.subject_id "
        "WHERE s.class_name = ? "
        "GROUP BY s.student_id, s.name "
        "ORDER BY gpa DESC"
        );
    query.addBindValue(className);

    // 显示结果对话框
    QDialog dialog(this);
    dialog.setWindowTitle(QString("班级绩点排名 - %1").arg(className));
    dialog.resize(700, 500);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *titleLabel = new QLabel(QString("<h2>%1 班级绩点排名</h2>").arg(className), &dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels(QStringList() << "排名" << "学号" << "姓名"
                                                   << "平均成绩" << "总学分" << "平均绩点");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            for (int col = 0; col < 6; col++) {
                QString text;
                if (col == 0) text = QString::number(query.value(5).toInt()); // 排名
                else if (col == 1) text = query.value(0).toString(); // 学号
                else if (col == 2) text = query.value(1).toString(); // 姓名
                else if (col == 3) text = QString::number(query.value(2).toDouble(), 'f', 2); // 平均成绩
                else if (col == 4) text = QString::number(query.value(3).toDouble(), 'f', 1); // 总学分
                else if (col == 5) text = QString::number(query.value(4).toDouble(), 'f', 2); // 绩点

                QTableWidgetItem *item = new QTableWidgetItem(text);
                if (col == 0) { // 排名列特殊处理
                    int rank = query.value(5).toInt();
                    item->setForeground(QBrush(rank <= 3 ? Qt::red : Qt::black));
                    QFont font = item->font();
                    font.setBold(rank <= 3);
                    item->setFont(font);
                }
                table->setItem(row, col, item);
            }
            row++;
        }
    }

    layout->addWidget(table);

    QPushButton *exportButton = new QPushButton("导出Excel", &dialog);
    QPushButton *printButton = new QPushButton("打印", &dialog);
    QPushButton *closeButton = new QPushButton("关闭", &dialog);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(printButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    connect(exportButton, &QPushButton::clicked, [&]() {
        // 导出Excel
        QString fileName = QFileDialog::getSaveFileName(&dialog, "保存报表",
                                                        QDir::homePath() + QString("/%1_绩点排名.xlsx").arg(className),
                                                        "Excel文件 (*.xlsx)");
        if (!fileName.isEmpty()) {
            // 实现导出逻辑
            QMessageBox::information(&dialog, "提示", "导出功能正在开发中...");
        }
    });

    connect(printButton, &QPushButton::clicked, [&]() {
        // 打印功能
        QMessageBox::information(&dialog, "提示", "打印功能正在开发中...");
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void TeacherMainWindow::exportAllStudentsGPAReport()
{
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery query(db);
    query.exec(
        "SELECT s.student_id, s.name, s.class_name, "
        "AVG(g.score) as avg_score, "
        "SUM(sub.credit) as total_credit, "
        "AVG(CASE "
        "WHEN g.score >= 90 THEN 4.0 "
        "WHEN g.score >= 80 THEN 3.0 "
        "WHEN g.score >= 70 THEN 2.0 "
        "WHEN g.score >= 60 THEN 1.0 "
        "ELSE 0.0 END) as gpa, "
        "RANK() OVER (ORDER BY AVG(CASE "
        "WHEN g.score >= 90 THEN 4.0 "
        "WHEN g.score >= 80 THEN 3.0 "
        "WHEN g.score >= 70 THEN 2.0 "
        "WHEN g.score >= 60 THEN 1.0 "
        "ELSE 0.0 END) DESC) as ranking "
        "FROM grades g "
        "JOIN students s ON g.student_id = s.student_id "
        "JOIN subjects sub ON g.subject_id = sub.subject_id "
        "GROUP BY s.student_id, s.name, s.class_name "
        "ORDER BY gpa DESC"
        );

    // 显示所有学生绩点排名
    QDialog dialog(this);
    dialog.setWindowTitle("所有学生绩点排名");
    dialog.resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *titleLabel = new QLabel("<h2>所有学生绩点排名</h2>", &dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(QStringList() << "排名" << "学号" << "姓名"
                                                   << "班级" << "平均成绩" << "总学分" << "平均绩点");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            for (int col = 0; col < 7; col++) {
                QString text;
                if (col == 0) text = QString::number(query.value(6).toInt());
                else if (col == 1) text = query.value(0).toString();
                else if (col == 2) text = query.value(1).toString();
                else if (col == 3) text = query.value(2).toString();
                else if (col == 4) text = QString::number(query.value(3).toDouble(), 'f', 2);
                else if (col == 5) text = QString::number(query.value(4).toDouble(), 'f', 1);
                else if (col == 6) text = QString::number(query.value(5).toDouble(), 'f', 2);

                QTableWidgetItem *item = new QTableWidgetItem(text);
                if (col == 0) {
                    int rank = query.value(6).toInt();
                    item->setForeground(QBrush(rank <= 10 ? Qt::red : Qt::black));
                    QFont font = item->font();
                    font.setBold(rank <= 10);
                    item->setFont(font);
                }
                table->setItem(row, col, item);
            }
            row++;
        }
    }

    layout->addWidget(table);

    // 统计信息
    QGroupBox *statsBox = new QGroupBox("统计信息", &dialog);
    QGridLayout *statsLayout = new QGridLayout(statsBox);

    // 计算统计数据
    int totalStudents = table->rowCount();
    double avgGPA = 0;
    double maxGPA = 0;
    double minGPA = 5.0;

    for (int i = 0; i < totalStudents; i++) {
        double gpa = table->item(i, 6)->text().toDouble();
        avgGPA += gpa;
        if (gpa > maxGPA) maxGPA = gpa;
        if (gpa < minGPA) minGPA = gpa;
    }

    if (totalStudents > 0) avgGPA /= totalStudents;

    statsLayout->addWidget(new QLabel("学生总数:", statsBox), 0, 0);
    statsLayout->addWidget(new QLabel(QString::number(totalStudents), statsBox), 0, 1);
    statsLayout->addWidget(new QLabel("平均绩点:", statsBox), 0, 2);
    statsLayout->addWidget(new QLabel(QString::number(avgGPA, 'f', 2), statsBox), 0, 3);
    statsLayout->addWidget(new QLabel("最高绩点:", statsBox), 1, 0);
    statsLayout->addWidget(new QLabel(QString::number(maxGPA, 'f', 2), statsBox), 1, 1);
    statsLayout->addWidget(new QLabel("最低绩点:", statsBox), 1, 2);
    statsLayout->addWidget(new QLabel(QString::number(minGPA, 'f', 2), statsBox), 1, 3);

    layout->addWidget(statsBox);

    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *exportButton = new QPushButton("导出Excel", &dialog);
    QPushButton *printButton = new QPushButton("打印", &dialog);
    QPushButton *closeButton = new QPushButton("关闭", &dialog);

    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(printButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    connect(exportButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getSaveFileName(&dialog, "保存报表",
                                                        QDir::homePath() + "/所有学生绩点排名.xlsx",
                                                        "Excel文件 (*.xlsx)");
        if (!fileName.isEmpty()) {
            QMessageBox::information(&dialog, "提示", "导出功能正在开发中...");
        }
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void TeacherMainWindow::exportSubjectClassReport()
{
    // 选择学科和班级
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    // 获取所有学科
    QSqlQuery subjectQuery(db);
    subjectQuery.exec("SELECT subject_id, subject_name FROM subjects ORDER BY subject_id");

    QStringList subjectList;
    QMap<QString, QString> subjectMap;
    while (subjectQuery.next()) {
        QString subjectId = subjectQuery.value(0).toString();
        QString subjectName = subjectQuery.value(1).toString();
        QString displayText = QString("%1 - %2").arg(subjectId).arg(subjectName);
        subjectList << displayText;
        subjectMap[displayText] = subjectId;
    }

    if (subjectList.isEmpty()) {
        QMessageBox::information(this, "提示", "没有学科数据！");
        return;
    }

    // 获取所有班级
    QSqlQuery classQuery(db);
    classQuery.exec("SELECT DISTINCT class_name FROM students "
                    "WHERE class_name IS NOT NULL AND class_name != '' "
                    "ORDER BY class_name");

    QStringList classList;
    while (classQuery.next()) {
        classList << classQuery.value(0).toString();
    }

    if (classList.isEmpty()) {
        QMessageBox::information(this, "提示", "没有班级数据！");
        return;
    }

    // 选择对话框
    QDialog selectDialog(this);
    selectDialog.setWindowTitle("选择学科和班级");
    selectDialog.resize(400, 200);

    QVBoxLayout *selectLayout = new QVBoxLayout(&selectDialog);

    selectLayout->addWidget(new QLabel("选择学科:"));
    QComboBox *subjectCombo = new QComboBox(&selectDialog);
    subjectCombo->addItems(subjectList);
    selectLayout->addWidget(subjectCombo);

    selectLayout->addWidget(new QLabel("选择班级:"));
    QComboBox *classCombo = new QComboBox(&selectDialog);
    classCombo->addItems(classList);
    selectLayout->addWidget(classCombo);

    // 选择学期（可选）
    selectLayout->addWidget(new QLabel("选择学期（可选）:"));
    QLineEdit *semesterEdit = new QLineEdit(&selectDialog);
    semesterEdit->setPlaceholderText("留空表示所有学期");
    selectLayout->addWidget(semesterEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("确定", &selectDialog);
    QPushButton *cancelButton = new QPushButton("取消", &selectDialog);

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    selectLayout->addLayout(buttonLayout);

    QString selectedSubjectId;
    QString selectedClassName;
    QString selectedSemester;

    connect(okButton, &QPushButton::clicked, [&]() {
        selectedSubjectId = subjectMap[subjectCombo->currentText()];
        selectedClassName = classCombo->currentText();
        selectedSemester = semesterEdit->text().trimmed();
        selectDialog.accept();
    });

    connect(cancelButton, &QPushButton::clicked, &selectDialog, &QDialog::reject);

    if (selectDialog.exec() != QDialog::Accepted) return;

    // 查询单科班级成绩排名
    QSqlQuery query(db);
    QString sql =
        "SELECT s.student_id, s.name, g.score, "
        "RANK() OVER (ORDER BY g.score DESC) as ranking "
        "FROM grades g "
        "JOIN students s ON g.student_id = s.student_id "
        "WHERE g.subject_id = ? AND s.class_name = ?";

    QVariantList params;
    params << selectedSubjectId << selectedClassName;

    if (!selectedSemester.isEmpty()) {
        sql += " AND g.semester = ?";
        params << selectedSemester;
    }

    sql += " ORDER BY g.score DESC";

    query.prepare(sql);
    for (const QVariant &param : params) {
        query.addBindValue(param);
    }

    // 获取学科名称
    QString subjectName;
    QSqlQuery nameQuery(db);
    nameQuery.prepare("SELECT subject_name FROM subjects WHERE subject_id = ?");
    nameQuery.addBindValue(selectedSubjectId);
    if (nameQuery.exec() && nameQuery.next()) {
        subjectName = nameQuery.value(0).toString();
    }

    // 显示结果
    QDialog resultDialog(this);
    resultDialog.setWindowTitle(QString("%1 - %2 成绩排名").arg(selectedClassName).arg(subjectName));
    resultDialog.resize(600, 400);

    QVBoxLayout *resultLayout = new QVBoxLayout(&resultDialog);

    QString titleText = QString("<h2>%1 班级 %2 成绩排名</h2>")
                            .arg(selectedClassName).arg(subjectName);
    if (!selectedSemester.isEmpty()) {
        titleText += QString("<h3>学期：%1</h3>").arg(selectedSemester);
    }

    QLabel *titleLabel = new QLabel(titleText, &resultDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    resultLayout->addWidget(titleLabel);

    QTableWidget *table = new QTableWidget(&resultDialog);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QStringList() << "排名" << "学号" << "姓名" << "成绩");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            for (int col = 0; col < 4; col++) {
                QString text;
                if (col == 0) text = QString::number(query.value(3).toInt());
                else if (col == 1) text = query.value(0).toString();
                else if (col == 2) text = query.value(1).toString();
                else if (col == 3) text = QString::number(query.value(2).toDouble(), 'f', 2);

                QTableWidgetItem *item = new QTableWidgetItem(text);
                if (col == 0) {
                    int rank = query.value(3).toInt();
                    item->setForeground(QBrush(rank <= 3 ? Qt::red : Qt::black));
                    QFont font = item->font();
                    font.setBold(rank <= 3);
                    item->setFont(font);
                } else if (col == 3) {
                    double score = query.value(2).toDouble();
                    if (score >= 90) item->setForeground(QBrush(Qt::darkGreen));
                    else if (score >= 80) item->setForeground(QBrush(Qt::blue));
                    else if (score >= 60) item->setForeground(QBrush(Qt::black));
                    else item->setForeground(QBrush(Qt::red));
                }
                table->setItem(row, col, item);
            }
            row++;
        }
    }

    resultLayout->addWidget(table);

    // 统计信息
    int totalStudents = table->rowCount();
    double avgScore = 0;
    double maxScore = 0;
    double minScore = 100;
    int passCount = 0;

    for (int i = 0; i < totalStudents; i++) {
        double score = table->item(i, 3)->text().toDouble();
        avgScore += score;
        if (score > maxScore) maxScore = score;
        if (score < minScore) minScore = score;
        if (score >= 60) passCount++;
    }

    if (totalStudents > 0) avgScore /= totalStudents;
    double passRate = totalStudents > 0 ? (passCount * 100.0 / totalStudents) : 0;

    QGroupBox *statsBox = new QGroupBox("统计信息", &resultDialog);
    QGridLayout *statsLayout = new QGridLayout(statsBox);

    statsLayout->addWidget(new QLabel("参考人数:", statsBox), 0, 0);
    statsLayout->addWidget(new QLabel(QString::number(totalStudents), statsBox), 0, 1);
    statsLayout->addWidget(new QLabel("平均分:", statsBox), 0, 2);
    statsLayout->addWidget(new QLabel(QString::number(avgScore, 'f', 2), statsBox), 0, 3);
    statsLayout->addWidget(new QLabel("最高分:", statsBox), 1, 0);
    statsLayout->addWidget(new QLabel(QString::number(maxScore, 'f', 2), statsBox), 1, 1);
    statsLayout->addWidget(new QLabel("最低分:", statsBox), 1, 2);
    statsLayout->addWidget(new QLabel(QString::number(minScore, 'f', 2), statsBox), 1, 3);
    statsLayout->addWidget(new QLabel("及格人数:", statsBox), 2, 0);
    statsLayout->addWidget(new QLabel(QString::number(passCount), statsBox), 2, 1);
    statsLayout->addWidget(new QLabel("及格率:", statsBox), 2, 2);
    statsLayout->addWidget(new QLabel(QString::number(passRate, 'f', 1) + "%", statsBox), 2, 3);

    resultLayout->addWidget(statsBox);

    // 按钮
    QHBoxLayout *resultButtonLayout = new QHBoxLayout();
    QPushButton *exportButton = new QPushButton("导出Excel", &resultDialog);
    QPushButton *closeButton = new QPushButton("关闭", &resultDialog);

    resultButtonLayout->addWidget(exportButton);
    resultButtonLayout->addStretch();
    resultButtonLayout->addWidget(closeButton);

    resultLayout->addLayout(resultButtonLayout);

    connect(exportButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getSaveFileName(&resultDialog, "保存报表",
                                                        QDir::homePath() +
                                                            QString("/%1_%2_成绩排名.xlsx")
                                                                .arg(selectedClassName).arg(subjectName),
                                                        "Excel文件 (*.xlsx)");
        if (!fileName.isEmpty()) {
            QMessageBox::information(&resultDialog, "提示", "导出功能正在开发中...");
        }
    });

    connect(closeButton, &QPushButton::clicked, &resultDialog, &QDialog::accept);

    resultDialog.exec();
}

void TeacherMainWindow::exportSubjectAllReport()
{
    // 选择学科
    DatabaseManager* dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager->getTeacherDatabase(m_teacherId);

    if (!db.isOpen()) return;

    QSqlQuery subjectQuery(db);
    subjectQuery.exec("SELECT subject_id, subject_name FROM subjects ORDER BY subject_id");

    QStringList subjectList;
    QMap<QString, QString> subjectMap;
    while (subjectQuery.next()) {
        QString subjectId = subjectQuery.value(0).toString();
        QString subjectName = subjectQuery.value(1).toString();
        QString displayText = QString("%1 - %2").arg(subjectId).arg(subjectName);
        subjectList << displayText;
        subjectMap[displayText] = subjectId;
    }

    if (subjectList.isEmpty()) {
        QMessageBox::information(this, "提示", "没有学科数据！");
        return;
    }

    bool ok;
    QString selectedText = QInputDialog::getItem(this, "选择学科",
                                                 "请选择学科:", subjectList, 0, false, &ok);

    if (!ok || selectedText.isEmpty()) return;

    QString selectedSubjectId = subjectMap[selectedText];
    QString subjectName = selectedText.split(" - ")[1];

    // 选择学期（可选）
    QString semester = QInputDialog::getText(this, "选择学期",
                                             "请输入学期（留空表示所有学期）:",
                                             QLineEdit::Normal, "", &ok);

    if (!ok) return; // 用户点击了取消

    // 查询单科所有学生成绩排名
    QSqlQuery query(db);
    QString sql =
        "SELECT s.student_id, s.name, s.class_name, g.score, "
        "RANK() OVER (ORDER BY g.score DESC) as ranking "
        "FROM grades g "
        "JOIN students s ON g.student_id = s.student_id "
        "WHERE g.subject_id = ?";

    QVariantList params;
    params << selectedSubjectId;

    if (!semester.trimmed().isEmpty()) {
        sql += " AND g.semester = ?";
        params << semester.trimmed();
    }

    sql += " ORDER BY g.score DESC, s.class_name, s.student_id";

    query.prepare(sql);
    for (const QVariant &param : params) {
        query.addBindValue(param);
    }

    // 显示结果
    QDialog dialog(this);
    dialog.setWindowTitle(QString("%1 所有学生成绩排名").arg(subjectName));
    dialog.resize(700, 500);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QString titleText = QString("<h2>%1 所有学生成绩排名</h2>").arg(subjectName);
    if (!semester.trimmed().isEmpty()) {
        titleText += QString("<h3>学期：%1</h3>").arg(semester.trimmed());
    }

    QLabel *titleLabel = new QLabel(titleText, &dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(QStringList() << "排名" << "学号" << "姓名" << "班级" << "成绩");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            table->insertRow(row);
            for (int col = 0; col < 5; col++) {
                QString text;
                if (col == 0) text = QString::number(query.value(4).toInt());
                else if (col == 1) text = query.value(0).toString();
                else if (col == 2) text = query.value(1).toString();
                else if (col == 3) text = query.value(2).toString();
                else if (col == 4) text = QString::number(query.value(3).toDouble(), 'f', 2);

                QTableWidgetItem *item = new QTableWidgetItem(text);
                if (col == 0) {
                    int rank = query.value(4).toInt();
                    item->setForeground(QBrush(rank <= 10 ? Qt::red : Qt::black));
                    QFont font = item->font();
                    font.setBold(rank <= 10);
                    item->setFont(font);
                } else if (col == 4) {
                    double score = query.value(3).toDouble();
                    if (score >= 90) item->setForeground(QBrush(Qt::darkGreen));
                    else if (score >= 80) item->setForeground(QBrush(Qt::blue));
                    else if (score >= 60) item->setForeground(QBrush(Qt::black));
                    else item->setForeground(QBrush(Qt::red));
                }
                table->setItem(row, col, item);
            }
            row++;
        }
    }

    layout->addWidget(table);

    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *exportButton = new QPushButton("导出Excel", &dialog);
    QPushButton *closeButton = new QPushButton("关闭", &dialog);

    buttonLayout->addWidget(exportButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    connect(exportButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getSaveFileName(&dialog, "保存报表",
                                                        QDir::homePath() +
                                                            QString("/%1_所有学生成绩排名.xlsx")
                                                                .arg(subjectName),
                                                        "Excel文件 (*.xlsx)");
        if (!fileName.isEmpty()) {
            QMessageBox::information(&dialog, "提示", "导出功能正在开发中...");
        }
    });

    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

// 7. 辅助方法
double TeacherMainWindow::calculateGPA(double score)
{
    // 简单的GPA计算方法
    if (score >= 90) return 4.0;
    else if (score >= 80) return 3.0;
    else if (score >= 70) return 2.0;
    else if (score >= 60) return 1.0;
    else return 0.0;
}

QString TeacherMainWindow::getCurrentSemester()
{
    QDate currentDate = QDate::currentDate();
    int year = currentDate.year();
    int month = currentDate.month();

    // 假设学期1：9月-次年1月，学期2：2月-6月
    if (month >= 9 || month <= 1) {
        return QString("%1-%2").arg(year).arg(1);
    } else {
        return QString("%1-%2").arg(year).arg(2);
    }
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
    // QString keyword = ui->lineEdit->text().trimmed();
    // searchStudents(keyword);
}


void TeacherMainWindow::on_pushButton_search_clicked()
{
    on_lineEdit_search_returnPressed();
}


void TeacherMainWindow::on_listWidget_classes_itemClicked(QListWidgetItem *item)
{
    QString text = item->text();

    if (text == "+ 添加班级") {
        // 弹出输入对话框添加新班级
        bool ok;
        QString className = QInputDialog::getText(this, "添加班级",
                                                  "请输入班级名称:",
                                                  QLineEdit::Normal,
                                                  "", &ok);

        if (ok && !className.trimmed().isEmpty()) {
            // 检查班级是否已存在
            for (int i = 0; i < ui->listWidget_classes->count() - 1; i++) { // -1 排除"添加班级"项
                QListWidgetItem *existingItem = ui->listWidget_classes->item(i);
                if (existingItem->text() == className) {
                    QMessageBox::warning(this, "提示", "该班级已存在！");
                    return;
                }
            }

            // 插入新班级（在"全部学生"之后，"添加班级"之前）
            int insertIndex = ui->listWidget_classes->count() - 1;
            QListWidgetItem *newItem = new QListWidgetItem(className);
            ui->listWidget_classes->insertItem(insertIndex, newItem);
            ui->listWidget_classes->setCurrentItem(newItem);

            // 更新显示
            loadClassStudents(className);
        }
    } else if (text == "全部学生") {
        // 显示所有学生
        refreshStudentList();
    } else {
        // 显示指定班级的学生
        loadClassStudents(text);
    }
}


void TeacherMainWindow::on_tableWidget_students_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    // 从表格获取学生信息
    QString studentId = ui->tableWidget_students->item(row, 0)->text();
    QString name = ui->tableWidget_students->item(row, 1)->text();
    QString gender = ui->tableWidget_students->item(row, 2)->text();
    int age = ui->tableWidget_students->item(row, 3)->text().toInt();
    QString className = ui->tableWidget_students->item(row, 4)->text();
    QString phone = ui->tableWidget_students->item(row, 5)->text();
    QString address = ui->tableWidget_students->item(row, 6)->text();

    // 修改：传递教师ID
    EditStudentDialog dialog(m_teacherId, studentId, name, gender, age, className, phone, address, this);

    // 获取当前所有班级列表
    QStringList classList;
    for (int i = 1; i < ui->listWidget_classes->count() - 1; i++) {
        QListWidgetItem *item = ui->listWidget_classes->item(i);
        if (item && item->text() != "全部学生" && item->text() != "+ 添加班级") {
            classList.append(item->text());
        }
    }

    // 设置班级下拉框选项
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
            QMessageBox::information(this, "成功", "学生信息更新成功！");
            refreshStudentList();
        } else {
            QMessageBox::critical(this, "错误", "更新失败！");
        }
    }
}




void TeacherMainWindow::on_action_addStudent_triggered()
{
    AddStudentDialog dialog(this);

    // 获取当前所有班级列表
    QStringList classList;
    for (int i = 1; i < ui->listWidget_classes->count() - 1; i++) {
        QListWidgetItem *item = ui->listWidget_classes->item(i);
        if (item && item->text() != "全部学生" && item->text() != "+ 添加班级") {
            classList.append(item->text());
        }
    }

    // 设置班级下拉框选项
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
            refreshStudentList();
        } else {
            QMessageBox::critical(this, "错误", "添加失败！");
        }
    }
}


void TeacherMainWindow::on_action_deleteStudent_triggered()
{
    int row = ui->tableWidget_students->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "警告", "请先选择要删除的学生！");
        return;
    }

    // 获取选中学生的信息
    QString studentId = ui->tableWidget_students->item(row, 0)->text();
    QString studentName = ui->tableWidget_students->item(row, 1)->text();

    // 显示确认删除对话框
    ConfirmDeleteDialog dialog(studentId, studentName, this);

    if (dialog.exec() == QDialog::Accepted) {
        // 用户点击了"确认删除"
        if (deleteStudent(studentId)) {
            QMessageBox::information(this, "成功", "学生删除成功！");
            refreshStudentList();
        } else {
            QMessageBox::critical(this, "错误", "删除失败！");
        }
    }
    // 如果用户点击"取消删除"，什么都不做
}


void TeacherMainWindow::on_action_editStudent_triggered()
{
    int row = ui->tableWidget_students->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的学生！");
        return;
    }

    // 从表格获取当前学生的信息
    QString studentId = ui->tableWidget_students->item(row, 0)->text();
    QString name = ui->tableWidget_students->item(row, 1)->text();
    QString gender = ui->tableWidget_students->item(row, 2)->text();
    int age = ui->tableWidget_students->item(row, 3)->text().toInt();
    QString className = ui->tableWidget_students->item(row, 4)->text();
    QString phone = ui->tableWidget_students->item(row, 5)->text();
    QString address = ui->tableWidget_students->item(row, 6)->text();

    // 修改：传递教师ID
    EditStudentDialog dialog(m_teacherId, studentId, name, gender, age, className, phone, address, this);

    // 获取当前所有班级列表
    QStringList classList;
    for (int i = 1; i < ui->listWidget_classes->count() - 1; i++) {
        QListWidgetItem *item = ui->listWidget_classes->item(i);
        if (item && item->text() != "全部学生" && item->text() != "+ 添加班级") {
            classList.append(item->text());
        }
    }

    // 设置班级下拉框选项
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
            QMessageBox::information(this, "成功", "学生信息更新成功！");
            refreshStudentList();
        } else {
            QMessageBox::critical(this, "错误", "更新失败！");
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
    DatabaseManager* dbManager = DatabaseManager::instance();
    QString username = dbManager->getUsernameByUserId(m_teacherId);

    if (username.isEmpty()) {
        QMessageBox::warning(this, "警告", "无法获取当前用户信息！");
        return;
    }

    ChangePasswordDialog dialog(username, this);
    dialog.exec();
}

void TeacherMainWindow::on_action_changequestions_triggered()
{
    DatabaseManager* dbManager = DatabaseManager::instance();
    QString username = dbManager->getUsernameByUserId(m_teacherId);

    if (username.isEmpty()) {
        QMessageBox::warning(this, "警告", "无法获取当前用户信息！");
        return;
    }

    ChangeQuestionsDialog dialog(username, this);
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
        // 重命名班级
        bool ok;
        QString newName = QInputDialog::getText(this, "重命名班级",
                                                "请输入新的班级名称:",
                                                QLineEdit::Normal,
                                                text, &ok);

        if (ok && !newName.trimmed().isEmpty() && newName != text) {
            // 检查新名称是否已存在
            for (int i = 0; i < ui->listWidget_classes->count() - 1; i++) {
                QListWidgetItem *existingItem = ui->listWidget_classes->item(i);
                if (existingItem != item && existingItem->text() == newName) {
                    QMessageBox::warning(this, "提示", "该班级名称已存在！");
                    return;
                }
            }

            // 更新数据库中的班级名称
            QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
            if (db.isOpen()) {
                QSqlQuery query(db);
                query.prepare("UPDATE students SET class_name = ? WHERE class_name = ?");
                query.addBindValue(newName);
                query.addBindValue(text);

                if (query.exec()) {
                    item->setText(newName);
                    QMessageBox::information(this, "成功", "班级重命名成功！");
                }
            }
        }
    } else if (selectedAction == deleteAction) {
        // 删除班级确认
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认删除",
                                      QString("确定要删除班级【%1】吗？\n（不会删除学生，只会移除班级分类）").arg(text),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // 将班级学生的班级字段清空
            QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
            if (db.isOpen()) {
                QSqlQuery query(db);
                query.prepare("UPDATE students SET class_name = '' WHERE class_name = ?");
                query.addBindValue(text);

                if (query.exec()) {
                    delete item; // 从列表移除
                    QMessageBox::information(this, "成功", "班级已删除！");
                }
            }
        }
    }
}


void TeacherMainWindow::on_action_add_class_triggered()
{
    // 模拟点击"添加班级"按钮
    QListWidgetItem *addItem = nullptr;

    // 找到"+ 添加班级"这个item
    for (int i = 0; i < ui->listWidget_classes->count(); i++) {
        QListWidgetItem *item = ui->listWidget_classes->item(i);
        if (item->text() == "+ 添加班级") {
            addItem = item;
            break;
        }
    }

    if (addItem) {
        // 模拟点击这个item
        ui->listWidget_classes->setCurrentItem(addItem);
        // 触发itemClicked信号（会由on_listWidget_classes_itemClicked处理）
        emit ui->listWidget_classes->itemClicked(addItem);
    }
}


void TeacherMainWindow::on_action_edit_class_triggered()
{
    // 获取当前选中的班级
    QListWidgetItem *currentItem = ui->listWidget_classes->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "提示", "请先选择一个班级");
        return;
    }

    QString text = currentItem->text();
    if (text == "全部学生" || text == "+ 添加班级") {
        QMessageBox::information(this, "提示", "该条目不可编辑");
        return;
    }

    // 直接调用已有的右键菜单逻辑
    // 模拟右键菜单中的重命名操作
    bool ok;
    QString newName = QInputDialog::getText(this, "重命名班级",
                                            "请输入新的班级名称:",
                                            QLineEdit::Normal,
                                            text, &ok);

    if (ok && !newName.trimmed().isEmpty() && newName != text) {
        // 检查新名称是否已存在
        for (int i = 0; i < ui->listWidget_classes->count() - 1; i++) {
            QListWidgetItem *existingItem = ui->listWidget_classes->item(i);
            if (existingItem != currentItem && existingItem->text() == newName) {
                QMessageBox::warning(this, "提示", "该班级名称已存在！");
                return;
            }
        }

        // 更新数据库中的班级名称
        QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
        if (db.isOpen()) {
            QSqlQuery query(db);
            query.prepare("UPDATE students SET class_name = ? WHERE class_name = ?");
            query.addBindValue(newName);
            query.addBindValue(text);

            if (query.exec()) {
                currentItem->setText(newName);
                QMessageBox::information(this, "成功", "班级重命名成功！");
            }
        }
    }
}


void TeacherMainWindow::on_action_delete_class_triggered()
{
    // 获取当前选中的班级
    QListWidgetItem *currentItem = ui->listWidget_classes->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "提示", "请先选择一个班级");
        return;
    }

    QString text = currentItem->text();
    if (text == "全部学生" || text == "+ 添加班级") {
        QMessageBox::information(this, "提示", "该条目不可删除");
        return;
    }

    // 直接调用已有的删除逻辑
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除班级【%1】吗？\n（不会删除学生，只会移除班级分类）").arg(text),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 将班级学生的班级字段清空
        QSqlDatabase db = DatabaseManager::instance()->getTeacherDatabase(m_teacherId);
        if (db.isOpen()) {
            QSqlQuery query(db);
            query.prepare("UPDATE students SET class_name = '' WHERE class_name = ?");
            query.addBindValue(text);

            if (query.exec()) {
                delete currentItem; // 从列表移除
                QMessageBox::information(this, "成功", "班级已删除！");
            }
        }
    }
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



