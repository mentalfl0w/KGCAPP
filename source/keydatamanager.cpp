#include "keydatamanager.h"
#include <QMutex>

KeyDataManager::KeyDataManager(QObject *parent) : QObject(parent) {}

KeyDataManager* KeyDataManager::instance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    static KeyDataManager *singleton = nullptr;
    if (!singleton) {
        singleton = new KeyDataManager();
    }
    return singleton;
}
