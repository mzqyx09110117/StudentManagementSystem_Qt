#include "restoredialog.h"
#include "ui_restoredialog.h"

#include "databasemanager.h"

#include <QApplication>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

RestoreDialog::RestoreDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RestoreDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("数据库恢复"));

    connect(ui->pushButton_selectBackup, &QPushButton::clicked,
            this, &RestoreDialog::on_pushButton_selectBackup_clicked);
    connect(ui->pushButton_analyzeBackup, &QPushButton::clicked,
            this, &RestoreDialog::on_pushButton_analyzeBackup_clicked);
    connect(ui->pushButton_startRestore, &QPushButton::clicked,
            this, &RestoreDialog::on_pushButton_startRestore_clicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked,
            this, &RestoreDialog::on_pushButton_cancel_clicked);

    ui->pushButton_startRestore->setEnabled(false);
    ui->label_restoreStatus->setText(QStringLiteral("请选择备份文件并点击“校验备份”"));
    ui->label_backupTime->setText(QStringLiteral("未选择"));
    const QString currentDb = DatabaseManager::instance()->databasePath();
    const qint64 currentSize = QFileInfo(currentDb).size()
                               + QFileInfo(currentDb + QStringLiteral("-wal")).size();
    ui->label_systemDbRestore->setText(QStringLiteral("StudentSystem.db（%1 KB）")
                                           .arg(currentSize / 1024.0, 0, 'f', 1));
    ui->label_systemDbRestore->setToolTip(currentDb);
    ui->label_teacherDbRestore->setText(QStringLiteral("选择备份后显示"));
    ui->label_totalRestore->setText(QStringLiteral("--"));
    ui->plainTextEdit_teachers->setPlainText(
        QStringLiteral("恢复会用备份覆盖当前数据库：\n"
                       "1. 先校验备份完整性与模式版本\n"
                       "2. 自动保存当前数据库的 .bak 安全副本\n"
                       "3. 替换数据库并执行完整性/外键检查\n"
                       "4. 任一环节失败会自动回滚"));
    ui->lineEdit_backupPath->setPlaceholderText(QStringLiteral("请选择 .db 备份文件"));

    for (QLabel *label : {ui->label_systemDbRestore, ui->label_backupTime,
                          ui->label_teacherDbRestore, ui->label_totalRestore,
                          ui->label_restoreStatus}) {
        label->setStyleSheet(QStringLiteral("font-size:9pt;"));
    }
}

RestoreDialog::~RestoreDialog()
{
    delete ui;
}

void RestoreDialog::on_pushButton_selectBackup_clicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this, QStringLiteral("选择备份文件"),
        DatabaseManager::instance()->dataDirectory(),
        QStringLiteral("SQLite 数据库 (*.db)"));
    if (filePath.isEmpty())
        return;

    m_backupPath = filePath;
    m_validated = false;
    ui->lineEdit_backupPath->setText(filePath);
    ui->pushButton_startRestore->setEnabled(false);
    ui->label_restoreStatus->setText(QStringLiteral("已选择文件，请点击“校验备份”"));
}

void RestoreDialog::on_pushButton_analyzeBackup_clicked()
{
    m_backupPath = ui->lineEdit_backupPath->text().trimmed();
    if (m_backupPath.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("请先选择备份文件"));
        return;
    }

    QString error;
    DatabaseManager *manager = DatabaseManager::instance();
    if (!manager->verifyBackup(m_backupPath, &error)) {
        m_validated = false;
        ui->pushButton_startRestore->setEnabled(false);
        ui->label_restoreStatus->setText(QStringLiteral("校验失败：%1").arg(error));
        QMessageBox::critical(this, QStringLiteral("校验失败"), error);
        return;
    }

    m_validated = true;
    const QFileInfo info(m_backupPath);

    // 只读预览备份内容
    const QString connection = QStringLiteral("RESTORE_PREVIEW");
    int teachers = 0, students = 0, subjects = 0, grades = 0;
    QString versionText;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connection);
        db.setDatabaseName(info.absoluteFilePath());
        db.setConnectOptions(QStringLiteral("QSQLITE_OPEN_READONLY"));
        if (db.open()) {
            QSqlQuery q(db);
            auto count = [&q](const QString &table) -> int {
                if (q.exec(QStringLiteral("SELECT COUNT(*) FROM %1").arg(table)) && q.next())
                    return q.value(0).toInt();
                return 0;
            };
            teachers = count(QStringLiteral("teachers"));
            students = count(QStringLiteral("students"));
            subjects = count(QStringLiteral("subjects"));
            grades = count(QStringLiteral("grades"));
            if (q.exec(QStringLiteral("SELECT version FROM schema_meta WHERE id = 1")) && q.next())
                versionText = q.value(0).toString();
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(connection);

    ui->label_backupTime->setText(
        info.lastModified().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")));
    ui->label_teacherDbRestore->setText(
        QStringLiteral("教师 %1 · 学生 %2 · 课程 %3 · 成绩 %4")
            .arg(teachers).arg(students).arg(subjects).arg(grades));
    ui->label_totalRestore->setText(
        QStringLiteral("%1 KB · 模式版本 %2")
            .arg(info.size() / 1024.0, 0, 'f', 1)
            .arg(versionText));
    ui->plainTextEdit_teachers->setPlainText(
        QStringLiteral("备份文件：%1\n大小：%2 KB\n\n"
                       "恢复步骤：\n"
                       "1. 校验备份文件完整性（已通过）\n"
                       "2. 为当前数据库创建 .bak 安全副本\n"
                       "3. 关闭连接并替换数据库文件\n"
                       "4. 重新打开并执行 integrity_check / foreign_key_check\n"
                       "5. 任一环节失败会自动回滚到安全副本")
            .arg(info.absoluteFilePath())
            .arg(info.size() / 1024.0, 0, 'f', 1));
    ui->label_restoreStatus->setText(QStringLiteral("校验通过，可以开始恢复"));
    ui->pushButton_startRestore->setEnabled(true);
}

void RestoreDialog::on_pushButton_startRestore_clicked()
{
    if (!m_validated) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("请先校验备份文件"));
        return;
    }

    QMessageBox box(QMessageBox::Warning, QStringLiteral("确认恢复"),
                    QStringLiteral("恢复会用备份覆盖当前数据库，确定继续吗？"),
                    QMessageBox::Yes | QMessageBox::No, this);
    box.setInformativeText(QStringLiteral(
        "当前数据库会自动保存为 .bak 文件；恢复成功后程序将退出，请重新启动。"));
    if (box.exec() != QMessageBox::Yes)
        return;

    ui->pushButton_startRestore->setEnabled(false);
    ui->label_restoreStatus->setText(QStringLiteral("正在恢复..."));

    QString error;
    DatabaseManager *manager = DatabaseManager::instance();
    if (!manager->restoreFrom(m_backupPath, &error)) {
        ui->pushButton_startRestore->setEnabled(true);
        ui->label_restoreStatus->setText(QStringLiteral("恢复失败：%1").arg(error));
        QMessageBox::critical(this, QStringLiteral("恢复失败"), error);
        return;
    }

    ui->label_restoreStatus->setText(QStringLiteral("恢复成功，请重新启动程序"));
    QMessageBox::information(this, QStringLiteral("恢复成功"),
                             QStringLiteral("数据库恢复成功。\n程序将退出，请重新启动后使用。"));
    accept();
    QApplication::quit();
}

void RestoreDialog::on_pushButton_cancel_clicked()
{
    reject();
}
