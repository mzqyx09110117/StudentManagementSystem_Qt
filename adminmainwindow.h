#ifndef ADMINMAINWINDOW_H
#define ADMINMAINWINDOW_H

#include "databasemanager.h"
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QDebug>


namespace Ui {
class AdminMainWindow;
}

class AdminMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminMainWindow(const QString &adminId, QWidget *parent = nullptr);
    ~AdminMainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionDelete_triggered();
    void on_actionRefresh_triggered();
    void on_actionBackup_triggered();
    void on_actionRestore_triggered();
    void on_actionExit_triggered();
    void on_tableWidget_itemSelectionChanged();
    void on_action_new_triggered();
    void on_action_delete_triggered();
    void on_action_backup_triggered();
    void on_action_restore_triggered();
    void on_action_exit_triggered();
    void on_action_refresh_triggered();
    void on_action_dataanalysis_triggered();
    void on_action_changepassword_triggered();
    void on_action_changequestoions_triggered();

private:
    Ui::AdminMainWindow *ui;
    DatabaseManager *m_dbManager;
    QString m_adminId;
    QString m_searchKeyword;

    void initUI();
    void loadTeacherData();
    void deleteSelectedTeacher();
    void openStatistics();
    void openAuditLog();
};

#endif // ADMINMAINWINDOW_H
