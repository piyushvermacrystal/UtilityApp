#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include "jsonutils.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    JsonUtils* jsonUtils = new JsonUtils();
    QString dbgDirPath = "/home/piyush/Documents/Logs/toParse/";
    QFile csvFile("/home/piyush/Documents/Logs/toParse/csvFile.csv");
    bool retVal = jsonUtils->convertJsonFileDirToCSV(dbgDirPath, csvFile);
    if(retVal) {
        qDebug() << "Successfully added json logs to csv file";
    } else {
        qDebug() << "Json log to CSV conversion failed. Check the log trace";
    }

    return 1;
}
