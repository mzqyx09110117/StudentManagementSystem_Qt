#include "auditlogdialog.h"

#include "databasemanager.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidget>
#include <QVBoxLayout>

AuditLogDialog::AuditLogDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("审计日志"));
    resize(1100, 620);

    auto *layout = new QVBoxLayout(this);
    auto *filterLayout = new QHBoxLayout();

    m_operationCombo = new QComboBox(this);
    m_operationCombo->addItem(QStringLiteral("全部操作"), QString());
    m_operationCombo->addItem(QStringLiteral("新增成绩"), QStringLiteral("INSERT"));
    m_operationCombo->addItem(QStringLiteral("修改成绩"), QStringLiteral("UPDATE"));
    m_operationCombo->addItem(QStringLiteral("删除成绩"), QStringLiteral("DELETE"));

    auto *refreshButton = new QPushButton(QStringLiteral("刷新"), this);
    filterLayout->addWidget(new QLabel(QStringLiteral("操作类型："), this));
    filterLayout->addWidget(m_operationCombo);
    filterLayout->addWidget(refreshButton);
    filterLayout->addStretch();
    layout->addLayout(filterLayout);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels(QStringList()
        << QStringLiteral("时间") << QStringLiteral("操作者") << QStringLiteral("表")
        << QStringLiteral("操作") << QStringLiteral("记录键")
        << QStringLiteral("旧值") << QStringLiteral("新值"));
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setColumnWidth(0, 150);
    m_table->setColumnWidth(1, 110);
    m_table->setColumnWidth(2, 80);
    m_table->setColumnWidth(3, 80);
    m_table->setColumnWidth(4, 220);
    layout->addWidget(m_table, 1);

    auto *hint = new QLabel(QStringLiteral("审计日志由数据库触发器自动写入，最多显示最近 500 条。"), this);
    hint->setStyleSheet(QStringLiteral("color:#6b7280;"));
    layout->addWidget(hint);

    auto *buttons = new QHBoxLayout();
    auto *closeButton = new QPushButton(QStringLiteral("关闭"), this);
    buttons->addStretch();
    buttons->addWidget(closeButton);
    layout->addLayout(buttons);

    connect(refreshButton, &QPushButton::clicked, this, &AuditLogDialog::loadData);
    connect(m_operationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) { loadData(); });
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    loadData();
}

void AuditLogDialog::loadData()
{
    m_table->setRowCount(0);

    QString sql = QStringLiteral(
        "SELECT log_time, operator_name, table_name, operation, record_key, "
        "COALESCE(old_value,''), COALESCE(new_value,'') "
        "FROM v_audit_display");
    QVariantList params;
    const QString operation = m_operationCombo->currentData().toString();
    if (!operation.isEmpty()) {
        sql += QStringLiteral(" WHERE operation = ?");
        params << operation;
    }
    sql += QStringLiteral(" ORDER BY id DESC LIMIT 500");

    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare(sql);
    for (const QVariant &param : params)
        query.addBindValue(param);
    if (!query.exec()) {
        QMessageBox::warning(this, QStringLiteral("查询失败"), query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        m_table->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            auto *item = new QTableWidgetItem(query.value(col).toString());
            item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            m_table->setItem(row, col, item);
        }
        ++row;
    }
}
