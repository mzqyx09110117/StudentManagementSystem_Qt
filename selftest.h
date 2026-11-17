#ifndef SELFTEST_H
#define SELFTEST_H

#include <QString>

/**
 * @brief 控制台自检：验证登录、约束、级联、审计与备份恢复。
 * 结果写入日志文件，返回 0 表示全部通过。
 */
int runSelfTest(const QString &logPath);

#endif // SELFTEST_H
