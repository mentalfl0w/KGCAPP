#include "util.h"

void writeToFile(const QString& filePath, const QString& content)
{
    QFile file(filePath);

    // 尝试以写入方式打开文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {

        QTextStream out(&file);

        out << content;

        file.close();
        qDebug() << "File written successfully.";
    }
    else
    {
        qDebug() << "Failed to write:" << file.errorString();
    }
}

char* QStringToCharArray(const QString& str)
{
    // 将 QString 转换为 UTF-8 编码的 QByteArray
    QByteArray byteArray = str.toUtf8();

    // 分配足够的内存来存储字符串，包括结尾的空字符
    char* charArray = new char[byteArray.size() + 1];

    // 将 QByteArray 中的数据复制到 char* 中
    std::memcpy(charArray, byteArray.constData(), byteArray.size());

    // 添加字符串结尾的空字符
    charArray[byteArray.size()] = '\0';

    return charArray;
}

QString BigToQString(big big)
{
    char* charArray = new char[257];

    cotstr(big, charArray);

    QString str = QString::fromUtf8(charArray);

    delete[] charArray;

    return str;

}

void createFolder(const QString &folderName, const QString &basePath) {

    QString folderPath = basePath + "/" + folderName;

    QDir dir(basePath);
    if (!dir.exists(folderName)) {
        if (dir.mkdir(folderName))
        {
            qDebug() << "Directory" << folderPath << "created successfully.";
        }
        else
        {
            qDebug() << "Failed to create directory" << folderPath;
        }
    }
    else
    {
        qDebug() << "Directory" << folderPath << "already exists.";
    }
}

void setRandSeed()
{
    time_t seed;
    time(&seed); // 用系统时间做种子
    irand((long)seed);
    return;
}


QPair<QString, QString> PointToQString(epoint *PO)
{
    big PO_x = mirvar(0);
    big PO_y = mirvar(0);
    epoint_get(PO, PO_x, PO_y);

    QString xStr = BigToQString(PO_x);
    QString yStr = BigToQString(PO_y);
    QPair<QString, QString> pointStrings(xStr, yStr);

    mirkill(PO_x);
    mirkill(PO_y);

    return pointStrings;
}

void release_sources(big a, big b, big c, big d, epoint *A, epoint *B, epoint *C)
{
    mirkill(a);
    mirkill(b);
    mirkill(c);
    mirkill(d);
    epoint_free(A);
    epoint_free(B);
    epoint_free(C);

    return;
}

/***********************************************   sha256   ************************************************/

void sha256_update_string(sha256 sh, const char *data, long data_len)
{
    for (long i = 0; i < data_len; i++)
    {
        shs256_process(&sh, data[i]);
    }
}

void sha256_update_point(sha256 sh, epoint *point)
{
    big point_x = mirvar(0);
    big point_y = mirvar(0);
    char point_x_string[256] = {0};
    char point_y_string[256] = {0};
    epoint_get(point, point_x, point_y);
    cotstr(point_x, point_x_string);
    cotstr(point_y, point_y_string);

    for (unsigned int i = 0; i < strlen(point_x_string); i++)
    {
        shs256_process(&sh, point_x_string[i]);
    }

    for (unsigned int i = 0; i < strlen(point_y_string); i++)
    {
        shs256_process(&sh, point_y_string[i]);
    }

    mirkill(point_x);
    mirkill(point_y);
}

void hash1(char *ID, epoint *Q, epoint *PK_pub, big p, big h_1_big)
{
    // 计算hash值H_1(ID, R, PK_pub)
    //hash1(ID, Q, PK_pub)
    sha256 sh;
    char h_1[33] = {0};

    shs256_init(&sh);
    sha256_update_string(sh, ID, strlen(ID));
    sha256_update_point(sh, Q);
    sha256_update_point(sh, PK_pub);
    shs256_hash(&sh, h_1);

    bytes_to_big(32, h_1, h_1_big);
    power(h_1_big, 1, p, h_1_big); // mod p
}

void hash2(char *ID, epoint *X, big p, big h_2_big)
{
    // 计算hash值H_2(ID, X)
    sha256 sh;
    char h_2[33] = {0};

    shs256_init(&sh);
    sha256_update_string(sh, ID, strlen(ID));
    sha256_update_point(sh, X);
    shs256_hash(&sh, h_2);

    bytes_to_big(32, h_2, h_2_big);
    power(h_2_big, 1, p, h_2_big); // mod p
}

void hash3(
    char *ID,
    char *msg,
    epoint *Q,
    epoint *U,
    epoint *PK_pub,
    big p,
    big h_3_big
    )
{
    sha256 sh;
    char h_3[33] = {0};

    shs256_init(&sh);
    sha256_update_string(sh, ID, strlen(ID));
    sha256_update_string(sh, msg, strlen(msg));
    sha256_update_point(sh, Q);
    sha256_update_point(sh, U);
    sha256_update_point(sh, PK_pub);
    shs256_hash(&sh, h_3);

    bytes_to_big(32, h_3, h_3_big);
    power(h_3_big, 1, p, h_3_big); // mod p
}

