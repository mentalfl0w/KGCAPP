#include "keyparams.h"

KeyParams::KeyParams(QObject *parent) : QObject(parent)
{

    qDebug() << "成功进入keyparams构造函数！";
    database = DataBase::getInstance();
    manager = KeyDataManager::instance();


    setDefaultParams();
    createFolder("key");

    miracl *mip = mirsys(512, 16); // 初始化MIRACL系统，512位，16进制数
    mip->IOBASE = 16;              // 设置大整数为16进制

    connect(manager, &KeyDataManager::need_reset_key_properties, this, &KeyParams::reset_key_slot);
    connect(manager, &KeyDataManager::need_save_key_properties, this, &KeyParams::save_key_slot);
    connect(manager, &KeyDataManager::need_save_key_to_database, this, &KeyParams::save_key_to_database);
    connect(manager, &KeyDataManager::need_save_key_to_file, this, &KeyParams::save_key_to_file);
    connect(manager, &KeyDataManager::need_search_key, this, &KeyParams::search_key_slot);
}

KeyParams::~KeyParams() {}

/***********************************************slots************************************************/

void KeyParams::reset_key_slot()
{
    manager->a(defaultParams.a);
    manager->b(defaultParams.b);
    manager->q(defaultParams.q);
    manager->P_x(defaultParams.P_x);
    manager->P_y(defaultParams.P_y);
    manager->O_P(defaultParams.N);

}

void KeyParams::save_key_slot()
{
    Curve curve;
    curve.a = manager->a();
    curve.b = manager->b();
    curve.q = manager->q();
    curve.P_x = manager->P_x();
    curve.P_y = manager->P_y();
    curve.N = manager->O_P();
    QString id = manager->ID();
    qDebug() << id;

    qDebug() << curve.a << curve.b << ID << curve.N << curve.P_y;

    if(curve.a.isEmpty() || curve.b.isEmpty() || curve.q.isEmpty() || curve.P_x.isEmpty()
        || curve.P_y.isEmpty() || curve.N.isEmpty() || id.isEmpty())
    {
        qDebug() << "missing params";
        return;
    }

    gen_Key(curve, ID);

    qDebug() << "generated key successfully!";
}

void KeyParams::save_key_to_database()
{
    DataBase::Params sqlparams;
    sqlparams.ID = manager->ID();
    sqlparams.a = manager->a();
    sqlparams.b = manager->b();
    sqlparams.q = manager->q();
    sqlparams.P_x = manager->P_x();
    sqlparams.P_y = manager->P_y();
    sqlparams.OP = manager->O_P();

    sqlparams.x = x_t;
    sqlparams.Q_x = Qs.first;
    sqlparams.Q_y = Qs.second;
    sqlparams.s = s_t;
    sqlparams.PK_x = PKs.first;
    sqlparams.PK_y = PKs.second;

    bool result;
    result = database->insertParams(sqlparams.ID, sqlparams.Q_x, sqlparams.Q_y, sqlparams.s, sqlparams.PK_x, sqlparams.PK_y,
                                    sqlparams.x ,sqlparams.a, sqlparams.b, sqlparams.q, sqlparams.P_x, sqlparams.P_y, sqlparams.OP);
    if(!result)
    {
        qDebug() << "Database insertion failed";
    }

}

void KeyParams::save_key_to_file()
{
    QString Qdata = manager->Q_x() + manager->Q_y();
    QString PKdata = manager->Pk_x() + manager->Pk_y();

    writeToFile("./key/pubkey.txt", Qdata);
    writeToFile("./key/kgcpub.txt", PKdata);
    writeToFile("./key/prikey.txt", manager->S());
    writeToFile("./key/userid.txt", manager->ID());

}

void KeyParams::search_key_slot()
{
    //ClearScreen();
    if (!database->findParams(manager->ID()))
    {
        qDebug() << "Could not find the specified ID";
        return;
    }
    else
    {
        DataBase::Params sqlparams;
        sqlparams = database->getParams(manager->ID());

        manager->a(sqlparams.a);
        manager->b(sqlparams.b);
        manager->q(sqlparams.q);
        manager->P_x(sqlparams.P_x);
        manager->P_y(sqlparams.P_y);
        manager->O_P(sqlparams.OP);
        manager->x(sqlparams.x);
        manager->Q_x(sqlparams.Q_x);
        manager->Q_y(sqlparams.Q_y);
        manager->S(sqlparams.s);
        manager->Pk_x(sqlparams.PK_x);
        manager->Pk_y(sqlparams.PK_y);

        qDebug() << "return sqlite params successfully";

    }
}


/****************************************functions************************************************/

void KeyParams::gen_Key(Curve curve, QString ID)
{
    //TODO convert QString to Char* and init ECC_PARAMS
    ECC_PARAMS ecc;

    ecc.a = mirvar(0);
    ecc.b = mirvar(0);
    ecc.q = mirvar(0);
    ecc.p = mirvar(0);
    ecc.P_x = mirvar(0);
    ecc.P_y = mirvar(0);
    ecc.P = epoint_init();

    Init_Params(ecc, curve);
    ecurve_init(ecc.a, ecc.b, ecc.q, MR_PROJECTIVE);

    if (!judgeEcurve(ecc))
    {
        qDebug() << "ECC setup error.";
        return;
    }

    setRandSeed();                 // 随机数种子

    big msk = mirvar(0);              //私钥
    epoint *PK_pub = epoint_init();   //公钥
    big d_A = mirvar(0);              //用户A产生的部分私钥
    big x_A = mirvar(0);              //用户产生的秘密值
    epoint *X_A = epoint_init();
    big s_A = mirvar(0);             //用户完整私钥
    epoint *Q_A = epoint_init();      //用户完整公钥

    genKGCkey(ecc, msk, PK_pub);

    genSecret(ecc, x_A, X_A);

    if (!genPPK_std(ecc, msk, PK_pub, ID, d_A, Q_A, X_A))
    {
        qDebug() << "Successfully generated public key and partial private key.";
    }

    getFullkey(ecc, ID, d_A, x_A, X_A, s_A);

    //TODO convert big to QString and show them

    x_t = BigToQString(x_A);
    s_t = BigToQString(s_A);
    PKs = PointToQString(PK_pub);
    Qs = PointToQString(Q_A);

    setText(x_t, Qs.first, Qs.second, s_t, PKs.first, PKs.second);

    //TODO release sources
    release_sources(msk, d_A, x_A, s_A, PK_pub, X_A, Q_A);

}


void KeyParams::setText(QString x, QString Q_x, QString Q_y, QString s, QString PK_x, QString PK_y)
{
    manager->x(x);
    manager->Q_x(Q_x);
    manager->Q_y(Q_y);
    manager->S(s);
    manager->Pk_x(PK_x);
    manager->Pk_y(PK_y);
}

bool KeyParams::judgeEcurve(ECC_PARAMS ecc)
{

    if (!epoint_set(ecc.P_x, ecc.P_y, 0, ecc.P))
    {
        freeEcurve(ecc);
        qDebug() << "The point is not on the curve.";
        return false;
    }

    epoint *P_test = epoint_init();
    ecurve_mult(ecc.p, ecc.P, P_test);
    if (!point_at_infinity(P_test))
    {
        qDebug() << "Point P is not the base point of p.";
        epoint_free(P_test);
        return false;
    }
    return true;

}

void KeyParams::genKGCkey(ECC_PARAMS params, big msk, epoint *PK_pub)
{
    bigrand(params.p, msk); // 产生小于p的随机数
    ecurve_mult(msk, params.P, PK_pub);
}

void KeyParams::genSecret(ECC_PARAMS params, big x, epoint* X)
{
    bigrand(params.p, x);  //产生小于阶p的big值
    ecurve_mult(x, params.P, X);
}

bool KeyParams::genPPK_std(ECC_PARAMS params, big msk, epoint* PK_pub,
                QString ID, big d, epoint* Q, epoint* X)
{
    char* id = QStringToCharArray(ID);
    // 产生随机数r,计算R=rP
    epoint *R_A = epoint_init();
    big r = mirvar(0);
    bigrand(params.p, r);
    ecurve_mult(r, params.P, R_A);

    //计算h_2 = H_2(ID,X)
    big h_2_big = mirvar(0);
    hash2(id, X, params.p, h_2_big);


    //计算h_2 * X
    epoint *h2X = epoint_init();
    ecurve_mult(h_2_big, X, h2X);

    //计算Q = R + h_2 * X
    ecurve_add(R_A, Q);
    ecurve_add(h2X, Q);

    //计算h_1 = H_1(ID,Q,PK_pub)
    big h_1_big = mirvar(0);
    hash1(id, Q, PK_pub, params.p, h_1_big);

    // 计算d = r + msk * h_1 mod p
    big tmp = mirvar(0);
    multiply(msk, h_1_big, tmp);
    add(r, tmp, d);
    power(d, 1, params.p, d); // mod p

    //计算h_1 * PK_pub
    epoint *h1PK = epoint_init();
    ecurve_mult(h_1_big, PK_pub, h1PK);

    // 用d * P = Q - h2 * X + h1 * PK_pub验证一下(d,Q)是否正确
    // 点的减法 pa = pa - a Function: void ecurve_sub(p,pa)
    epoint *left = epoint_init();
    ecurve_mult(d, params.P, left);
    epoint *right = epoint_init();
    ecurve_add(Q, right);
    ecurve_sub(h2X, right);
    ecurve_add(h1PK, right);

    bool bRv = false;
    if (epoint_comp(left, right))
    {
        bRv = true;
    }
    else
    {
        bRv = false;
        qDebug() << "Error: The equation is not equal on the left and right sides.";
    }

    mirkill(r);
    mirkill(h_1_big);
    mirkill(h_2_big);
    mirkill(tmp);
    delete[] id;

    epoint_free(R_A);
    epoint_free(left);
    epoint_free(right);
    epoint_free(h1PK);
    epoint_free(h2X);

    return bRv;
}

void KeyParams::getFullkey(ECC_PARAMS params, QString ID, big d, big x, epoint *X, big s)
{
    // 计算hash值H_2(ID, X)
    char* id = QStringToCharArray(ID);
    big h_2_big = mirvar(0);
    hash2(id, X, params.p, h_2_big);

    // 计算sa = d + h_2*x mod p
    big tmp = mirvar(0);
    multiply(x, h_2_big, tmp);
    add(d, tmp, s);
    power(s, 1, params.p, s); // mod p

    mirkill(h_2_big);
    mirkill(tmp);
    delete[] id;
}

void KeyParams::setDefaultParams()
{
    // 设置默认值
    //defaultParams.ID = "1234567890111213141516171819202122232425";
    defaultParams.q = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF";
    defaultParams.a = "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC";
    defaultParams.b = "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B";
    defaultParams.N = "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551";
    defaultParams.P_x = "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296";
    defaultParams.P_y = "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5";

}

void KeyParams::Init_Params(ECC_PARAMS ecc, Curve curve)
{

    char * C_a = QStringToCharArray(curve.a);
    char * C_b = QStringToCharArray(curve.b);
    char * C_q = QStringToCharArray(curve.q);
    char * C_Px = QStringToCharArray(curve.P_x);
    char * C_Py = QStringToCharArray(curve.P_y);
    char * C_p = QStringToCharArray(curve.N);

    cinstr(ecc.a, C_a);
    cinstr(ecc.b, C_b);
    cinstr(ecc.q, C_q);
    cinstr(ecc.P_x, C_Px);
    cinstr(ecc.P_y, C_Py);
    cinstr(ecc.p, C_p);

    delete[] C_a;
    delete[] C_b;
    delete[] C_q;
    delete[] C_Px;
    delete[] C_Py;
    delete[] C_p;
}

void KeyParams::freeEcurve(KeyParams::ECC_PARAMS params)
{
    mirkill(params.a);
    mirkill(params.b);
    mirkill(params.q);
    mirkill(params.p);
    mirkill(params.P_x);
    mirkill(params.P_y);

    epoint_free(params.P);
}

void KeyParams::ClearScreen()
{
    manager->a("");
    manager->b("");
    manager->q("");
    manager->P_x("");
    manager->P_y("");
    manager->O_P("");
    manager->x("");
    manager->Q_x("");
    manager->Q_y("");
    manager->S("");
    manager->Pk_x("");
    manager->Pk_y("");
}

