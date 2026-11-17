#include "validation.h"

#include <QRegularExpression>

namespace Validation {

QString passwordError(const QString &password)
{
    if (password.isEmpty())
        return QStringLiteral("密码不能为空");
    if (password.length() < 8)
        return QStringLiteral("密码长度至少 8 个字符");
    if (password.length() > 40)
        return QStringLiteral("密码长度最多 40 个字符");

    bool hasLetter = false;
    bool hasDigit = false;
    bool hasSpecial = false;
    for (const QChar &ch : password) {
        if (ch.isLetter())
            hasLetter = true;
        else if (ch.isDigit())
            hasDigit = true;
        else if (!ch.isSpace())
            hasSpecial = true;
    }
    const int types = (hasLetter ? 1 : 0) + (hasDigit ? 1 : 0) + (hasSpecial ? 1 : 0);
    if (types < 2)
        return QStringLiteral("密码必须包含字母、数字、特殊字符中的至少两类");
    return QString();
}

QString phoneError(const QString &phone)
{
    const QString value = phone.trimmed();
    if (value.isEmpty())
        return QString(); // 电话可选
    static const QRegularExpression re(QStringLiteral("^[0-9+\\-\\s]{7,20}$"));
    if (!re.match(value).hasMatch())
        return QStringLiteral("电话只能包含数字、空格、+、-，长度为 7-20 位");
    int digits = 0;
    for (const QChar &ch : value) {
        if (ch.isDigit())
            ++digits;
    }
    if (digits < 7)
        return QStringLiteral("电话号码至少包含 7 位数字");
    return QString();
}

QString accountIdError(const QString &id)
{
    const QString value = id.trimmed();
    if (value.isEmpty())
        return QStringLiteral("学号/工号不能为空");
    static const QRegularExpression re(QStringLiteral("^[A-Za-z0-9_-]{3,20}$"));
    if (!re.match(value).hasMatch())
        return QStringLiteral("学号/工号只能包含字母、数字、下划线或连字符，长度 3-20 位");
    return QString();
}

QString nameError(const QString &name)
{
    const QString value = name.trimmed();
    if (value.isEmpty())
        return QStringLiteral("姓名不能为空");
    if (value.length() > 50)
        return QStringLiteral("姓名长度不能超过 50 个字符");
    return QString();
}

bool isValidSemester(const QString &semester)
{
    static const QRegularExpression re(QStringLiteral("^(\\d{4})-(\\d{4})-([12])$"));
    const QRegularExpressionMatch match = re.match(semester.trimmed());
    if (!match.hasMatch())
        return false;
    const int startYear = match.captured(1).toInt();
    const int endYear = match.captured(2).toInt();
    return endYear == startYear + 1;
}

QString semesterForDate(const QDate &date)
{
    const int year = date.year();
    const int month = date.month();
    if (month >= 9)
        return QStringLiteral("%1-%2-1").arg(year).arg(year + 1);
    if (month == 1)
        return QStringLiteral("%1-%2-1").arg(year - 1).arg(year);
    return QStringLiteral("%1-%2-2").arg(year - 1).arg(year);
}

QStringList recentSemesters(int count)
{
    QStringList result;
    const QString current = semesterForDate();
    int startYear = current.left(4).toInt();
    int term = current.right(1).toInt();

    for (int i = 0; i < count; ++i) {
        result << QStringLiteral("%1-%2-%3").arg(startYear).arg(startYear + 1).arg(term);
        if (term == 2) {
            term = 1;
        } else {
            term = 2;
            --startYear;
        }
    }
    return result;
}

QString escapeHtml(const QString &text)
{
    return text.toHtmlEscaped();
}

QString normalizeAnswer(const QString &answer)
{
    return answer.simplified().toCaseFolded();
}

} // namespace Validation
