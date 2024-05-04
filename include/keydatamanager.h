#ifndef KEYDATAMANAGER_H
#define KEYDATAMANAGER_H

#include <QQuickItem>
#include <definitions.h>

class KeyDataManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_NAMED_ELEMENT(KeyDataManager)
    Q_PROPERTY_RW(QString, q)
    Q_PROPERTY_RW(QString, a)
    Q_PROPERTY_RW(QString, b)
    Q_PROPERTY_RW(QString, P_x)
    Q_PROPERTY_RW(QString, P_y)
    Q_PROPERTY_RW(QString, O_P)
    Q_PROPERTY_RW(QString, ID)
    Q_PROPERTY_RW(QString, x)
    Q_PROPERTY_RW(QString, Q_x)
    Q_PROPERTY_RW(QString, Q_y)
    Q_PROPERTY_RW(QString, S)
    Q_PROPERTY_RW(QString, Pk_x)
    Q_PROPERTY_RW(QString, Pk_y)
public:
    static KeyDataManager* instance();
    static KeyDataManager* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine){return instance();}
private:
    KeyDataManager(QObject *parent = nullptr);
    Q_DISABLE_COPY_MOVE(KeyDataManager);
signals:
    void need_reset_key_properties();
    void need_save_key_properties();
    void need_save_key_to_database();
    void need_save_key_to_file();
    void need_search_key();
};

#endif // KEYDATAMANAGER_H
