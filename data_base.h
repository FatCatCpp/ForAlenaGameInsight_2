#ifndef DATA_BASE
#define DATA_BASE

#include <QObject>
#include <QtSql>
#include <QSqlQueryModel>

class Data_base
{

// ************************
// пользовательские методы
public:

    Data_base();
    ~Data_base();

    //****************************************

    // Загрузить драйвер базы данных
    bool load_driver(const QString &);

    // Загрузить базу данных
    bool load_DataBase(const QString &);

    // Запрос на выполненик команд SQL
    bool run_sql_query(const QString &);

    // Получить ответ запроса
    QSqlQuery & get_responce_query(void);

    // Получить структуру ошибок базы данных
    const QSqlError & get_error(void);

    // количество таблиц в БД
    void countTables();
    QSqlQueryModel test_query(QSqlQueryModel &);

    QSqlTableModel MyModel();


private:
    QSqlDatabase DataBase_Users;
    QSqlQuery Query_DB1;
    QSqlTableModel model;

};

#endif // DATA_BASE

