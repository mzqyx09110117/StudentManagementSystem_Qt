#include "sqlscript.h"

namespace {

bool isWordChar(QChar c)
{
    return c.isLetterOrNumber() || c == QLatin1Char('_');
}

// 判断一段 SQL 是否含有真正可执行的代码（排除空白与纯注释）
bool hasCode(const QString &statement)
{
    bool inLineComment = false;
    bool inBlockComment = false;
    QChar quote;

    for (int i = 0; i < statement.size(); ++i) {
        const QChar c = statement.at(i);
        const QChar next = (i + 1 < statement.size()) ? statement.at(i + 1) : QChar();

        if (inLineComment) {
            if (c == QLatin1Char('\n'))
                inLineComment = false;
            continue;
        }
        if (inBlockComment) {
            if (c == QLatin1Char('*') && next == QLatin1Char('/')) {
                inBlockComment = false;
                ++i;
            }
            continue;
        }
        if (!quote.isNull()) {
            if (c == quote) {
                if (next == quote) {
                    ++i; // 转义引号
                } else {
                    quote = QChar();
                }
            }
            continue;
        }
        if (c == QLatin1Char('-') && next == QLatin1Char('-')) {
            inLineComment = true;
            ++i;
            continue;
        }
        if (c == QLatin1Char('/') && next == QLatin1Char('*')) {
            inBlockComment = true;
            ++i;
            continue;
        }
        if (c == QLatin1Char('\'') || c == QLatin1Char('"') || c == QLatin1Char('`')) {
            quote = c;
            continue;
        }
        if (!c.isSpace())
            return true;
    }
    return false;
}

} // namespace

QStringList SqlScript::split(const QString &script)
{
    QStringList statements;
    QString current;
    current.reserve(script.size() / 8);

    QChar quote;
    bool inLineComment = false;
    bool inBlockComment = false;
    int triggerDepth = 0;

    auto flush = [&]() {
        const QString trimmed = current.trimmed();
        if (!trimmed.isEmpty() && hasCode(trimmed))
            statements.append(trimmed);
        current.clear();
    };

    for (int i = 0; i < script.size(); ++i) {
        const QChar c = script.at(i);
        const QChar next = (i + 1 < script.size()) ? script.at(i + 1) : QChar();

        if (inLineComment) {
            current.append(c);
            if (c == QLatin1Char('\n'))
                inLineComment = false;
            continue;
        }

        if (inBlockComment) {
            current.append(c);
            if (c == QLatin1Char('*') && next == QLatin1Char('/')) {
                current.append(next);
                ++i;
                inBlockComment = false;
            }
            continue;
        }

        if (!quote.isNull()) {
            current.append(c);
            if (c == quote) {
                if (next == quote) {
                    current.append(next);
                    ++i; // 转义引号
                } else {
                    quote = QChar();
                }
            }
            continue;
        }

        if (c == QLatin1Char('-') && next == QLatin1Char('-')) {
            current.append(c);
            current.append(next);
            ++i;
            inLineComment = true;
            continue;
        }

        if (c == QLatin1Char('/') && next == QLatin1Char('*')) {
            current.append(c);
            current.append(next);
            ++i;
            inBlockComment = true;
            continue;
        }

        if (c == QLatin1Char('\'') || c == QLatin1Char('"') || c == QLatin1Char('`')) {
            quote = c;
            current.append(c);
            continue;
        }

        if (c == QLatin1Char(';') && triggerDepth == 0) {
            current.append(c);
            flush();
            continue;
        }

        if (isWordChar(c)) {
            const int start = i;
            current.append(c);
            while (i + 1 < script.size() && isWordChar(script.at(i + 1))) {
                ++i;
                current.append(script.at(i));
            }
            const QString word = script.mid(start, i - start + 1).toUpper();
            if (word == QLatin1String("BEGIN")) {
                ++triggerDepth;
            } else if (word == QLatin1String("END") && triggerDepth > 0) {
                --triggerDepth;
            }
            continue;
        }

        current.append(c);
    }

    flush();
    return statements;
}
