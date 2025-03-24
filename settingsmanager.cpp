#include "settingsmanager.h"
#include <QFile>
#include <QJsonDocument>

bool SettingsManager::SaveToFile(const QString &filePath, const QJsonArray &shapes){
    QJsonDocument doc(shapes);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        return true; // Save successful
    }
    return false; // Save failed
}

bool SettingsManager::LoadFromFile(const QString &filePath, QJsonArray &shapes){
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if(!doc.isArray()){
        return false;
    }

    shapes = doc.array();
    return true;
}
