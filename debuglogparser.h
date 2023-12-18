#ifndef DEBUGLOGPARSER_H
#define DEBUGLOGPARSER_H

#include <QString>

#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>

struct LogEntry {
    int cycleNumber;
    QString messageType;
    QString tag;
    QString message;
    int finalSerialNumber;
};

class DebugLogParser
{
public:
    DebugLogParser();

    bool parseDebugLogs(const QString &dbgLogPath);

private:
    void processLogFile(const QString &filePath, QVector<LogEntry> &logEntries);
    void batchInsertData(const QVector<LogEntry> &logEntries);

    void getFinalSerialNumber(const QString& jsonString, int& out);
    void updatePrevEntriesForFinalSerialNumber(int cycleNumber, int finalSerialNumber, QVector<LogEntry> &logEntries);

    QSqlDatabase db;
};

#endif // DEBUGLOGPARSER_H
