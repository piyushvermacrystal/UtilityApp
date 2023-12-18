#include "debuglogparser.h"
#include "defines.h"

#include <QDebug>
#include <QSqlQuery>
#include <QDir>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>

DebugLogParser::DebugLogParser() {}

void DebugLogParser::getFinalSerialNumber(const QString &jsonString, int &out) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toLatin1());
    QJsonObject jsonObject = jsonDoc.object();
    out = jsonObject[MeterFinalSerialNumber].toVariant().toInt();
}

bool DebugLogParser::parseDebugLogs(const QString &dbgLogPath) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("meterlog.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database";
        return false;
    }

    QSqlQuery query;
    const QString createTableQuery = "CREATE TABLE IF NOT EXISTS MeterLogData ("
                                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                     "cycleNumber INTEGER NOT NULL, "
                                     "messageType TEXT, "
                                     "tag TEXT, "
                                     "message TEXT, "
                                     "finalSerialNumber INTEGER"
                                     ")";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table:" << query.lastError().text();
        db.close();
        return false;
    }

    QDir logDir(dbgLogPath);
    QStringList logFiles = logDir.entryList(QStringList() << "*.log", QDir::Files);

    QVector<LogEntry> logEntries;  // Container to store log entries

    for (const QString &logFileName : logFiles) {
        QString filePath = logDir.filePath(logFileName);
        processLogFile(filePath, logEntries);
    }

    // Batch insert log entries into the database
    batchInsertData(logEntries);

    db.close();
    return true;
}

void DebugLogParser::processLogFile(const QString &filePath, QVector<LogEntry> &logEntries) {
    QFile logFile(filePath);
    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open log file" << filePath;
        return;
    }

    QRegExp cycleRegex(regexExpCycleNo);

    QTextStream logStream(&logFile);

    int currentCycleNumber = 0;
    bool withinCycleRange = false;
    int currentFinalSerialNumber = 0;  // Default finalSerialNumber

    while (!logStream.atEnd()) {
        QString line = logStream.readLine();

        LogEntry logEntry;
        logEntry.cycleNumber = 0;

        if (line.contains(CycleStart)) {
            if (cycleRegex.indexIn(line) != -1) {
                currentCycleNumber = cycleRegex.cap(1).toInt();
                withinCycleRange = true;
                currentFinalSerialNumber = 0;
            } else {
                qDebug() << "Error: Cycle number not found in the input string";
            }
            continue;
        }

        if (withinCycleRange && !line.contains(CycleEnd)) {
            if (line.startsWith("Debug{")) {
                logEntry.message = line.mid(5);  // Remove "Debug"
                logEntry.messageType = "Json";
                logEntry.tag = "Json";
                getFinalSerialNumber(logEntry.message, logEntry.finalSerialNumber);

                // Update previous entries with the same cycleNumber
                updatePrevEntriesForFinalSerialNumber(currentCycleNumber, logEntry.finalSerialNumber, logEntries);

                currentFinalSerialNumber = logEntry.finalSerialNumber;
            } else {
                int firstSpaceIndex = line.indexOf(' ');

                if (firstSpaceIndex != -1) {
                    logEntry.messageType = "Debug";
                    QString completeType = line.left(firstSpaceIndex);
                    logEntry.tag = completeType.mid(5);  // Remove "Debug"
                    logEntry.message = line.mid(firstSpaceIndex + 1).trimmed();

                    if (logEntry.tag.startsWith("\"") && logEntry.tag.endsWith("\"")) {
                        logEntry.tag = logEntry.tag.mid(1, logEntry.tag.length() - 2);
                    }
                } else {
                    qDebug() << "Error: No space found in the string";
                    continue;
                }

                logEntry.finalSerialNumber = currentFinalSerialNumber;
            }

            logEntry.cycleNumber = currentCycleNumber;
            logEntries.append(logEntry);
        } else {
            withinCycleRange = false;
        }
    }

    logFile.close();
}

void DebugLogParser::batchInsertData(const QVector<LogEntry> &logEntries) {
    if (logEntries.isEmpty()) {
        return;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO MeterLogData (cycleNumber, messageType, tag, message, finalSerialNumber) "
                        "VALUES (:cycleNumber, :messageType, :tag, :message, :finalSerialNumber)");

    if (!db.transaction()) {
        qDebug() << "Error starting database transaction:" << db.lastError().text();
        return;
    }

    for (const LogEntry &logEntry : logEntries) {
        insertQuery.bindValue(":cycleNumber", logEntry.cycleNumber);
        insertQuery.bindValue(":messageType", logEntry.messageType);
        insertQuery.bindValue(":tag", logEntry.tag);
        insertQuery.bindValue(":message", logEntry.message);
        insertQuery.bindValue(":finalSerialNumber", logEntry.finalSerialNumber);

        if (!insertQuery.exec()) {
            qDebug() << "Error inserting data into database:" << insertQuery.lastError().text();
            db.rollback();
            return;
        }
    }

    if (!db.commit()) {
        qDebug() << "Error committing database transaction:" << db.lastError().text();
    }
}

void DebugLogParser::updatePrevEntriesForFinalSerialNumber(int cycleNumber, int finalSerialNumber, QVector<LogEntry> &logEntries) {
    if (cycleNumber == 0) {
        return;
    }

    for(auto& it : logEntries)
        if(it.cycleNumber == cycleNumber)
            it.finalSerialNumber = finalSerialNumber;
}
