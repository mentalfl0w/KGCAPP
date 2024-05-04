#ifndef KEYPARAMS_H
#define KEYPARAMS_H

#include <QObject>
#include <QPair>
#include <QDir>

#include "keydatamanager.h"
#include "util.h"
#include "database.h"

extern "C"
{
    #include "miracl.h"
    #include "mirdef.h"
}

class KeyParams : public QObject
{
    Q_OBJECT
public:

    struct Curve
    {
        QString q;    // 有限域的模p
        QString a;    // 曲线方程系数a
        QString b;    // 曲线方程系数b
        QString P_x;  // 基点P的x坐标
        QString P_y;  // 基点P的y坐标
        QString N;    // 基点P的阶：令nP=O的最小整数
    };

    struct ECC_PARAMS
    {
        big a;     // 椭圆曲线方程系数a
        big b;     // 椭圆曲线方程系数b
        big q;     // 模
        big p;     // 阶
        big P_x;   // 基点横坐标
        big P_y;   // 基点纵坐标
        epoint *P; // 基点
    };

    KeyParams(QObject *parent = nullptr);

    ~KeyParams();

    void setDefaultParams();

    void ClearScreen();

    void freeEcurve(ECC_PARAMS params);

    bool judgeEcurve(ECC_PARAMS params);

    void genSecret(ECC_PARAMS params, big x, epoint* X);

    void genKGCkey(ECC_PARAMS params, big msk, epoint* PK_pub);

    bool genPPK_std(ECC_PARAMS params, big msk, epoint* PK_pub,
                    QString ID, big d, epoint* Q, epoint* X);

    void Init_Params(ECC_PARAMS ecc, Curve curve);

    void getFullkey(ECC_PARAMS params, QString ID, big d, big x,
                    epoint *X, big s);

    void setText(QString x, QString Q_x, QString Q_y, QString s,
                 QString PK_x, QString PK_y);

public slots:

    void reset_key_slot();

    void save_key_slot();

    void save_key_to_database();

    void save_key_to_file();

    void search_key_slot();

    void gen_Key(Curve curve, QString ID);


private:

    QString ID;
    QString x; //用户秘密值
    QString Q_x; //用户公钥
    QString Q_y;
    QString s;  //用户私钥
    QString PK_x;
    QString PK_y;

    QString x_t;
    QString s_t;
    QPair<QString, QString> PKs;
    QPair<QString, QString> Qs;

    Curve defaultParams;

    DataBase* database;

    KeyDataManager* manager;
};

#endif // KEYPARAMS_H
