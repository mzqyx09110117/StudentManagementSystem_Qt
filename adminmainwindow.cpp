#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"

#include "auditlogdialog.h"
#include "backupdialog.h"
#include "changepassworddialog.h"
#include "changequestionsdialog.h"
#include "registerdialog.h"
#include "restoredialog.h"
#include "statsdialog.h"

#include <QAction>
#include <QApplication>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QVariant>

AdminMainWindow::AdminMainWindow(const QString &adminId, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminMainWindow)
    , m_dbManager(DatabaseManager::instance())
    , m_adminId(adminId)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("学生信息管理系统 - 管理员端"));
    initUI();
    loadTeacherData();
}

AdminMainWindow::~AdminMainWindow()
{
    delete ui;
}

void AdminMainWindow::initUI()
{
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()
        << QStringLiteral("教师ID") << QStringLiteral("姓名") << QStringLiteral("账号")
        << QStringLiteral("任课课程数") << QStringLiteral("创建时间") << QStringLiteral("状态"));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 130);
    ui->tableWidget->setColumnWidth(2, 150);
    ui->tableWidget->setColumnWidth(3, 110);
    ui->tableWidget->setColumnWidth(4, 180);

    // 单页即可容纳全部教师，隐藏原设计中的死分页控件
    const QList<QPushButton *> pageButtons = {
        ui->pushButton_page1, ui->pushButton_page2, ui->pushButton_page3,
        ui->pushButton_page4, ui->pushButton_page5};
    for (QPushButton *button : pageButtons)
        button->setVisible(false);
    ui->pushButton_prev->setVisible(false);
    ui->pushButton_next->setVisible(false);
    ui->pushButton_ellipsis->setVisible(false);
    ui->label_pageInfo->setVisible(false);
    if (ui->comboBox)
        ui->comboBox->setVisible(false);

    connect(ui->lineEdit_search, &QLineEdit::returnPressed, this, [this]() {
        m_searchKeyword = ui->lineEdit_search->text().trimmed();
        loadTeacherData();
    });
    connect(ui->pushButton_search, &QPushButton::clicked, this, [this]() {
        m_searchKeyword = ui->lineEdit_search->text().trimmed();
        loadTeacherData();
    });

    // 统计分析菜单
    QMenu *statsMenu = menuBar()->addMenu(QStringLiteral("统计分析"));
    QAction *dashboardAction = statsMenu->addAction(QStringLiteral("数据看板"));
    QAction *auditAction = statsMenu->addAction(QStringLiteral("审计日志"));
    connect(dashboardAction, &QAction::triggered, this, &AdminMainWindow::openStatistics);
    connect(auditAction, &QAction::triggered, this, &AdminMainWindow::openAuditLog);
}

void AdminMainWindow::loadTeacherData()
{
    ui->tableWidget->setRowCount(0);

    QString sql = QStringLiteral(
        "SELECT u.user_id, t.name, u.username, COUNT(DISTINCT sub.subject_id) AS course_count, "
        "u.created_time, u.is_active "
        "FROM users u "
        "JOIN teachers t ON t.teacher_id = u.user_id "
        "LEFT JOIN subjects sub ON sub.teacher_id = u.user_id "
        "WHERE u.user_type = 'teacher'");
    QVariantList params;
    if (!m_searchKeyword.isEmpty()) {
        const QString like = QStringLiteral("%") + m_searchKeyword + QStringLiteral("%");
        sql += QStringLiteral(" AND (u.user_id LIKE ? OR u.username LIKE ? OR t.name LIKE ?)");
        params << like << like << like;
    }
    sql += QStringLiteral(
        " GROUP BY u.user_id, t.name, u.username, u.created_time, u.is_active "
        "ORDER BY u.created_time DESC");

    QSqlQuery query(m_dbManager->database());
    query.prepare(sql);
    for (const QVariant &param : params)
        query.addBindValue(param);

    if (!query.exec()) {
        QMessageBox::warning(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 6; ++col) {
            QString text = query.value(col).toString();
            if (col == 5)
                text = (query.value(5).toInt() == 1) ? QStringLiteral("启用")
                                                     : QStringLiteral("停用");
            auto *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->tableWidget->setItem(row, col, item);
        }
        ++row;
    }

    ui->statusbar->showMessage(QStringLiteral("共 %1 名教师").arg(row));
}

void AdminMainWindow::deleteSelectedTeacher()
{
    const int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("请先选择要删除的教师"));
        return;
    }

    QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
    QTableWidgetItem *nameItem = ui->tableWidget->item(row, 1);
    const QString teacherId = idItem ? idItem->text() : QString();
    const QString teacherName = nameItem ? nameItem->text() : QString();
    if (teacherId.isEmpty())
        return;

    int subjectCount = 0;
    QSqlQuery countQuery(m_dbManager->database());
    countQuery.prepare(QStringLiteral(
        "SELECT COUNT(*) FROM subjects WHERE teacher_id = ?"));
    countQuery.addBindValue(teacherId);
    if (countQuery.exec() && countQuery.next())
        subjectCount = countQuery.value(0).toInt();

    QMessageBox box(QMessageBox::Question, QStringLiteral("确认删除"),
                    QStringLiteral("确定要删除教师【%1 - %2】吗？").arg(teacherName, teacherId),
                    QMessageBox::Yes | QMessageBox::No, this);
    box.setInformativeText(QStringLiteral(
        "该教师的 %1 门课程将变为未分配，历史成绩和审计日志会保留；账号删除后不可恢复。")
                               .arg(subjectCount));
    if (box.exec() != QMessageBox::Yes)
        return;

    if (!m_dbManager->deleteTeacher(teacherId)) {
        QMessageBox::critical(this, QStringLiteral("删除失败"), m_dbManager->lastError());
        return;
    }

    loadTeacherData();
    QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("教师删除成功"));
}

void AdminMainWindow::openStatistics()
{
    StatsDialog dialog(this);
    dialog.exec();
}

void AdminMainWindow::openAuditLog()
{
    AuditLogDialog dialog(this);
    dialog.exec();
}

void AdminMainWindow::on_actionNew_triggered()
{
    RegisterDialog registerDialog(this);
    registerDialog.exec();
    loadTeacherData();
}

void AdminMainWindow::on_actionDelete_triggered()
{
    deleteSelectedTeacher();
}

void AdminMainWindow::on_actionRefresh_triggered()
{
    loadTeacherData();
}

void AdminMainWindow::on_actionBackup_triggered()
{
    BackupDialog dialog(this);
    dialog.exec();
}

void AdminMainWindow::on_actionRestore_triggered()
{
    RestoreDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
        loadTeacherData();
}

void AdminMainWindow::on_actionExit_triggered()
{
    if (QMessageBox::question(this, QStringLiteral("确认退出"),
                              QStringLiteral("确定要退出系统吗？"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void AdminMainWindow::on_tableWidget_itemSelectionChanged() {}

void AdminMainWindow::on_action_new_triggered() { on_actionNew_triggered(); }
void AdminMainWindow::on_action_delete_triggered() { on_actionDelete_triggered(); }
void AdminMainWindow::on_action_backup_triggered() { on_actionBackup_triggered(); }
void AdminMainWindow::on_action_restore_triggered() { on_actionRestore_triggered(); }
void AdminMainWindow::on_action_exit_triggered() { on_actionExit_triggered(); }
void AdminMainWindow::on_action_refresh_triggered() { on_actionRefresh_triggered(); }

void AdminMainWindow::on_action_dataanalysis_triggered()
{
    openStatistics();
}

void AdminMainWindow::on_action_changepassword_triggered()
{
    ChangePasswordDialog dialog(m_adminId, false, this);
    dialog.exec();
}

void AdminMainWindow::on_action_changequestoions_triggered()
{
    ChangeQuestionsDialog dialog(m_adminId, this);
    dialog.exec();
}
