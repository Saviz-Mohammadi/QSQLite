#include <QFileInfo>
#include "DatabaseSQLite.hpp"

DatabaseSQLite *DatabaseSQLite::m_Instance = nullptr;

DatabaseSQLite::DatabaseSQLite(QObject *parent)
    : QObject{parent}
{
    // Just for insurance.
#ifdef QT_DEBUG
    qDebug() << "List of SQL drivers:";
    qDebug() << QSqlDatabase::drivers();
#endif
}

DatabaseSQLite::~DatabaseSQLite()
{
#ifdef QT_DEBUG
    qDebug() << "Closing the DatabaseSQLite connection.";
#endif

    // Close the connection to DatabaseSQLite.
    m_QSqlDatabaseSQLite.close();
}

DatabaseSQLite *DatabaseSQLite::cppInstance(QObject *parent)
{
    if (m_Instance)
    {
        return (qobject_cast<DatabaseSQLite *>(DatabaseSQLite::m_Instance));
    }

    auto instance = new DatabaseSQLite(parent);
    m_Instance = instance;
    return (instance);
}

// I decided to separate the creation of the object through the constructor
// from the actual process of establishing a connection for maximum compatibility.
void DatabaseSQLite::establishConnection(const QString &path)
{
    // Connection already exists, abort operation.
    if (connectionExists)
    {
        return;
    }

    m_QSqlDatabaseSQLite = QSqlDatabase::addDatabase("QSQLITE");

    m_QSqlDatabaseSQLite.setDatabaseName(path);

    bool connectionFailed = !m_QSqlDatabaseSQLite.open();

    if (connectionFailed)
    {
#ifdef QT_DEBUG
        qDebug() << "Connection failed!" << m_QSqlDatabaseSQLite.lastError().text();
        qDebug() << "Path given to the DatabaseSQLite file: " << path
                 << " Does path exist: " << QFileInfo::exists(path);
#endif

        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Connection established!";
#endif

    QSqlQuery query;

    bool query_success = query.exec(
        "CREATE TABLE IF NOT EXISTS tasks (id INTEGER PRIMARY KEY, task TEXT);");

    if (!query_success) {
#ifdef QT_DEBUG
        qDebug() << "Failed to run query. Reason: " << query.lastError().text();
#endif

        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Query executed successfully. Table is now created if not already existed.";
#endif

    connectionExists = (true);
}

bool DatabaseSQLite::searchTask(const QString &text)
{
    QString command("SELECT COUNT(*) FROM tasks WHERE task = :text");

    QSqlQuery query;
    query.prepare(command);
    query.bindValue(":text", text);

    bool operation_success = (query.exec());

    if (!operation_success) {
#ifdef QT_DEBUG
        qDebug() << "Operation \"search\" failed. Reason: " << query.lastError().text();
#endif

        return (operation_success);
    }

#ifdef QT_DEBUG
    qDebug() << "Operation \"search\" successful.";
#endif

    if (query.next()) { // Move to the first (and only) record
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

bool DatabaseSQLite::addTask(const QString &text)
{
    QString command("INSERT INTO tasks(task) VALUES(:text)");

    QSqlQuery query;
    query.prepare(command);
    query.bindValue(":text", text);

    bool operation_success = (query.exec());

    if (!operation_success) {
#ifdef QT_DEBUG
        qDebug() << "Operation \"addTask\" failed. Reason: " << query.lastError().text();
#endif
        return (operation_success);
    }

#ifdef QT_DEBUG
    qDebug() << "Operation \"addTask\" successful.";
#endif

    return (operation_success);
}

bool DatabaseSQLite::removeTask(const QString &text)
{
    //qDebug() << text;

    QString command("DELETE FROM tasks WHERE task = :text");

    QSqlQuery query;
    query.prepare(command);
    query.bindValue(":text", text);

    bool operation_success = (query.exec());

    if (!operation_success) {
#ifdef QT_DEBUG
        qDebug() << "Operation \"removeTask\" failed. Reason: " << query.lastError().text();
#endif

        return (operation_success);
    }

#ifdef QT_DEBUG
    qDebug() << "Operation \"removeTask\" successful.";
#endif

    return (operation_success);
}

QStringList DatabaseSQLite::getAllTasks()
{
    QString command("SELECT task FROM tasks WHERE task is not NULL");

    QSqlQuery query;
    query.prepare(command);
    query.exec();

    QStringList list;

    // Iterate using a cursor.
    while (query.next())
    {
        QString record = query.value(0).toString();

        list.append(record);

#ifdef QT_DEBUG
        qDebug() << "Found record" << record;
#endif
    }

    return (list);
}
