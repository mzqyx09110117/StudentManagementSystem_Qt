#ifndef SQLSCRIPT_H
#define SQLSCRIPT_H

#include <QString>
#include <QStringList>

/**
 * @brief SQL 脚本分割器。
 *
 * 把 schema.sql / seed.sql 拆成单条语句交给 QSqlQuery 执行。
 * 需要正确处理：
 *   - 单引号、双引号、反引号中的分号
 *   - -- 行注释与块注释
 *   - CREATE TRIGGER ... BEGIN ... END; 内部的多个分号
 */
class SqlScript
{
public:
    static QStringList split(const QString &script);
};

#endif // SQLSCRIPT_H
