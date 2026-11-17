#ifndef VALIDATION_H
#define VALIDATION_H

#include <QDate>
#include <QString>
#include <QStringList>

/**
 * @brief 统一的输入校验与格式化工具，避免各对话框重复实现。
 */
namespace Validation {

// 密码策略：8-40 位，至少包含字母/数字/特殊字符中的两类；返回空串表示通过
QString passwordError(const QString &password);

// 手机/固定电话：允许数字、空格、+、-，7-20 位；空串视为可选通过
QString phoneError(const QString &phone);

// 学号/工号：3-20 位字母、数字、下划线或连字符
QString accountIdError(const QString &id);

// 姓名：非空且不超过 50 字
QString nameError(const QString &name);

// 学期格式：2024-2025-1 / 2024-2025-2
bool isValidSemester(const QString &semester);

// 根据日期推断当前学年学期
QString semesterForDate(const QDate &date = QDate::currentDate());

// 最近若干学期的下拉选项（含当前学期）
QStringList recentSemesters(int count = 6);

// 把纯文本转成可安全放入富文本标签的内容
QString escapeHtml(const QString &text);

// 密保答案归一化：去首尾空格 + 大小写折叠
QString normalizeAnswer(const QString &answer);

} // namespace Validation

#endif // VALIDATION_H
