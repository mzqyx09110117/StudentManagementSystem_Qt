#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"
#include "registerdialog.h"
#include "backupdialog.h"
#include "restoredialog.h"
#include "changepassworddialog.h"
#include "changequestionsdialog.h"

AdminMainWindow::AdminMainWindow(const QString &adminId, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminMainWindow),
    m_dbManager(DatabaseManager::instance()),
    m_adminId(adminId)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("教师管理系统 - 管理员模式"));

    initUI();
    loadTeacherData();
}

void AdminMainWindow::initUI()
{
    // 设置表格属性
    ui->tableWidget->setColumnCount(4);  // ID, 姓名, 账号, 创建时间
    QStringList headers;
    headers << "教师ID" << "姓名" << "账号" << "创建时间";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 设置表格样式
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置列宽
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setColumnWidth(2, 200);
    ui->tableWidget->setColumnWidth(3, 200);
}

void AdminMainWindow::loadTeacherData()
{
    // 清空表格
    ui->tableWidget->setRowCount(0);

    QSqlDatabase db = m_dbManager->getSystemDatabase();
    if (!db.isOpen()) {
        QMessageBox::warning(this, "警告", "无法连接到数据库");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT user_id, teacher_name, username, created_time FROM users WHERE user_type = 'teacher' ORDER BY created_time DESC");

    if (!query.exec()) {
        QMessageBox::warning(this, "查询失败", query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);

        // 教师ID
        QTableWidgetItem *itemId = new QTableWidgetItem(query.value(0).toString());
        ui->tableWidget->setItem(row, 0, itemId);

        // 教师姓名
        QTableWidgetItem *itemName = new QTableWidgetItem(query.value(1).toString());
        ui->tableWidget->setItem(row, 1, itemName);

        // 账号
        QTableWidgetItem *itemAccount = new QTableWidgetItem(query.value(2).toString());
        ui->tableWidget->setItem(row, 2, itemAccount);

        // 创建时间
        QTableWidgetItem *itemTime = new QTableWidgetItem(query.value(3).toString());
        ui->tableWidget->setItem(row, 3, itemTime);

        row++;
    }

    ui->statusbar->showMessage(QString("共 %1 名教师").arg(row));
}

AdminMainWindow::~AdminMainWindow()
{
    delete ui;
}

bool AdminMainWindow::deleteTeacherDatabase(const QString &teacherId)
{
    // 删除教师文件夹
    QString teacherDir = QDir::currentPath() + "/" + teacherId;
    QDir dir(teacherDir);

    if (dir.exists()) {
        // 先关闭数据库连接
        m_dbManager->closeTeacherDatabase(teacherId);

        // 删除文件夹及其内容
        return dir.removeRecursively();
    }

    return true;
}





void AdminMainWindow::on_actionNew_triggered()
{
    // 打开注册对话框
    RegisterDialog regDialog;
    regDialog.exec();

    on_actionRefresh_triggered();  // 用于注册之后及时刷新
}


void AdminMainWindow::on_actionDelete_triggered()
{
    // 获取当前选中的行
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "警告", "请先选择要删除的教师");
        return;
    }

    // 获取教师ID
    QString teacherId = ui->tableWidget->item(currentRow, 0)->text();
    QString teacherName = ui->tableWidget->item(currentRow, 1)->text();

    // 确认删除
    int reply = QMessageBox::question(this, "确认删除",
                                      QString("确定要删除教师 '%1' 吗？此操作不可撤销！").arg(teacherName),
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 从系统数据库中删除教师
        QSqlDatabase db = m_dbManager->getSystemDatabase();
        if (db.isOpen()) {
            QSqlQuery query(db);
            query.prepare("DELETE FROM users WHERE user_id = ? AND user_type = 'teacher'");
            query.addBindValue(teacherId);

            if (query.exec()) {
                // 删除教师个人数据库
                deleteTeacherDatabase(teacherId);

                QMessageBox::information(this, "成功", "教师删除成功！");
                loadTeacherData();  // 刷新列表
            } else {
                QMessageBox::warning(this, "失败", "删除失败：" + query.lastError().text());
            }
        }
    }
}


void AdminMainWindow::on_actionRefresh_triggered()
{
    loadTeacherData();
}


void AdminMainWindow::on_actionBackup_triggered()
{
    BackupDialog backupDialog(this);
    backupDialog.exec();
}


void AdminMainWindow::on_actionRestore_triggered()
{
    RestoreDialog restoreDialog(this);
    restoreDialog.exec();

    // 恢复后刷新教师列表
    if (restoreDialog.result() == QDialog::Accepted) {
        loadTeacherData();
    }
}


void AdminMainWindow::on_actionExit_triggered()
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


void AdminMainWindow::on_tableWidget_itemSelectionChanged()
{

}


void AdminMainWindow::on_action_new_triggered()
{
    on_actionNew_triggered();
}


void AdminMainWindow::on_action_delete_triggered()
{
    on_actionDelete_triggered();
}


void AdminMainWindow::on_action_backup_triggered()
{
    on_actionBackup_triggered();
}


void AdminMainWindow::on_action_restore_triggered()
{
    on_actionRestore_triggered();
}


void AdminMainWindow::on_action_exit_triggered()
{
    on_actionExit_triggered();
}



void AdminMainWindow::on_action_refresh_triggered()
{
    loadTeacherData();
}


void AdminMainWindow::on_action_dataanalysis_triggered()
{

}


void AdminMainWindow::on_action_changepassword_triggered()
{
    DatabaseManager* dbManager = DatabaseManager::instance();
    QString username = dbManager->getUsernameByUserId(m_adminId);

    if (username.isEmpty()) {
        QMessageBox::warning(this, "警告", "无法获取当前用户信息！");
        return;
    }

    ChangePasswordDialog dialog(username, this);
    dialog.exec();
}


void AdminMainWindow::on_action_changequestoions_triggered()
{
    DatabaseManager* dbManager = DatabaseManager::instance();
    QString username = dbManager->getUsernameByUserId(m_adminId);

    if (username.isEmpty()) {
        QMessageBox::warning(this, "警告", "无法获取当前用户信息！");
        return;
    }

    ChangeQuestionsDialog dialog(username, this);
    dialog.exec();
}

