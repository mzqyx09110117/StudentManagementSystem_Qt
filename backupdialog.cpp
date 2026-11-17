#include "backupdialog.h"
#include "ui_backupdialog.h"

#include "databasemanager.h"

#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

namespace {

QString humanSize(qint64 bytes)
{
    if (bytes < 1024)
        return QStringLiteral("%1 B").arg(bytes);
    if (bytes < 1024 * 1024)
        return QStringLiteral("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    return QStringLiteral("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 2);
}

} // namespace

BackupDialog::BackupDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BackupDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("数据库备份"));

    const QString documentDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    const QString defaultPath = QDir(documentDir).filePath(
        QStringLiteral("student_backup_")
        + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss"))
        + QStringLiteral(".db"));
    ui->lineEdit_backupPath->setText(defaultPath);
    ui->lineEdit_backupPath->setCursorPosition(0);

    for (QLabel *label : {ui->label_systemDbInfo, ui->label_teacherDbInfo,
                          ui->label_totalSize, ui->label_status}) {
        label->setStyleSheet(QStringLiteral("font-size:9pt;"));
    }

    connect(ui->pushButton_selectFolder, &QPushButton::clicked,
            this, &BackupDialog::on_pushButton_selectFolder_clicked);
    connect(ui->pushButton_startBackup, &QPushButton::clicked,
            this, &BackupDialog::on_pushButton_startBackup_clicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked,
            this, &BackupDialog::on_pushButton_cancel_clicked);

    updateInfo();
}

BackupDialog::~BackupDialog()
{
    delete ui;
}

void BackupDialog::updateInfo()
{
    DatabaseManager *manager = DatabaseManager::instance();
    const QString dbPath = manager->databasePath();
    const qint64 totalSize = QFileInfo(dbPath).size()
                             + QFileInfo(dbPath + QStringLiteral("-wal")).size();

    ui->label_systemDbInfo->setText(
        QStringLiteral("StudentSystem.db（%1）").arg(humanSize(totalSize)));
    ui->label_totalSize->setText(QStringLiteral("文件大小：%1").arg(humanSize(totalSize)));

    QSqlQuery query(manager->database());
    int students = 0, subjects = 0, grades = 0;
    if (query.exec(QStringLiteral("SELECT COUNT(*) FROM students")) && query.next())
        students = query.value(0).toInt();
    if (query.exec(QStringLiteral("SELECT COUNT(*) FROM subjects")) && query.next())
        subjects = query.value(0).toInt();
    if (query.exec(QStringLiteral("SELECT COUNT(*) FROM grades")) && query.next())
        grades = query.value(0).toInt();

    ui->label_teacherDbInfo->setText(
        QStringLiteral("学生 %1 · 课程 %2 · 成绩 %3").arg(students).arg(subjects).arg(grades));
    ui->label_status->setText(QStringLiteral("VACUUM INTO 一致性快照，可安全恢复"));
    ui->label_systemDbInfo->setToolTip(dbPath);
}

void BackupDialog::on_pushButton_selectFolder_clicked()
{
    const QString filePath = QFileDialog::getSaveFileName(
        this, QStringLiteral("选择备份文件"),
        ui->lineEdit_backupPath->text(),
        QStringLiteral("SQLite 数据库 (*.db)"));
    if (!filePath.isEmpty())
        ui->lineEdit_backupPath->setText(filePath);
}

void BackupDialog::on_pushButton_startBackup_clicked()
{
    const QString filePath = ui->lineEdit_backupPath->text().trimmed();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("提示"),
                             QStringLiteral("请选择备份文件路径"));
        return;
    }

    ui->pushButton_startBackup->setEnabled(false);
    ui->label_status->setText(QStringLiteral("正在备份..."));

    QString error;
    DatabaseManager *manager = DatabaseManager::instance();
    const bool ok = manager->backupTo(filePath, &error);

    ui->pushButton_startBackup->setEnabled(true);

    if (ok) {
        const QFileInfo info(filePath);
        ui->label_status->setText(QStringLiteral("备份成功：%1").arg(filePath));
        ui->label_totalSize->setText(QStringLiteral("备份文件大小：%1").arg(humanSize(info.size())));
        QMessageBox::information(this, QStringLiteral("备份成功"),
                                 QStringLiteral("数据库已备份到：\n%1\n\n大小：%2")
                                     .arg(filePath, humanSize(info.size())));
    } else {
        ui->label_status->setText(QStringLiteral("备份失败：%1").arg(error));
        QMessageBox::critical(this, QStringLiteral("备份失败"), error);
    }
}

void BackupDialog::on_pushButton_cancel_clicked()
{
    reject();
}
