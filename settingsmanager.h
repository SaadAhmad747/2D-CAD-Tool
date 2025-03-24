#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QJsonArray>

class SettingsManager
{
public:
    static bool SaveToFile(const QString &filePath, const QJsonArray &shapes);
    static bool LoadFromFile(const QString &filePath, QJsonArray &shapes);
};

#endif // SETTINGSMANAGER_H
