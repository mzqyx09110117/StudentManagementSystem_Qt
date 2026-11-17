#include "changequestionsdialog.h"
#include "ui_changequestionsdialog.h"

#include "databasemanager.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

ChangeQuestionsDialog::ChangeQuestionsDialog(const QString &userId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeQuestionsDialog)
    , m_userId(userId)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("修改密保问题"));

    DatabaseManager *manager = DatabaseManager::instance();
    const QString username = manager->getUsernameByUserId(userId);
    const QPair<QString, QString> questions = manager->getSecurityQuestions(username);

    if (!questions.first.isEmpty())
        ui->lineEdit_question1->setText(questions.first);
    else
        ui->lineEdit_question1->setPlaceholderText(QStringLiteral("例如：你最喜欢的颜色是什么？"));

    if (!questions.second.isEmpty())
        ui->lineEdit_question2->setText(questions.second);
    else
        ui->lineEdit_question2->setPlaceholderText(QStringLiteral("例如：你第一只宠物的名字是什么？"));
}

ChangeQuestionsDialog::~ChangeQuestionsDialog()
{
    delete ui;
}

void ChangeQuestionsDialog::on_pushButton_confirmquestions_clicked()
{
    const QString question1 = ui->lineEdit_question1->text().trimmed();
    const QString answer1 = ui->lineEdit_answer1->text().trimmed();
    const QString question2 = ui->lineEdit_question2->text().trimmed();
    const QString answer2 = ui->lineEdit_answer2->text().trimmed();

    if (question1.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请输入密保问题 1"));
        ui->lineEdit_question1->setFocus();
        return;
    }
    if (answer1.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请输入密保答案 1"));
        ui->lineEdit_answer1->setFocus();
        return;
    }
    if (question2.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请输入密保问题 2"));
        ui->lineEdit_question2->setFocus();
        return;
    }
    if (answer2.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请输入密保答案 2"));
        ui->lineEdit_answer2->setFocus();
        return;
    }
    if (question1 == question2) {
        QMessageBox::warning(this, QStringLiteral("警告"),
                             QStringLiteral("两个密保问题不能相同"));
        ui->lineEdit_question2->setFocus();
        return;
    }

    bool ok = false;
    const QString password = QInputDialog::getText(
        this, QStringLiteral("身份验证"), QStringLiteral("请输入当前密码："),
        QLineEdit::Password, QString(), &ok);
    if (!ok)
        return;
    if (password.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("请输入当前密码"));
        return;
    }

    DatabaseManager *manager = DatabaseManager::instance();
    if (manager->changeSecurityQuestions(m_userId, password, question1, answer1,
                                         question2, answer2)) {
        QMessageBox::information(this, QStringLiteral("成功"),
                                 QStringLiteral("密保问题修改成功"));
        accept();
    } else {
        QMessageBox::critical(this, QStringLiteral("修改失败"), manager->lastError());
    }
}

void ChangeQuestionsDialog::on_pushButton_cancel_clicked()
{
    reject();
}
