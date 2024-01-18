#include "widget.h"
#include "sourcepage.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include "jsonutils.h"
#include "debuglogparser.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    JsonUtils* jsonUtils = new JsonUtils();
//    QString dbgDirPath = "/home/piyush/Documents/Logs/toParse/";
//    QFile csvFile("/home/piyush/Documents/Logs/toParse/csvFile.csv");
//    bool retVal = jsonUtils->convertJsonFileDirToCSV(dbgDirPath, csvFile);
//    if(retVal) {
//        qDebug() << "Successfully added json logs to csv file";
//    } else {
//        qDebug() << "Json log to CSV conversion failed. Check the log trace";
//    }

//    QString dbgDirPath = "C:\\Users\\DivyanshuKumar\\OneDrive - Sinhal Udyog pvt ltd\\Documents\\2023-11-22";
//    DebugLogParser *dbgLogParser = new DebugLogParser();
//    dbgLogParser->parseDebugLogs(dbgDirPath);

    SourcePage *s = new SourcePage();
    s->show();

    return a.exec();
}
