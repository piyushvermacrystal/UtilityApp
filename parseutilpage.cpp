#include "parseutilpage.h"
#include "ui_parseutilpage.h"

#include "debuglogparser.h"

ParseUtilPage::ParseUtilPage(const QString& path, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParseUtilPage),
    logDirPath(path)
{
    ui->setupUi(this);

    connect(ui->parseLogFilesButton, SIGNAL(clicked()), this, SLOT(parseLogFilesToDB()));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(handleBackButton()));

    ui->parseLogFileResLineEdit->setReadOnly(true);
}

ParseUtilPage::~ParseUtilPage()
{
    delete ui;
}

void ParseUtilPage::parseLogFilesToDB()
{
    DebugLogParser *dbgLogParser = new DebugLogParser();
    bool retVal = dbgLogParser->parseDebugLogs(logDirPath);
    if(retVal) {
        ui->parseLogFileResLineEdit->setText("Successfully parsed log files to the database.");
    } else {
        ui->parseLogFileResLineEdit->setText("Prasring log files to the database failed.");
    }
}

void ParseUtilPage::handleBackButton() {
    this->hide();
    emit backButtonClicked();
}

void ParseUtilPage::handleCloseButton() {
    this->close();
    emit closeButtonClicked();
}
