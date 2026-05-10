#include "adminmainwindow.h"
#include "logindialog.h"
#include "databasemanager.h"
#include "adminmainwindow.h"
#include "teachermainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用程序名称
    a.setApplicationName("教师管理系统");
    a.setOrganizationName("MyCompany");

    // 初始化数据库管理器
    if (!DatabaseManager::instance()->initializeDatabases()) {
        QMessageBox::critical(nullptr, "系统错误",
                              "数据库初始化失败！\n程序将退出。");
        return -1;
    }

    // 显示登录对话框
    LoginDialog loginDialog;

    // 设置登录对话框为模态
    if (loginDialog.exec() == QDialog::Accepted) {
        // 获取登录成功的用户信息
        QString userType = loginDialog.getUserType();
        QString userId = loginDialog.getUserId();

        qDebug() << "用户登录成功，类型：" << userType << "，ID：" << userId;

        // 根据用户类型显示不同的主窗口
        if (userType == "admin") {
            AdminMainWindow *adminWindow = new AdminMainWindow(userId);
            adminWindow->show();
        } else if (userType == "teacher") {
            TeacherMainWindow *teacherWindow = new TeacherMainWindow(userId);
            teacherWindow->show();
        } else {
            QMessageBox::critical(nullptr, "系统错误", "未知的用户类型");
            return -1;
        }

        return a.exec();
    } else {
        qDebug() << "用户取消登录";
    }

    DatabaseManager::destroy();

    return 0;
}
