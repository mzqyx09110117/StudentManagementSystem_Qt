#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>

class QLabel;
class QChart;
class QBarSet;
class QBarCategoryAxis;
class QValueAxis;

/**
 * @brief 管理员/教师数据看板：核心计数、班级平均分、分数段分布。
 */
class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatsDialog(QWidget *parent = nullptr);

private:
    void loadData();
    void refreshCharts();

    QLabel *m_studentValue = nullptr;
    QLabel *m_teacherValue = nullptr;
    QLabel *m_classValue = nullptr;
    QLabel *m_subjectValue = nullptr;
    QLabel *m_gradeValue = nullptr;
    QLabel *m_avgValue = nullptr;

    QChart *m_classChart = nullptr;
    QBarSet *m_classSet = nullptr;
    QBarCategoryAxis *m_classAxis = nullptr;
    QChart *m_distChart = nullptr;
    QBarSet *m_distSet = nullptr;
    QBarCategoryAxis *m_distAxis = nullptr;
    QValueAxis *m_distValueAxis = nullptr;
};

#endif // STATSDIALOG_H
