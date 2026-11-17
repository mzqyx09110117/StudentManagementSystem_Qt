#include "statsdialog.h"

#include "databasemanager.h"

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QValueAxis>
#include <QVBoxLayout>

namespace {

QFrame *makeCard(const QString &title, QLabel **valueLabel, QWidget *parent)
{
    auto *card = new QFrame(parent);
    card->setObjectName(QStringLiteral("statCard"));
    card->setStyleSheet(QStringLiteral(
        "QFrame#statCard { background:#ffffff; border:1px solid #e5e7eb; border-radius:8px; }"
        "QLabel { background:transparent; }"));
    auto *layout = new QVBoxLayout(card);
    layout->setContentsMargins(14, 10, 14, 10);
    layout->setSpacing(3);

    auto *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet(QStringLiteral("color:#6b7280;font-size:9pt;"));
    auto *value = new QLabel(QStringLiteral("--"), card);
    value->setStyleSheet(QStringLiteral("color:#111827;font-size:17pt;font-weight:600;"));
    layout->addWidget(titleLabel);
    layout->addWidget(value);
    *valueLabel = value;
    return card;
}

} // namespace

StatsDialog::StatsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("数据看板"));
    resize(1060, 680);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 14, 16, 12);
    layout->setSpacing(12);

    auto *title = new QLabel(QStringLiteral("<h2>学生信息管理系统 · 数据看板</h2>"), this);
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    auto *cards = new QHBoxLayout();
    cards->setSpacing(10);
    cards->addWidget(makeCard(QStringLiteral("学生总数"), &m_studentValue, this));
    cards->addWidget(makeCard(QStringLiteral("教师总数"), &m_teacherValue, this));
    cards->addWidget(makeCard(QStringLiteral("班级总数"), &m_classValue, this));
    cards->addWidget(makeCard(QStringLiteral("课程总数"), &m_subjectValue, this));
    cards->addWidget(makeCard(QStringLiteral("成绩记录"), &m_gradeValue, this));
    cards->addWidget(makeCard(QStringLiteral("总体平均分"), &m_avgValue, this));
    layout->addLayout(cards);

    auto *chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(10);

    // 各班平均分
    m_classChart = new QChart();
    m_classChart->setTitle(QStringLiteral("各班平均分"));
    m_classChart->legend()->hide();
    m_classChart->setBackgroundBrush(Qt::white);
    auto *classSeries = new QBarSeries();
    m_classSet = new QBarSet(QStringLiteral("平均分"));
    m_classSet->setColor(QColor(QStringLiteral("#2563eb")));
    classSeries->append(m_classSet);
    m_classChart->addSeries(classSeries);
    m_classAxis = new QBarCategoryAxis();
    m_classAxis->setLabelsAngle(-20);
    m_classChart->addAxis(m_classAxis, Qt::AlignBottom);
    classSeries->attachAxis(m_classAxis);
    auto *classAxisY = new QValueAxis();
    classAxisY->setRange(0, 100);
    classAxisY->setTitleText(QStringLiteral("分数"));
    m_classChart->addAxis(classAxisY, Qt::AlignLeft);
    classSeries->attachAxis(classAxisY);

    auto *classView = new QChartView(m_classChart);
    classView->setRenderHint(QPainter::Antialiasing);
    classView->setMinimumHeight(300);
    chartsLayout->addWidget(classView);

    // 分数段分布
    m_distChart = new QChart();
    m_distChart->setTitle(QStringLiteral("成绩分布"));
    m_distChart->legend()->hide();
    m_distChart->setBackgroundBrush(Qt::white);
    auto *distSeries = new QBarSeries();
    m_distSet = new QBarSet(QStringLiteral("人数"));
    m_distSet->setColor(QColor(QStringLiteral("#0d9488")));
    distSeries->append(m_distSet);
    m_distChart->addSeries(distSeries);
    m_distAxis = new QBarCategoryAxis();
    m_distAxis->append(QStringList() << QStringLiteral("<60") << QStringLiteral("60-69")
                                     << QStringLiteral("70-79") << QStringLiteral("80-89")
                                     << QStringLiteral("90-100"));
    m_distChart->addAxis(m_distAxis, Qt::AlignBottom);
    distSeries->attachAxis(m_distAxis);
    m_distValueAxis = new QValueAxis();
    m_distValueAxis->setRange(0, 10);
    m_distValueAxis->setTitleText(QStringLiteral("人次"));
    m_distChart->addAxis(m_distValueAxis, Qt::AlignLeft);
    distSeries->attachAxis(m_distValueAxis);

    auto *distView = new QChartView(m_distChart);
    distView->setRenderHint(QPainter::Antialiasing);
    distView->setMinimumHeight(300);
    chartsLayout->addWidget(distView);

    layout->addLayout(chartsLayout, 1);

    auto *buttons = new QHBoxLayout();
    auto *refreshButton = new QPushButton(QStringLiteral("刷新"), this);
    auto *closeButton = new QPushButton(QStringLiteral("关闭"), this);
    buttons->addStretch();
    buttons->addWidget(refreshButton);
    buttons->addWidget(closeButton);
    layout->addLayout(buttons);

    connect(refreshButton, &QPushButton::clicked, this, [this]() { loadData(); });
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    loadData();
}

void StatsDialog::loadData()
{
    QSqlQuery query(DatabaseManager::instance()->database());

    auto scalar = [&query](const QString &sql) -> int {
        if (query.exec(sql) && query.next())
            return query.value(0).toInt();
        return 0;
    };

    m_studentValue->setText(QString::number(scalar(QStringLiteral("SELECT COUNT(*) FROM students"))));
    m_teacherValue->setText(QString::number(scalar(QStringLiteral("SELECT COUNT(*) FROM teachers"))));
    m_classValue->setText(QString::number(scalar(QStringLiteral("SELECT COUNT(*) FROM classes"))));
    m_subjectValue->setText(QString::number(scalar(QStringLiteral("SELECT COUNT(*) FROM subjects"))));
    m_gradeValue->setText(QString::number(scalar(QStringLiteral("SELECT COUNT(*) FROM grades"))));

    query.exec(QStringLiteral("SELECT ROUND(AVG(score),1) FROM grades"));
    if (query.next())
        m_avgValue->setText(QString::number(query.value(0).toDouble(), 'f', 1));
    else
        m_avgValue->setText(QStringLiteral("0.0"));

    refreshCharts();
}

void StatsDialog::refreshCharts()
{
    // 各班平均分
    if (m_classSet->count() > 0)
        m_classSet->remove(0, m_classSet->count());
    m_classAxis->clear();

    QStringList classLabels;
    QSqlQuery classQuery(DatabaseManager::instance()->database());
    if (classQuery.exec(QStringLiteral(
            "SELECT COALESCE(class_name,'未分班') AS c, ROUND(AVG(score),1) "
            "FROM v_student_grades GROUP BY class_name ORDER BY class_name"))) {
        while (classQuery.next()) {
            QString label = classQuery.value(0).toString();
            label.replace(QStringLiteral("人工智能"), QStringLiteral("AI"));
            classLabels << label;
            m_classSet->append(classQuery.value(1).toDouble());
        }
    }
    if (classLabels.isEmpty()) {
        classLabels << QStringLiteral("暂无数据");
        m_classSet->append(0);
    }
    m_classAxis->append(classLabels);

    // 分数段分布
    if (m_distSet->count() > 0)
        m_distSet->remove(0, m_distSet->count());

    int below60 = 0, s60 = 0, s70 = 0, s80 = 0, s90 = 0;
    QSqlQuery distQuery(DatabaseManager::instance()->database());
    if (distQuery.exec(QStringLiteral(
            "SELECT "
            "SUM(CASE WHEN score < 60 THEN 1 ELSE 0 END), "
            "SUM(CASE WHEN score >= 60 AND score < 70 THEN 1 ELSE 0 END), "
            "SUM(CASE WHEN score >= 70 AND score < 80 THEN 1 ELSE 0 END), "
            "SUM(CASE WHEN score >= 80 AND score < 90 THEN 1 ELSE 0 END), "
            "SUM(CASE WHEN score >= 90 THEN 1 ELSE 0 END) FROM grades"))
        && distQuery.next()) {
        below60 = distQuery.value(0).toInt();
        s60 = distQuery.value(1).toInt();
        s70 = distQuery.value(2).toInt();
        s80 = distQuery.value(3).toInt();
        s90 = distQuery.value(4).toInt();
    }
    *m_distSet << below60 << s60 << s70 << s80 << s90;
    m_distValueAxis->setRange(
        0, qMax(10, qMax(qMax(below60, s60), qMax(qMax(s70, s80), s90)) + 2));
}
