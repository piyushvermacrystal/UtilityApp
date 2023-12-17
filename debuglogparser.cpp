#include "debuglogparser.h"
#include "defines.h"

#include <QDebug>
#include <QSqlQuery>
#include <QDir>
#include <QSqlError>

DebugLogParser::DebugLogParser() {}

bool DebugLogParser::parseDebugLogs(const QString &dbgLogPath) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
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
                                     "message TEXT"
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
    batchInsertData(logEntries, db);

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

    while (!logStream.atEnd()) {
        QString line = logStream.readLine();

        LogEntry logEntry;
        logEntry.cycleNumber = 0;

        // Check for Cycle Start
        if (line.contains(CycleStart)) {
            if (cycleRegex.indexIn(line) != -1) {
                QString cycleNumberString = cycleRegex.cap(1);
                currentCycleNumber = cycleNumberString.toInt();
                withinCycleRange = true;
            } else {
                qDebug() << "Error: Cycle number not found in the input string";
            }
            continue;
        }

        // Process lines within the cycle
        if (withinCycleRange && !line.contains(CycleEnd)) {
            // Extract message information from the line
            if (line.startsWith("Debug{")) {
                logEntry.message = line.mid(5);  // Remove "Debug"
                logEntry.messageType = "Json";
                logEntry.tag = "Json";
            } else {
                int firstSpaceIndex = line.indexOf(' ');

                // Check if the space was found
                if (firstSpaceIndex != -1) {
                    // Extract the two parts
                    // logEntry.messageType = line.left(firstSpaceIndex);
                    logEntry.messageType = "Debug";
                    QString completeType = line.left(firstSpaceIndex);
                    logEntry.tag = completeType.mid(5);  // Remove "Debug"
                    logEntry.message = line.mid(firstSpaceIndex + 1).trimmed();

                    // Check if the tag is enclosed in double quotes
                    if (logEntry.tag.startsWith("\"") && logEntry.tag.endsWith("\"")) {
                        // Remove double quotes from the tag
                        logEntry.tag = logEntry.tag.mid(1, logEntry.tag.length() - 2);
                    }
                } else {
                    // Output an error message if no space was found
                    qDebug() << "Error: No space found in the string";
                    continue;  // Skip processing this line
                }
            }

            logEntry.cycleNumber = currentCycleNumber;
            logEntries.append(logEntry);
        } else {
            withinCycleRange = false;
        }
    }

    logFile.close();
}

void DebugLogParser::batchInsertData(const QVector<LogEntry> &logEntries, QSqlDatabase &db) {
    if (logEntries.isEmpty()) {
        return;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO MeterLogData (cycleNumber, messageType, tag, message) "
                        "VALUES (:cycleNumber, :messageType, :tag, :message)");

    // Use a transaction for batch insert
    if (!db.transaction()) {
        qDebug() << "Error starting database transaction:" << db.lastError().text();
        return;
    }

    for (const LogEntry &logEntry : logEntries) {
        insertQuery.bindValue(":cycleNumber", logEntry.cycleNumber);
        insertQuery.bindValue(":messageType", logEntry.messageType);
        insertQuery.bindValue(":tag", logEntry.tag);
        insertQuery.bindValue(":message", logEntry.message);

        if (!insertQuery.exec()) {
            qDebug() << "Error inserting data into database:" << insertQuery.lastError().text();
            db.rollback();  // Rollback the transaction in case of an error
            return;
        }
    }

    // Commit the transaction if all inserts are successful
    if (!db.commit()) {
        qDebug() << "Error committing database transaction:" << db.lastError().text();
    }
}
