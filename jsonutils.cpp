#include "jsonutils.h"
#include <QDebug>

const QString jsonLogPattern = "Debug{";
//const QString header = "failedAt,functionTestResult_common_isTestPass,functionTestResult_common_lastState,functionTestResult_functionTest_coverClosed,functionTestResult_functionTest_magnetStat1,functionTestResult_functionTest_magnetStat2,functionTestResult_functionTest_neutralCurrentOff,functionTestResult_functionTest_neutralCurrentOn,functionTestResult_functionTest_phaseCurrentOff,functionTestResult_functionTest_phaseCurrentOn,functionTestResult_functionTest_pushButton,functionTestResult_pcbNumber_pcbNumber,functionTestResult_rtc_rtcDriftSecond,functionTestResult_vi_neutralCurrent,functionTestResult_vi_phaseCurrent,functionTestResult_vi_voltage,highCurrentTestResult_common_isTestPass,highCurrentTestResult_common_lastState,highCurrentTestResult_cumulativeEnergy,highCurrentTestResult_meterEnergyWH,highCurrentTestResult_neutralEnergy,highCurrentTestResult_neutralReactiveEnergy,highCurrentTestResult_neutralWattHour,highCurrentTestResult_phaseEnergy,highCurrentTestResult_phaseReactiveEnergy,highCurrentTestResult_phaseWattHour,highCurrentTestResult_startingCumlativeEnergy,highCurrentTestResult_testCurrent,highCurrentTestResult_testVoltage,highCurrentTestResult_timeMS,hvTestResult_isTestPass,hvTestResult_lastState,laserEngraveResult_loaNumber,laserEngraveResult_mfgDate,laserEngraveResult_serialNumber,lowCurrentTestResult_common_isTestPass,lowCurrentTestResult_common_lastState,lowCurrentTestResult_cumulativeEnergy,lowCurrentTestResult_meterEnergyWH,lowCurrentTestResult_neutralEnergy,lowCurrentTestResult_neutralReactiveEnergy,lowCurrentTestResult_neutralWattHour,lowCurrentTestResult_phaseEnergy,lowCurrentTestResult_phaseReactiveEnergy,lowCurrentTestResult_phaseWattHour,lowCurrentTestResult_startingCumlativeEnergy,lowCurrentTestResult_testCurrent,lowCurrentTestResult_testVoltage,lowCurrentTestResult_timeMS,megarTestResult_isTestPass,megarTestResult_lastState,meterFinalSerialNumber,meterTmpSerialNumber,neutralCalibration_isTestPass,neutralCalibration_lastState,nicSyncTestResult_channelAddrss,nicSyncTestResult_common_isTestPass,nicSyncTestResult_common_lastState,nicSyncTestResult_dlmsLastState,nicSyncTestResult_isDLMSPass,nicSyncTestResult_meterMemoryNumber,nicSyncTestResult_meterNumberPrint,nicSyncTestResult_netowrkAddress,nicSyncTestResult_rfKey,phaseCalibration_isTestPass,phaseCalibration_lastState,startingCurrentTestResult_common_isTestPass,startingCurrentTestResult_common_lastState,startingCurrentTestResult_cumulativeEnergy,startingCurrentTestResult_meterEnergyWH,startingCurrentTestResult_neutralEnergy,startingCurrentTestResult_neutralReactiveEnergy,startingCurrentTestResult_neutralWattHour,startingCurrentTestResult_phaseEnergy,startingCurrentTestResult_phaseReactiveEnergy,startingCurrentTestResult_phaseWattHour,startingCurrentTestResult_startingCumlativeEnergy,startingCurrentTestResult_testCurrent,startingCurrentTestResult_testVoltage,startingCurrentTestResult_timeMS\n"
const QString header =  "failedAt,meterTmpSerialNumber,meterMemoryNumber,meterFinalSerialNumber,"
                        "FTisTestPass,FTlastState,FTcoverClosed,FTmagnetStat1,FTmagnetStat2,FTneutralCurrentOff,FTneutralCurrentOn,FTphaseCurrentOff,FTphaseCurrentOn,FTpushButton,FTrtcDriftSecond,PHCisTestPass,PHClastState,NECisTestPass,NEClastState,"
                        "SCisTestPass,SClastState,SCphaseWattHour,SCneutralWattHour,SCphaseEnergy,SCneutralEnergy,SCphaseReactiveEnergy,SCneutralReactiveEnergy,SCcumulativeEnergy,SCtheoryEnergy,SCpowerWH,"
                        "E1isTestPass,E1lastState,E1phaseWattHour,E1neutralWattHour,E1phaseEnergy,E1neutralEnergy,E1phaseReactiveEnergy,E1neutralReactiveEnergy,E1cumulativeEnergy,E1theoryEnergy,E1powerWH,"
                        "E2isTestPass,E2lastState,E2phaseWattHour,E2neutralWattHour,E2phaseEnergy,E2neutralEnergy,E2phaseReactiveEnergy,E2neutralReactiveEnergy,E2cumulativeEnergy,E2theoryEnergy,E2powerWH,"
                        "NSisTestPass,NSlastState,NSmeterMemoryNumber,LASloaNumber,LASmfgDate,LASserialNumber\r\n";

JsonUtils::JsonUtils()
{

}

void JsonUtils::flattenJson(const QJsonObject &jsonObject, QStringList &values)
{
    for (const QString& key : jsonObject.keys()) {
        QJsonValue value = jsonObject[key];
        if (value.isObject()) {
            flattenJson(value.toObject(), values);
        } else {
            values.append(value.toVariant().toString());
        }
    }
}

bool JsonUtils::convertJsonToCSV(QTextStream& in, QTextStream& out)
{
    static bool parseNext = false;

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if(!parseNext){
            if(line.contains("--WatchMe--"))
                parseNext = true;
        }
        if(!parseNext)
            continue;

        int index = line.indexOf(jsonLogPattern);
        if(index != -1)
        {
            parseNext = false;
            line = line.mid(5);
            QStringList values;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(line.toLatin1());
            if (!jsonDoc.isObject()) {
              return 1;
            }
            QJsonObject jsonObject = jsonDoc.object();
//            flattenJson(jsonObject, values);
            QJsonObject functionTestResult = jsonObject.value("functionTestResult").toObject();
            qDebug() << functionTestResult;

            QJsonObject comman;
            comman = functionTestResult.value("rtc").toObject();
            qDebug() << comman;
            qDebug() << comman.value("rtcDriftSecond");
            qDebug() << comman.value("rtcDriftSecond").toString();
            qDebug() << comman.value("rtcDriftSecond").toInt();
            qDebug() << jsonObject.value("meterFinalSerialNumber").toString();

            values << jsonObject.value("failedAt").toString();
            values << QString::number(jsonObject.value("meterTmpSerialNumber").toInt());
            values << jsonObject.value("nicSyncTestResult").toObject().value("meterMemoryNumber").toString();
            values << QString::number(jsonObject.value("meterFinalSerialNumber").toInt());

            values << (jsonObject.value("functionTestResult").toObject().value("common").toObject().value("isTestPass").toBool() ? "True" : "False");
            values << jsonObject.value("functionTestResult").toObject().value("common").toObject().value("lastState").toString();
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("coverClosed").toString());
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("magnetStat1").toString());
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("magnetStat2").toString());
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("neutralCurrentOff").toString());
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("neutralCurrentOn").toString());
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("phaseCurrentOff").toString());
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("phaseCurrentOn").toString());
            values << (jsonObject.value("functionTestResult").toObject().value("functionTest").toObject().value("pushButton").toString());
            values << QString::number(jsonObject.value("functionTestResult").toObject().value("rtc").toObject().value("rtcDriftSecond").toInt());

            values << (jsonObject.value("phaseCalibration").toObject().value("isTestPass").toBool() ?  "True" : "False");
            values << jsonObject.value("phaseCalibration").toObject().value("lastState").toString();

            values << (jsonObject.value("neutralCalibration").toObject().value("isTestPass").toBool() ?  "True" : "False");
            values << jsonObject.value("neutralCalibration").toObject().value("lastState").toString();

//            "SCisTestPass,SClastState,SCphaseWattHour,SCneutralWattHour,SCphaseEnergy,SCneutralEnergy,SCphaseReactiveEnergy,SCneutralReactiveEnergy,"
//            "SCcumulativeEnergy,SCtheoryEnergy,SCpowerWH"

            values << (jsonObject.value("startingCurrentTestResult").toObject().value("common").toObject().value("isTestPass").toBool() ? "True" : "False");
            values << jsonObject.value("startingCurrentTestResult").toObject().value("common").toObject().value("lastState").toString();
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("phaseWattHour").toDouble());
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("neutralWattHour").toDouble());
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("phaseEnergy").toDouble()); //VAH
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("neutralEnergy").toDouble()); // VAH
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("phaseReactiveEnergy").toDouble()); //VARH
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("neutralReactiveEnergy").toDouble()); // VAH
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("cumulativeEnergy").toDouble());
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("theoryEnergy").toDouble());
            values << QString::number(jsonObject.value("startingCurrentTestResult").toObject().value("powerWH").toDouble()); //powerWH

            values << (jsonObject.value("lowCurrentTestResult").toObject().value("common").toObject().value("isTestPass").toBool() ? "True" : "False");
            values << jsonObject.value("lowCurrentTestResult").toObject().value("common").toObject().value("lastState").toString();
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("phaseWattHour").toDouble());
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("neutralWattHour").toDouble());
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("phaseEnergy").toDouble()); //VAH
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("neutralEnergy").toDouble()); // VAH
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("phaseReactiveEnergy").toDouble()); //VARH
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("neutralReactiveEnergy").toDouble()); // VAH
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("cumulativeEnergy").toDouble());
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("theoryEnergy").toDouble());
            values << QString::number(jsonObject.value("lowCurrentTestResult").toObject().value("powerWH").toDouble()); //powerWH

            values << (jsonObject.value("highCurrentTestResult").toObject().value("common").toObject().value("isTestPass").toBool() ? "True" : "False");
            values << jsonObject.value("highCurrentTestResult").toObject().value("common").toObject().value("lastState").toString();
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("phaseWattHour").toDouble());
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("neutralWattHour").toDouble());
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("phaseEnergy").toDouble()); //VAH
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("neutralEnergy").toDouble()); // VAH
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("phaseReactiveEnergy").toDouble()); //VARH
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("neutralReactiveEnergy").toDouble()); // VAH
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("cumulativeEnergy").toDouble());
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("theoryEnergy").toDouble());
            values << QString::number(jsonObject.value("highCurrentTestResult").toObject().value("powerWH").toDouble()); //powerWH

            values << (jsonObject.value("nicSyncTestResult").toObject().value("common").toObject().value("isTestPass").toBool() ? "True" : "False");
            values << jsonObject.value("nicSyncTestResult").toObject().value("common").toObject().value("lastState").toString();
            values << jsonObject.value("nicSyncTestResult").toObject().value("meterMemoryNumber").toString();

            values << jsonObject.value("laserEngraveResult").toObject().value("loaNumber").toString();
            values << jsonObject.value("laserEngraveResult").toObject().value("mfgDate").toString();
            values << jsonObject.value("laserEngraveResult").toObject().value("serialNumber").toString();

            for (const QString& value : values) {
              out << "\"" << value << "\",";
            }
            out << "\n";
        }
    }
    return true;
}

bool JsonUtils::convertJsonFileToCSV(const QString& dbgFilePath, QFile& csvFile)
{
    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Cannot open csv file";
        return false;
    }

    QTextStream out(&csvFile);
    out << header;

    QFile file(dbgFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open debug file " << file.errorString();
        return false;
    }

    QTextStream in(&file);
    bool result = convertJsonToCSV(in, out);
    file.close();
    out.flush();
    return result;
}

bool JsonUtils::convertJsonFileDirToCSV(const QString& dbgDirPath, QFile& csvFile)
{
    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Cannot open csv file";
        return false;
    }

    QTextStream out(&csvFile);
    out << header;

    QDir dir(dbgDirPath);
    QStringList files = dir.entryList(QDir::Files);
    for(QString&  fileName : files) {
        fileName = dbgDirPath + "/" + fileName;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Cannot open debug file " << file.errorString();
            return false;
        }

        QTextStream in(&file);
        bool result = convertJsonToCSV(in, out);
        file.close();
        if (!result) {
            return false;
        }
    }
    out.flush();
    return true;
}


