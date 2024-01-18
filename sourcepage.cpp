#include "sourcepage.h"
#include "ui_sourcepage.h"

#include "jsonutilpage.h"
#include "parseutilpage.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

SourcePage::SourcePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SourcePage)
{
    ui->setupUi(this);
    connect(ui->SelectLogDirPushButton, SIGNAL(clicked()), this, SLOT(setLogDirectory()));
    connect(ui->jsonUtilButton, SIGNAL(clicked()), this, SLOT(handleJsonUtilCalls()));
    connect(ui->parseUtilButton, SIGNAL(clicked()), this, SLOT(handleParseUtilCalls()));

    ui->logDirPathLineEdit->setReadOnly(true);
}

SourcePage::~SourcePage()
{
    delete ui;
}

void SourcePage::setLogDirectory()
{
    // Open a directory dialog to select the directory
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::homePath());

    // Check if the user selected a directory or canceled the dialog
    if (!dirPath.isEmpty()) {
        // Set the selected directory path to the member variable
        logDirPath = dirPath;

        // Display the selected directory path in lineEditDIR
        ui->logDirPathLineEdit->setText(logDirPath);
    } else {
        // User canceled the dialog
        QMessageBox::warning(this, tr("Warning"), tr("Directory selection canceled."));
    }
}

void SourcePage::handleJsonUtilCalls()
{
    if(logDirPath.size() == 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Select debug log directory first."));
    } else {
        this->hide();
        JsonUtilPage *jsonUtilPage = new JsonUtilPage(logDirPath);
        jsonUtilPage->show();
        connect(jsonUtilPage, SIGNAL(backButtonClicked()), this, SLOT(show()));
        connect(jsonUtilPage, SIGNAL(closeButtonClicked()), this, SLOT(close()));
    }
}

void SourcePage::handleParseUtilCalls()
{
    if(logDirPath.size() == 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Select debug log directory first."));
    } else {
        this->hide();
        ParseUtilPage *parseUtilPage = new ParseUtilPage(logDirPath);
        parseUtilPage->show();
        connect(parseUtilPage, SIGNAL(backButtonClicked()), this, SLOT(show()));
        connect(parseUtilPage, SIGNAL(closeButtonClicked()), this, SLOT(close()));
    }
}
