#include "jsonutilpage.h"
#include "ui_jsonutilpage.h"

#include "jsonutils.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

JsonUtilPage::JsonUtilPage(const QString& path, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JsonUtilPage),
    logDirPath(path)
{
    ui->setupUi(this);

    connect(ui->selectCsvFilePathButton, SIGNAL(clicked()), this, SLOT(setCSVFileName()));
    connect(ui->convertJsonToCSVButton, SIGNAL(clicked()), this, SLOT(convertJsonDirToCSV()));
    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(handleBackButton()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(handleCloseButton()));

    ui->csvFilePathLineEdit->setReadOnly(true);
    ui->conversionResultLineEdit->setReadOnly(true);
}

JsonUtilPage::~JsonUtilPage()
{
    delete ui;
}

void JsonUtilPage::setCSVFileName() {
    // Open a file dialog to select the CSV file name
    QString csvFilePath_user = QFileDialog::getSaveFileName(this, tr("Save CSV File"), QDir::homePath(), tr("CSV Files (*.csv)"));

    // Check if the user selected a file name or canceled the dialog
    if (!csvFilePath_user.isEmpty()) {
        // Set the selected CSV file name to the member variable
        csvFilePath = csvFilePath_user;

        // Display the selected CSV file name in lineEditCSVName
        ui->csvFilePathLineEdit->setText(csvFilePath);
    } else {
        // User canceled the dialog
        QMessageBox::warning(this, tr("Warning"), tr("CSV file name selection canceled."));
    }
}

void JsonUtilPage::convertJsonDirToCSV() {
    if (csvFilePath.isEmpty()) {
        ui->conversionResultLineEdit->setText("Specify CSV file name first!");
        QMessageBox::warning(this, tr("Warning"), tr("Select CSV file path first."));
        return;
    }

    JsonUtils* jsonUtils = new JsonUtils();
    QFile csvFile(csvFilePath);
    qDebug() << csvFilePath;
    qDebug() << logDirPath;
    bool retVal = jsonUtils->convertJsonFileDirToCSV(logDirPath, csvFile);

    if (retVal) {
        ui->conversionResultLineEdit->setText("Successfully added JSON logs to CSV file");
    } else {
        ui->conversionResultLineEdit->setText("JSON log to CSV conversion failed. Check the log trace");
    }
}

void JsonUtilPage::handleBackButton() {
    this->hide();
    emit backButtonClicked();
}

void JsonUtilPage::handleCloseButton() {
    this->close();
    emit closeButtonClicked();
}
