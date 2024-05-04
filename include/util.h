#ifndef UTIL_H
#define UTIL_H
#include <QString>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDir>
#include <QPair>


extern "C"
{
    #include "miracl.h"
    #include "mirdef.h"
}

void setRandSeed();

void writeToFile(const QString& filePath, const QString& content);

char* QStringToCharArray(const QString& hexString);

QString BigToQString(big big);

QPair<QString, QString> PointToQString(epoint *PO);

void createFolder(const QString &folderName, const QString &basePath = QDir::currentPath());

void release_sources(big a, big b, big c, big d, epoint *A, epoint *B, epoint *C);

void sha256_update_string(sha256 sh, const char *data, long data_len);

void sha256_update_point(sha256 sh, epoint *point);

void hash1(char *ID, epoint *Q, epoint *PK_pub, big p, big h_1_big);

void hash2(char *ID, epoint *X, big p, big h_2_big);

void hash3(char *ID, char *msg, epoint *Q, epoint *U, epoint *PK_pub, big p, big h_3_big);

#endif
