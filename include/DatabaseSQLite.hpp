#ifndef DatabaseSQLite_H
#define DatabaseSQLite_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class DatabaseSQLite : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseSQLite(QObject *parent = nullptr);
    ~DatabaseSQLite();

private:
    static DatabaseSQLite *m_Instance;
    QSqlDatabase m_QSqlDatabaseSQLite;
    bool connectionExists = false;

public:
    static DatabaseSQLite *cppInstance(QObject *parent = nullptr);

public:
    void establishConnection(const QString &path);
    bool searchTask(const QString &text);
    bool addTask(const QString &text);
    bool removeTask(const QString &text);
    QStringList getAllTasks();
};

#endif // DatabaseSQLite_H
