#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMutex>
#include "database.h"

DataBase *DataBase::getInstance()
{
    static DataBase sqldb;
    return &sqldb;
}

DataBase::DataBase()
{
    //先判断连接是否存在
    if(QSqlDatabase::contains("testConnection"))
    {
        db_ = QSqlDatabase::addDatabase("testConnection");
    }
    else
    {
        // 加载数据库驱动并设置数据库名
        db_ = QSqlDatabase::addDatabase("QSQLITE", "testConnection");
        db_.setDatabaseName("params.db");
        qDebug () << db_.databaseName();
        qDebug () << db_.connectionName();//打印连接名称
    }

    //打开数据库
    if(db_.open())
    {
        qDebug() << "open success";
    }
    else
    {
        qDebug() << "open:" << db_.lastError().text();
        return;
    }

    QString sql = "create table if not exists table_params(id varchar(70) primary key, Qx varchar(65),Qy varchar(65),"
                  "s varchar(65),PKx varchar(65),PKy varchar(65), a varchar(65), b varchar(65),"
                  "q varchar(65), x varchar(65), Px varchar(65), Py varchar(65), OP varchar(65));";

    QSqlQuery query(db_);

    if (query.exec(sql))
    {
        qDebug() << "create success.";
    }
    db_.close();
}

DataBase::~DataBase()
{
    // 在析构函数中关闭数据库连接
    if (db_.isOpen())
    {
        db_.close();
    }
}

//插入一套参数信息
bool DataBase::insertParams(QString ID, QString Qx, QString Qy, QString s,QString PKx, QString x,
                            QString PKy, QString a, QString b, QString q, QString Px, QString Py, QString OP)
{
    QSqlQuery query(db_);

    // 打开数据库
    if (!db_.open())
    {
        qDebug() << "open:" << db_.lastError().text();
        return false;
    }

    qDebug() << ID << Qx <<Qy << s << PKx<<PKy<<a<<b<<q<<Px<<Py<<OP<<x;
    // 执行sql语句
    query.prepare("INSERT INTO table_params (id, Qx, Qy, s, PKx, PKy, a, b, q, x, Px, Py, OP) "
                  "VALUES (:id, :Qx, :Qy, :s, :PKx, :PKy, :a, :b, :q, :x, :Px, :Py, :OP);");

    query.bindValue(":id", ID);
    query.bindValue(":Qx", Qx);
    query.bindValue(":Qy", Qy);
    query.bindValue(":s", s);
    query.bindValue(":PKx", PKx);
    query.bindValue(":PKy", PKy);
    query.bindValue(":a", a);
    query.bindValue(":b", b);
    query.bindValue(":q", q);
    query.bindValue(":x", x);
    query.bindValue(":Px", Px);
    query.bindValue(":Py", Py);
    query.bindValue(":OP", OP);

    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError();
        return false;
    }

    //关闭数据库
    db_.close();
    return true;

}


//根据ID返回参数
bool DataBase::findParams(QString ID)
{
    QSqlQuery query(db_);

    //打开数据库
        if(!db_.open())
    {
        qDebug() << "open fail" << db_.lastError().text();
        exit(-1);
    }

    QString sql = QString("SELECT * FROM table_params WHERE id = :id;");

    //执行插入操作

    query.prepare(sql);
    query.bindValue(":id", ID);
    query.exec();

    //查询成功
    if(query.next())
    {
        if(ID == query.value(0).toString()) //用户存在
        {
            return true;
        }
    }

    db_.close();

    return false;
}

DataBase::Params DataBase::getParams(QString ID)
{
    QSqlQuery query(db_);

    // 打开数据库
    if (!db_.open())
    {
        qDebug() << "open fail" << db_.lastError().text();
        exit(-1);
    }

    QString sql = QString("SELECT * FROM table_params WHERE id = :id;");

    // 执行查询操作
    query.prepare(sql);
    query.bindValue(":id", ID);
    query.exec();

    // 查询成功
    if (query.next())
    {
        Params params;
        params.x = query.value("x").toString();
        params.Q_x = query.value("Qx").toString();
        params.Q_y = query.value("Qy").toString();
        params.s = query.value("s").toString();
        params.PK_x = query.value("PKx").toString();
        params.PK_y = query.value("PKy").toString();
        params.a = query.value("a").toString();
        params.b = query.value("b").toString();
        params.q = query.value("q").toString();
        params.P_x = query.value("Px").toString();
        params.P_y = query.value("Py").toString();
        params.OP = query.value("OP").toString();

        db_.close();
        return params;
    }

    db_.close();
    return Params(); // 如果查询失败，返回一个空的 Params 对象
}

bool DataBase::deleteParams(QString ID)
{
    QSqlQuery query(db_);


    // 打开数据库
    if (!db_.open())
    {
        qDebug() << "open fail" << db_.lastError().text();
        return false;
    }

    QString sql = QString("DELETE FROM table_params WHERE id = :id;");

    // 执行删除操作
    query.prepare(sql);
    query.bindValue(":id", ID);

    if (!query.exec())
    {
        qDebug() << "Delete failed:" << query.lastError();
        db_.close(); // 在查询失败时关闭数据库连接
        return false;
    }

    db_.close();
    return true;
}

