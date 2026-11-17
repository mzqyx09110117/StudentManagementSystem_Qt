#include "studentmainwindow.h"

#include "changepassworddialog.h"
#include "changequestionsdialog.h"
#include "databasemanager.h"

#include <QAction>
#include <QApplication>
#include <QColor>
#include <QComboBox>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QStatusBar>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>

namespace {

QFrame *createStatCard(const QString &title, QLabel **valueLabel, QWidget *parent)
{
    auto *card = new QFrame(parent);
    card->setObjectName(QStringLiteral("statCard"));
    card->setStyleSheet(QStringLiteral(
        "QFrame#statCard { background:#ffffff; border:1px solid #e5e7eb; border-radius:8px; }"
        "QLabel { background:transparent; }"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(4);

    auto *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet(QStringLiteral("color:#6b7280;font-size:9pt;"));
    auto *value = new QLabel(QStringLiteral("--"), card);
    value->setStyleSheet(QStringLiteral("color:#111827;font-size:18pt;font-weight:600;"));
    layout->addWidget(titleLabel);
    layout->addWidget(value);
    *valueLabel = value;
    return card;
}

} // namespace

StudentMainWindow::StudentMainWindow(const QString &userId, QWidget *parent)
    : QMainWindow(parent)
    , m_userId(userId)
{
    buildUi();
    if (!loadStudent()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
                              QStringLiteral("未找到与当前账号关联的学生记录。"));
        QTimer::singleShot(0, this, &QWidget::close);
        return;
    }
    loadSummary();
    loadGrades();
}

StudentMainWindow::~StudentMainWindow() = default;

void StudentMainWindow::buildUi()
{
    setWindowTitle(QStringLiteral("学生信息管理系统 - 学生端"));
    resize(1180, 760);
    setMinimumSize(980, 640);

    auto *central = new QWidget(this);
    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(18, 16, 18, 12);
    root->setSpacing(12);

    // ---------- 个人信息卡 ----------
    auto *profile = new QFrame(central);
    profile->setObjectName(QStringLiteral("profileCard"));
    profile->setStyleSheet(QStringLiteral(
        "QFrame#profileCard { background:#ffffff; border:1px solid #e5e7eb; border-radius:10px; }"
        "QLabel { background:transparent; }"));
    auto *profileLayout = new QHBoxLayout(profile);
    profileLayout->setContentsMargins(22, 16, 22, 16);

    auto *nameBox = new QVBoxLayout();
    m_nameLabel = new QLabel(QStringLiteral("学生"), profile);
    m_nameLabel->setStyleSheet(QStringLiteral("font-size:20pt;font-weight:600;color:#111827;"));
    m_metaLabel = new QLabel(profile);
    m_metaLabel->setStyleSheet(QStringLiteral("color:#6b7280;font-size:10pt;"));
    nameBox->addWidget(m_nameLabel);
    nameBox->addWidget(m_metaLabel);
    profileLayout->addLayout(nameBox);
    profileLayout->addStretch();

    auto *passwordButton = new QPushButton(QStringLiteral("修改密码"), profile);
    passwordButton->setCursor(Qt::PointingHandCursor);
    connect(passwordButton, &QPushButton::clicked, this, &StudentMainWindow::onChangePassword);
    profileLayout->addWidget(passwordButton);
    root->addWidget(profile);

    // ---------- 统计卡片 ----------
    auto *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(12);
    statsLayout->addWidget(createStatCard(QStringLiteral("绩点 GPA"), &m_gpaValue, central));
    statsLayout->addWidget(createStatCard(QStringLiteral("学分加权平均分"), &m_avgValue, central));
    statsLayout->addWidget(createStatCard(QStringLiteral("已修学分"), &m_creditValue, central));
    statsLayout->addWidget(createStatCard(QStringLiteral("班级排名"), &m_rankValue, central));
    root->addLayout(statsLayout);

    // ---------- 筛选 ----------
    auto *filterLayout = new QHBoxLayout();
    auto *filterLabel = new QLabel(QStringLiteral("学期："), central);
    m_semesterCombo = new QComboBox(central);
    m_semesterCombo->setMinimumWidth(180);
    auto *refreshButton = new QPushButton(QStringLiteral("刷新"), central);
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(m_semesterCombo);
    filterLayout->addWidget(refreshButton);
    filterLayout->addStretch();
    root->addLayout(filterLayout);

    connect(m_semesterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) { loadGrades(); });
    connect(refreshButton, &QPushButton::clicked, this, &StudentMainWindow::refreshGrades);

    // ---------- 成绩表 ----------
    m_table = new QTableWidget(central);
    m_table->setColumnCount(8);
    m_table->setHorizontalHeaderLabels(QStringList()
        << QStringLiteral("学期") << QStringLiteral("课程编号") << QStringLiteral("课程名称")
        << QStringLiteral("学分") << QStringLiteral("成绩") << QStringLiteral("等级")
        << QStringLiteral("任课教师") << QStringLiteral("考试日期"));
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setColumnWidth(0, 110);
    m_table->setColumnWidth(1, 90);
    m_table->setColumnWidth(2, 170);
    m_table->setColumnWidth(3, 70);
    m_table->setColumnWidth(4, 80);
    m_table->setColumnWidth(5, 80);
    m_table->setColumnWidth(6, 110);
    root->addWidget(m_table, 1);

    setCentralWidget(central);
    statusBar()->showMessage(QStringLiteral("数据来源：v_student_grades / v_student_overview 视图"));

    // ---------- 菜单 ----------
    QMenu *personalMenu = menuBar()->addMenu(QStringLiteral("个人设置"));
    QAction *changePassword = personalMenu->addAction(QStringLiteral("修改密码"));
    QAction *changeQuestions = personalMenu->addAction(QStringLiteral("修改密保问题"));
    personalMenu->addSeparator();
    QAction *exitAction = personalMenu->addAction(QStringLiteral("退出"));
    connect(changePassword, &QAction::triggered, this, &StudentMainWindow::onChangePassword);
    connect(changeQuestions, &QAction::triggered, this, &StudentMainWindow::onChangeQuestions);
    connect(exitAction, &QAction::triggered, this, &StudentMainWindow::onExit);

    QMenu *helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
    QAction *aboutAction = helpMenu->addAction(QStringLiteral("关于"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, QStringLiteral("关于"),
                           QStringLiteral("学生信息管理系统 1.0\n"
                                          "Qt 6.11 + SQLite\n"
                                          "数据库原理课程设计作品"));
    });
}

bool StudentMainWindow::loadStudent()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare(QStringLiteral(
        "SELECT student_id, name, COALESCE(class_name,'未分班'), status "
        "FROM students WHERE user_id = ?"));
    query.addBindValue(m_userId);
    if (!query.exec() || !query.next())
        return false;

    m_studentId = query.value(0).toString();
    m_name = query.value(1).toString();
    m_className = query.value(2).toString();
    m_status = query.value(3).toString();

    m_nameLabel->setText(m_name);
    m_metaLabel->setText(QStringLiteral("学号：%1　·　班级：%2　·　状态：%3")
                             .arg(m_studentId, m_className, m_status));
    return true;
}

void StudentMainWindow::loadSummary()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare(QStringLiteral(
        "SELECT subject_count, total_credit, weighted_avg, gpa, class_rank, grade_rank "
        "FROM v_student_overview WHERE student_id = ?"));
    query.addBindValue(m_studentId);
    if (!query.exec() || !query.next()) {
        m_gpaValue->setText(QStringLiteral("0.000"));
        m_avgValue->setText(QStringLiteral("0.00"));
        m_creditValue->setText(QStringLiteral("0"));
        m_rankValue->setText(QStringLiteral("--"));
        return;
    }

    m_gpaValue->setText(QString::number(query.value(3).toDouble(), 'f', 3));
    m_avgValue->setText(QString::number(query.value(2).toDouble(), 'f', 2));
    m_creditValue->setText(QString::number(query.value(1).toDouble(), 'f', 1));
    const int classRank = query.value(4).toInt();
    const int gradeRank = query.value(5).toInt();
    m_rankValue->setText(QStringLiteral("第 %1 名（年级第 %2）").arg(classRank).arg(gradeRank));
}

void StudentMainWindow::loadGrades()
{
    // 学期下拉：保留“全部学期”
    const QString selected = m_semesterCombo->currentData().toString();
    QSqlQuery semesterQuery(DatabaseManager::instance()->database());
    semesterQuery.prepare(QStringLiteral(
        "SELECT DISTINCT semester FROM v_student_grades WHERE student_id = ? "
        "ORDER BY semester DESC"));
    semesterQuery.addBindValue(m_studentId);

    m_semesterCombo->blockSignals(true);
    m_semesterCombo->clear();
    m_semesterCombo->addItem(QStringLiteral("全部学期"), QString());
    if (semesterQuery.exec()) {
        while (semesterQuery.next())
            m_semesterCombo->addItem(semesterQuery.value(0).toString(),
                                     semesterQuery.value(0).toString());
    }
    const int selectedIndex = m_semesterCombo->findData(selected);
    m_semesterCombo->setCurrentIndex(selectedIndex >= 0 ? selectedIndex : 0);
    m_semesterCombo->blockSignals(false);

    const QString semester = m_semesterCombo->currentData().toString();
    QString sql = QStringLiteral(
        "SELECT semester, subject_id, subject_name, credit, score, grade_level, "
        "teacher_name, COALESCE(exam_date,'') "
        "FROM v_student_grades WHERE student_id = ?");
    if (!semester.isEmpty())
        sql += QStringLiteral(" AND semester = ?");
    sql += QStringLiteral(" ORDER BY semester DESC, subject_id");

    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare(sql);
    query.addBindValue(m_studentId);
    if (!semester.isEmpty())
        query.addBindValue(semester);

    m_table->setRowCount(0);
    if (!query.exec()) {
        statusBar()->showMessage(QStringLiteral("查询成绩失败：%1").arg(query.lastError().text()));
        return;
    }

    int row = 0;
    while (query.next()) {
        m_table->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            const QString text = (col == 3)
                                     ? QString::number(query.value(col).toDouble(), 'f', 1)
                                 : (col == 4)
                                     ? QString::number(query.value(col).toDouble(), 'f', 1)
                                     : query.value(col).toString();
            auto *item = new QTableWidgetItem(text);
            item->setTextAlignment((col == 4) ? Qt::AlignCenter : Qt::AlignVCenter | Qt::AlignLeft);
            if (col == 4) {
                const double score = query.value(4).toDouble();
                if (score < 60)
                    item->setForeground(QBrush(QColor(QStringLiteral("#dc2626"))));
                else if (score >= 90)
                    item->setForeground(QBrush(QColor(QStringLiteral("#15803d"))));
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
            }
            m_table->setItem(row, col, item);
        }
        ++row;
    }
    statusBar()->showMessage(QStringLiteral("共 %1 条成绩记录").arg(row));
}

void StudentMainWindow::refreshGrades()
{
    loadSummary();
    loadGrades();
}

void StudentMainWindow::onChangePassword()
{
    ChangePasswordDialog dialog(m_userId, false, this);
    dialog.exec();
}

void StudentMainWindow::onChangeQuestions()
{
    ChangeQuestionsDialog dialog(m_userId, this);
    dialog.exec();
}

void StudentMainWindow::onExit()
{
    if (QMessageBox::question(this, QStringLiteral("确认退出"),
                              QStringLiteral("确定要退出系统吗？"),
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes) {
        qApp->quit();
    }
}
