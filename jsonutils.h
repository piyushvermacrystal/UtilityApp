#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTextStream>

class JsonUtils
{

public:
    JsonUtils();

    bool convertJsonFileToCSV(const QString& dbgFilePath, QFile& csvFile);
    bool convertJsonFileDirToCSV(const QString& dbgDirPath, QFile& csvFile);

private:
    void flattenJson(const QJsonObject& jsonObject, QStringList& values);

    bool convertJsonToCSV(QTextStream& in, QTextStream& out);
};

#endif // JSONUTILS_H
