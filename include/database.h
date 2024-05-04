#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVector>
#include <QString>

class DataBase
{
public:

    static DataBase *getInstance();  //静态的获取数据库对象的函数

    DataBase();

    ~DataBase();

    struct Params
    {
        QString q;    // 有限域的模p
        QString a;    // 曲线方程系数a
        QString b;    // 曲线方程系数b
        QString P_x;  // 基点P的x坐标
        QString P_y;  // 基点P的y坐标
        QString OP;    // 基点P的阶：令nP=O的最小整数
        QString ID;
        QString x; //用户秘密值
        QString Q_x; //用户公钥
        QString Q_y;
        QString s;  //用户私钥
        QString PK_x;
        QString PK_y;
    };

    bool insertParams(QString ID, QString Qx, QString Qy, QString s,
                      QString PKx, QString PKy, QString a, QString x,
                      QString b, QString q, QString Px, QString Py, QString OP);

    bool findParams(QString ID);

    bool deleteParams(QString ID);

    Params getParams(QString ID);

private:

    QSqlDatabase db_;


};

#endif // DATABASE_H

