#include "changequestionsdialog.h"
#include "ui_changequestionsdialog.h"
#include "databasemanager.h"


ChangeQuestionsDialog::ChangeQuestionsDialog(const QString &username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeQuestionsDialog)
    , m_username(username)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("修改密保问题"));

    // 加载现有密保问题
    DatabaseManager* dbManager = DatabaseManager::instance();
    auto questions = dbManager->getSecurityQuestions(username);

    if (!questions.first.isEmpty() && !questions.second.isEmpty()) {
        ui->lineEdit_question1->setText(questions.first);
        ui->lineEdit_question2->setText(questions.second);
    } else {
        ui->lineEdit_question1->setText("未设置");
        ui->lineEdit_question2->setText("未设置");
    }
}

ChangeQuestionsDialog::~ChangeQuestionsDialog()
{
    delete ui;
}

void ChangeQuestionsDialog::on_pushButton_confirmquestions_clicked()
{
    QString newQuestion1 = ui->lineEdit_question1->text().trimmed();
    QString newAnswer1 = ui->lineEdit_answer1->text().trimmed();
    QString newQuestion2 = ui->lineEdit_question2->text().trimmed();
    QString newAnswer2 = ui->lineEdit_answer2->text().trimmed();

    // 验证输入
    if (newQuestion1.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入密保问题1！");
        ui->lineEdit_question1->setFocus();
        return;
    }

    if (newAnswer1.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入密保答案1！");
        ui->lineEdit_answer1->setFocus();
        return;
    }

    if (newQuestion2.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入密保问题2！");
        ui->lineEdit_question2->setFocus();
        return;
    }

    if (newAnswer2.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入密保答案2！");
        ui->lineEdit_answer2->setFocus();
        return;
    }

    // 两个问题不能相同
    if (newQuestion1 == newQuestion2) {
        QMessageBox::warning(this, "警告", "两个密保问题不能相同！");
        ui->lineEdit_question2->setFocus();
        return;
    }

    // 更新到数据库
    QSqlDatabase db = DatabaseManager::instance()->getSystemDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "错误", "数据库连接失败！");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE users SET question1 = ?, answer1 = ?, "
                  "question2 = ?, answer2 = ? WHERE username = ?");
    query.addBindValue(newQuestion1);
    query.addBindValue(newAnswer1);
    query.addBindValue(newQuestion2);
    query.addBindValue(newAnswer2);
    query.addBindValue(m_username);

    if (query.exec()) {
        QMessageBox::information(this, "成功", "密保问题修改成功！");

        // 更新显示当前密保问题
        ui->lineEdit_question1->setText(newQuestion1);
        ui->lineEdit_question2->setText(newQuestion2);

        accept();
    } else {
        QMessageBox::critical(this, "错误", "修改失败：" + query.lastError().text());
    }
}


void ChangeQuestionsDialog::on_pushButton_cancel_clicked()
{
    reject();
}

